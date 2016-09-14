/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.intel.realsensesamples.console;

import intel.rssdk.PXCMAudioSource;
import intel.rssdk.PXCMSession;
import intel.rssdk.PXCMSpeechRecognition;
import intel.rssdk.pxcmStatus;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * How to configure RealSense Java environment:
 * https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/devguide_java_application.html
 *
 * @author fpedroso
 */
public class SpeechRecognitionSample {

    public enum RecognitionType {
        DICTATION, COMMAND
    };

    public static final RecognitionType DemoRecognitionType = RecognitionType.DICTATION;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Creating a session.
        PXCMSession session = PXCMSession.CreateInstance();

        // Querying the SDK version
        PXCMSession.ImplVersion sdkVersion = session.QueryVersion();
        System.out.println(String.format("RealSense SDK Version %d.%d", sdkVersion.major, sdkVersion.minor));

        // General information about Speech Recognition.
        // Creating an AudioSource
        PXCMAudioSource audioSource = session.CreateAudioSource();

        // Listing the audio devices
        audioSource.ScanDevices();
        int devicesCount = audioSource.QueryDeviceNum();

        if (devicesCount <= 0) {
            System.out.println("There is no audio device available.");
            return;
        }

        System.out.println("Listing audio devices:");

        for (int i = 0; i < devicesCount; i++) {
            PXCMAudioSource.DeviceInfo deviceInfo = new PXCMAudioSource.DeviceInfo();
            audioSource.QueryDeviceInfo(i, deviceInfo);
            System.out.println(String.format("\t%s: %s", deviceInfo.did, deviceInfo.name));
        }

        // Selecting the first device
        PXCMAudioSource.DeviceInfo deviceInfo = new PXCMAudioSource.DeviceInfo();
        audioSource.QueryDeviceInfo(0, deviceInfo);
        audioSource.SetDevice(deviceInfo);

        // Adjusting the audio recording volume. 
        audioSource.SetVolume(0.2f);

        // Creating the SpeechRecognition instance  
        PXCMSpeechRecognition speechRecognition = new PXCMSpeechRecognition();
        pxcmStatus speechRecognitionStatus = session.CreateImpl(speechRecognition);

        if (speechRecognitionStatus != pxcmStatus.PXCM_STATUS_NO_ERROR) {
            System.out.println("Failed to create the SpeechRecognition instance");
            return;
        }

        // Configuring the speech module
        List<PXCMSpeechRecognition.ProfileInfo> availableProfiles = new ArrayList<PXCMSpeechRecognition.ProfileInfo>();
        System.out.println("Listing the available profiles:");
        for (int i = 0;; i++) {
            PXCMSpeechRecognition.ProfileInfo profileInfo = new PXCMSpeechRecognition.ProfileInfo();
            pxcmStatus profileQueryStatus = speechRecognition.QueryProfile(i, profileInfo);

            if (profileQueryStatus != pxcmStatus.PXCM_STATUS_NO_ERROR) {
                break;
            }
            System.out.println(String.format("\t%s", profileInfo.language));
            availableProfiles.add(profileInfo);
        }

        // Setting the first profile 
        speechRecognition.SetProfile(availableProfiles.get(0));

        if (DemoRecognitionType == RecognitionType.DICTATION) // To change the recognition mode, please change the value of the DemoRecognitionType constant at the beginning of this file.
        {
            //  Seting the dictation mode
            pxcmStatus setDictationStatus = speechRecognition.SetDictation();

            if (setDictationStatus != pxcmStatus.PXCM_STATUS_NO_ERROR) {
                System.out.println("Failed to create the SpeechRecognition instance");
                return;
            }
        } else {
            // Building a list of commands
            String[] commandsGrammar1 = {"One", "Two", "Three"}; // English
            speechRecognition.BuildGrammarFromStringList(1, commandsGrammar1, null, commandsGrammar1.length);

            String[] commandsGrammar2 = {"Um", "Dois", "Três"};
            speechRecognition.BuildGrammarFromStringList(2, commandsGrammar2, null, commandsGrammar2.length);

            // Selecting the grammar
            speechRecognition.SetGrammar(1);
        }

        // Handling recognition events
        PXCMSpeechRecognition.Handler handler = new PXCMSpeechRecognition.Handler() {
            @Override
            public void OnAlert(PXCMSpeechRecognition.AlertData alertData) {
                System.out.println(alertData.label);
            }

            @Override
            public void OnRecognition(PXCMSpeechRecognition.RecognitionData recognitionData) {
                switch (DemoRecognitionType) { // To change the recognition mode, please change the value of the DemoRecognitionType constant at the beginning of this file.
                    case DICTATION:
                        processDictation(recognitionData);
                        break;
                    case COMMAND:
                    default:
                        processCommands(recognitionData);
                        break;
                }
            }
            
            public void OnRecognized(PXCMSpeechRecognition.RecognitionData recognitionData) {
                OnRecognition(recognitionData);
            }
        };

        speechRecognition.StartRec(audioSource, handler);

        // Waiting a key to stop voice recognition
        //Console.ReadKey();
        speechRecognition.StopRec();
    }

    private static void processCommands(PXCMSpeechRecognition.RecognitionData data) {
//        for (int i = 0; i < PXCMSpeechRecognition.NBEST_SIZE; i++) {
                // Scores esta privada - testa recompilando
//            int label = data.scores[i].label;
//            int confidence = data.scores[i].confidence;
//            if (label < 0 || confidence < 48) {
//                continue;
//            }
//
//            System.out.println(data.scores[i].sentence);
//            if (data.scores[i].tags.Length > 0) {
//                System.out.println(data.scores[i].tags);
//            }
//        }
    }

    private static void processDictation(PXCMSpeechRecognition.RecognitionData data) {
//        for (PXCMSpeechRecogni`tion.NBest score : data.scores) {
//            System.out.println("{0}", score.sentence);
//        }
    }

}
