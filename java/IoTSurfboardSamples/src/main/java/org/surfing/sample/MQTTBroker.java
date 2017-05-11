package org.surfing.sample;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 *
 * @author vsenger
 */
public class MQTTBroker  {
    private static MQTTBroker instance = new MQTTBroker();
    public static MQTTBroker getInstance() {
        return instance;
    }
    private MQTTBroker() {}
    MqttClient client;

    public String MQTT_SERVER = "tcp://192.168.1.108:1883";
    public void setListener(MQTTListener l) {
        client.setCallback(l);
    }
    public void subscribe(String queue) throws MqttException {
        client.subscribe(queue);
    }
    
    public void publish(String queue,String msg) throws MqttException {
        try {
            if (client == null) {
                fixConnection();
            }
            if (!client.isConnected()) {
                fixConnection();
            }
            Logger.getLogger(MQTTBroker.class.getName()).log(Level.INFO, "Sending Message MQTT {0} to Queue {1} on server {2}", new Object[]{msg, queue, client.getServerURI()});

            MqttMessage message = new MqttMessage();
            message.setPayload(msg.getBytes());
            client.publish(queue, message);
            Logger.getLogger(MQTTBroker.class.getName()).log(Level.INFO, "Message sent!");

        } catch (MqttException ex) {
            Logger.getLogger(MQTTBroker.class.getName()).log(Level.SEVERE, "Error sending MQTT message " + ex.getMessage());
            try {
                fixConnection();
            } catch (MqttException e) {
                Logger.getLogger(MQTTBroker.class.getName()).log(Level.SEVERE, "Error fixing connecting " + e.getMessage());
                throw new MqttException(e);
            }
        }

    }

    public void fixConnection() throws MqttException {
        try {
            if (client != null && client.isConnected()) {
                client.disconnect();
            }
        } catch (MqttException ex) {
        }
        String clientName = "sample202030302292-" + ((int) (Math.random() * 3000) + 1);
        Logger.getLogger(MQTTBroker.class.getName()).log(Level.INFO, "Stablishing new MQTT Connection " + MQTT_SERVER);

        client = new MqttClient(MQTT_SERVER, clientName);
        client.connect();
        Logger.getLogger(MQTTBroker.class.getName()).log(Level.INFO, "Connection Stablished!");

    }

    public void close() throws MqttException {
        client.disconnect();
    }

    public void stop() {
        try {
            close();
        } catch (MqttException ex) {
            Logger.getLogger(MQTTBroker.class.getName()).log(Level.SEVERE, "Error closing MQTT Connection!", ex);
        }
    }

}
