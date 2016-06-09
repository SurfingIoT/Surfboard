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
import com.cinterion.io.ATCommand;
import com.cinterion.io.ATCommandFailedException;
import com.cinterion.io.ATCommandListener;

import java.io.DataInputStream;
import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

public class GSMHandler implements ATCommandListener {

    private ATCommand atc = null;
    private ThingsMQTT mainML;

    public GSMHandler(ThingsMQTT mainML) {
        System.out.println("GSMHandler:GSMHandler()+");
        this.mainML = mainML;

        if (atc == null) {
            try {
                atc = new ATCommand(false);
            } catch (IllegalStateException e) {
                System.out
                        .println("Can't init AT command IllegalStateException: "
                                + e.getMessage());
            } catch (ATCommandFailedException e) {
                System.out
                        .println("Can't init AT command ATCommandFailedException: "
                                + e.getMessage());
            }
        }
        atc.addListener(this);

        // sendATC("AT^SPOW=1,0,0\r");
        sendATC("ATE0\r");
        //to support SMS text
        sendATC("at+cmgf=1\r");
        sendATC("at+cnmi=2,1\r");
        System.out.println("GSMHandler:GSMHandler()-");
    }

    // make sure all AT commands sent over one interface
    public synchronized String sendATC(String atcmd) {
        System.out.println("GSMHandler:sendATC()+");
        String rsp = "";

        // add CR if forgotten
        if (atcmd.indexOf("\r") <= 0) {
            atcmd += "\r";
        }

        // send the AT command
        try {
            rsp = atc.send(atcmd);
        } catch (ATCommandFailedException ex) {
            System.out.println("ATCommandFailedException: " + ex.getMessage());
        } catch (IllegalStateException ex) {
            System.out.println("IllegalStateException: " + ex.getMessage());
        } catch (IllegalArgumentException ex) {
            System.out.println("IllegalArgumentException: " + ex.getMessage());
        }

        // add 100ms delay after each command
        try {
            Thread.sleep(100);
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }

        System.out.println("GSMHandler:sendATC()");
        return rsp;
    }

    // wait until module is registered to the network
    public boolean waitForNetRegistration(long timeout_ms) {
        System.out.println("GSMHandler:waitForNetRegistration()+");
        long timestamp = System.currentTimeMillis();
        boolean registered = false;
        String response = "";
        while (!registered) {
            response = sendATC("AT+CREG?\r");
            if (response.indexOf(",1") > 0 || response.indexOf(",5") > 0) {
                registered = true;
            } else {
                System.out.print("#");
            }
            if ((System.currentTimeMillis() - timestamp) > timeout_ms) {
                break;
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
            }
        }
        System.out.println("GSMHandler:waitForNetRegistration()-");
        return registered;
    }

    // wait until module is data package attached
    public boolean waitForGPRSAttach(long timeout_ms) {
        System.out.println("GSMHandler:waitForGPRSAttach()+");
        long timestamp = System.currentTimeMillis();
        boolean attached = false;
        String rsp = "";
        rsp = sendATC("AT+CGATT=1\r");
        while ((!attached) && (rsp.indexOf("ERROR") < 0)) {
            rsp = sendATC("AT+CGATT?\r");
            if (rsp.indexOf("+CGATT: 1") > 0) {
                attached = true;
            } else {
                System.out.print("#");
            }
            if ((System.currentTimeMillis() - timestamp) > timeout_ms) {
                break;
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println("InterruptedException: " + e.getMessage());
            }
        }
        System.out.println("GSMHandler:waitForGPRSAttach()-");
        return attached;
    }

    // enter SIM PIN
    // ATTENTION: make sure the correct PIN is used
    public boolean enterPIN(String pin) {
        System.out.println("GSMHandler:enterPIN()+");
        String response;
        // checks, if SIM PIN is required
        response = sendATC("AT+CPIN?\r");
        if (response.indexOf("READY") > -1) {
            System.out.println("No SIM PIN required");
            System.out.println("GSMHandler:enterPIN()1-");
            return true;
        }
        System.out.println("AT+CPIN=\"" + pin + "\"\r");
        sendATC("AT+CPIN=\"" + pin + "\"\r");

        try {
            Thread.sleep(1000);
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }

        // check if SIM PIN was entered correctly
        response = sendATC("AT+CPIN?\r");
        if (response.indexOf("READY") > -1) {
            System.out.println("PIN accepted");
            System.out.println("GSMHandler:enterPIN()2-");
            return true;
        }
        System.out.println("GSMHandler:enterPIN()3-");
        return false;
    }

    // identifying Home MCC MNC of used SIM
    // accordingly to the MCC MNC a data connection APN can be defined
    // ATTENTION: only 2 digits MNC will be recognized
    public String getHomeMCCMNC() {
        System.out.println("GSMHandler:getHomeMCCMNC()+");
        String mccmnc = null;
        String rsp = null;
        rsp = sendATC("AT+CIMI\r");
        // HINT: CIMI returns without quotes
        if (rsp != null && (0 > rsp.indexOf("ERROR"))) {
            // check if echo is used
            if (rsp.toUpperCase().indexOf("CIMI") > -1) {
                rsp = rsp.substring(8).trim();
            }
            int start_ix_mccmnc = (rsp.indexOf("OK"));
            rsp = rsp.substring(0, start_ix_mccmnc).trim();
            System.out.println("IMSI: " + rsp);
        }
        mccmnc = rsp.substring(0, 5);
        System.out.println("MCCMNC: " + mccmnc);
        System.out.println("GSMHandler:getHomeMCCMNC()-");
        return mccmnc;
    }

    // parse the MNO name
    public String getMNO() {
        System.out.println("GSMHandler:getMNO()+");
        String res = "";
        String copsRes = "";
        int startIx, endIx = 0;

        copsRes = sendATC("AT+COPS?\r");

        if (copsRes != null) {
            startIx = copsRes.indexOf("\"");
            endIx = copsRes.indexOf("\"", startIx + 1);

            res = copsRes.substring(startIx + 1, endIx);
        }
        System.out.println("GSMHandler:getMNO()-");
        return res;
    }

    public void ATEvent(String urc) {
        System.out.println("GSMHandler:ATEvent()+");
        String event = "";
        if (urc != null && urc.indexOf("+CMTI") > 0) {
            String content = getSmsContent(urc, false);
            System.out.println("Sms content: " + content);
            //if(ThingsMQTT.AUDIT_SMS_TO_MQTT
            mainML.publishValue(ThingsMQTT.queueSMS, 0, content);
            mainML.executeAction(content);
        } else if ((urc != null) && (urc.indexOf("CMT") > 0)) {
            System.out.println("SMS URC");
            event = "SMS arrived!";
        } else if ((urc != null) && (urc.indexOf("CIEV") > 0)) {
            System.out.println("CIEV URC");
            event = urc.substring(2, urc.length() - 4);
        } else if ((urc != null) && (urc.indexOf("CREG") > 0)) {
            System.out.println("CREG URC");
            event = getMNO();
        } else {
            System.out.println("Some URC");
            event = urc.substring(1, urc.length() - 1);
        }

        if (!event.equals("")) {
            mainML.gsmEvent(event);
        }
        System.out.println("GSMHandler:ATEvent()-");
    }

    private String getSmsContent(String arg0, boolean deleteSms) {
        // index of received SMS message in the memory
        int idx = arg0.indexOf(",");
        String sms_positionInMemory = arg0.substring(idx + 1, idx + 3);
        // get the SMS content
        String contentOfSms = "";
        try {
            // get the message from the index position
            String response = atc.send("AT+CMGR=" + sms_positionInMemory + "\r");
            System.out.println("Complete Response: " + response);
            for(int x=0;x<response.length();x++) {
                System.out.print(response.charAt(x) + " = ");
                int c = (int) response.charAt(x);
                System.out.println(c);
            }
            //contentOfSms = contentOfSms.substring(contentOfSms.indexOf("\n"), contentOfSms.length());
            
            // delete the SMS
            if (deleteSms) {
                atc.send("AT+CMGD=" + sms_positionInMemory + "\r");
            }
        } catch (Exception e) {
            e.printStackTrace();
            contentOfSms = null;
        }
        return contentOfSms;
    }

    public void RINGChanged(boolean bln) {
        try {
            System.out.println("GSMHandler:RINGChanged()+");
            mainML.surfboard.execute("speaker?1");
            Thread.sleep(200);
            mainML.surfboard.execute("speaker?0");
            Thread.sleep(600);

            System.out.println("GSMHandler:RINGChanged()-");
        } catch (IOException ex) {
            ex.printStackTrace();
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }
    }

    public void DCDChanged(boolean bln) {
        System.out.println("GSMHandler:DCDChanged()+");
        System.out.println("GSMHandler:DCDChanged()-");
    }

    public void DSRChanged(boolean bln) {
        System.out.println("GSMHandler:DSRChanged()+");
        System.out.println("GSMHandler:DSRChanged()-");
    }

    public void CONNChanged(boolean bln) {
        System.out.println("GSMHandler:CONNChanged()+");
        System.out.println("GSMHandler:CONNChanged()-");
    }

    // read APN from a file
    public String getAPN(String mccmnc, String apnFile) {
        System.out.println("GSMHandler:getAPN()+");
        String apn = null;
        String apn_usr = null;
        String apn_pwd = null;
        DataInputStream dis = null;

        if (mccmnc == null) {
            mccmnc = getHomeMCCMNC();
        }

        if (mccmnc == null) {
            return null;
        }

        if (apnFile == null) {
            apnFile = "file:///a:/apncfg.txt";
        }

        try {
            FileConnection fconn = (FileConnection) Connector
                    .open(apnFile);
            // If no exception is thrown, then the URI is valid, but the file
            // may or may not exist.
            if (!fconn.exists()) {
                System.out.println("File doesn't exist");
                fconn.close();
                return null;
            }

            dis = fconn.openDataInputStream();
            if (dis == null) {
                System.out.println("Can't open DIS");
                fconn.close();
                return null;
            }

            byte[] fileBuff = new byte[dis.available()];

            dis.readFully(fileBuff);

            String apncfg = new String(fileBuff);

            int ix_mcc = apncfg.indexOf(mccmnc);

            // if no appropriate MCCMNC found, use 00000 for default
            if (ix_mcc < 0) {
                ix_mcc = apncfg.indexOf("00000");
            }

            if (ix_mcc < 0) {
                return null;
            }

            ix_mcc = ix_mcc + mccmnc.length() + 1;

            apn = apncfg.substring(ix_mcc, apncfg.indexOf(',', ix_mcc));

            apn_usr = apncfg.substring(ix_mcc + apn.length() + 1, apncfg.indexOf(',', ix_mcc + apn.length() + 1));

            apn_pwd = apncfg.substring(ix_mcc + apn.length() + 1 + apn_usr.length() + 1, apncfg.indexOf(';', ix_mcc));

            System.out.println("##### APN:" + apn);
            System.out.println("##### APN_USR:" + apn_usr);
            System.out.println("##### APN_PWD:" + apn_pwd);

        } catch (IOException ioe) {
            System.out.println("Can't open file: " + ioe);
        }
        System.out.println("GSMHandler:getAPN()-");
        return apn;
    }
}
