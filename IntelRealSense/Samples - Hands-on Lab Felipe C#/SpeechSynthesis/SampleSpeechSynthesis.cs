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
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;

namespace SpeechSynthesis
{
    /// <summary>
    /// This samples shows how to do Speech Synthesis using RealSense SDK.
    /// </summary>
    class SampleSpeechSynthesis
    {
        static void Main(string[] args)
        {
            // Creating a SDK session
            PXCMSession session = PXCMSession.CreateInstance();

            // Querying the SDK version
            PXCMSession.ImplVersion sdkVersion = session.QueryVersion();
            Console.WriteLine("RealSense SDK Version {0}.{1}", sdkVersion.major, sdkVersion.minor);

            // Creating the SpeechSynthesis instance
            PXCMSpeechSynthesis speechSynthesis;
            pxcmStatus speechSynthesisStatus = session.CreateImpl<PXCMSpeechSynthesis>(out speechSynthesis);

            if (speechSynthesisStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the speech synthesis");
                return;
            }

            // Listing the available profile
            List<PXCMSpeechSynthesis.ProfileInfo> availableProfiles = new List<PXCMSpeechSynthesis.ProfileInfo>();

            Console.WriteLine("Listing available profiles: ");
            for (int  i= 0;  ; i++)
            {
                PXCMSpeechSynthesis.ProfileInfo profileInfo;
                pxcmStatus queryProfileStatus = speechSynthesis.QueryProfile(i, out profileInfo);

                if (queryProfileStatus !=  pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }

                Console.WriteLine("\t{0}", profileInfo.language);
                availableProfiles.Add(profileInfo);
            }

            if (availableProfiles.Count <= 0)
            {
                Console.WriteLine("There is no profile available.");
            }
            
            // Selecting the first profile available
            speechSynthesis.SetProfile(availableProfiles[0]);

            // Building the sentence
            speechSynthesis.BuildSentence(1, "Testing Speech Synthesis. Is it working?"); // English
            //speechSynthesis.BuildSentence(1, "Testando Síntese de Voz. Está funcionando?"); // Portuguese

            // Querying the buffers available
            int bufferNum = speechSynthesis.QueryBufferNum(1);

            for (int i = 0; i < bufferNum; i++)
            {
                // Querying the audio.
                PXCMAudio audio = speechSynthesis.QueryBuffer(1, i);

                // Acquiring access to the Audio file
                PXCMAudio.AudioData audioData;
                pxcmStatus audioAcquireAccessStatus = audio.AcquireAccess(PXCMAudio.Access.ACCESS_READ, PXCMAudio.AudioFormat.AUDIO_FORMAT_PCM, out audioData);

                if (audioAcquireAccessStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    Console.WriteLine("Failed to acquire access to AudioData");
                }

                // Converting to WAV and Playing the audio. 
                PlayPcmSound(audioData, availableProfiles[0].outputs);

                // Releasing the access to the audio
                audio.ReleaseAccess(audioData);
            }

            Console.ReadKey();
        }

        private static void PlayPcmSound(PXCMAudio.AudioData audioData, PXCMAudio.AudioInfo audioInfo)
        {
            MemoryStream memoryStream = new MemoryStream();
            BinaryWriter binaryWriter = new BinaryWriter(memoryStream);

            // Conversion extracted from the sample voice_synthesis.cs (RSSDK\framework\CSharp\voice_synthesis.cs)
            binaryWriter.Write((int)0x46464952);  // chunkIdRiff:'FFIR'
            binaryWriter.Write((int)0);           // chunkDataSizeRiff
            binaryWriter.Write((int)0x45564157);  // riffType:'EVAW'
            binaryWriter.Write((int)0x20746d66);  // chunkIdFmt:' tmf'
            binaryWriter.Write((int)0x12);        // chunkDataSizeFmt
            binaryWriter.Write((short)1);         // compressionCode
            binaryWriter.Write((short)audioInfo.nchannels);  // numberOfChannels
            binaryWriter.Write((int)audioInfo.sampleRate);   // sampleRate
            binaryWriter.Write((int)(audioInfo.sampleRate * 2 * audioInfo.nchannels)); // averageBytesPerSecond
            binaryWriter.Write((short)(audioInfo.nchannels * 2));   // blockAlign
            binaryWriter.Write((short)16);        // significantBitsPerSample
            binaryWriter.Write((short)0);         // extraFormatSize
            binaryWriter.Write((int)0x61746164);  // chunkIdData:'atad'
            binaryWriter.Write((int)0);           // chunkIdSizeData

            // Writing the audio data
            binaryWriter.Write(audioData.ToByteArray());

            long pos = binaryWriter.Seek(0, SeekOrigin.Current);
            binaryWriter.Seek(0x2a, SeekOrigin.Begin); // chunkDataSizeData
            binaryWriter.Write((int)(pos - 46));
            binaryWriter.Seek(0x04, SeekOrigin.Begin); // chunkDataSizeRiff
            binaryWriter.Write((int)(pos - 8));

            binaryWriter.Seek(0, SeekOrigin.Begin);

            // Playing the sound
            SoundPlayer soundPlayer = new SoundPlayer(memoryStream);
            soundPlayer.PlaySync();
            soundPlayer.Dispose();

            binaryWriter.Close();
            memoryStream.Close();
        }

    }
}
