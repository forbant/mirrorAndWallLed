#include <Arduino.h>
#include "wifi_manager.h"
#include "led_controller.h"
#include "web_server.h"
#include "button_handler.h"
#include "eeprom_manager.h"

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  setupWiFi();
  setupLEDs();
  setupWebServer();
  setupButton();

  EEPROM.begin(512);
  loadFromEEPROM();
  
  handleState();
}

void loop() {
  handleDNS();
  handleWebServer();
  handleButton();
}