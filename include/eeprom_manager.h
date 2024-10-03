#include <Arduino.h>
#include <EEPROM.h>
#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

void saveToEEPROM();
void saveToEEPROM(uint32_t color_mirror, uint32_t color_wall);
void loadFromEEPROM();

#endif