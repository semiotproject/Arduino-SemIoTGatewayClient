#include "SemIoTGatewayClient.h"

SemIoTGatewayClient::SemIoTGatewayClient(WiFiUDP *udp, int udpPort, HardwareSerial *debugSerial,int debugLedPin):
    _udp(udp),
    _udpPort(udpPort),
    _debugLedPin(debugLedPin),
    _debugSerial(debugSerial)
{
    if (_debugLedPin>=0) {
        _debugLed = true;
    }
    else {
        _debugLed = false;
    }
}

SemIoTGatewayClient::~SemIoTGatewayClient()
{

}

void SemIoTGatewayClient::connectToSemIoTGateway()
{
    while (WiFi.status() != WL_CONNECTED) {
        connectToWPS();
    }
    WiFi.macAddress(_mac);
    gtwSearch();
}

void SemIoTGatewayClient::connectToWPS()
{
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

void SemIoTGatewayClient::gtwSearch()
{
    if (_udp) {
        _udp->begin(_udpPort);
        _gatewayIp = (~WiFi.subnetMask()) | WiFi.gatewayIP();
        // TODO: timeout
        for(;;) {
            if (_debugSerial) {
                _debugSerial->println("sending broadcast:");
                _debugSerial->println(_gatewayIp[0],DEC);
                _debugSerial->println(_gatewayIp[1],DEC);
                _debugSerial->println(_gatewayIp[2],DEC);
                _debugSerial->println(_gatewayIp[3],DEC);
            }
            if (_udp->beginPacket(_gatewayIp, _udpPort)) {
                _udp->write(UDP_GTW_PING_WORD);
                _udp->endPacket();
                if (_debugSerial) {
                    _debugSerial->println("...");
                }
                delay(1500);
                int sz = _udp->parsePacket();
                if (_debugSerial) {
                    _debugSerial->print("sz=");
                    _debugSerial->println(sz,DEC);
                }
                if (sz==UDP_GTW_OK_SIZE) {
                    _udp->read(_gtwOkBuffer,UDP_GTW_OK_SIZE);
                    if (memcmp(_gtwOkBuffer, UDP_GTW_OK_WORD, UDP_GTW_OK_SIZE) == 0) {
                        if (_debugSerial) {
                            _debugSerial->println("found semiot-gateway!");
                        }
                        _gatewayIp=_udp->remoteIP();
                        break;
                    }
                }
            }
        }
    }
    else {
        if (_debugSerial) {
            _debugSerial->println("udp pointer is null");
        }
    }
}

IPAddress SemIoTGatewayClient::gatewayIp()
{
    return _gatewayIp;
}

byte *SemIoTGatewayClient::mac()
{
    return _mac;
}

void SemIoTGatewayClient::readCountersFromEeprom() {
    /*
    //EEPROM.begin(16);
    // TODO: "M201 and newline"
    // NOTE: platform specific:
    // FIXME: magic numbers from packet format:
    int _low_counter = ((unsigned char)(EEPROM.read(4)) << 8) + (unsigned char)EEPROM.read(5);
    if (_low_counter<MAX_COUNTER_LOW_NUMBER) {
        low_counter=_low_counter;
        if (_debugSerial) {
            _debugSerial->print("EEPROM _low_counter= ");
            _debugSerial->println(_low_counter,DEC);
        }
        int _high_counter = ((unsigned char)(EEPROM.read(6)) << 24) + ((unsigned char)(EEPROM.read(7)) << 16) + ((unsigned char)(EEPROM.read(8)) << 8) + (unsigned char)EEPROM.read(9);
        if (_debugSerial) {
            _debugSerial->print("EEPROM _high_counter= ");
            _debugSerial->println(_high_counter,DEC);
        }
        high_counter=_high_counter;
    }
    //EEPROM.end();
    */
}

void SemIoTGatewayClient::writeCountersToEeprom() {
    /*
    // NOTE: platform specific:
    // TODO: "M201 and newline"
    // FIXME: magic numbers from packet format:
    //EEPROM.begin(16);
    EEPROM.write(4,(low_counter >> 8) & 0xFF);
    EEPROM.write(5,(low_counter >> 0) & 0xFF);

    EEPROM.write(6,(high_counter >> 24) & 0xFF);
    EEPROM.write(7,(high_counter >> 16) & 0xFF);
    EEPROM.write(8,(high_counter >> 8) & 0xFF);
    EEPROM.write(9,(high_counter >> 0) & 0xFF);
    EEPROM.commit();
    //EEPROM.end();
    */
}
