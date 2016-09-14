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
package com.intel.realsensesamples.javafx;

import intel.rssdk.PXCMCapture;
import intel.rssdk.PXCMImage;
import intel.rssdk.PXCMSenseManager;
import intel.rssdk.PXCMSession;
import intel.rssdk.pxcmStatus;
import java.awt.image.BufferedImage;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 * @author fpedroso
 */
public class StreamsFX extends Application {

    // Stream consts
    private final PXCMCapture.StreamType streamType = PXCMCapture.StreamType.STREAM_TYPE_DEPTH;
    public final int DEFAULT_RESOLUTION_WIDTH = 640;
    public final int DEFAULT_RESOLUTION_HEIGHT = 480;
    public final int DEFAULT_FPS = 30;
    
    // RealSense Manager
    private RealSenseManager realSenseManager;
    
    // Image View to show stream image
    private ImageView imageView;

    @Override
    public void start(Stage primaryStage) {

        // Add image to the scene
        StackPane root = new StackPane();
        imageView = new ImageView();
        root.getChildren().add(imageView);
        
        // Create scene based in stream consts
        Scene scene = new Scene(root, DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT);
        
        // Set Scene to the Stage
        primaryStage.setTitle("RealSense + JavaFX Streams Sample");
        primaryStage.setScene(scene);
        
        // Action to stop realSenseManager when user closes window
        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent event) {
                try {
                    realSenseManager.stop();
                    realSenseManager = null;
                } catch (InterruptedException ex) {
                    System.out.println("Failed to stop the RealSense Thread");
                    System.exit(3);
                }
            }
        });
        
        // Show window
        primaryStage.show();

        // Instantiate manager and start
        realSenseManager = new RealSenseManager();
        realSenseManager.start();
    }
    
    /**
     * Update image
     * @param image 
     */
    private void updateImagePreview(WritableImage image) {
        if (image != null && imageView != null) {
            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    imageView.setImage(image);
                }
            });
        }
    }
    
    /**
     * Manages RealSense objects and aquire frames in a loop
     */
    private class RealSenseManager implements Runnable {
        
        private boolean canRun;
        
        // Manager's Thread
        private Thread realSenseThread;
        
        // RealSense components
        private PXCMSession session;
        private PXCMSenseManager senseManager;
        
        /**
         * Starts RealSenseManager
         */
        public void start() {
            realSenseThread = new Thread(this);
            realSenseThread.start();
        }
        
        /**
         * Stops RealSenseManager
         * @throws InterruptedException 
         */
        public void stop() throws InterruptedException {
            if (!canRun) {
                return;
            }

            canRun = false;
            realSenseThread.join();
        }
        
        /**
         * Init RealSenseManager
         * @return 
         */
        public boolean initialize() {
            if (canRun) {
                return false;
            }

            // Creating the session
            session = PXCMSession.CreateInstance();
            if (session == null) {
                System.out.println("Failed to create a RealSense SDK session.");
                return false;
            }

            // Creating the session manager
            senseManager = session.CreateSenseManager();
            if (senseManager == null) {
                System.out.println("Failed to create a SenseManager instance.");
                return false;
            }

            pxcmStatus sts = senseManager.EnableStream(streamType, DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, DEFAULT_FPS);

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                System.out.println(String.format("Failed to enable the %s stream.", streamType.toString()));
                return false;
            }

            // Initializing the SenseManager
            sts = senseManager.Init();

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                System.out.println("Failed to initialize the SenseManager.");
                return false;
            }

            return true;
        }
        
        /**
         * Dispose Face Module objects and RealSense components
         */
        private void dispose() {
            // Disposing the SenseManager instance and the Session instance
            senseManager.Close();
            session.close();
        }
        
        /**
         * Run RealSense Loop
         */
        @Override
        public void run() {
            canRun = initialize();

            while (canRun) {

                if (senseManager.AcquireFrame(true).ordinal() > pxcmStatus.PXCM_STATUS_NO_ERROR.ordinal()) {
                    break;
                }

                // Querying the images samples
                PXCMCapture.Sample sample = senseManager.QuerySample();
                if (sample != null) {
                    PXCMImage image;

                    switch (streamType) {
                        case STREAM_TYPE_DEPTH:
                            image = sample.depth;
                            break;
                        case STREAM_TYPE_IR:
                            image = sample.ir;
                            break;
                        case STREAM_TYPE_LEFT:
                            image = sample.left;
                            break;
                        case STREAM_TYPE_RIGHT:
                            image = sample.right;
                            break;
                        case STREAM_TYPE_COLOR:
                        default:
                            image = sample.color;
                            break;
                    }

                    PXCMImage.ImageInfo imageInfo = image.QueryInfo();

                    if (imageInfo == null) {
                        System.out.println("Failed to get the image info.");
                        break;
                    }

                    int imageWidth = imageInfo.width;
                    int imageHeight = imageInfo.height;

                    // Acquiring access to the image data
                    PXCMImage.ImageData imageData = new PXCMImage.ImageData();
                    pxcmStatus acquireAccessStatus = image.AcquireAccess(PXCMImage.Access.ACCESS_READ, PXCMImage.PixelFormat.PIXEL_FORMAT_RGB32, imageData);
                    
                    if (acquireAccessStatus.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                        System.out.println("Failed to acquire access to the image. " + acquireAccessStatus);
                        continue;
                    }

                    int[] buffer = new int[imageData.pitches[0] / 4 * imageHeight];

                    imageData.ToIntArray(0, buffer);
                    BufferedImage bufferedImage = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_INT_RGB);
                    bufferedImage.setRGB(0, 0, imageWidth, imageHeight, buffer, 0, imageData.pitches[0] / 4);

                    updateImagePreview(SwingFXUtils.toFXImage(bufferedImage, null));

                    image.ReleaseAccess(imageData);
                }
                
                // Releasing the acquired frame
                senseManager.ReleaseFrame();
            }
              
            dispose();
        }
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }

}
