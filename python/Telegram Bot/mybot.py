#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
import sys, time
import telebot
import serial

SERIAL_PORT = "/dev/ttyUSB0"
HOST_PORT = 1883

bot = telebot.TeleBot("238691867:AAEE3dODXUXJPpbeH-n7JX-yPUZzLb3QEW0")

@bot.message_handler(commands=['start', 'help'])
def send_welcome(message):
    bot.reply_to(message, "/claridade \n /temperatura \n /humidade \n /potencia \n /ledvermelho_on \n /ledvermelho_off \n /luz_on \n /luz_off")

# Telegram respond message bot
@bot.message_handler(commands=['claridade'])
def send_welcome(message):
    bot.reply_to(message, "Vou consultar a luminosidade, claridade do ambiente...")
    comport.write("light")
    # Time to wait until write and read.
    time.sleep(1.8)
    # get value from serial port
    VALUE_TEMP = comport.readline()
    bot.reply_to(message, VALUE_TEMP)

# Telegram respond message bot
@bot.message_handler(commands=['temperatura'])
def send_welcome(message):
    bot.reply_to(message, "Vou consultar a temperatura do ambiente...")
    comport.write("temp")
    # Time to wait until write and read.
    time.sleep(1.8)
    # get value from serial port
    VALUE_TEMP = comport.readline()
    bot.reply_to(message, VALUE_TEMP)

# Telegram respond message bot
@bot.message_handler(commands=['humidade'])
def send_welcome(message):
    bot.reply_to(message, "Vou consultar a humidade do ambiente...")
    comport.write("humidity")
    # Time to wait until write and read.
    time.sleep(1.8)
    # get value from serial port
    VALUE_TEMP = comport.readline()
    bot.reply_to(message, VALUE_TEMP)

# Telegram respond message bot
@bot.message_handler(commands=['potencia'])
def send_welcome(message):
    bot.reply_to(message, "Vou consultar o valor do potenciômetro...")
    comport.write("pot")
    # Time to wait until write and read.
    time.sleep(1.8)
    # get value from serial port
    VALUE_TEMP = comport.readline()
    bot.reply_to(message, VALUE_TEMP)

# Telegram respond message bot
@bot.message_handler(commands=['ledvermelho_on'])
def send_welcome(message):
    bot.reply_to(message, "Vou ligar o led vermelho...")
    comport.write("red?255")

# Telegram respond message bot
@bot.message_handler(commands=['ledvermelho_off'])
def send_welcome(message):
    bot.reply_to(message, "Vou desligar o led vermelho...")
    comport.write("red?0")

# Telegram respond message bot
@bot.message_handler(commands=['luz_on'])
def send_welcome(message):
    bot.reply_to(message, "Vou ligar a luz...")
    comport.write("relay?1")

# Telegram respond message bot
@bot.message_handler(commands=['luz_off'])
def send_welcome(message):
    bot.reply_to(message, "Vou desligar a luz...")
    comport.write("relay?0")

@bot.message_handler(func=lambda message: True)
def echo_all(message):
    bot.reply_to(message, message.text)

# connect serial port with surfboard
comport = serial.Serial(SERIAL_PORT, 9600, timeout = 3)

# Time to wait until write and read.
time.sleep(1.8)

bot.polling()