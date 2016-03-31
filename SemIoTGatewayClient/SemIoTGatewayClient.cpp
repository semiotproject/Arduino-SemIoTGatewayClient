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
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.macAddress(_mac);
        gtwSearch();
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
