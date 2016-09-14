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

import intel.rssdk.PXCMHandConfiguration;
import intel.rssdk.PXCMHandData;
import intel.rssdk.PXCMHandModule;
import intel.rssdk.PXCMPoint3DF32;
import intel.rssdk.PXCMPoint4DF32;
import intel.rssdk.PXCMSenseManager;
import intel.rssdk.PXCMSession;
import intel.rssdk.pxcmStatus;
import java.util.Random;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.PerspectiveCamera;
import javafx.scene.Scene;
import javafx.scene.SceneAntialiasing;
import javafx.scene.SubScene;
import javafx.scene.paint.Color;
import javafx.scene.transform.Translate;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 * @author fpedroso
 */
public class HandsFX extends Application {
    
    // Manager Instance
    private RealSenseManager realSenseManager;
    
    // JavaFX Scene
    private Scene scene;
    
    // Object to generate random numbers
    private Random random = new Random();
    
    // Cube Instance
    private Cube box;
    
    // Movement const
    private final float MOVEMENT_FACTOR = 5.0f;
    
    // Cube Color
    private Color CUBE_DEFAULT_COLOR = Color.RED;

    @Override
    public void start(Stage primaryStage) {
        // Instantiate Cube
        box = new Cube(1, CUBE_DEFAULT_COLOR);
        
        // Add camera, box to the Java FX Scene
        PerspectiveCamera camera = new PerspectiveCamera(true);
        camera.getTransforms().add(new Translate(0, 0, -10));

        Group root = new Group();
        root.getChildren().add(box);

        SubScene subScene = new SubScene(root, 640, 480, true, SceneAntialiasing.BALANCED);
        subScene.setCamera(camera);

        scene = new Scene(new Group(subScene));
        
        // Set Scene to the stage
        primaryStage.setTitle("RealSense + JavaFX Hand Tracking Sample");
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
     * Update Cube by Hand Tracking position and angles
     * @param hand 
     */
    private void updateHandPosition(PXCMHandData.IHand hand) {

        //  Querying hands coordinates and bringing from meters to virtual world
        PXCMPoint3DF32 massCenterWorld = hand.QueryMassCenterWorld();
        final double xTranslation = -massCenterWorld.x * MOVEMENT_FACTOR;
        final double yTranslation = -massCenterWorld.y * MOVEMENT_FACTOR;
        final double zTranslation = -massCenterWorld.z * MOVEMENT_FACTOR;

        // Querying the hand openness and adjusting the brightness according it
        double openness = hand.QueryOpenness();
        double newBrighness = (CUBE_DEFAULT_COLOR.getBrightness() * (openness / 100));
        final Color boxColor = Color.hsb(CUBE_DEFAULT_COLOR.getHue(), CUBE_DEFAULT_COLOR.getSaturation(), newBrighness, CUBE_DEFAULT_COLOR.getOpacity());
        
        // Querying the palm orientation (hand rotation) and rotating the cube
        PXCMPoint4DF32 palmOrientation = hand.QueryPalmOrientation();

        // Converting the Quartenion to Euler angles
        double x = palmOrientation.x;
        double y = palmOrientation.y;
        double z = palmOrientation.z;
        double w = palmOrientation.w;
        final double roll = -Math.toDegrees(Math.atan(2 * (x * y + z * w) / (x * x - y * y - z * z + w * w)));
        final double pitch = -Math.toDegrees(-Math.asin(2 * (y * w - x * z)));
        final double yaw = Math.toDegrees(Math.atan(2 * (x * w + y * z) / (x * x + y * y - z * z - w * w)));
        
        // Updating the JavaFX UI
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                // Applying the translation transforms
                box.setTranslateX(xTranslation);
                box.setTranslateY(yTranslation);
                box.setTranslateZ(zTranslation);

                // Changing the color
                box.setColor(boxColor);

                // Applying the rotation transforms
                box.setRotationX(yaw);
                box.setRotationZ(roll);
                box.setRotationY(pitch);
            }
        });
    }
    
    /**
     * Process Hands Gestures
     * @param handData 
     */
    private void processGestures(PXCMHandData handData) {
        // Create Gesture Data Object
        PXCMHandData.GestureData gestureData = new PXCMHandData.GestureData();
        if (handData.IsGestureFired("tap", gestureData)) {
            
            // Random RGB
            float r = random.nextFloat();
            float g = random.nextFloat();
            float b = random.nextFloat();
            
            // Generate new random color and change default
            CUBE_DEFAULT_COLOR = Color.color(r, g, b);
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
        
        // Hand module components
        private PXCMHandModule handModule;
        private PXCMHandConfiguration handConfiguration;
        private PXCMHandData handData;
        
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

            // Enabling the hand module
            pxcmStatus sts = senseManager.EnableHand(null);
            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                System.out.println("Failed to enable the Hand module.");
                return false;
            }

            // Initializing the SenseManager
            sts = senseManager.Init();

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) >= 0) {
                // Querying the HandModule instance
                handModule = senseManager.QueryHand();

                // Creating an active configuration
                handConfiguration = handModule.CreateActiveConfiguration();

                // Enabling gestures
                handConfiguration.EnableAllGestures();

                // Enabling alerts
                handConfiguration.EnableAllAlerts();

                // Applying changes and updating settings
                handConfiguration.ApplyChanges();
                handConfiguration.Update();

                // Creating the hand module output
                handData = handModule.CreateOutput();
            }

            return true;
        }
        
        /**
         * Process Hand Data
         * @param handData 
         */
        private void processHandData(PXCMHandData handData) {
            // Updating the hand module output
            handData.Update();
            
            // Process Hands Gestures according the hand data
            processGestures(handData);
            
            // Querying the hand data
            PXCMHandData.IHand hand = new PXCMHandData.IHand();
            pxcmStatus sts = handData.QueryHandData(PXCMHandData.AccessOrderType.ACCESS_ORDER_NEAR_TO_FAR, 0, hand);

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) >= 0) {
                // Updating the hand position according the hand data
                updateHandPosition(hand);
            }
        }
        
        /**
         * Dispose Hands Module objects and RealSense components
         */
        private void dispose() {
            // Disposing the elements related to the hand tracking
            handData.close();
            handConfiguration.close();
            handModule.close();

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

                // Processing Hands
                processHandData(handData);

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
