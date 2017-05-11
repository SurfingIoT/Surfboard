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

    static String msg = "{\"name\" : \"surfboard195\",\"firmware\" : \"3\",\"serial\" : \"416670035\",\"key\" : \"195555\",\"\n"
            + "components\" : [{\"name\" : \"alcohol\",\"value\" : \"0\"},{\"name\" : \"pot\",\"value\" : \"59\"},{\"name\" : \"light\",\"value\" :\n"
            + "\"30\"},{\"name\" : \"distance\",\"value\" : \"0\"},{\"name\" : \"clock\",\"value\" : \"7/28/2016 11:28:59\"},{\"name\" : \"temp\",\"\n"
            + "value\" : \"25.00\"},{\"name\" : \"humidity\",\"value\" : \"41.00\"}]}";
    static String msg1 = "{\"name\" : \"surfboard195\", \"firmware\" : \"3\", \"serial\" : \"416670035\", \"key\" : \"195555\", \"components\" : [{\"name\" : \"alcohol\",\"value\" : \"0\"},{\"name\" : \"pot\",\"value\" : \"59\"},{\"name\" : \"light\",\"value\" : \"30\"}, {\"name\" : \"distance\", \"value\" : \"0\"}, {\"name\" : \"clock\", \"value\" : \"7/28/2016 11:28:59\"}, {\"name\" : \"temp\", \"value\" : \"24.00\"},{\"name\" : \"humidity\",\"value\" : \"49.00\"}]}";
    public static void main(String[] args) throws Exception {
        try {
            MQTTBroker.getInstance().publish("/iot-surfboard/control/surfboard-ehs6t", "relay?1");
            MQTTBroker.getInstance().subscribe("/iot-surfboard/sensors/surfboard-ehs6t");
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
