/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.intel.realsensesamples.javafx;

import intel.rssdk.PXCMBase;
import intel.rssdk.PXCMCapture;
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
import static javafx.application.Application.launch;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.PerspectiveCamera;
import javafx.scene.Scene;
import javafx.scene.SceneAntialiasing;
import javafx.scene.SubScene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.transform.Translate;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 *
 * @author jnardari
 */
public class HandsFXCallbackBased extends Application {
    
    private RealSenseManager realSenseManager;
    private Scene scene;
    
    private Random random = new Random();
    
    private Cube box;
    private final float MOVEMENT_FACTOR = 5.0f;
    private Color CUBE_DEFAULT_COLOR = Color.RED;

    @Override
    public void start(Stage primaryStage) {
        box = new Cube(1, CUBE_DEFAULT_COLOR);

        PerspectiveCamera camera = new PerspectiveCamera(true);
        camera.getTransforms().add(new Translate(0, 0, -10));

        Group root = new Group();
        root.getChildren().add(box);

        SubScene subScene = new SubScene(root, 640, 480, true, SceneAntialiasing.BALANCED);
        subScene.setCamera(camera);

        scene = new Scene(new Group(subScene));

        primaryStage.setTitle("RealSense + JavaFX Hand Tracking Sample");
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

        realSenseManager = new RealSenseManager();
        realSenseManager.start();
    }

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
        double a = palmOrientation.x;
        double b = palmOrientation.y;
        double c = palmOrientation.z;
        double d = palmOrientation.w;
        final double roll = -Math.toDegrees(Math.atan(2 * (a * b + c * d) / (a * a - b * b - c * c + d * d)));
        final double pitch = -Math.toDegrees(-Math.asin(2 * (b * d - a * c)));
        final double yaw = Math.toDegrees(Math.atan(2 * (a * d + b * c) / (a * a + b * b - c * c - d * d)));
        
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
    
    private void processHandsGestures(PXCMHandData handData) {
        PXCMHandData.GestureData gestureData = new PXCMHandData.GestureData();
        if (handData.IsGestureFired("tap", gestureData)) {
            float r = random.nextFloat();
            float g = random.nextFloat();
            float b = random.nextFloat();
            CUBE_DEFAULT_COLOR = Color.color(r, g, b);
        }
    }

    private class RealSenseManager implements Runnable {

        private boolean canRun;
        private Thread realSenseThread;

        private PXCMSession session;
        private PXCMSenseManager senseManager;
        private PXCMHandModule handModule;
        private PXCMHandConfiguration handConfiguration;
        private PXCMHandData handData;
        
        
        class MyHandler implements PXCMSenseManager.Handler {
            
            PXCMHandData handData;
            
            @Override
            public pxcmStatus OnModuleProcessedFrame(int i, PXCMBase pxcmb, PXCMCapture.Sample sample) {
                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            }

            @Override
            public void OnStatus(int i, pxcmStatus p) {
                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            }

            @Override
            public pxcmStatus OnNewSample(int i, PXCMCapture.Sample sample) {
                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            }

            @Override
            public pxcmStatus OnConnect(PXCMCapture.Device device, boolean bln) {
                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            }

            @Override
            public pxcmStatus OnModuleSetProfile(int i, PXCMBase pxcmb) {
                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            }
            
            public MyHandler(PXCMHandData handData) {
                this.handData = handData;
            }
        }
        
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
            pxcmStatus sts = senseManager.EnableHand(null);
            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) < 0) {
                System.out.println("Failed to enable the Hand module.");
                return false;
            }
            
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
            
            // Initialize and stream data
            MyHandler handler=new MyHandler(handData);  // Instantiate the handler object
            
            // Initializing the SenseManager
            senseManager.Init(handler);
            senseManager.StreamFrames(true);
            
            return true;
        }

        private void processHandData(PXCMHandData handData) {
            // Updating the hand module output
            handData.Update();
            
            processHandsGestures(handData);
            
            // Querying the hand data
            PXCMHandData.IHand hand = new PXCMHandData.IHand();
            pxcmStatus sts = handData.QueryHandData(PXCMHandData.AccessOrderType.ACCESS_ORDER_NEAR_TO_FAR, 0, hand);

            if (sts.compareTo(pxcmStatus.PXCM_STATUS_NO_ERROR) >= 0) {
                // Updating the hand position according the hand data
                updateHandPosition(hand);

            }
        }

        private void dispose() {
            // Disposing the elements related to the hand tracking
            handData.close();
            handConfiguration.close();
            handModule.close();

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

                // Updating the hand data
                handData.Update();

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
