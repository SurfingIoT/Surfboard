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

namespace SpeechRecognitionDictation
{
    /// <summary>
    /// This samples shows how to do Speech Recognition using RealSense SDK.
    /// </summary>
    class SampleSpeechRecognition
    {
        public enum RecognitionType { DICTATION, COMMAND };

        public const RecognitionType DemoRecognitionType = RecognitionType.DICTATION;

        static void Main(string[] args)
        {
            // Creating a SDK session
            PXCMSession session = PXCMSession.CreateInstance();

            // Querying the SDK version
            PXCMSession.ImplVersion sdkVersion = session.QueryVersion();
            Console.WriteLine("RealSense SDK Version {0}.{1}", sdkVersion.major, sdkVersion.minor);

            // Creating an AudioSource
            PXCMAudioSource audioSource = session.CreateAudioSource();

            // Listing the audio devices
            audioSource.ScanDevices();
            int devicesCount = audioSource.QueryDeviceNum();

            Console.WriteLine("Listing audio devices:");
            PXCMAudioSource.DeviceInfo deviceInfo;
            for (int i = 0; i < devicesCount; i++)
            {
                deviceInfo = new PXCMAudioSource.DeviceInfo();
                audioSource.QueryDeviceInfo(i, out deviceInfo);

                Console.WriteLine("\t{0}", deviceInfo.name);
            }

            // Selecting the first device
            deviceInfo = new PXCMAudioSource.DeviceInfo();
            audioSource.QueryDeviceInfo(0, out deviceInfo);
            audioSource.SetDevice(deviceInfo);

            // Adjusting the audio recording volume
            audioSource.SetVolume(0.2f);

            // Creating the SpeechRecognition instance  
            PXCMSpeechRecognition speechRecognition;
            pxcmStatus speechRecognitionStatus = session.CreateImpl(out speechRecognition);

            if (speechRecognitionStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to create the SpeechRecognition instance");
                return;
            }

            // Configuring the speech module
            List<PXCMSpeechRecognition.ProfileInfo> availableProfiles = new List<PXCMSpeechRecognition.ProfileInfo>();
            Console.WriteLine("Listing the available profiles:");
            for (int i = 0; ; i++)
            {
                PXCMSpeechRecognition.ProfileInfo profileInfo = new PXCMSpeechRecognition.ProfileInfo();
                pxcmStatus profileQueryStatus = speechRecognition.QueryProfile(i, out profileInfo);

                if (profileQueryStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }
                Console.WriteLine("\t{0}", profileInfo.language);
                availableProfiles.Add(profileInfo);
            }

            if (availableProfiles.Count < 0)
            {
                Console.WriteLine("There is no profile available.");
                return;
            }

            // Setting the first profile 
            speechRecognition.SetProfile(availableProfiles[0]);

            if (DemoRecognitionType == RecognitionType.DICTATION) // To change the recognition mode, please change the value of the DemoRecognitionType constant at the beginning of this file.
            {
                //  Seting the dictation mode
                pxcmStatus setDictationStatus = speechRecognition.SetDictation();

                if (setDictationStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    Console.WriteLine("Failed to create the SpeechRecognition instance");
                    return;
                }
            }
            else {
                // Building a list of commands
                String[] commandsGrammar1 = { "One", "Two", "Three" }; // English
                speechRecognition.BuildGrammarFromStringList(1, commandsGrammar1, null);

                String[] commandsGrammar2 = { "Um", "Dois", "Três" };
                speechRecognition.BuildGrammarFromStringList(2, commandsGrammar2, null);

                // Selecting the grammar
                speechRecognition.SetGrammar(1);
            }

            // Handling recognition events
            PXCMSpeechRecognition.Handler handler = new PXCMSpeechRecognition.Handler();
            
            // Note: this sample was developed using anonymous methods but you can use regular methods from your classes to do that. 
            handler.onRecognition = delegate(PXCMSpeechRecognition.RecognitionData data)
            {
                switch (DemoRecognitionType) // To change the recognition mode, please change the value of the DemoRecognitionType constant at the beginning of this file.
                {
                    case RecognitionType.DICTATION:
                        ProcessDictation(data);
                        break;
                    case RecognitionType.COMMAND:
                    default:
                        ProcessCommands(data);
                        break;
                }
            };

            // Listening to warnings and errors
            handler.onAlert = delegate(PXCMSpeechRecognition.AlertData data)
            {
                Console.WriteLine(data.label);
            };

            speechRecognition.StartRec(audioSource, handler);
            
            // Waiting a key to stop voice recognition
            Console.ReadKey();
            speechRecognition.StopRec();
        }

        private static void ProcessCommands(PXCMSpeechRecognition.RecognitionData data)
        {
            for (int i = 0; i < PXCMSpeechRecognition.NBEST_SIZE; i++)
            {
                int label = data.scores[i].label;
                int confidence = data.scores[i].confidence;
                if (label < 0 || confidence < 48) continue;

                Console.WriteLine(data.scores[i].sentence);
                if (data.scores[i].tags.Length > 0)
                {
                    Console.WriteLine(data.scores[i].tags);
                }
            }
        }

        private static void ProcessDictation(PXCMSpeechRecognition.RecognitionData data)
        {
            foreach (PXCMSpeechRecognition.NBest score in data.scores)
            {
                Console.WriteLine("{0}", score.sentence);
            }
        }
    }
}
