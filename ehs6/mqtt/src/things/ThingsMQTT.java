package things;

/*
 * Copyright (C) 2013  Oleg Pachkovets
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, as
 * published by the Free Software Foundation..
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
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

/*
 * The CBdemoApp is a demo app for the data communication utilizing the MQTT protocol.
 * The MQTT client used by the CBdemoApp is a customized version of the Paho MQTT client
 * 
 * The app is designed, configured and tested to work with Cinterion EHS5 and EHS6 wireless modules
 * featuring Java ME 3.2
 * 
 * For demonstration purpose the app is monitoring the module free available 
 * RAM and the ADC (Analog-Digital-Converter) values and transmits the values to 
 * the publicly accessible data display powered by M2MGO
 * The data display can be accessed at www.m2mgo-display.com
 *  
 * The app implements following functionality
 * - reading the configuration data from JAD file
 * - establishing the cellular network connection (SIM PIN, Network registration)
 * - initiating and managing the MQTT connection as well as data communication by using MQTT
 * - monitoring the free available RAM
 * - monitoring the AD-Converter
 */
public class ThingsMQTT extends MIDlet {

    public static String DEF_BROCKER_URL = "tcp://iot.eclipse.org";
    public static int DEF_BROCKER_PORT = 1883;
    public static int DEF_QOS = 1;
    public static int DEF_BAUDRATE = 9600;
    public static long DEF_INTERVAL = 15000;
    public static String DEF_DEV_TYPE_ID = "d2447d4f-fb74-48d8-8eb8-45013932284b";

    private GSMHandler gsmH;
    private String apn;
    private String apn_usr;
    private String apn_pwd;
    private String pin;

    private String brokerUrl;
    private int brokerPort;
    private int baudRate;
    private long sensorsInterval;
    private String publishTopic;
    private String subscribeTopic;
    private int qos;
    private String deviceTypeID;
    private String clientID;

    private MQTTHandler mqttH;
    private ADC adc;
    private ADCListener adcListener;
    private Watchdog2 wd = null;

    private boolean initFinish = false;

    public ThingsMQTT() {
        System.out.println("CBdemoApp:CBdemoApp()+");
        System.out.println("CBdemoApp:CBdemoApp()-");
    }

    protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
        System.out.println("CBdemoApp:destroyApp()+");
        System.out.println("CBdemoApp:destroyApp()-");
    }

    protected void pauseApp() {
        System.out.println("CBdemoApp:pauseApp()+");
        System.out.println("CBdemoApp:pauseApp()-");
    }

    protected void startApp() throws MIDletStateChangeException {
        System.out.println("CBdemoApp:startApp()+");
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

        System.out.println("Los geht's!");
        try {
            // subscribe for receiving the data
            mqttH.subscribe(subscribeTopic, qos);

        } catch (MqttException e) {
            System.out.println("!Exception at connecting!");
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
        sensorsTimer.schedule(new SensorsTask(), 1000, 10000);
        initFinish = true;

        publishValue(null, qos, "Connected to Internet " + clientID);

        System.out.println("CBdemoApp:startApp()-");
    }

    public void gsmEvent(String event) {
        publishValue(null, qos, "event=" + event);
    }

    public void mqttMessageArrived(String topic, String message) {
        System.out.println("CBdemoApp:mqttMessageArrived()+");
        System.out.println("Msg arrived!");
        System.out.println("Topic: " + topic);
        System.out.println("Message: " + message);
        System.out.println("CBdemoApp:mqttMessageArrived()-");
        //Actions.messageToAction(message);
        String phone = message.substring(0, message.indexOf(";"));
        String msg = message.substring(message.indexOf(";") + 1, message.length());
        try {
            System.out.println("Phone to send: " + phone);
            System.out.println("Message to send: " + msg);
            String smsAtCommand = "AT+CMGS=\"" + phone + "\"\r";
            //simple SMS mode CMGF=1\
            System.out.println("Seding command AT+CMGF=1\r");
            System.out.println(gsmH.sendATC("AT+CMGF=1\r"));
            System.out.println("Command AT:" + smsAtCommand);
            System.out.println(gsmH.sendATC(smsAtCommand));
            System.out.println("Sending message:" + (msg + (char) 26));
            System.out.println(gsmH.sendATC(msg + (char) 26));
        } catch (Exception ex) {
            System.out.println("SMS failed " + ex.getMessage());
        }
        /*try {
         // subscribe for receiving the data
         mqttH.subscribe(subscribeTopic, qos);

         } catch (MqttException e) {
         System.out.println("!Exception at connecting!");
         }*/
    }

    public void mqttDeliveryComplete() {
        System.out.println("CBdemoApp:mqttDeliveryComplete()+");
        System.out.println("CBdemoApp:mqttDeliveryComplete()-");
    }

    public void mqttConnectionLost(Throwable cause) {
        System.out.println("CBdemoApp:mqttConnectionLost()+");
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
            mqttH.subscribe(subscribeTopic, qos);
        } catch (MqttSecurityException e1) {
            e1.printStackTrace();
        } catch (MqttException e1) {
            e1.printStackTrace();
        }
        System.out.println("CBdemoApp:mqttConnectionLost()-");
    }

    public void publishValue(String topic, int qos, String msg) {
        System.out.println("CBdemoApp:publishValue()+");

        if (!initFinish) {
            return;
        }

        if (topic == null) {
            topic = publishTopic;
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
        System.out.println("CBdemoApp:publishValue()-");
    }

    private String generateClientID() {
        System.out.println("CBdemoApp:generateClientID()+");
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
        System.out.println("CBdemoApp:generateClientID()-");

        return clientID;
    }

    private void readJADSettings() {
        System.out.println("CBdemoApp:readJADSettings()+");
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
                baudRate = Integer.parseInt(tmpVal);
            } catch (NumberFormatException e) {
                baudRate = DEF_BAUDRATE;
            }
        } else {
            baudRate = DEF_BAUDRATE;
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

        publishTopic = getAppProperty("Publish-Topic");
        if (publishTopic
                == null) {
            publishTopic = generatePublishTopic();
        }

        subscribeTopic = getAppProperty("Subscribe-Topic");
        if (subscribeTopic
                == null) {
            subscribeTopic = generateSubscribeTopic();
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
                "QoS: " + qos);
        System.out.println(
                "publishTopic: " + publishTopic);
        System.out.println(
                "subscribeTopic: " + subscribeTopic);
        System.out.println(
                "SIM PIN: " + pin);
        System.out.println(
                "####################################");

        System.out.println(
                "CBdemoApp:readJADSettings()-");
    }

    public String generatePublishTopic() {
        String topic = "";

        topic = "m2mgo/" + deviceTypeID + "/" + clientID + "/data";

        return topic;
    }

    public String generateSubscribeTopic() {
        String topic = "";

        topic = "m2mgo/" + deviceTypeID + "/" + clientID + "/command";

        return topic;

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
                String sensors = Surfboard.sensors();
                publishValue(null, qos, sensors);
            } catch (IOException ex) {
                ex.printStackTrace();
            }

        }
    }
}
