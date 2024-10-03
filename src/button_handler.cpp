#include "button_handler.h"
#include "led_controller.h"
#include "eeprom_manager.h"
#include <Arduino.h>

#define PIN_COLOR_CHANGE_BUTTON 5
#define LONG_PRESS_TIME 1500
#define TICK_TIME 10

bool colorChangeButtonFlag = false;
bool longPressDetected = false;
unsigned long buttonPushTime = 0;
unsigned long lastTickTime = 0;

void setupButton() {
  pinMode(PIN_COLOR_CHANGE_BUTTON, INPUT_PULLUP);
}

void handleButton() {
  unsigned long currentTime = millis();
  if (currentTime - lastTickTime >= TICK_TIME) {
    if (!digitalRead(PIN_COLOR_CHANGE_BUTTON)) {
      if (!colorChangeButtonFlag) {
        delay(50);
        if(!digitalRead(PIN_COLOR_CHANGE_BUTTON)) {
          colorChangeButtonFlag = true;
          buttonPushTime = currentTime;
        }
      }
      if (currentTime - buttonPushTime >= LONG_PRESS_TIME) {
        longPressDetected = true;
        colorUpdate();
      }
    } else {
      if (colorChangeButtonFlag) {
        colorChangeButtonFlag = false;
        if(longPressDetected) {
          longPressDetected = false;
          saveToEEPROM();
        } else {
          state = static_cast<LedState>((state + 1) % 3);
          handleState();
          saveToEEPROM();
        }
      }
      buttonPushTime = 0;
    }
    lastTickTime = currentTime;
  }
}
