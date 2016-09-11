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

namespace Hands
{
    /// <summary>
    /// This samples shows how to track the hands, fingers and gestures using RealSense SDK.
    /// </summary>
    class SampleHands
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

            // Enabling the Hand module
            pxcmStatus enablingModuleStatus = senseManager.EnableHand("Hand Module");

            if (enablingModuleStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the Hand Module");
                return;
            }

            // Getting the instance of the Hand Module
            PXCMHandModule handModule = senseManager.QueryHand();

            if (handModule == null)
            {
                Console.WriteLine("Failed to get the HandModule object.");
                return;
            }

            // Creating an active configuration
            PXCMHandConfiguration handConfiguration = handModule.CreateActiveConfiguration();

            if (handConfiguration == null)
            {
                Console.WriteLine("Failed to create the HandConfiguration object.");
                return;
            }

            // Listing the available gestures            
            int supportedGesturesCount = handConfiguration.QueryGesturesTotalNumber();
            if (supportedGesturesCount > 0)
            {
                Console.WriteLine("Supported gestures:");
                for (int i = 0; i < supportedGesturesCount; i++)
                {
                    string gestureName = string.Empty;

                    if (handConfiguration.QueryGestureNameByIndex(i, out gestureName) == pxcmStatus.PXCM_STATUS_NO_ERROR)
                    {
                        Console.WriteLine("\t" + gestureName);
                    }
                }
            }

            // Enabling some gestures
            String[] enabledGestures = { "full_pinch", "tap", "spreadfingers" };

            foreach (String gesture in enabledGestures)
            {
                if (!handConfiguration.IsGestureEnabled(gesture))
                {
                    handConfiguration.EnableGesture(gesture);
                }
            }
            handConfiguration.ApplyChanges();

            // Creating a data output object
            PXCMHandData handData = handModule.CreateOutput();

            if (handData == null)
            {
                Console.WriteLine("Failed to create the HandData object.");
                return;
            }

            // Initializing the SenseManager
            if (senseManager.Init() != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the SenseManager");
                return;
            }

            // Looping to query the hands information
            while (true)
            {
                // Acquiring a frame
                if (senseManager.AcquireFrame(true) < pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }

                // Updating the hand data
                if (handData != null)
                {
                    handData.Update();
                }

                // Processing Hands
                ProcessHands(handData);
                // Processing gestures
                ProcessGestures(handData);

                // Releasing the acquired frame
                senseManager.ReleaseFrame();
            }

            // Releasing resources
            if (handData != null) handData.Dispose();
            if (handConfiguration != null) handConfiguration.Dispose();

            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();
        }

        private static void ProcessHands(PXCMHandData handData)
        {
            // Querying how many hands were detected
            int numberOfHands = handData.QueryNumberOfHands();
            Console.WriteLine("{0} hand(s) were detected.", numberOfHands);

            // Querying the information about detected hands
            for (int i = 0; i < numberOfHands; i++)
            {
                // Querying hand id
                int handId;
                pxcmStatus queryHandIdStatus = handData.QueryHandId(PXCMHandData.AccessOrderType.ACCESS_ORDER_NEAR_TO_FAR, i, out handId);
                if (queryHandIdStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    Console.WriteLine("Failed to query the hand Id.");
                    continue;
                }
                Console.WriteLine("Hand id: {0}", handId);

                // Querying the hand data
                PXCMHandData.IHand hand;
                pxcmStatus queryHandStatus = handData.QueryHandDataById(handId, out hand);

                if (queryHandStatus == pxcmStatus.PXCM_STATUS_NO_ERROR && hand != null)
                {

                    // Querying the body side (Left/Right)
                    PXCMHandData.BodySideType bodySide = hand.QueryBodySide();
                    Console.WriteLine("Body Side: {0}", bodySide);

                    // Querying the hand openness
                    int handOpenness = hand.QueryOpenness();
                    Console.WriteLine("Hand openness: {0}", handOpenness);

                    // Querying Hand 2D Position
                    PXCMPointF32 massCenterImage = hand.QueryMassCenterImage();
                    Console.WriteLine("Hand position on image: {0} | {1}", massCenterImage.x, massCenterImage.y);

                    // Querying Hand 3D Position
                    PXCMPoint3DF32 massCenterWorld = hand.QueryMassCenterWorld();
                    Console.WriteLine("Hand position on world: {0} | {1} | {2}", massCenterWorld.x, massCenterWorld.y, massCenterWorld.z);

                    // Querying Hand Joints
                    if (hand.HasTrackedJoints())
                    {
                        foreach (PXCMHandData.JointType jointType in Enum.GetValues(typeof(PXCMHandData.JointType)))
                        {
                            PXCMHandData.JointData jointData;
                            pxcmStatus queryStatus = hand.QueryTrackedJoint(jointType, out jointData);

                            if (queryStatus == pxcmStatus.PXCM_STATUS_NO_ERROR && jointData != null)
                            {
                                // Printing joint label and tracking confidence
                                Console.WriteLine("Joint {0} with confidence {1}", jointType, jointData.confidence);

                                // Printing the 2D position (image)
                                Console.WriteLine("\t2D Position: {0} | {1}", jointData.positionImage.x, jointData.positionImage.y);

                                // Printing the 3D position (depth)
                                Console.WriteLine("\t3D Position: {0} | {1} | {2}", jointData.positionWorld.x, jointData.positionWorld.y, jointData.positionWorld.z);
                            }
                        }
                    }

                }

                Console.WriteLine("----------");
            }
        }

        private static void ProcessGestures(PXCMHandData handData)
        {
            // Querying how many gestures were detected
            int firedGesturesNumber = handData.QueryFiredGesturesNumber();

            // Querying which gestures were detected
            for (int i = 0; i < firedGesturesNumber; i++)
            {
                PXCMHandData.GestureData gestureData;

                if (handData.QueryFiredGestureData(i, out gestureData) == pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    // Getting the gesture name
                    String gestureName = gestureData.name;

                    // Getting the gesture state
                    PXCMHandData.GestureStateType gestureState = gestureData.state;

                    // Getting the gesture timestamp
                    long timestamp = gestureData.timeStamp;

                    // Getting the hand information about which hand fired the gesture
                    PXCMHandData.IHand handInfo;
                    if (handData.QueryHandDataById(gestureData.handId, out handInfo) != pxcmStatus.PXCM_STATUS_NO_ERROR)
                    {
                        continue;
                    }

                    // Getting which hand fired the gesture
                    PXCMHandData.BodySideType bodySideHand = handInfo.QueryBodySide();

                    Console.WriteLine("Gesture \"{0}\" was detected. State:{1}, Timestamp:{2}, BodySide:{3}", gestureName, gestureState, timestamp, bodySideHand);
                    Console.WriteLine("--------------------");
                }
            }
        }
    }
}
