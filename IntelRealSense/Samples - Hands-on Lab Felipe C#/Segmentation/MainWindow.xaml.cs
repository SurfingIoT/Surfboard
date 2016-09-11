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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Segmentation
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        RealSenseSegmentationStream segmentationStream;

        public MainWindow()
        {
            InitializeComponent();

            // Initializing the segmentation stream
            segmentationStream = new RealSenseSegmentationStream(); 
            segmentationStream.InitializeStream();
            segmentationStream.StartStream();
            segmentationStream.NewImageAvailable += segmentationStream_NewImageAvailable;
        }

        void segmentationStream_NewImageAvailable(object sender, RealSenseSegmentationStream.NewImageArgs args)
        {
            Dispatcher.Invoke(new Action(() => { imageComponent.Source = ImageUtils.ConvertBitmapToWpf(args.Bitmap); }));
        }

      

        private void Window_Closed(object sender, EventArgs e)
        {
            // Avoiding problems with Dispatcher
            segmentationStream.NewImageAvailable -= segmentationStream_NewImageAvailable;
            segmentationStream.StopStream();
        }
    }
}
