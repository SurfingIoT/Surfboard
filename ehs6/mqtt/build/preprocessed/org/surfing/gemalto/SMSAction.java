/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.surfing.gemalto;

/**
 *
 * @author vinic_000
 */
public class SMSAction {
    public static void execute(String message, ThingsMQTT instance) {
        String phone = message.substring(0, message.indexOf(";"));
        String msg = message.substring(message.indexOf(";") + 1, message.length());
        try {
            System.out.println("Phone to send: " + phone);
            System.out.println("Message to send: " + msg);
            String smsAtCommand = "AT+CMGS=\"" + phone + "\"\r";
            //simple SMS mode CMGF=1\
            System.out.println("Seding command AT+CMGF=1\r");
            System.out.println(instance.gsmH.sendATC("AT+CMGF=1\r"));
            System.out.println("Command AT:" + smsAtCommand);
            System.out.println(instance.gsmH.sendATC(smsAtCommand));
            System.out.println("Sending message:" + (msg + (char) 26));
            System.out.println(instance.gsmH.sendATC(msg + (char) 26));
        } catch (Exception ex) {
            System.out.println("SMS failed " + ex.getMessage());
        }
    }
}
