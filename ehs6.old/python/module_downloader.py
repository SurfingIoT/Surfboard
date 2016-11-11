#!/usr/bin/python

import serial
import os
import argparse
import binascii
from time import sleep


class WirelessModule(serial.Serial):
    '''
    Connects with Cinterion Wireless Module via com port
    Please check serial.Serial reference for general usage
    '''

    def call(self, cmd):
        ''' Calls AT command ''' 
        self.write( cmd + "\r\n")
    
    
    def read_till(self, *ans):
        '''
        Reads data line by line from the module until one of *ans is met
            returns tuple of (<output>, <matched_ans>)
            usage example: wm.read_till("OK","ERROR")            
        '''
        outbuf = ""
        while(True):
            line = self.readline()
            outbuf += line
            if line == "":
                return (outbuf , None)
                
            for a in ans:
                if a in line:
                    return (outbuf , a)
        
        
    def call_waiting(self, cmd, *ans):
        '''
        Call AT command and wait for one of *ans parameters
            returns tuple of (<output>, <matched_ans>)
            usage example: wm.read_till("ATI","OK","ERROR")
        '''
        self.call(cmd)
        return self.read_till(*ans)
                    
        
    def transmit(self, instream):
        ''' Transmit data from instream to the module until EOF is met '''
        buf = instream.read(1)
        while(buf != ""):
            self.write(buf)
            buf = instream.read(1)
    
    
    def transmit_file(self, path):
        ''' Transmit the whole file to the module '''
        filestream = open(path, 'rb')
        self.transmit(filestream)
        filestream.close()
    


def crc32_from_file(filename):
    fhandle = open(args.srcpath, 'rb')
    crc = binascii.crc32(fhandle.read())
    fhandle.close()
    return crc






if __name__ == "__main__":
    NOT_CONNECTED_ERROR = 1
    NOT_RESPONDING_ERROR = 2
    INCORRECT_RESPONSE_ERROR = 3
    SOURCE_FILE_NOT_FOUND_ERROR = 4
    FILE_NOT_DOWNLOADED_ERROR = 5
    
    
    ###### PARSE COMMAND LINE PARAMETERS ######
    parser = argparse.ArgumentParser()
    parser.add_argument("srcpath", help="path of file to send")
    parser.add_argument("dstpath", help="requested path of file on FFS")
    parser.add_argument("-p", "--port")
    parser.add_argument("-b", "--baudrate", type=int, default=115200)
    parser.add_argument("-t", "--timeout", type=int, default=5)
    args = parser.parse_args()
    
    try:
        ###### STARTING THE APPLICATION ######        
        print "======= MODULE DOWNLOADER ======="
        
        
        ###### CONNECT TO COM PORT ######
        # create WirelessModule object
        try:
            print "> opening %s with %dbps" % (args.port, args.baudrate)
            if args.port.upper().startswith("COM"):
                port = int(args.port[3:])-1
            else:
                port = args.port            
            
            wm = WirelessModule(port=port,
                                baudrate=args.baudrate,
                                timeout=args.timeout)
        except Exception as e:
            raise Exception("Error while connecting serial interface",NOT_CONNECTED_ERROR)
        
        
        ###### CHECKING FIRMWARE VERSION ######
        print "> checking the firmware version"
        
        
        # call ATI command
        fw_version = wm.call_waiting("ATI1","OK")
        # raise exception if timeout expired or the answer is not recognized
        if fw_version[1] == None:
            wm.close()
            raise Exception("The module is not responding",NOT_RESPONDING_ERROR)
        
        print fw_version[0]
        
        # wait for 100 ms
        sleep(0.1)
        
        
        ###### CHECKING SOURCE FILE ######
        print "> checking the source file"
        
        try:
            # checking file size
            # if the file doesn't exist, the exception is raised
            file_size = str(os.stat(args.srcpath).st_size)
            print "> file OK"
        except Exception as e:
            wm.close()
            raise Exception(e,SOURCE_FILE_NOT_FOUND_ERROR)
        
        
        ###### DOWNLOADING FILE ######
        print "> downloading file"
        
        # calling AT^SJDL and waiting for CONNECT answer
        ans = wm.call_waiting("AT^SJDL=1,"+file_size+",\""+args.dstpath+"\"", "CONNECT","ERROR")
        if ans[1] != "CONNECT":
            raise Exception("Incorrect module response", INCORRECT_RESPONSE_ERROR)
        
        # transmiting the content
        wm.transmit_file(args.srcpath)
        
        # check CRC
        response = wm.read_till("CRC32","ERROR")
        local_crc = ("0x%x" % (0xFFFFFFFF & crc32_from_file(args.srcpath)))
        remote_crc = response[0][-12:-2]
        print "> CRC: local %s, remote %s" % (local_crc, remote_crc)
        if local_crc == remote_crc:
            print "> CRC OK"
        else:
            wm.close()
            raise Exception("CRC error",FILE_NOT_DOWNLOADED_ERROR)
        
        # wait for final response
        response = wm.read_till("OK","ERROR")[1]
        if response == "OK":
            print "> file downloaded"
        else:
            wm.close()
            raise Exception("File not downloaded", FILE_NOT_DOWNLOADED_ERROR)
         
         
        ###### CLOSING APPLICATION ######
        print "> closing com port"
        wm.close()
        
        print "> closed"
        exit(0)
        
    except Exception as e:
        print e.args[0]
        exit(e.args[1])
