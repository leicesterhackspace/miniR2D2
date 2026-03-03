# miniR2D2
ESP32 code for a mini R2D2 based on a design by Matt Zwart

The original code used an Arduino and a bluetooth receiver
It has been modified to use an ESP32 which has built in wifi and bluetooth.

Currently under modification to use a smaller footprint TTGO device and also to enable OTA updates.

It is controlled by a Nintendo Wii controller.

Modifications are required to enable OTA updates.

The code is currently built using the Arduino interface and the following libraries arerequired:
  ESP32MX1508 motor driver
  Adafruit_PWMServoDriver.h
  Bluepad32.h

The first two libraries  are installed via the Arduino Library manager.

For bluepad32 follow these instructions
  https://github.com/ricardoquesada/bluepad32/blob/main/docs/plat_arduino.md

  Then make sure you select a board from the esp32+bluepad dropdown menu under boards manager
  
