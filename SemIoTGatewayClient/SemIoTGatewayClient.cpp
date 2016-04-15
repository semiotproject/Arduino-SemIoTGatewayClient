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
    _localUDP = new WiFiUDP();
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

void SemIoTGatewayClient::sendCounters(char *modelWord, char *idWord, unsigned int *counter, bool *counterChanged, bool *needToReconnect)
{
    if (*needToReconnect==true) {
        // connectToSemIoTGateway(); FIXME
        *needToReconnect = false;
    }
    if (*counterChanged) {
        // FIXME: check for overflow
        // TODO: writeCountersToEeprom();
        if (WiFi.status() == WL_CONNECTED) {
            // FIXME: _gatewayIp, _udpPort
            if (!_udp->beginPacket("88.201.205.72", 49470)) {
                *needToReconnect=true;
            }
            memcpy(modelWord,_packet.model,sizeof(_packet.model));
            memcpy(idWord,_packet.id,sizeof(_packet.id));
            _udp->write(modelWord);
            // TODO: separate to lib:
            /*
            _udp->write((*counter >> 24) & 0xFF);
            _udp->write((*counter >> 16) & 0xFF);
            _udp->write((*counter >> 8) & 0xFF);
            _udp->write((*counter >> 0) & 0xFF);
            // FIXME: cycle:
            _udp->write(_mac[0]);
            _udp->write(_mac[1]);
            _udp->write(_mac[2]);
            _udp->write(_mac[3]);
            _udp->write(_mac[4]);
            _udp->write(_mac[5]);
            */
            if (!_udp->endPacket()) {
                *needToReconnect=true;
            }
            if (!*needToReconnect) {
                *counterChanged=false;
            }
            if (_debugSerial) {
                _debugSerial->print("counter = ");
                _debugSerial->println(*counter,DEC);
            }
        }
        else {
            *needToReconnect=true;
        }
    }
}
