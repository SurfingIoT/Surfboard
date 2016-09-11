APPLICABLE LICENSES
Sample Code - See license.txt contained in this zip file
Intel RealSense SDK DLLS (libpxcclr.cs.dll, libpxccpp2c.dll) - Intel OBL RealSense Software Development Kit (SDK) Production License Agreement

DESCRIPTION
# Simple Intel® RealSense™ Code Samples

 This repository contains samples that were used in *RealSense™ HandsOn Lab Brazil*. The samples show how to use the RealSense™ SDK features in a simple and easy way, allowing developers to see how to use modules from the SDK and configure devices.

[RealSense™ Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/)

## Pre-Requisites to Run Samples

* RealSense™ SDK
* Intel® RealSense™ 3D camera
* Visual Studio 2010 or later
* 4th generation Intel® Core™ processors based on the Intel microarchitecture code name Haswell

## Important RealSense™ Documentation Links

* [SDK Architecture](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_architecture_and_programming.html)
* [Programming Guide](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_programming_guide.html)
* [Session Class](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_algorithms_and_i_o_modules.html)
* [SenseManager Class](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_streaming_through_pxcmsenseman.html)

## Samples

* [Camera Calibration](#camera-calibration)
* [Device](#device)
* [Emotion](#emotion)
* [Emotion with Callback](#emotion-with-callback)
* [Face](#face)
    * [Detection](#detection)
    * [Expressions](#expressions)
    * [Landmarks](#landmarks)
    * [Pose](#pose)
* [Face Recognition](#face-recognition)
* [Hands](#hands)
* [Object Tracking](#object-tracking)
* [Segmentation](#segmentation)
* [Speech Recognition](#speech-recognition)
* [Speech Synthesis](#speech-synthesis)
* [Streams](#streams)

### Camera Calibration

Library project that receives a device and a modality to set Depth Settings. This should be used to improve camera recognition quality when you use a specific module, for example Hand tracking.

* [Depth Settings for different modalities](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/devguide_working_with_multiple_modaliti.html)

### Device

Enumerate, select, get available streams and set configuration in a device. Note: this sample use Camera Calibration Library to configure device.

* [I/O Devices Operations](https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/manuals_i_o_devices_enumeration_and_se.html)
* [Enumerating Devices](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_enumerate_i_o_devices.html)
* [Enumerating Streams](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_enumerate_streams.html)
* [Enumerating Streams Configurations](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_enumerate_stream_configuration.html)

### Emotion

List emotions using SenseManager with a procedural implementation. This sample query all the emotion data and print each one with intensity. 

* [Emotion Detection](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_emotion_detection_preview.html)
* [Detection via SenseManager](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_emotion_detection_via_senseman.html)
* [Intensity and Evidence](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_emotion_data.html)
* [PXCMEmotion](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/pxcemotion.html)

### Emotion with Callback

*Emotion With Callback* has the same functionality of *Emotion* sample, but with a different implementation. *Emotion with Callback* shows how to use handlers in RealSense™ SDK to get modules data (in this case Emotion module), but you can use this implementation with others RealSense™ modules.

* [Emotion Detection with Callback](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_emotion_detection_via_senseman.html)

### Face

Face is a sample that implements differents funcionalities of Face module using RealSense™ SDK. Gets a PXCMFaceData object and process information separately in the functionalities below.

* [Face Tracking via SenseManager](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_general_procedure_face.html)
* [Configuration and Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_general_procedure_2_2.html)

#### Detection

Prints X, Y, width and height from a detected face. 

* [Location Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_face_location_data.html)

#### Expressions

Prints all detected expressions (one-by-one) with intensity from a detected face.

* [Expressions Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_facial_expression_data.html)
* [FaceExpression Enumerator](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?faceexpression_expressionsdata_pxcfacedata.html)

#### Landmarks

Prints all (max 78) landmarks from a detected face.

* [Landmark Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_face_landmark_data.html)

#### Pose

Prints X, Y, Z Euler Angles from a detected head.

* [Pose Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_face_pose_data.html)

### Face Recognition

Implements how to use face recognition functionality. Detects a face and check, in memory, if the user is registered. When the program detects a face not registered, the user can press Space Bar key to register. After registered, the sample prints the unique identifier from recognized face.

* [Face Recognition Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_face_recognition_data.html)

### Hands

Track hands, fingers and gestures. Prints how many hands are detected, hand positions (image and world), body side, joints and detected gestures.

* [Gesture Recognition Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_pose_and_gesture_recognition.html)
* [Gesture Interaction Guide](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_gesture_interaction_guide.html)
* [Hand Tracking via SenseManager](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_general_procedure.html)
* [Hand Tracking Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_geometric_nodes_and_hand_modeling.html)
* [Joint Type](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?jointtype_pxchanddata.html)
* [Joint Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?jointdata_pxchanddata.html)

### Object Tracking

Detects a 2D object using RealSense™ SDK. Uses a JPEG/PNG image to track X, Y, Z positions if object did detected by the RealSense™ camera. Note: To works, needs camera calibration with a specifically tag.

* [Object Tracking Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_object_tracking.html)
* [Object Tracking via SenseManager](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_object_tracking_via_sense_manager.html)
* [The Metaio Toolbox Calibration](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_the_metaio_toolbox.html)

### Segmentation

Stream demo that subtract background using 3d segmentation module from RealSense™ SDK. Shows a window with camera stream and update segmentation image frame-by-frame.

* [User Segmentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_user_segmentation.html)
* [Accessing Image and Audio Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_image_and_audio_data.html)
* [ImageData to Bitmap](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/tobitmap_imagedata_pxcmimage.html)

### Speech Recognition

Shows how to use two Speech Recognition modes: DICTATION or COMMAND. Dictation mode, recognizes all words that users are saying and prints on the screen. In Command mode, the program sets a grammar and, when the user says one of added commands, prints it on screen.  

* [Speech Recognition Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_speech_recognition_procedure.html)
* [Command Control and Dictation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_command_control_and_dictation.html)
* [Handling Recognition Events](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_handle_recognition_events.html)
* [RecognitionData Object](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?recognitiondata_pxcspeechrecognition.html)

### Speech Synthesis

Implementation of text-to-speech using RealSense™ SDK. Sets a sentence in the available profile, convert to audio and play. 

* [Speech Synthesis Documentation](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_speech_synthesis.html)
* [BuildSentence](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/buildsentence_pxcspeechsynthesis.html)
* [Acquiring Audio File](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_image_and_audio_data.html)

### Streams

Implementation of Stream module using RealSense™ SDK. Selects a type of stream and shows a window with the selected camera stream, updating image frame-by-frame in selected FPS configuration.

* [Enabling Streams](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/enablestreams_pxcsensemanager.html)
* [Capturing Individual Color or Depth Stream](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/manuals_capture_color_or_depth_samples.html)
* [Accessing Image and Audio Data](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/index.html?manuals_image_and_audio_data.html)
* [ImageData to Bitmap](https://software.intel.com/sites/landingpage/realsense/camera-sdk/2014gold/documentation/html/tobitmap_imagedata_pxcmimage.html)
