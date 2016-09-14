/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.intel.realsensesamples.console;

import intel.rssdk.PXCMAudio;
import intel.rssdk.PXCMSession;
import intel.rssdk.PXCMSpeechSynthesis;
import intel.rssdk.pxcmStatus;
import java.io.Console;
import static java.lang.System.out;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * How to configure RealSense Java environment:
 * https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/devguide_java_application.html
 *
 * @author fpedroso
 */
public class SpeechSysthesisSample {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Creating a session.
        PXCMSession session = PXCMSession.CreateInstance();

        // Querying the SDK version
        PXCMSession.ImplVersion sdkVersion = session.QueryVersion();
        System.out.println(String.format("RealSense SDK Version %d.%d", sdkVersion.major, sdkVersion.minor));
        
        // Creating the SpeechSynthesis instance
        PXCMSpeechSynthesis speechSynthesis = new PXCMSpeechSynthesis();

        pxcmStatus speechSynthesisStatus = session.CreateImpl(speechSynthesis);

        if (speechSynthesisStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
        {
            System.out.println("Failed to initialize the speech synthesis");
            return;
        }

        // Listing the available profile
        List<PXCMSpeechSynthesis.ProfileInfo> availableProfiles = new ArrayList<PXCMSpeechSynthesis.ProfileInfo>();

        System.out.println("Listing available profiles: ");
        for (int  i= 0;  ; i++)
        {
            PXCMSpeechSynthesis.ProfileInfo profileInfo = new PXCMSpeechSynthesis.ProfileInfo();
            pxcmStatus queryProfileStatus = speechSynthesis.QueryProfile(i, profileInfo);

            if (queryProfileStatus !=  pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                break;
            }
            
            System.out.println("\t" + profileInfo.language);
            availableProfiles.add(profileInfo);
        }

        if (availableProfiles.size() <= 0)
        {
            System.out.println("There is no profile available.");
        }

        // Selecting the first profile available
        speechSynthesis.SetProfile(availableProfiles.get(0));

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
            PXCMAudio.AudioData audioData = new PXCMAudio.AudioData();
            pxcmStatus audioAcquireAccessStatus = audio.AcquireAccess(PXCMAudio.Access.ACCESS_READ, PXCMAudio.AudioFormat.AUDIO_FORMAT_PCM, audioData);
            
            if (audioAcquireAccessStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                System.out.println("Failed to acquire access to AudioData");
            }
           
            // Converting to WAV and Playing the audio. 
            PlayPcmSound(audioData, availableProfiles.get(0).outputs);

            // Releasing the access to the audio
            audio.ReleaseAccess(audioData);
        }
    }

    private static void PlayPcmSound(PXCMAudio.AudioData audioData, PXCMAudio.AudioInfo audioInfo) {
        // TODO - Bytes to Audio
    }
    
}
