package org.surfing.gemalto;

import java.io.IOException;

import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttSecurityException;

import com.cinterion.io.ADC;
import com.cinterion.io.ADCListener;
import com.cinterion.misc.Watchdog2;
import com.cwm.util.GPRSConnectOptions;

public class ThingsMQTT extends MIDlet {

    public static String DEF_BROCKER_URL = "tcp://iot.eclipse.org";
    public static int DEF_BROCKER_PORT = 1883;
    public static int DEF_QOS = 1;
    public static int DEF_BAUDRATE = 9600;
    public static long DEF_INTERVAL = 15000;
    public static String DEF_DEV_TYPE_ID = "d2447d4f-fb74-48d8-8eb8-45013932284b";

    public GSMHandler gsmH;
    private String apn;
    private String apn_usr;
    private String apn_pwd;
    private String pin;

    private String brokerUrl;
    private int brokerPort;
    private long sensorsInterval;
    public static String queueBroadcast;
    public static String queueListener;
    public static String queueSensors;
    public static String queueRendezvous;
    public static String queueSMS;

    //private String publishTopic;
    // private String subscribeTopic;
    private int qos;
    private String deviceTypeID;
    private String clientID;

    public MQTTHandler mqttH;
    private ADC adc;
    private ADCListener adcListener;
    private Watchdog2 wd = null;
    public Surfboard surfboard;
    private boolean initFinish = false;

    public ThingsMQTT() {
        System.out.println("ThingsMQTT:ThingsMQTT+");
        System.out.println("ThingsMQTT:ThingsMQTT-");
    }

    protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
        System.out.println("ThingsMQTT:destroyApp()+");
        System.out.println("ThingsMQTT:destroyApp()-");
    }

    protected void pauseApp() {
        System.out.println("ThingsMQTT:pauseApp()+");
        System.out.println("ThingsMQTT:pauseApp()-");
    }

    protected void startApp() throws MIDletStateChangeException {
        System.out.println("ThingsMQTT:startApp()+");
// ########## GSM handling ##########
        gsmH = new GSMHandler(this);

        // reaad settings from JAD file
        readJADSettings();

        // generate client/device ID
        generateClientID();

        // enter SIM-PIN if available
        if ((pin != null) && (!pin.equals(""))) {
            if (!gsmH.enterPIN(pin)) {
                destroyApp(true);
                return;
            }
        }

        // waiting for network registration for 2 minutes
        System.out.print("Waiting for network registration...");
        if (gsmH.waitForNetRegistration(1000 * 120)) {
            System.out.println("Registered!");
        } else {
            System.out.println("No network registration");
            destroyApp(true);
            return;
        }

        System.out.println("APN: " + apn);
        // if APN is not defined in the JAD file try to read the APN from the
        // apn config file
        if ((apn == null) || (apn.equals(""))) {
            apn = gsmH.getAPN(null, "file:///a:/apncfg.txt");
        }

        System.out.println("Home APN: " + apn);

        GPRSConnectOptions.getConnectOptions().setAPN(apn);

        // waiting for GPRS attach for 2 minutes
        System.out.println("Waiting for GPRS attach...");
        if (gsmH.waitForGPRSAttach(1000 * 120)) {
            System.out.println("Attached!");

        } else {
            System.out.println("No attach possible");
            destroyApp(true);
            return;
        }

        // ########## MQTT handling ##############
        mqttH = new MQTTHandler(clientID, brokerUrl + ":" + brokerPort, this);

        // connect to MQTT Brocker
        System.out.println("Connecting to MQTT brocker...");
        try {
            mqttH.connectToBrocker();
        } catch (MqttSecurityException e1) {
            e1.printStackTrace();
        } catch (MqttException e1) {
            e1.printStackTrace();
        }

        try {
            subscribeTopics();

        } catch (MqttException e) {
            System.out.println("!Exception at connecting!");
        }
        System.out.println("Starting Serial connection with Surfboard...");
        surfboard = new Surfboard();
        try {
            surfboard.open();
        } catch (IOException ex) {
            System.out.println("!Exception at connecting with Surfboard!");
            ex.printStackTrace();
        }

        // ############# demo functions ###################
        // monitor and post the the memory status
        new Thread(new MyMemoryMoniThread()).start();

        // initiate the example ADC listener
        adcListener = new MyADCListener();
        try {
            // disable ADC if it was enabled.
            gsmH.sendATC("AT^SRADC=0\r");

            // enable ADC
            System.out.println("Enable ADC");
            adc = new ADC(0, 5000);
        } catch (IOException ex) {
            System.out.println("ADC failed " + ex.getMessage());
            ex.printStackTrace();
        }
        adc.addListener(adcListener);

        // enable the watchdog
        wd = new Watchdog2();
        wd.start(35);

        // kick the watchdog every 30sec
        Timer watchdogTimer = new Timer();
        watchdogTimer.schedule(new MyWatchdogTimer(), 1000, 30000);
        Timer sensorsTimer = new Timer();
        sensorsTimer.schedule(new SensorsTask(), 1000, sensorsInterval);
        initFinish = true;

        publishValue(queueRendezvous, qos, "EHS6 device Connected to Internet " + clientID);

        System.out.println("ThingsMQTT:startApp()-");
    }

    public void gsmEvent(String event) {
        publishValue(null, qos, "event=" + event);
    }
    public void executeAction(String message) {
        if (message.toUpperCase().startsWith("SMS://")) {
            String command = message.substring(6, message.length());
            synchronized (this) {
                SMSAction.execute(command, this);
            }
        } else if (message.toUpperCase().startsWith("BOARD://")) {
            try {
                String command = message.substring(8, message.length());
                synchronized (this) {
                    surfboard.execute(command);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        } else {
            try {
                synchronized (this) {
                    surfboard.execute(message);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
        
    }
    public void mqttMessageArrived(String topic, String message) {
        System.out.println("ThingsMQTT:mqttMessageArrived()+");
        System.out.println("Msg arrived!");
        System.out.println("Topic: " + topic);
        System.out.println("Message: " + message);
        executeAction(message);
        System.out.println("ThingsMQTT:mqttMessageArrived()-");
    }

    public void mqttDeliveryComplete() {
        System.out.println("ThingsMQTT:mqttDeliveryComplete()+");
        System.out.println("ThingsMQTT:mqttDeliveryComplete()-");
    }

    public void mqttConnectionLost(Throwable cause) {
        System.out.println("ThingsMQTT:mqttConnectionLost()+");
        System.out.println("!!!!! Connection to MQTT broker lost !!!!!");
        if (cause != null) {
            cause.printStackTrace();
        } else {
            System.out.println("cause != null");
        }

        System.out.println("Re-Connecting to MQTT brocker...");
        try {
            mqttH.connectToBrocker();
            // subscribe for receiving the data
            subscribeTopics();
        } catch (MqttSecurityException e1) {
            e1.printStackTrace();
        } catch (MqttException e1) {
            e1.printStackTrace();
        }
        System.out.println("ThingsMQTT:mqttConnectionLost()-");
    }

    public void subscribeTopics() throws MqttException {
        mqttH.subscribe(queueListener, qos);
        mqttH.subscribe(queueSMS, qos);
        mqttH.subscribe(queueBroadcast, qos);

    }

    public void publishValue(String topic, int qos, String msg) {
        System.out.println("ThingsMQTT:publishValue()+");

        if (!initFinish) {
            return;
        }

        if (topic == null) {
            topic = queueSensors;
        }

        try {
            mqttH.publish(topic, qos, msg.getBytes());
            System.out.println("topic: " + topic);
            System.out.println("qos:" + qos);
            System.out.println("message: " + msg);
        } catch (MqttException ex) {
            if (ex != null) {
                System.out.println("Can't publish the message!");
            }
        }
        System.out.println("ThingsMQTT:publishValue()-");
    }

    private String generateClientID() {
        System.out.println("ThingsMQTT:generateClientID()+");
        String rspIMEI = "";
        String rspModule = "";

        rspIMEI = gsmH.sendATC("AT+CGSN\r");
        rspModule = gsmH.sendATC("AT+CGMM\r");

        if (rspModule != null && !rspModule.equals("")) {
            clientID = rspModule.substring(1, 6);
        } else {
            clientID = "XXX";
        }

        if (rspIMEI != null && !rspIMEI.equals("")) {
            clientID = clientID + rspIMEI.substring(2, 16);
        } else {
            clientID = clientID + "12345";
        }
        System.out.println("DeviceID: " + clientID);
        System.out.println("ThingsMQTT:generateClientID()-");

        return clientID;
    }

    private void readJADSettings() {
        System.out.println("ThingsMQTT:readJADSettings()+");
        // read configuration properties from .jad file

        deviceTypeID = getAppProperty("Device-Type-ID");
        if (deviceTypeID == null) {
            deviceTypeID = DEF_DEV_TYPE_ID;
        }

        clientID = getAppProperty("Client-ID");
        if (clientID == null) {
            clientID = generateClientID();
        }

        brokerUrl = getAppProperty("Broker-URL");
        if (brokerUrl == null) {
            brokerUrl = DEF_BROCKER_URL;
        }

        String tmpVal = getAppProperty("Baudrate");
        if (tmpVal != null) {

            try {
                Surfboard.baudRate = Integer.parseInt(tmpVal);
            } catch (NumberFormatException e) {
                Surfboard.baudRate = DEF_BAUDRATE;
            }
        } else {
            Surfboard.baudRate = DEF_BAUDRATE;
        }
        tmpVal = getAppProperty("Sensors-Interval");
        if (tmpVal != null) {
            try {
                sensorsInterval = Long.parseLong(tmpVal);
            } catch (NumberFormatException e) {
                sensorsInterval = DEF_INTERVAL;
            }
        } else {
            sensorsInterval = DEF_INTERVAL;
        }
        tmpVal = getAppProperty("Broker-Port");
        if (tmpVal
                != null) {
            try {
                brokerPort = Integer.parseInt(tmpVal);
            } catch (NumberFormatException e) {
                brokerPort = DEF_BROCKER_PORT;
            }
        } else {
            brokerPort = DEF_BROCKER_PORT;
        }

        tmpVal = getAppProperty("QoS");
        if (tmpVal
                != null) {
            try {
                qos = Integer.parseInt(tmpVal);
            } catch (NumberFormatException e) {
                qos = DEF_QOS;
            }
        } else {
            qos = DEF_QOS;
        }

        queueSensors = getAppProperty("Queue-Sensors");
        if (queueSensors
                == null) {
            queueSensors = "/iot-surfboard/sensors/default";
        }
        queueListener = getAppProperty("Queue-Listener");
        if (queueListener
                == null) {
            queueListener = "/iot-surfboard/control/default";
        }
        queueBroadcast = getAppProperty("Queue-Broadcast");
        if (queueBroadcast
                == null) {
            queueBroadcast = "/iot-surfboard/everything";
        }
        queueRendezvous = getAppProperty("Queue-Rendevouz");
        if (queueRendezvous
                == null) {
            queueRendezvous = "/iot-surfboard";
        }
        queueSMS = getAppProperty("Queue-SMS");
        if (queueSMS
                == null) {
            queueSMS = "/iot-surfboard/sms/surfboard-ehs6t";
        }
        Surfboard.COMPort = getAppProperty("COM-Port");
        if (Surfboard.COMPort
                == null) {
            Surfboard.COMPort = "COM0";
        }

        pin = getAppProperty("SIM-PIN");

        apn = getAppProperty("APN");

        apn_usr = getAppProperty("APN-USR");

        apn_pwd = getAppProperty("APN-PWD");

        System.out.println(
                "############ Settings from JAD ##########");
        System.out.println(
                "Client ID: " + clientID);
        System.out.println(
                "Device Type ID: " + deviceTypeID);
        System.out.println(
                "Data APN: " + apn);
        System.out.println(
                "APN USR:" + apn_usr);
        System.out.println(
                "APN USR:" + apn_pwd);
        System.out.println(
                "brokerUrl: " + brokerUrl);
        System.out.println(
                "brokerPort: " + brokerPort);
        System.out.println(
                "baudRate: " + Surfboard.baudRate);
        System.out.println(
                "Sensors Interval: " + sensorsInterval);
        System.out.println(
                "COM Port: " + Surfboard.COMPort);
        System.out.println(
                "QoS: " + qos);
        System.out.println(
                "Queue-Sensors: " + queueSensors);
        System.out.println(
                "Queue-Listener: " + queueListener);
        System.out.println(
                "Queue-Broadcast: " + queueBroadcast);
        System.out.println(
                "Queue-Rendezvous: " + queueRendezvous);
        System.out.println(
                "Queue-SMS: " + queueSMS);
        System.out.println(
                "SIM PIN: " + pin);
        System.out.println(
                "####################################");

        System.out.println(
                "ThingsMQTT:readJADSettings()-");
    }

    class MyADCListener implements ADCListener {

        int adcValue = 0;
        int lastValue = 0;
        int refValue = 0;
        String adcMsg = null;

        // String topic = null;
        public void adcValues(int[] values) {
            adcValue = 0;
            // calculate avarege from taken samples
            for (int i = 0; i < values.length; i++) {
                adcValue += values[i];
            }
            if (values.length > 0) {
                adcValue = adcValue / values.length;
            }

            if (lastValue != adcValue) {

                refValue = (lastValue - adcValue);
                if (refValue < 0) {
                    refValue = (refValue * -1);
                }
                if (refValue < 10) {
                    return;
                }

                adcMsg = "adc=" + (adcValue * 0.1);

                if ((adcMsg != null) && adcMsg.length() > 8) {
                    adcMsg = adcMsg.substring(0, 8);
                }

                if (adcValue * 0.1 > 99) {
                    publishValue(null, qos, "YOU DRINK!!! ADC == " + adcMsg);
                }

                lastValue = adcValue;
            }
        }
    }

    class MyMemoryMoniThread extends Thread {

        String msgFreeMem = "";
        long freeMem, lastFreeMem = 0;

        public void run() {
            while (true) {
                if (freeMem != (Runtime.getRuntime().freeMemory() / 1024 / 1024)) {

                    freeMem = (Runtime.getRuntime().freeMemory() / 1024 / 1024);

                    msgFreeMem = "freemem=" + freeMem;

                    publishValue(null, qos, msgFreeMem);
                }
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }

    class MyWatchdogTimer extends TimerTask {

        public void run() {
            wd.kick();
            //publishValue(null, qos, "event=Hello There! " + System.currentTimeMillis());

            System.out.println("Kick watchdog");
        }

    }

    class SensorsTask extends TimerTask {

        public void run() {
            try {
                String sensors = surfboard.execute("sensors");
                publishValue(queueSensors, qos, sensors);
            } catch (IOException ex) {
                ex.printStackTrace();
            }

        }
    }
}
