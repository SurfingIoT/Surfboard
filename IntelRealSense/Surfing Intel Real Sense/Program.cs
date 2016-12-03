using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace RealSenseMqqt
{
    class Program
    {
        private const string GESTURE_CLICK = "click";
        private const string GESTURE_VSIGN = "v_sign";
        private const string GESTURE_FIST = "fist";
        private const string GESTURE_SPREADFINGERS = "spreadfingers";
        private static string currentGesture = "";
        private static int currentEmotion;
        public static MqttClient client;
        public static PXCMRotation rotationHelper;

        static IoT_RealSense_Surfing iot_form;
        static Boolean keepLooping = true;

        public static void InitializeMqqtClient()
        {
            client = new MqttClient(iot_form.textMQTTBroker.Text, 1883, false, null);
            string clientId = Guid.NewGuid().ToString();
            client.Connect(clientId);
        }
        public static void start(IoT_RealSense_Surfing frm)
        {
            keepLooping = true;
            iot_form = frm;
            InitializeMqqtClient();
            PXCMSession session = PXCMSession.CreateInstance();
            // Querying the SDK version
            Console.WriteLine(frm.comboTarget.SelectedText);
            if (session != null)
            {
                // Optional steps to send feedback to Intel Corporation to understand how often each SDK sample is used.
                PXCMMetadata md = session.QueryInstance<PXCMMetadata>();
                if (md != null)
                {
                    string sample_name = "Emotion Viewer CS";
                    md.AttachBuffer(1297303632, System.Text.Encoding.Unicode.GetBytes(sample_name));
                }
                //Application.Run(new MainForm(session));
                //session.Dispose();
            }
            //PXCMSession.ImplVersion version = session.QueryVersion();
            //Console.WriteLine("RealSense SDK Version {0}.{1}", version.major, version.minor);

            session.CreateImpl<PXCMRotation>(out rotationHelper);

            // Creating the SenseManager
            PXCMSenseManager senseManager = session.CreateSenseManager();
            if (senseManager == null)
            {
                Console.WriteLine("Failed to create the SenseManager object.");
                return;
            }

            // Enabling Emotion Module
            pxcmStatus enablingModuleStatus = senseManager.EnableEmotion();
            if (enablingModuleStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the Emotion Module");
                return;
            }

            // Getting the instance of the Emotion Module
            PXCMEmotion emotionModule = senseManager.QueryEmotion();
            if (emotionModule == null)
            {
                Console.WriteLine("Failed to query the emotion module");
                return;
            }

            // Initializing the camera
            if (senseManager.Init() < pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to initialize the SenseManager");
                return;
            }

            // Enabling the Hand module
            pxcmStatus enablingModuleStatus1 = senseManager.EnableHand("Hand Module");
            if (enablingModuleStatus1 != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine("Failed to enable the Hand Module");
                return;
            }

            // Getting the instance of the Hand Module
            PXCMHandModule handModule = senseManager.QueryHand();
            if (handModule == null)
            {
                Console.WriteLine("Failed to get the HandModule object.");
                return;
            }

            // Creating an active configuration
            PXCMHandConfiguration handConfiguration = handModule.CreateActiveConfiguration();
            if (handConfiguration == null)
            {
                Console.WriteLine("Failed to create the HandConfiguration object.");
                return;
            }

            // Listing the available gestures            
            int supportedGesturesCount = handConfiguration.QueryGesturesTotalNumber();
            if (supportedGesturesCount > 0)
            {
                Console.WriteLine("Supported gestures:");
                for (int i = 0; i < supportedGesturesCount; i++)
                {
                    string gestureName = string.Empty;

                    if (handConfiguration.QueryGestureNameByIndex(i, out gestureName) == pxcmStatus.PXCM_STATUS_NO_ERROR)
                    {
                        Console.WriteLine("\t" + gestureName);
                    }
                }
            }

            // Enabling some gestures
            String[] enabledGestures = { GESTURE_CLICK, GESTURE_VSIGN, GESTURE_FIST, GESTURE_SPREADFINGERS };
            foreach (String gesture in enabledGestures)
            {
                if (!handConfiguration.IsGestureEnabled(gesture))
                {
                    handConfiguration.EnableGesture(gesture);
                }
            }
            handConfiguration.ApplyChanges();

            // Creating a data output object
            PXCMHandData handData = handModule.CreateOutput();
            if (handData == null)
            {
                Console.WriteLine("Failed to create the HandData object.");
                return;
            }

            // Initializing the SenseManager
            if (senseManager.Init() != pxcmStatus.PXCM_STATUS_NO_ERROR)
            {
                Console.WriteLine(senseManager.Init());
                return;
            }

            // Looping to query the hands information
            while (keepLooping)
            {
                // Acquiring a frame
                if (senseManager.AcquireFrame(true) < pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }

                // Updating the hand data
                if (handData != null)
                {
                    handData.Update();
                }
                //ProcessHands(handData);
                ProcessGestures(handData);
                ProcessEmotions(emotionModule);
                // Releasing the acquired frame
                senseManager.ReleaseFrame();
                
                /* using another frame to process different stuff? may be...
                // Acquiring a frame
                if (senseManager.AcquireFrame(true) < pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    break;
                }

                // Processing Emotions
                ProcessEmotions(emotionModule);

                // Releasing the acquired frame
                senseManager.ReleaseFrame();*/

            }


            // Releasing resources
            if (handData != null) handData.Dispose();
            if (handConfiguration != null) handConfiguration.Dispose();
            rotationHelper.Dispose();

            senseManager.Close();
            senseManager.Dispose();
            session.Dispose();
            client.Disconnect();
        }
        public static void Main(string[] args)
        {
            Application.Run(new IoT_RealSense_Surfing());
            // Creating a SDK session
        }

        private static void ProcessGestures(PXCMHandData handData)
        {
            // Processing gestures
            // Querying how many gestures were detected
            int firedGesturesNumber = handData.QueryFiredGesturesNumber();

            // Querying which gestures were detected
            for (int i = 0; i < firedGesturesNumber; i++)
            {
                PXCMHandData.GestureData gestureData;

                if (handData.QueryFiredGestureData(i, out gestureData) == pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    // Getting the gesture name
                    String gestureName = gestureData.name;

                    // Getting the gesture state
                    PXCMHandData.GestureStateType gestureState = gestureData.state;

                    // Getting the gesture timestamp
                    long timestamp = gestureData.timeStamp;

                    // Getting the hand information about which hand fired the gesture
                    PXCMHandData.IHand handInfo;
                    if (handData.QueryHandDataById(gestureData.handId, out handInfo) != pxcmStatus.PXCM_STATUS_NO_ERROR)
                    {
                        continue;
                    }

                    // Getting which hand fired the gesture
                    PXCMHandData.BodySideType bodySideHand = handInfo.QueryBodySide();

                    
                    if (gestureState == PXCMHandData.GestureStateType.GESTURE_STATE_START)
                    {
                        Console.WriteLine("Gesture \"{0}\" was detected. State:{1}, Timestamp:{2}, BodySide:{3}", gestureName, gestureState, timestamp, bodySideHand);
                        Console.WriteLine("--------------------");

                        if (gestureName.CompareTo(GESTURE_FIST) == 0)
                        {
                            if (currentGesture.CompareTo(GESTURE_FIST) != 0)
                            {
                                //SendMqttMessage("surfboard2/relay?0");
                                String target = (String) iot_form.comboTarget.SelectedItem;
                                String hclosed = (String) iot_form.comboHandsClosed.SelectedItem;
                                //if (target.Equals("")) target = "*";

                                SendMqttMessage(target + "/" + hclosed);
                                SendMqttMessage(hclosed);
                                currentGesture = GESTURE_FIST;
                            }
                        }
                        else if (gestureName.CompareTo(GESTURE_SPREADFINGERS) == 0)
                        {
                            if (currentGesture.CompareTo(GESTURE_SPREADFINGERS) != 0)
                            {
                                String target = (String)iot_form.comboTarget.SelectedItem;
                                String hopen = (String)iot_form.comboGive5Action.SelectedItem;
                                //if (target.Equals("")) target = "*";
                                SendMqttMessage(hopen);
                                SendMqttMessage(target + "/" + hopen);
                                //System.Diagnostics.Process.Start("http://www.globalcode.com.br");
                                //SendMqttMessage("surfboard2/relay?1");
                                currentGesture = GESTURE_SPREADFINGERS;
                            }
                        }
                        else if (gestureName.CompareTo(GESTURE_CLICK) == 0)
                        {
                            if (currentGesture.CompareTo(GESTURE_CLICK) != 0)
                            {
                                SendMqttMessage("green?255");
                                currentGesture = GESTURE_CLICK;
                            }
                        }
                        else if (gestureName.CompareTo(GESTURE_VSIGN) == 0)
                        {
                            if (currentGesture.CompareTo(GESTURE_VSIGN) != 0)
                            {
                                //System.Diagnostics.Process.Start("https://www.youtube.com/watch?v=MVm5hcQYJ-U");
                                SendMqttMessage("green?0");
                                keepLooping = false;
                                currentGesture = GESTURE_VSIGN;
                            }
                        }

                    }
                }
            }
        }

        static void ProcessEmotions(PXCMEmotion emotionModule)
        {
            // Querying how many faces were detected
            int numberOfFaces = emotionModule.QueryNumFaces();
            Console.WriteLine("{0} face(s) were detected.", numberOfFaces);

            // Querying the emotion information about detected faces
            for (int i = 0; i < numberOfFaces; i++)
            {
                PXCMEmotion.EmotionData[] allEmotionData;
                emotionModule.QueryAllEmotionData(i, out allEmotionData);

                // Print emotions detected and intensity
                foreach (PXCMEmotion.EmotionData emotionData in allEmotionData)
                {
                    if(emotionData.intensity>0.6)
                    {
                        if(emotionData.eid.ToString().Equals("EMOTION_PRIMARY_JOY"))
                        {
                            if (currentEmotion!=1)
                            {
                                String target = (String)iot_form.comboTarget.SelectedItem;
                                String hopen = (String)iot_form.comboSmile.SelectedItem;
                                SendMqttMessage(target + "/" + hopen);
                                SendMqttMessage(hopen);
                                currentEmotion = 1;
                                //System.Diagnostics.Process.Start("http://www.globalcode.com.br");
                                //SendMqttMessage("surfboard2/relay?1");
                            }
                        }
                        if (emotionData.eid.ToString().Equals("EMOTION_PRIMARY_SURPRISE"))
                        {
                            if (currentEmotion != 2)
                            {
                                String target = (String)iot_form.comboTarget.SelectedItem;
                                String hopen = (String)iot_form.comboSurprise.SelectedItem;
                                SendMqttMessage(target + "/" + hopen);
                                SendMqttMessage(hopen);
                                currentEmotion = 2;
                                //System.Diagnostics.Process.Start("http://www.globalcode.com.br");
                                //SendMqttMessage("surfboard2/relay?1");
                            }
                        }
                    }
                    Console.WriteLine("{0} - Intensity {1}", emotionData.eid, emotionData.intensity);
                }
            }
        }

        private static void ProcessHands(PXCMHandData handData)
        {
            // Querying how many hands were detected
            int numberOfHands = handData.QueryNumberOfHands();
            //Console.WriteLine("{0} hand(s) were detected.", numberOfHands);

            // Querying the information about detected hands
            for (int i = 0; i < numberOfHands; i++)
            {
                // Querying hand id
                int handId;
                pxcmStatus queryHandIdStatus = handData.QueryHandId(PXCMHandData.AccessOrderType.ACCESS_ORDER_NEAR_TO_FAR, i, out handId);
                if (queryHandIdStatus != pxcmStatus.PXCM_STATUS_NO_ERROR)
                {
                    Console.WriteLine("Failed to query the hand Id.");
                    continue;
                }
                //Console.WriteLine("Hand id: {0}", handId);

                // Querying the hand data
                PXCMHandData.IHand hand;
                pxcmStatus queryHandStatus = handData.QueryHandDataById(handId, out hand);

                if (queryHandStatus == pxcmStatus.PXCM_STATUS_NO_ERROR && hand != null)
                {

                    // Querying the body side (Left/Right)
                    PXCMHandData.BodySideType bodySide = hand.QueryBodySide();
                    //Console.WriteLine("Body Side: {0}", bodySide);

                    // Querying the hand openness
                    int handOpenness = hand.QueryOpenness();
                    //Console.WriteLine("Hand openness: {0}", handOpenness);

                    //SendMqttMessage(String.Format("surfboard2/red?{0}", (255 *handOpenness)/100));

                    // Querying Hand 2D Position
                    PXCMPointF32 massCenterImage = hand.QueryMassCenterImage();
                    //Console.WriteLine("Hand position on image: {0} | {1}", massCenterImage.x, massCenterImage.y);

                    PXCMPoint4DF32 palmOrientation = hand.QueryPalmOrientation();
                    rotationHelper.SetFromQuaternion(palmOrientation);

                    PXCMPoint3DF32 rotationEuler = rotationHelper.QueryEulerAngles();

                    double angleInDegrees = (180 * rotationEuler.y / Math.PI) + 180;
                    Console.WriteLine("Angle in degrees: {0}", angleInDegrees);
                    SendMqttMessage(String.Format("servo?{0}", (int)angleInDegrees));
            
                    
                    // Console.WriteLine("Rotation x:{0},y:{1},z:{2}", rotationEuler.x, rotationEuler.y, rotationEuler.z);

                    // Querying Hand 3D Position
                    PXCMPoint3DF32 massCenterWorld = hand.QueryMassCenterWorld();
                    //Console.WriteLine("Hand position on world: {0} | {1} | {2}", massCenterWorld.x, massCenterWorld.y, massCenterWorld.z);

                    /*
                    // Querying Hand Joints
                    if (hand.HasTrackedJoints())
                    {
                        foreach (PXCMHandData.JointType jointType in Enum.GetValues(typeof(PXCMHandData.JointType)))
                        {
                            PXCMHandData.JointData jointData;
                            pxcmStatus queryStatus = hand.QueryTrackedJoint(jointType, out jointData);

                            if (queryStatus == pxcmStatus.PXCM_STATUS_NO_ERROR && jointData != null)
                            {
                                // Printing joint label and tracking confidence
                                Console.WriteLine("Joint {0} with confidence {1}", jointType, jointData.confidence);

                                // Printing the 2D position (image)
                                Console.WriteLine("\t2D Position: {0} | {1}", jointData.positionImage.x, jointData.positionImage.y);

                                // Printing the 3D position (depth)
                                Console.WriteLine("\t3D Position: {0} | {1} | {2}", jointData.positionWorld.x, jointData.positionWorld.y, jointData.positionWorld.z);
                            }
                        }
                    }*/

                }

                //Console.WriteLine("----------");
                //Console.Clear();
            }
        }

        public static DateTime LastTimeMessage;

        public static void SendMqttMessage(String message) {
            DateTime now = DateTime.Now;

            if (LastTimeMessage == null || now.Subtract(LastTimeMessage).TotalMilliseconds >= 100)
            {
                Console.WriteLine(string.Format("Executou comando '{0}' Timeframe: {1}", message, now));
                //client.Publish("globalcode/things/command", Encoding.UTF8.GetBytes(message), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
                String queue = iot_form.textQUEUE.Text;
                client.Publish(queue, Encoding.UTF8.GetBytes(message), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
                LastTimeMessage = now;
            }
        }


    }
}
