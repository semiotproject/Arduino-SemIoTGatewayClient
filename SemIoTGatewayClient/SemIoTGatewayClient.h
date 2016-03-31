#ifndef SEMIOTGATEWAYCLIENT_H
#define SEMIOTGATEWAYCLIENT_H
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <HardwareSerial.h>

#define UDP_GTW_OK_SIZE 5
#define UDP_GTW_OK_WORD "GTWOK"
#define UDP_GTW_PING_WORD "GTW"
#define DEBUG_LED_LIGHT LOW
#define DEBUG_LED_DARK HIGH

class SemIoTGatewayClient
{
public:
    // FIXME: custom func instead of HardwareSerial pointer
    // (for udp, for example)
    SemIoTGatewayClient(WiFiUDP *udp=NULL, int udpPort = 33333, HardwareSerial *debugSerial=NULL, int debugLedPin = -1);
    ~SemIoTGatewayClient();
    void connectToSemIoTGateway();
    void gtwSearch();
    IPAddress gatewayIp();
    byte *mac();
    void sendCounters(char *modelWord, unsigned int *counter, bool *counterChanged, bool *needToReconnect);
private:
    WiFiUDP *_udp;
    int _udpPort;
    IPAddress _gatewayIp;
    int _debugLedPin;
    HardwareSerial *_debugSerial;
    bool _debugLed;
    byte _mac[6];
    char _gtwOkBuffer[UDP_GTW_OK_SIZE];
};

#endif // SEMIOTGATEWAYCLIENT_H
