#ifndef WIFIEX_H
#define WIFIEX_H

// Arduino ESP8266WiFi Utils library

#include <HardwareSerial.h>
#include <ESP8266WiFi.h>

#ifdef  __cplusplus
extern "C"

#define DEBUG_LED_LIGHT LOW
#define DEBUG_LED_DARK HIGH

// FIXME: custom C++11 funtion not HardwareSerial pointer
    void connectToWPS(HardwareSerial *_debugSerial=0, int _debugLedPin=-1);
#endif

#endif /* WIFIEX_H */
