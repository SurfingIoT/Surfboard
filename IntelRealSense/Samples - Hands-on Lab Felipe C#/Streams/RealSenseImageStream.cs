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

namespace Streams
{
    class RealSenseImageStream
    {
        #region RealSense SDK attributes

        public PXCMSession Session { get; private set; }
        public PXCMSenseManager SenseManager { get; private set; }
        public PXCMCapture.StreamType StreamType { get; private set; }
        
        #endregion

        #region Stream parameters

        public float FramesPerSecond { get; private set; }
        public int ResolutionHeight { get; private set; }
        public int ResolutionWidth { get; private set; }

        private const int DEFAULT_RESOLUTION_WIDTH = 640;
        private const int DEFAULT_RESOLUTION_HEIGHT = 480;
        private const float DEFAULT_FPS = 60;

        #endregion

        public bool IsRunning { get; private set; }
        public Thread CaptureThread { get; private set; }
        public event NewImageEventHandler NewImageAvailable;

        public RealSenseImageStream(PXCMCapture.StreamType streamType) 
        {
            StreamType = streamType;
        }

        public void InitializeStream()
        {
            InitializeStream(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, DEFAULT_FPS);
        }

        public void InitializeStream(int resolutionWidth, int resolutionHeight, float framesPerSecond)
        {
            if (Session != null)
            {
                // TODO: reinitialize the stream 
                return;
            }

            // Initializing parameters for future reference
            FramesPerSecond = framesPerSecond;
            ResolutionWidth = resolutionWidth;
            ResolutionHeight = resolutionHeight;

            // Creating a SDK session
            Session = PXCMSession.CreateInstance();

            // Creating the SenseManager
            SenseManager = Session.CreateSenseManager();

            // Enabling the stream
            pxcmStatus enableStreamStatus = SenseManager.EnableStream(StreamType, resolutionWidth, resolutionHeight, framesPerSecond);

            if (enableStreamStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                throw new InvalidRealSenseStatusException(enableStreamStatus, string.Format("Failed to enable the {0} stream. Return code: {1}", StreamType, enableStreamStatus));
            }

            // Initializing the SenseManager
            pxcmStatus initSenseManagerStatus = SenseManager.Init();

            // Initializing the SenseManager
            if (initSenseManagerStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                throw new InvalidRealSenseStatusException(enableStreamStatus, string.Format("Failed to initialize the SenseManager. Return code: {0}", initSenseManagerStatus));
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

                    // Querying the images samples
                    PXCMCapture.Sample sample = SenseManager.QuerySample();

                    if (sample != null)
                    {
                        PXCMImage image = null;

                        switch (StreamType)
                        {
                            case PXCMCapture.StreamType.STREAM_TYPE_DEPTH:
                                image = sample.depth;
                                break;
                            case PXCMCapture.StreamType.STREAM_TYPE_IR:
                                image = sample.ir;
                                break;
                            case PXCMCapture.StreamType.STREAM_TYPE_LEFT:
                                image = sample.left;
                                break;
                            case PXCMCapture.StreamType.STREAM_TYPE_RIGHT:
                                image = sample.right;
                                break;
                            case PXCMCapture.StreamType.STREAM_TYPE_COLOR:
                            default:
                                image = sample.color;
                                break;
                        }

                        if (image != null)
                        {
                            // Acquiring access to the image data
                            PXCMImage.ImageData imageData = null;

                            pxcmStatus acquireAccessStatus = image.AcquireAccess(PXCMImage.Access.ACCESS_READ, PXCMImage.PixelFormat.PIXEL_FORMAT_RGB32, out imageData);

                            if (acquireAccessStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                            {
                                throw new InvalidRealSenseStatusException(acquireAccessStatus, string.Format("Failed to acquire access to the image. Return code: {0}", acquireAccessStatus));
                            }

                            // Converting the image to System.Drawing.Bitmap
                            System.Drawing.Bitmap bitmap = imageData.ToBitmap(0, image.info.width, image.info.height);

                            if (NewImageAvailable != null)
                            {
                                NewImageAvailable(this, new NewImageArgs(StreamType, bitmap));
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
            public PXCMCapture.StreamType StreamType { get; private set; }

            public NewImageArgs(PXCMCapture.StreamType streamType, System.Drawing.Bitmap bitmap)
            {
                Bitmap = bitmap;
                StreamType = streamType;
            }
        }
    }
}
