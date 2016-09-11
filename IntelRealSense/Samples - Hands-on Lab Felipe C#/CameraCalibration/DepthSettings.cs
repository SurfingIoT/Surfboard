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

namespace CameraCalibration
{
    /// <summary>
    /// This sample shows how configure a RealSense camera.
    /// </summary>
    public enum Modality
    {
        FACTORY_DEFAULT,
        FACE_TRACKING,
        HAND_TRACKING,
        USER_SEGMENTATION,
        OBJECT_TRACKING,
        TOUCHLESS_CONTROLLER
    };

    public class DepthSettings
    {
        // Depth Settings for different modalities
        public static void SetDeviceDepthSetting(PXCMCapture.Device device, Modality modality) 
        {
            // Map modality
            switch (modality)
            {
                case Modality.FACE_TRACKING:
                    SetDepthSettingValues(device, 1, 6, 16, 21, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_MEDIAN);
                    break;
                case Modality.HAND_TRACKING:
                    SetDepthSettingValues(device, 1, 6, 16, 0, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_MEDIAN);
                    break;
                case Modality.USER_SEGMENTATION:
                    SetDepthSettingValues(device, 0, 6, 16, 21, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_COARSE);
                    break;
                case Modality.OBJECT_TRACKING:
                    SetDepthSettingValues(device, 6, 5, 0, 16, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_MEDIAN);
                    break;
                case Modality.TOUCHLESS_CONTROLLER:
                    SetDepthSettingValues(device, 1, 6, 16, 0, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_MEDIAN);
                    break;
                case Modality.FACTORY_DEFAULT:
                default:
                    SetDepthSettingValues(device, 6, 5, 16, 0, PXCMCapture.Device.IVCAMAccuracy.IVCAM_ACCURACY_MEDIAN);
                    break;
            }
        }

        private static void SetDepthSettingValues(PXCMCapture.Device device, 
            ushort confidenceThreshold, 
            ushort filterOption, 
            ushort laserPower, 
            ushort motionRangeTradeOff, 
            PXCMCapture.Device.IVCAMAccuracy accuracy)
        {
            // Setting Confidence Threshold
            device.SetDepthConfidenceThreshold(confidenceThreshold);

            // Setting Filter Option
            device.SetIVCAMFilterOption(filterOption);

            // Setting Laser Power
            device.SetIVCAMLaserPower(laserPower);

            // Setting Motion Range Trade off
            device.SetIVCAMMotionRangeTradeOff(motionRangeTradeOff);

            // Setting Accuracy
            device.SetIVCAMAccuracy(accuracy);

            Console.WriteLine("Depth Setting - OK");
        }
    }
}
