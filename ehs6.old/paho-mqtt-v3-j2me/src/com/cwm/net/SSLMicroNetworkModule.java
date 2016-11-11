/* @start_prolog@
 * Version: %Z% %W% %I% %E% %U%  
 * ============================================================================
 *   <copyright 
 *   notice="oco-source" 
 *   pids="5724-H72," 
 *   years="2010,2012" 
 *   crc="2914692611" > 
 *   IBM Confidential 
 *    
 *   OCO Source Materials 
 *    
 *   5724-H72, 
 *    
 *   (C) Copyright IBM Corp. 2010, 2012 
 *    
 *   The source code for the program is not published 
 *   or otherwise divested of its trade secrets, 
 *   irrespective of what has been deposited with the 
 *   U.S. Copyright Office. 
 *   </copyright> 
 * ============================================================================
 * @end_prolog@
 */
package com.cwm.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;
import javax.microedition.io.SecureConnection;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.internal.NetworkModule;

import com.cwm.logging.Logger;
import com.cwm.logging.LoggerFactory;
import com.cwm.util.GPRSConnectOptions;

/**
 * A network module for connecting over SSL from Java ME (CLDC profile).
 */
public class SSLMicroNetworkModule implements NetworkModule {
	private String uri;
	private SecureConnection connection;
	private InputStream in;
	private OutputStream out;
	private GPRSConnectOptions connOptions;
	final static String className = SSLMicroNetworkModule.class.getName();
	Logger log = LoggerFactory.getLogger(LoggerFactory.MQTT_CLIENT_MSG_CAT,
			className);

	/**
	 * Constructs a new SSLMicroNetworkModule using the specified host and port.
	 * 
	 * @param host
	 *            the host name to connect to
	 * @param port
	 *            the port to connect to
	 * @param secure
	 *            whether or not to use SSL.
	 */
	public SSLMicroNetworkModule(String host, int port) {
		connOptions = GPRSConnectOptions.getConnectOptions();
		this.uri = "ssl://" + host + ":" + port
				+ ";bearer_type="
				+ connOptions.getBearerType() 
				+ ";access_point="
				+ connOptions.getAPN() 
				+ ";username="
				+ connOptions.getUser() 
				+ ";password="
				+ connOptions.getPasswd() 
				+ ";timeout="
				+ connOptions.getTimeout();
	}

	/**
	 * Starts the module, by creating a TCP socket to the server.
	 */
	public void start() throws IOException, MqttException {
		final String methodName = "start";
		try {
			log.fine(className, methodName, "252", new Object[] { uri });
			connection = (SecureConnection) Connector.open(uri);
			connection.setSocketOption(SocketConnection.LINGER, 5); 
			
			in = connection.openInputStream();
			out = connection.openOutputStream();
		} catch (IOException ex) {
			// @TRACE 250=Failed to create TCP socket
			log.fine(className, methodName, "250", null, ex);
			throw new MqttException(
					MqttException.REASON_CODE_SERVER_CONNECT_ERROR, ex);
		}
	}

	public InputStream getInputStream() {
		return in;
	}

	public OutputStream getOutputStream() {
		return out;
	}

	/**
	 * Stops the module, by closing the TCP socket.
	 */
	public void stop() throws IOException {
		in.close();
		out.close();
		connection.close();
	}
}
