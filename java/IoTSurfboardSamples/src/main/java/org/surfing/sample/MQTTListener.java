package org.surfing.sample;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public abstract class MQTTListener implements MqttCallback {
    @Override
    public void deliveryComplete(IMqttDeliveryToken imdt) {

    }
    @Override
    public void connectionLost(Throwable thrwbl) {
        Logger.getLogger(MQTTListener.class.getName()).log(Level.INFO, "MQTT Receiver Lost Connection, trying to recovery once");
    }

    @Override
    public void messageArrived(String queue, MqttMessage mm) throws Exception {

        String msg = mm.toString();
        if (msg == null) {
            return;
        }
        Logger.getLogger(MQTTListener.class.getName()).log(Level.INFO, "Message arrived!{0}", msg);

        processMessage(queue, msg);
    }

    public abstract void processMessage(String queue, String msg);

}
