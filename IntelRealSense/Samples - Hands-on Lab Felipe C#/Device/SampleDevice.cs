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
﻿using CameraCalibration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Device
{
    class SampleDevice
    {
        /// <summary>
        /// This sample shows how to list Devices, configure and calibrate to use RealSense.
        /// </summary>
        static void Main(string[] args)
        {
            // Creating a SDK session
            PXCMSession session = PXCMSession.CreateInstance();

            // Querying the SDK version
            PXCMSession.ImplVersion version = session.QueryVersion();
            Console.WriteLine("RealSense SDK Version {0}.{1}", version.major, version.minor);

            // Implementation description module
            PXCMSession.ImplDesc moduleDescription = new PXCMSession.ImplDesc();
            moduleDescription.group = PXCMSession.ImplGroup.IMPL_GROUP_SENSOR;
            moduleDescription.subgroup = PXCMSession.ImplSubgroup.IMPL_SUBGROUP_VIDEO_CAPTURE;

            for (int m = 0; ; m++)
            {
                // Filter implementation description
                PXCMSession.ImplDesc filteredModuleDescription;
                if (session.QueryImpl(moduleDescription, m, out filteredModuleDescription) < pxcmStatus.PXCM_STATUS_NO_ERROR) break;
                Console.WriteLine("Module[{0}]: {1}", m, filteredModuleDescription.friendlyName);

                // Create Capture implementation
                PXCMCapture capture;
                session.CreateImpl<PXCMCapture>(filteredModuleDescription, out capture);

                // Query All Video Devices
                for (int d = 0; ; d++)
                {
                    // Create device interface instance
                    PXCMCapture.Device device = capture.CreateDevice(d);

                    // Query Device Information
                    PXCMCapture.DeviceInfo deviceInfo;
                    if (capture.QueryDeviceInfo(d, out deviceInfo) < pxcmStatus.PXCM_STATUS_NO_ERROR) break;

                    // Enumerating device streams
                    printDeviceStreams(deviceInfo, d);

                    // Printing device streams configurations
                    printStreamsConfigurations(device);

                    // Setting device properties
                    DepthSettings.SetDeviceDepthSetting(device, Modality.FACTORY_DEFAULT);

                    // Creating the SenseManager
                    PXCMSenseManager senseManager = session.CreateSenseManager();

                    // Setting device
                    senseManager.captureManager.FilterByDeviceInfo(deviceInfo);
                }

                // Releasing resources
                capture.Dispose();
            }

            // Pause Console
            Console.ReadKey();
        }

        static void printDeviceStreams(PXCMCapture.DeviceInfo deviceInfo, int deviceIndex)
        {
            // Print Device name
            Console.WriteLine("\tDevice[{0}]: {1}", deviceIndex, deviceInfo.name);

            // Check if device supports Color Stream
            if ((deviceInfo.streams & PXCMCapture.StreamType.STREAM_TYPE_COLOR) != 0)
            {
                Console.WriteLine("\t\tCOLOR STREAM");
            }

            // Check if device supports Depth Stream
            if ((deviceInfo.streams & PXCMCapture.StreamType.STREAM_TYPE_DEPTH) != 0)
            {
                Console.WriteLine("\t\tDEPTH STREAM");
            }
        }

        static void printStreamsConfigurations(PXCMCapture.Device device) 
        {
            // Device Stream Configurations
            PXCMCapture.StreamType streams = PXCMCapture.StreamType.STREAM_TYPE_COLOR | PXCMCapture.StreamType.STREAM_TYPE_DEPTH;

            // Query All Stream Profiles
            for (int p = 0; ; p++)
            {
                // Query Stream Profile
                PXCMCapture.Device.StreamProfileSet profiles;
                pxcmStatus sts = device.QueryStreamProfileSet(streams, p, out profiles);
                if (sts < pxcmStatus.PXCM_STATUS_NO_ERROR) break;

                // Print info about stream
                Console.WriteLine("\t\tcolor[{0}]: {1}x{2}", p, profiles.color.imageInfo.width, profiles.color.imageInfo.height);
            }
        }
    }
}
