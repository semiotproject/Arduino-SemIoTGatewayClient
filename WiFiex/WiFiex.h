#ifndef WIFIEX_H
#define WIFIEX_H

// Arduino ESP8266WiFi Utils library

#include <HardwareSerial.h>
#include <ESP8266WiFi.h>

#ifdef  __cplusplus
extern "C"

#define SOFT_SSID "please-reconfigure-me"
#define SOFT_PASS "str0ngpass"

#define DEBUG_LED_LIGHT LOW
#define DEBUG_LED_DARK HIGH
    void connectToWiFi();
    void connectToWPS();
#endif

#endif /* WIFIEX_H */
