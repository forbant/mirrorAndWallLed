#include "wifi_manager.h"

#define APSSID "ESPap"
#define APPSK "thereisnospoon"

const char *ssid = APSSID;
const char *password = APPSK;

DNSServer dnsServer;

void setupWiFi() {
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    dnsServer.start(53, "*", myIP);
}

void handleDNS() {
    dnsServer.processNextRequest();
}