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

namespace Emotion
{
    /// <summary>
    /// This sample shows how to detect Emotions using RealSense SDK.
    /// </summary>
    class SampleEmotion
    {
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

            // Enabling Emotion Module
            pxcmStatus enablingModuleStatus = senseManager.EnableEmotion();

            if (enablingModuleStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the Emotion Module");
                return;
            }

            // Getting the instance of the Emotion Module
            PXCMEmotion emotionModule = senseManager.QueryEmotion();
            if (emotionModule == null)
            {
                Console.WriteLine("Failed to query the emotion module");
                return;
            }
            
            // Initializing the camera
            if (senseManager.Init() < pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the SenseManager");
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

                // Processing Emotions
                ProcessEmotions(emotionModule);

                // Releasing the acquired frame
                senseManager.ReleaseFrame();
            }

            Console.Write("\nPress any key to continue...");
            Console.ReadKey();

            // Releasing resources
            emotionModule.Dispose();
            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();

        }

        static void ProcessEmotions(PXCMEmotion emotionModule)
        {
            // Querying how many faces were detected
            int numberOfFaces = emotionModule.QueryNumFaces();
            Console.WriteLine("{0} face(s) were detected.", numberOfFaces);

            // Querying the emotion information about detected faces
            for (int i = 0; i < numberOfFaces; i++)
            {
                PXCMEmotion.EmotionData[] allEmotionData;
                emotionModule.QueryAllEmotionData(i, out allEmotionData);

                // Print emotions detected and intensity
                foreach (PXCMEmotion.EmotionData emotionData in allEmotionData)
                {
                    Console.WriteLine("{0} - Intensity {1}", emotionData.eid, emotionData.intensity);
                }
            }
        }
    }
}
