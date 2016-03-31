# Arduino-libraries
Arduino Library (currently only for ESP8266) aims to simplify Internet of Thing devices prototyping (WPS, impulse counters EEPROM saving, etc).

## SemIoTGatewayClient

Library to connect your Arduino-based device to WiFi-network via WPS and search for server via UDP.

### examples

+ WPS Wi-Fi connection

Connect your Arduino-based device to WiFi-network via WPS:

    #include "SemIoTGatewayClient.h"

    SemIoTGatewayClient semiotGtwClient;

    void setup() {
        Serial.begin(115200);
        Serial.println("Start WPS connection via your router");
        semiotGtwClient.connectToWPS();
    }

    void loop() {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Connected to WPS");
            delay(1000);
        }
    }



## EEPROMex

From [https://github.com/thijse/Arduino-EEPROMEx](https://github.com/thijse/Arduino-EEPROMEx)

## Energomera CE102

From [https://github.com/semiotproject/Energomera-CE102-lib.git](https://github.com/semiotproject/Energomera-CE102-lib.git)
