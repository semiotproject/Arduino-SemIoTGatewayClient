#include "WiFiex.h"

void connectToWPS()
{
    ESP.eraseConfig();
    WiFi.mode(WIFI_AP_STA); // WIFI_STA is ok, WIFI_AP_STA causes problems
    WiFi.softAP(ssid, password);
    while(WiFi.waitForConnectResult() != WL_CONNECTED) {
        WiFi.beginWPSConfig();
    }
}
