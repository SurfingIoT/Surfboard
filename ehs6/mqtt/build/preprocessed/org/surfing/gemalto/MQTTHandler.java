package org.surfing.gemalto;
/*
 * Copyright (C) 2013  Oleg Pachkovets
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttSecurityException;
import org.eclipse.paho.client.mqttv3.MqttTopic;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import com.cwm.util.MqttConnectOptions;

public class MQTTHandler implements MqttCallback {
	private MqttClient client;
	private String clientId;
	private String brockerURL;
	private ThingsMQTT mainML;
	private MqttConnectOptions mqttOptions;

	public MQTTHandler(String clientId, String brockerURL, ThingsMQTT mainML1) {
		System.out.println("MQTTHandler:MQTTHandler()+");
		this.clientId = clientId;
		this.brockerURL = brockerURL;
                this.mainML=mainML1;

		// define connection options
		mqttOptions = new MqttConnectOptions();
		mqttOptions.setKeepAliveInterval(20 * 60); // Calculate minutes value in
													// seconds 20 min x 60 sec
		//mqttOptions.setUserName("m2mgo");
		//mqttOptions.setPassword("m2mgo".toCharArray());

		System.out.println("MQTTHandler:MQTTHandler()-");
	}

	public synchronized void connectToBrocker() throws MqttSecurityException,
			MqttException {

		System.out.println("MQTTHandler:connectToBrocker()+");

		System.out.println("MQTT client ID: " + clientId);
		System.out.println("Connecting to: " + brockerURL);

		// Create the MqttClient instance
		client = new MqttClient(brockerURL, clientId, new MemoryPersistence());

		// Set this wrapper as the callback handler
		client.setCallback(this);
                
		// Connect to the server
		try {
			client.connect(mqttOptions);
			System.out.println("Connected!");
		} catch (MqttException e) {
			System.out.println("!Exception at connecting!");
			throw e;
		}
	}

	public void publish(String topicName, int qos, byte[] payload)
			throws MqttException {

		if (client == null) {
			connectToBrocker();
		}

		// Get an instance of the topic
		MqttTopic topic = client.getTopic(topicName);

		MqttMessage message = new MqttMessage(payload);

		message.setQos(qos);

		if (client.isConnected()) {
			// Publish the message
			topic.publish(message);
		} else {
			// if not connected, try to connect and to publish again
			connectToBrocker();
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				System.out.println("Exception at sleeping ");
			}
			if (client.isConnected()) {
				// Publish the message
				topic.publish(message);
			} else {
				throw new MqttException(
						MqttException.REASON_CODE_CLIENT_NOT_CONNECTED);
			}
		}
	}

	public void subscribe(String topicName, int qos) throws MqttException {
		if (client.isConnected()) {
			// Subscribe to the topic
			client.subscribe(topicName, qos);
			System.out.println("Subscribed: " + topicName);
		} else {
			connectToBrocker();
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				System.out.println("Exception at sleeping ");
			}
			if (client.isConnected()) {
				// Subscribe to the topic
				client.subscribe(topicName, qos);
			} else {
				throw new MqttException(
						MqttException.REASON_CODE_CLIENT_NOT_CONNECTED);
			}
		}
	}

	public void disconnect(long timeout) {
		if ((client != null) && client.isConnected()) {
			System.out.println("Disconnecting from brocker");
			try {
				client.disconnect(timeout);
				System.out.println("Disconnected MQTT");
			} catch (MqttException e) {
				System.out.println("Exception at disconnecting");
			}
		}
	}

	public void connectionLost(Throwable cause) {
		if (mainML != null) {
			mainML.mqttConnectionLost(cause);
		}
	}

	public void messageArrived(String topic, MqttMessage message)
			throws Exception {
            System.out.println("Message Arrived...");
		if (mainML != null) {
                    //System.out.println("By pass delivering msg.." );
			mainML.mqttMessageArrived(topic, new String(message.getPayload()));
		}
	}

	public void deliveryComplete(IMqttDeliveryToken token) {
		if (mainML != null) {
			mainML.mqttDeliveryComplete();
		}
	}

}
