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

    public static String boardId = "surfboard177";

    public static void main(String[] args) {
        try {
            MQTTBroker pub = new MQTTBroker();
            pub.publish("globalcode/things", boardId + "/relay?0");
            MQTTBroker sub = new MQTTBroker();
            sub.subscribe("globalcode/things/" + boardId);
            sub.setListener(new MQTTListener() {
                public void processMessage(String queue, String message) {
                    System.out.println("Queue   " + queue);
                    System.out.println("Message " + message);
                    JSONObject jsonObject = (JSONObject) JSONValue.parse(message);
                    if (jsonObject == null || jsonObject.keySet() == null || jsonObject.keySet().iterator() == null) {
                        //System.out.println("Erro json " + data);
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

                        if (thing.equals("light")) {
                            try {
                                Integer v = Integer.valueOf(value);
                                if (v < 50) {
                                    System.out.println("switch on the lights");
                                    pub.publish("globalcode/things", boardId + "/relay?1");
                                } else {
                                    System.out.println("switch off the lights");
                                    pub.publish("globalcode/things", boardId + "/relay?0");
                                }
                            } catch (MqttException e) {
                                Logger.getLogger(HelloMQTT.class.getName()).log(Level.SEVERE, null, e);
                            }
                        }
                    }
                }
            });
        } catch (MqttException ex) {
            Logger.getLogger(HelloMQTT.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
