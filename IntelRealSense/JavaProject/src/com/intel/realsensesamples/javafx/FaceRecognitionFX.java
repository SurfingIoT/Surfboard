/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
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
 */
public class FaceRecognitionFX extends Application {
    
    private FaceRecognitionFX.RealSenseManager realSenseManager;
    private Scene scene;
    
    private final Random random = new Random();
    
    private Cube leftEye;
    private Cube rightEye;
    private final float MOVEMENT_FACTOR = 5.0f;
    private final Color CUBE_DEFAULT_COLOR = Color.RED;
    
    @Override
    public void start(Stage primaryStage) {
        PerspectiveCamera camera = new PerspectiveCamera(true);
        camera.getTransforms().add(new Translate(0, 0, -10));

        Group root = new Group();
        SubScene subScene = new SubScene(root, 640, 480, true, SceneAntialiasing.BALANCED);
        subScene.setCamera(camera);

        scene = new Scene(new Group(subScene));

        primaryStage.setTitle("RealSense + JavaFX Face Recognition Sample");
        primaryStage.setScene(scene);

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

        primaryStage.show();

        realSenseManager = new FaceRecognitionFX.RealSenseManager();
        realSenseManager.start();
    }
    
    // FIXME - Reuse RealSenseManager (HandsFX+FaceFX) (?)
    private class RealSenseManager implements Runnable {

        private boolean canRun;
        private Thread realSenseThread;

        private PXCMSession session;
        private PXCMSenseManager senseManager;
        private PXCMFaceModule faceModule;
        private PXCMFaceConfiguration faceConfiguration;
        private PXCMFaceData faceData;

        public void start() {
            realSenseThread = new Thread(this);
            realSenseThread.start();
        }

        public void stop() throws InterruptedException {
            if (!canRun) {
                return;
            }

            canRun = false;
            realSenseThread.join();
        }

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
                
                // Recognition Configuration
                PXCMFaceConfiguration.RecognitionConfiguration recognitionConfiguration = faceConfiguration.QueryRecognition();
                
                // DOESNT WORK! NullPointerException
                recognitionConfiguration.Enable();
                
                // Create a recognition database
                PXCMFaceConfiguration.RecognitionConfiguration.RecognitionStorageDesc desc=new PXCMFaceConfiguration.RecognitionConfiguration.RecognitionStorageDesc();
                desc.maxUsers=10;
                recognitionConfiguration.CreateStorage("MyDB", desc);
                recognitionConfiguration.UseStorage("MyDB");
                recognitionConfiguration.SetRegistrationMode(PXCMFaceConfiguration.RecognitionConfiguration.RecognitionRegistrationMode.REGISTRATION_MODE_CONTINUOUS);
                
                // Applying changes and updating settings
                faceConfiguration.ApplyChanges();
                faceConfiguration.Update();

                // Creating the face module output
                faceData = faceModule.CreateOutput();
            }

            return true;
        }

        private void processFaceData(PXCMFaceData faceData) {
            // Updating the face module output
            faceData.Update();
            
            // Read data
            for (int fidx=0; ; fidx++) {
                PXCMFaceData.Face face = faceData.QueryFaceByIndex(fidx);
                if (face==null) break;
                PXCMFaceData.DetectionData detectData = face.QueryDetection(); 
                if (detectData == null) { break; }
                
                // fdata is a PXCMFaceData instance
                int nfaces= faceData.QueryNumberOfDetectedFaces();
                for (int i=0;i<nfaces;i++) {
                    // Retrieve the recognition data instance
                    PXCMFaceData.Face face2=faceData.QueryFaceByIndex(i);
                    PXCMFaceData.RecognitionData rdata = face2.QueryRecognition();
 
                    // recognize the current face?
                    int uid=rdata.QueryUserID();
                    System.out.println(uid);
                    if (uid>=0) {
                        // do something with the recognized user.
                        System.out.println("User Recognized");
                    } else {
                        System.out.println("Not Recognized");
                    }
                }
            }
        }

        private void dispose() {
            // Disposing the elements related to the face tracking
            faceData.close();
            faceConfiguration.close();
            faceModule.close();

            // Disposing the SenseManager instance and the Session instance
            senseManager.Close();
            session.close();
        }

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
