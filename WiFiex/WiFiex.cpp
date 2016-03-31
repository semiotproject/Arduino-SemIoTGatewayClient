#include "WiFiex.h"

void connectToWPS(HardwareSerial *_debugSerial, int _debugLedPin)
{
    bool _debugLed = false;
    if (_debugLedPin>-1) {
        _debugLed = true;
    }
    // WiFi.begin("",""); // decided to get rid of this
    // Long delay required especially soon after power on.
    delay(4000);
    // Check if WiFi is already connected and if not, begin the WPS process.
    if (WiFi.status() != WL_CONNECTED) {
        if (_debugSerial) {
            _debugSerial->println("\nAttempting connection ...");
        }
        if (_debugLed) {
            digitalWrite(_debugLedPin, DEBUG_LED_LIGHT);
        }
        WiFi.beginWPSConfig();
        // Another long delay required.
        delay(3000);
        if (_debugLed) {
            digitalWrite(_debugLedPin, DEBUG_LED_DARK);
        }
        if (WiFi.status() == WL_CONNECTED) {
            if (_debugSerial) {
                _debugSerial->println("Connected!");
                _debugSerial->println(WiFi.localIP());
                _debugSerial->println(WiFi.SSID());
                _debugSerial->println(WiFi.macAddress());
            }
            if (_debugLed) {
                digitalWrite(_debugLedPin, DEBUG_LED_LIGHT);
            }
        }
        else {
            if (_debugSerial) {
                _debugSerial->println("Connection failed!");
            }
            if (_debugLed) {
                digitalWrite(_debugLedPin, DEBUG_LED_DARK);
            }
        }
    }
    else {
        if (_debugSerial) {
            _debugSerial->println("\nConnection already established.");
        }
        if (_debugLed) {
            digitalWrite(_debugLedPin, DEBUG_LED_LIGHT);
        }
    }
}
