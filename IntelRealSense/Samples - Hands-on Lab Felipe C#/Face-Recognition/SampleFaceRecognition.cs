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
using System.Threading.Tasks;

namespace Face_Recognition
{
    /// <summary>
    /// This sample shows how to recognize faces using RealSense SDK.
    /// </summary>
    class SampleFaceRecognition
    {

        static bool isRegistering = false;

        static void Main(string[] args)
        {
            // Creating a SDK session
            PXCMSession session = PXCMSession.CreateInstance();

            // Querying the SDK version
            PXCMSession.ImplVersion version = session.QueryVersion();
            Console.WriteLine("RealSense SDK Version {0}.{1}", version.major, version.minor);

            // Creating the SenseManager
            PXCMSenseManager senseManager = session.CreateSenseManager();

            if (senseManager == null)
            {
                Console.WriteLine("Failed to create the SenseManager object.");
                return;
            }

            // Enabling the Face module
            pxcmStatus enablingModuleStatus = senseManager.EnableFace();

            if (enablingModuleStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the Face Module");
                return;
            }

            // Getting the instance of the Face Module
            PXCMFaceModule faceModule = senseManager.QueryFace();
            if (faceModule == null)
            {
                Console.WriteLine("Failed to query the face module");
                return;
            }

            // Creating an active configuration
            PXCMFaceConfiguration faceConfiguration = faceModule.CreateActiveConfiguration();

            if (faceConfiguration == null)
            {
                Console.WriteLine("Failed to create the face configuration");
                return;
            }

            // Track 2D Face
            // faceConfiguration.SetTrackingMode(PXCMFaceConfiguration.TrackingModeType.FACE_MODE_COLOR); 

            // Track 3D Face
            faceConfiguration.SetTrackingMode(PXCMFaceConfiguration.TrackingModeType.FACE_MODE_COLOR_PLUS_DEPTH);

            // Enabling Recognition
            PXCMFaceConfiguration.RecognitionConfiguration recognitionConfiguration = faceConfiguration.QueryRecognition();
            if (recognitionConfiguration == null)
            {
                Console.WriteLine("Failed to create the recognition configuration");
                return;
            }
            recognitionConfiguration.Enable();

            // Set the recognition registration mode
            recognitionConfiguration.SetRegistrationMode(PXCMFaceConfiguration.RecognitionConfiguration.RecognitionRegistrationMode.REGISTRATION_MODE_CONTINUOUS);

            Console.WriteLine("Accurracy {0} - Registration Mode {1}", recognitionConfiguration.GetAccuracryThreshold(), recognitionConfiguration.GetRegistrationMode());

            // faceConfiguration.strategy = PXCMFaceConfiguration.TrackingStrategyType.STRATEGY_RIGHT_TO_LEFT;

            // Applying the settings
            if (faceConfiguration.ApplyChanges() < pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to apply the configuration");
                return;
            }

            // Initializing the camera
            if (senseManager.Init() < pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the SenseManager");
                return;
            }

            // Creating the Face output           
            PXCMFaceData faceData = faceModule.CreateOutput();
            if (faceData == null)
            {
                Console.WriteLine("Failed to create the facedata object");
                return;
            }

            // Looping to query the faces information
            while (true)
            {
                // Acquiring a frame
                if (senseManager.AcquireFrame(true) < pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }

                // Updating the face data
                if (faceData != null)
                {
                    faceData.Update();
                }

                // Querying how many faces were detected
                int numberOfFaces = faceData.QueryNumberOfDetectedFaces();
                Console.WriteLine("{0} face(s) were detected.", numberOfFaces);

                // Querying the information about detected faces
                for (int i = 0; i < numberOfFaces; i++)
                {

                    // Query Face Object
                    PXCMFaceData.Face faceObject = faceData.QueryFaceByIndex(i);
                    if (faceObject == null)
                    {
                        continue;
                    }

                    // Processing Recognition
                    ProcessRecognition(faceObject);
                }

                // Releasing the acquired frame
                senseManager.ReleaseFrame();
            }

            Console.Write("\nPress any key to continue...");
            Console.ReadKey();

            // Releasing resources
            if (faceData != null) faceData.Dispose();
            if (faceConfiguration != null) faceConfiguration.Dispose();

            faceModule.Dispose();
            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();
        }

        static void ProcessRecognition(PXCMFaceData.Face faceObject)
        {
            // Retrieve the recognition data instance
            PXCMFaceData.RecognitionData recognitionData = faceObject.QueryRecognition();

            // Recognize the current face
            if (recognitionData.IsRegistered())
            {
                Int32 userId = recognitionData.QueryUserID();
                Console.WriteLine("User Recognized {0}", userId);
                isRegistering = false;
            }
            else
            {
                Console.WriteLine("Not recognized");
                
                // Read Key pressed to Register User
                if (Console.KeyAvailable)
                {
                    ConsoleKeyInfo key = Console.ReadKey(true);
                    switch (key.Key)
                    {
                        case ConsoleKey.Spacebar:
                            recognitionData.RegisterUser();
                            Console.WriteLine("Registering User");
                            break;
                    }
                }
            }
        }
    }
}
