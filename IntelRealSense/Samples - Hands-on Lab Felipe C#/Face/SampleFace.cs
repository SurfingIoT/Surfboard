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

namespace Face
{
    /// <summary>
    /// This samples shows how to track Face/Head position, landmarks, pose and expressions using RealSense SDK.
    /// </summary>
    class SampleFace
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

            // Enabling Detection
            faceConfiguration.detection.maxTrackedFaces = 4;
            faceConfiguration.detection.isEnabled = true;

            // Enabling Landmarks
            faceConfiguration.landmarks.maxTrackedFaces = 4;
            faceConfiguration.landmarks.isEnabled = true;

            // Enabling Poses
            faceConfiguration.pose.maxTrackedFaces = 4;
            faceConfiguration.pose.isEnabled = true;

            // Enabling Expressions
            PXCMFaceConfiguration.ExpressionsConfiguration expressionsConfiguration = faceConfiguration.QueryExpressions();
            if (expressionsConfiguration == null)
            {
                Console.WriteLine("Failed to create the expressions configuration.");
                return;
            }
            expressionsConfiguration.properties.maxTrackedFaces = 4;
            expressionsConfiguration.EnableAllExpressions();
            expressionsConfiguration.Enable();

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

                    // Processing Face Position
                    ProcessPosition(faceObject);

                    // Processing Face Landmarks
                    ProcessLandmarks(faceObject);

                    // Processing Face Pose
                    ProcessPose(faceObject);

                    // Processing Face Expressions
                    ProcessExpressions(faceObject);

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

        static void ProcessPosition(PXCMFaceData.Face faceObject)
        {
            // Querying Face Position
            PXCMFaceData.DetectionData faceDetectionData = faceObject.QueryDetection();
            PXCMRectI32 rectangle;
            faceDetectionData.QueryBoundingRect(out rectangle);
            Console.WriteLine("Face position on image: {0} | {1} | {2} | {3}", rectangle.x, rectangle.y, rectangle.w, rectangle.h);
        }

        static void ProcessLandmarks(PXCMFaceData.Face faceObject)
        {
            // Retrieve the face landmark data instance
            PXCMFaceData.LandmarksData faceLandmarksData = faceObject.QueryLandmarks();

            if (faceLandmarksData != null)
            {
                // Get the landmark data
                PXCMFaceData.LandmarkPoint[] points;
                faceLandmarksData.QueryPoints(out points);

                // Print all landmark points
                foreach (PXCMFaceData.LandmarkPoint point in points)
                {
                    Console.WriteLine("Face landmark {0} on image: {1} | {2} | {3} ", point.source.index, point.world.x, point.world.y, point.world.z);
                }
            }
        }

        static void ProcessPose(PXCMFaceData.Face faceObject)
        {
            // Retrieve the face landmark data instance
            PXCMFaceData.PoseData facePoseData = faceObject.QueryPose();

            if (facePoseData != null)
            {
                // Get head position
                PXCMFaceData.HeadPosition headPosition;
                facePoseData.QueryHeadPosition(out headPosition);

                // Get euler angles
                PXCMFaceData.PoseEulerAngles angles;
                facePoseData.QueryPoseAngles(out angles);

                Console.WriteLine("Face Pose HeadPosition -> {0} | {1} | {2}", headPosition.headCenter.x, headPosition.headCenter.y, headPosition.headCenter.z);
                Console.WriteLine("Face Pose Angles -> Yaw: {0} | Pitch: {1} | Roll {2} ", angles.yaw, angles.pitch, angles.roll);
            }
        }

        static void ProcessExpressions(PXCMFaceData.Face faceObject)
        {
            PXCMFaceData.ExpressionsData expressionsData = faceObject.QueryExpressions();

            foreach (PXCMFaceData.ExpressionsData.FaceExpression expression in Enum.GetValues(typeof(PXCMFaceData.ExpressionsData.FaceExpression)))
            {
                PXCMFaceData.ExpressionsData.FaceExpressionResult expressionResult;
                expressionsData.QueryExpression(expression, out expressionResult);
                Console.WriteLine("{0} - intensity: {1}", expression, expressionResult.intensity);
            }
        }
    }
}
