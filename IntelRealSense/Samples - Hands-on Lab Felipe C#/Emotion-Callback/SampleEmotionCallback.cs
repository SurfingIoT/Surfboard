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

namespace Emotion_Callback
{
    class SampleEmotionCallback
    {
        /// <summary>
        /// This samples show how to detect Emotions using SenseManager Callbacks in RealSense SDK.
        /// </summary>
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
            
            // Creating Sense Manager Handler
            PXCMSenseManager.Handler handler = new PXCMSenseManager.Handler
            {
                onModuleProcessedFrame = OnModuleProcessedFrame
            };

            // Initializing the camera with Handler
            if (senseManager.Init(handler) < pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the SenseManager");
                return;
            }

            // Streaming data
            senseManager.StreamFrames(true);

            // Release resources
            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();
        }

        static pxcmStatus OnModuleProcessedFrame(Int32 mid, PXCMBase module, PXCMCapture.Sample sample) {
            // check if the callback is from the emotion module.
            if (mid == PXCMEmotion.CUID) {
                PXCMEmotion emotionModule = module.QueryInstance<PXCMEmotion>();

                // Processing Emotions
                ProcessEmotions(emotionModule);
            }
            
            // return NO_ERROR to continue, or any error to abort.
            return pxcmStatus.PXCM_STATUS_NO_ERROR;
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
