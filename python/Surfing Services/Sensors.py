#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
import sys, time
import serial
import json
import paho.mqtt.client as mqtt
from pymongo import MongoClient
from threading import Thread

sys.path.insert(0, '..')

#sandbox
#HOST_NAME = "iot.eclipse.org" 
#docker cloud
#HOST_NAME = "mosquitto-89d8f9d2-1.541de20a.cont.dockerapp.io"
#respberry
HOST_NAME = "localhost" 
HOST_PORT = 1883
# LINUX
SERIAL_PORT = "/dev/ttyUSB0"
# MAC
#SERIAL_PORT = "/dev/cu.wchusbserialfd120"
CHANNEL_SUB = "myhomework/surfboard177"
CHANNEL_SENSORS = "myhomework/surfboard177/sensors"
PARAM_INPUT = "sensors"
#mlab
#MONGO_URI = "mongodb://pi:senhapi@ds036069.mlab.com:36069/vamoscore"
#raspberry
MONGO_URI = "mongodb://pi:senhapi@localhost:27017/vamoscore"

# Class to run on thread
class ListRemoteCommands(Thread):
    def __init__ (self):
        Thread.__init__(self)
    
    def run(self):
        mqttClient.loop_forever()

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print("Connected with MQTT")
    # subscrobe on a remote channel to recive commands
    client.subscribe(CHANNEL_SUB)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # if they are from the same channel
    if msg.topic == CHANNEL_SUB:
        print(msg.topic + " " + str(msg.payload))
        # Write command on serial port
        comport.write(msg.payload)

# listen the serial port and get the sensor's data and post via mqtt, and nosql remote data history 
def publish():
    # MongoDB Client
    mongoClient = MongoClient(MONGO_URI)
    # Mongo remote db name
    db = mongoClient.vamoscore;
    # Infinite loop
    while True:
        try:
            # Write 'sensors' on board to reacive all sensors names and values updated from serial port.
            comport.write(PARAM_INPUT)
            # Time to wait until write and read.
            time.sleep(1.8)
            # get value from serial port
            VALUE_SERIAL = comport.readline()
            # parse to json object
            jsonSave = json.loads(VALUE_SERIAL)
            if jsonSave: # if sucess on parse json
                # print '\n %s' % (jsonSave)
                # save remote on nosql remote db
                db.components.insert_one(jsonSave)
                # publish on mqtt broker
                mqttClient.publish(CHANNEL_SENSORS, VALUE_SERIAL)
        except Exception:
            pass

# connect serial port with surfboard
comport = serial.Serial(SERIAL_PORT, 9600, timeout = 3)

# Time to wait until write and read.
time.sleep(1.8)

# mqtt
mqttClient = mqtt.Client()
mqttClient.on_connect = on_connect
mqttClient.on_message = on_message
mqttClient.connect(HOST_NAME, HOST_PORT, 60)

# Main Execution
a = ListRemoteCommands()
a.start()
time.sleep(5)
publish()