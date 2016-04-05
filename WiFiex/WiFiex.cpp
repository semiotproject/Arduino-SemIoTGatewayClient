#include "WiFiex.h"

void connectToWPS(HardwareSerial *_debugSerial, int _debugLedPin)
{
    bool _debugLed = false;
    if (_debugLedPin>-1) {
        _debugLed = true;
    }
    if (_debugLed) {
        digitalWrite(_debugLedPin, DEBUG_LED_DARK);
    }
    ESP.eraseConfig();
    WiFi.mode(WIFI_AP_STA); // WIFI_STA is ok, WIFI_AP_STA causes problems
    if (_debugSerial) {
        _debugSerial->println("Start WPS connection via your router");
    }
    // TODO:
    while(WiFi.waitForConnectResult() != WL_CONNECTED) {
        if (_debugSerial) {
            _debugSerial->println("WiFi disconnected, trying to connect.");
        }
        WiFi.beginWPSConfig();
        // TODO: delay?
    }
    if (_debugSerial) {
        _debugSerial->println("connected!");
    }
    if (_debugLed) {
        digitalWrite(_debugLedPin, DEBUG_LED_LIGHT);
    }
}
