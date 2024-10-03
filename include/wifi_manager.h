#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>

void setupWiFi();
void handleDNS();

extern DNSServer dnsServer;

#endif