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

namespace ObjectTracking
{
    /// <summary>
    /// This sample shows how to track 2d objects using RealSense SDK.
    /// </summary>
    class Sample2dObjectTracking
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

            // Enabling the object tracking module
            pxcmStatus enablingModuleStatus = senseManager.EnableTracker();

            if (enablingModuleStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the object tracking Module");
                return;
            }

            // Getting the instance of the Tracker Module
            PXCMTracker trackerModule = senseManager.QueryTracker();
            if (trackerModule == null)
            {
                Console.WriteLine("Failed to query the tracker module");
                return;
            }

            // Creating an active configuration
            int codIdentifier;

            // Get Project Path and Tag Path
            String projectPath = Environment.CurrentDirectory;
            String tagPath = projectPath.Substring(0, projectPath.LastIndexOf("bin")) + "tags\\tag1.jpg";

            Console.WriteLine(tagPath);
            
            // Add file to 2d tracking
            trackerModule.Set2DTrackFromFile(tagPath, out codIdentifier);

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

                if (trackerModule != null)
                {
                    // Get number of tracking values
                    int numTracked = trackerModule.QueryNumberTrackingValues();

                    // Query Tracking Values
                    PXCMTracker.TrackingValues[] trackArr = new PXCMTracker.TrackingValues[numTracked];
                    trackerModule.QueryAllTrackingValues(out trackArr);

                    for (int i = 0; i < numTracked; i++)
                    {
                        // Get CodID
                        Console.WriteLine("CodID {0}", trackArr[i].cosID);
                        if (trackArr[i].quality > 0)
                        {
                            // Get Translation - X, Y and Z 
                            float xPosition = trackArr[i].translation.x;
                            float yPosition = trackArr[i].translation.y;
                            float zPosition = trackArr[i].translation.z;
                            Console.WriteLine("Position x: ({0:#.##}) y: ({1:#.##}) z: ({2:#.##})", xPosition, yPosition, zPosition);
                        }
                    }
                }

                // Releasing the acquired frame
                senseManager.ReleaseFrame();
            }

            Console.Write("\nPress any key to continue...");
            Console.ReadKey();

            // Releasing resources
            trackerModule.Dispose();
            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();

        }
    }
}
