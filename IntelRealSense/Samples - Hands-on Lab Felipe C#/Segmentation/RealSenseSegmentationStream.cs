//--------------------------------------------------------------------------------------
// Copyright 2015 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED 'AS IS.'
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//--------------------------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace Segmentation
{
    class RealSenseSegmentationStream
    {
        #region RealSense SDK attributes

        public PXCMSession Session { get; private set; }
        public PXCMSenseManager SenseManager { get; private set; }
        
        #endregion

        public bool IsRunning { get; private set; }
        public Thread CaptureThread { get; private set; }
        public event NewImageEventHandler NewImageAvailable;

        public RealSenseSegmentationStream() 
        {
        }

        public void InitializeStream()
        {
            if (Session != null)
            {
                // TODO: reinitialize the stream 
                return;
            }

            // Creating a SDK session
            Session = PXCMSession.CreateInstance();

            // Creating the SenseManager
            SenseManager = Session.CreateSenseManager();

            // Enabling the 3D Segmentation
            pxcmStatus enable3dSegmentationStatus = SenseManager.Enable3DSeg();

            if (enable3dSegmentationStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                throw new InvalidRealSenseStatusException(enable3dSegmentationStatus, string.Format("Failed to enable the 3D Segmentation. Return code: {0}", enable3dSegmentationStatus));
            }

            // Initializing the SenseManager
            pxcmStatus initSenseManagerStatus = SenseManager.Init();

            // Initializing the SenseManager
            if (initSenseManagerStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                throw new InvalidRealSenseStatusException(initSenseManagerStatus, string.Format("Failed to initialize the SenseManager. Return code: {0}", initSenseManagerStatus));
            }
        }

        public void StartStream() 
        {
            if (SenseManager == null)
            {
                throw new NullReferenceException("The SenseManager isn't initialized. Please check if you already called the InitializeStream method.");
            }

            IsRunning = true;

            CaptureThread = new Thread(() => {
                

                while (IsRunning)
                {
                    if (SenseManager == null || !SenseManager.IsConnected())
                    {
                        throw new Exception("The SenseManager is not ready to stream.");
                    }

                    // Acquiring the frames to query aligned samples
                    pxcmStatus acquireFrameStatus = SenseManager.AcquireFrame(true);
                    if (acquireFrameStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                    {
                        throw new InvalidRealSenseStatusException(acquireFrameStatus, string.Format("Failed to acquire a frame. Return code: {0}", acquireFrameStatus));
                    }

                    // Querying the 3D segmentation
                    PXCM3DSeg segmentation = SenseManager.Query3DSeg();

                    if (segmentation != null)
                    {
                        PXCMImage image = segmentation.AcquireSegmentedImage();

                        if (image != null)
                        {
                            // Acquiring access to the image data
                            PXCMImage.ImageData imageData = null;

                            pxcmStatus acquireAccessStatus = image.AcquireAccess(PXCMImage.Access.ACCESS_READ, PXCMImage.PixelFormat.PIXEL_FORMAT_RGB32, out imageData);

                            if (acquireAccessStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                            {
                                throw new InvalidRealSenseStatusException(acquireAccessStatus, string.Format("Failed to acquire access to the image. Return code: {0}", acquireAccessStatus));
                            }

                            // Implementing a simple filter to make the background gray. 
                            // This algorithm was extracted from the Segmentation sample (RSSDK\framework\CSharp\3dseg.cs) and refactored to make the code readable. 
                            // TODO: check if the if inside the for could cause performance problems.
                            const byte GREY = 0x7f;
                            int imageHeight = image.QueryInfo().height;
                            int imageWidth = image.QueryInfo().width;

                            for (int y = 0; y < imageHeight; y++)
                            {
                                unsafe
                                {
                                    byte* pixel = (byte*)imageData.planes[0] + y * imageData.pitches[0];
                                    
                                    for (int x = 0; x < imageWidth; x++)
                                    {
                                        byte alphaValue = pixel[3];

                                        float blendFactor = (float)alphaValue / (float)255;

                                        for (int channel = 0; channel < 3; channel++)
                                        {
                                            byte notVisibleChannel = (byte)((pixel[channel] >> 4) + GREY); // This operation will make the RGB channels vary between 127 and 142, making then become "more gray".

                                            if (alphaValue > 0) // If the alpha channel is smaller or equal to zero we need to make the channel "not visible".
                                            {
                                                pixel[channel] = (byte)(pixel[channel] * blendFactor + notVisibleChannel * (1.0f - blendFactor)); // Blending the channel
                                            }else{
                                                pixel[channel] = notVisibleChannel;
                                            }
                                        }
                                        pixel += 4; // moving to the next pixel.
                                    }
                                }
                            }


                            // Converting the image to System.Drawing.Bitmap
                            System.Drawing.Bitmap bitmap = imageData.ToBitmap(0, image.info.width, image.info.height);

                            if (NewImageAvailable != null)
                            {
                                NewImageAvailable(this, new NewImageArgs(bitmap));
                            }

                            image.ReleaseAccess(imageData);
                        }
                    }

                    SenseManager.ReleaseFrame();
                }
            });

            CaptureThread.Start();
        }

        public void StopStream()
        {
            if (CaptureThread != null)
            {
                IsRunning = false;
                //CaptureThread.Join();
                CaptureThread = null;
            }
        }

        public class InvalidRealSenseStatusException : Exception
        {
            public pxcmStatus InvalidStatus { get; private set; }

            public InvalidRealSenseStatusException(pxcmStatus status, string message) : base(message)
            {
                InvalidStatus = status;
            }
        }

        public delegate void NewImageEventHandler(object sender, NewImageArgs args);

        public class NewImageArgs : EventArgs
        {
            public System.Drawing.Bitmap Bitmap { get; private set; }

            public NewImageArgs(System.Drawing.Bitmap bitmap)
            {
                Bitmap = bitmap;
            }
        }
    }
}
