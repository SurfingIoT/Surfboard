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

import intel.rssdk.PXCMFaceConfiguration;
import intel.rssdk.PXCMFaceData;
import intel.rssdk.PXCMFaceModule;
import intel.rssdk.PXCMRectI32;
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
 *
 * @author jnardari
 * 
 * FaceFX Sample - Track face position and angles to move cube in screen
 */
public class FaceFX extends Application {
    
    // Manager instance
    private FaceFX.RealSenseManager realSenseManager;
    
    // Java FX Scene
    private Scene scene;
    
    // Object to generate random numbers
    private Random random = new Random();
    
    // Cube Instance
    private Cube box;
    
    // Movement consts
    private final float RECT_FACTOR = 300.0f;
    private final float POSITION_FACTOR = 500.0f;
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
        primaryStage.setTitle("RealSense + JavaFX Face Tracking Sample");
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
        realSenseManager = new FaceFX.RealSenseManager();
        realSenseManager.start();
    }

    /**
     * Manages RealSense objects and aquire frames in a loop
     */
    private class RealSenseManager implements Runnable {

        private boolean canRun;
        
        // Manager's Thread
        private Thread realSenseThread;
        
        // RealSense Components
        private PXCMSession session;
        private PXCMSenseManager senseManager;
        
        // Realsense Face Module components
        private PXCMFaceModule faceModule;
        private PXCMFaceConfiguration faceConfiguration;
        private PXCMFaceData faceData;
        
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
            pxcmStatus sts = senseManager.EnableFace(null);
            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                System.out.println("Failed to enable the Hand module.");
                return false;
            }

            // Initializing the SenseManager
            sts = senseManager.Init();

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) >= 0) {
                // Querying the FaceModule instance
                faceModule = senseManager.QueryFace();

                // Creating an active configuration
                faceConfiguration = faceModule.CreateActiveConfiguration();
                faceConfiguration.SetTrackingMode(PXCMFaceConfiguration.TrackingModeType.FACE_MODE_COLOR_PLUS_DEPTH);
                faceConfiguration.detection.isEnabled = true;
                faceConfiguration.landmarks.isEnabled = true;
                faceConfiguration.QueryExpressions().EnableAllExpressions();
                faceConfiguration.pose.isEnabled = true;

                // Applying changes and updating settings
                faceConfiguration.ApplyChanges();
                faceConfiguration.Update();

                // Creating the face module output
                faceData = faceModule.CreateOutput();
            }

            return true;
        }
        
        /**
         * Process Face data
         * @param faceData 
         */
        private void processFaceData(PXCMFaceData faceData) {
            // Updating the face module output
            faceData.Update();

            // Read data
            for (int fidx = 0;; fidx++) {
                PXCMFaceData.Face face = faceData.QueryFaceByIndex(fidx);
                if (face == null) {
                    break;
                }
                
                PXCMFaceData.DetectionData detectData = face.QueryDetection();
                if (detectData != null) {
                    PXCMRectI32 rect = new PXCMRectI32();
                    if (detectData.QueryBoundingRect(rect)) {
                        final double xTranslation = -((rect.x / 2) / POSITION_FACTOR) * MOVEMENT_FACTOR + 1; // Sum 1 to adjust position
                        final double yTranslation = ((rect.y / 2) / POSITION_FACTOR) * MOVEMENT_FACTOR;
                        final double zTranslation = -(rect.h / RECT_FACTOR) * MOVEMENT_FACTOR;

                        Platform.runLater(new Runnable() {
                            @Override
                            public void run() {
                                // Applying the txranslate transforms
                                box.setTranslateX(xTranslation);
                                box.setTranslateY(yTranslation);
                                box.setTranslateZ(zTranslation);
                            }
                        });
                        System.out.println(String.format("Detection Position %f x %f x %f", xTranslation, yTranslation, zTranslation));
                    }
                } else {
                    break;
                }

                processPose(face);
            }
        }
        
        /**
         * Process Pose
         * @param face 
         */
        private void processPose(PXCMFaceData.Face face) {
            PXCMFaceData.PoseData poseData = face.QueryPose();
            if (poseData != null) {
                // Pose Euler Angles
                final PXCMFaceData.PoseEulerAngles eulerAngles = new PXCMFaceData.PoseEulerAngles();
                if (poseData.QueryPoseAngles(eulerAngles)) {
                    System.out.println(String.format("Angles - ROW:%f PITCH:%f YAW:%f", eulerAngles.roll, eulerAngles.pitch, eulerAngles.yaw));

                    // Updating the JavaFX UI
                    Platform.runLater(new Runnable() {
                        @Override
                        public void run() {
                            // Applying the rotation transforms
                            box.setRotationX(eulerAngles.pitch);
                            box.setRotationZ(eulerAngles.roll);
                            box.setRotationY(eulerAngles.yaw);
                        }
                    });
                }
            }
        }
        
        /**
         * Dispose Face Module objects and RealSense components
         */
        private void dispose() {
            // Disposing the elements related to the face tracking
            faceData.close();
            faceConfiguration.close();
            faceModule.close();

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

                // Processing Face
                processFaceData(faceData);

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
