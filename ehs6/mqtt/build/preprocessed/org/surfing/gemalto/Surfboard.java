/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.surfing.gemalto;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.microedition.io.CommConnection;
import javax.microedition.io.Connector;

/**
 *
 * @author vsenger
 */
public class Surfboard {

    CommConnection commConn;
    InputStream inStream;
    OutputStream outStream;
    static String COMPort;
    static int baudRate;

    public void open() throws IOException {
        System.out.println("Available COM-Ports: " + System.getProperty("microedition.commports"));
        String strCOM = "comm:" + COMPort + ";blocking=on;baudrate=" + baudRate;
        commConn = (CommConnection) Connector.open(strCOM);
        System.out.println("CommConnection(" + strCOM + ") opened");
        System.out.println("Real baud rate: " + commConn.getBaudRate());
        inStream = commConn.openInputStream();
        outStream = commConn.openOutputStream();

    }

    public synchronized String execute(String command) throws IOException {
        String strResponse = null;

        System.out.println("Sending command " + command + " to Surfboard");
        outStream.write(command.getBytes());
        byte response[] = new byte[2048];
        int counter = 0;
        try {
            Thread.sleep(200);
            System.out.println("Waiting response RXTX...");
            for (int x = 0; x < 10; x++) {
                Thread.sleep(40);
                while (inStream.available() > 0) {
                    response[counter++] = (byte) inStream.read();
                }
            }
            System.out.println("Response " + (counter - 1) + " bytes..");
            strResponse = new String(response);
            System.out.println(strResponse);
        } catch (InterruptedException ex) {
            ex.printStackTrace();
            System.out.println("Trying to re-stabilish serial connection...");
            close();
            open();
        }
        return strResponse;
    }

    public void close() throws IOException {
        inStream.close();
        outStream.close();
        commConn.close();
    }
}
