/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.surfing.sample;

import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

public class HelloMQTT {

    public static void main(String[] args) {
        try {
            MQTTBroker.getInstance().publish("globalcode/things", "*/relay?1");
            MQTTBroker.getInstance().subscribe("globalcode/things/surfboard177");
            MQTTBroker.getInstance().setListener(new MQTTListener() {
                public void processMessage(String queue, String message) {
                    System.out.println("Queue   " + queue);
                    System.out.println("Message " + message);
                    JSONObject jsonObject = (JSONObject) JSONValue.parse(message);
                    if (jsonObject == null || jsonObject.keySet() == null || jsonObject.keySet().iterator() == null) {
                        return;
                    }
                    JSONArray components = (JSONArray) jsonObject.get("components");
                    Iterator i = components.iterator();
                    while (i.hasNext()) {
                        Object oo = i.next();
                        JSONObject joo = (JSONObject) oo;
                        String thing = joo.get("name").toString();
                        String value = joo.get("value").toString();
                        System.out.println("Thing=" + thing + " Value = " + value);
                    }
                }
            });
        } catch (MqttException ex) {
            Logger.getLogger(HelloMQTT.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
