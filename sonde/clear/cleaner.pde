#include "avr/eeprom.h"

void setup() {
  Serial.begin( 38400 );
  eeprom_write_dword( 0, 0 );
  Serial.println("OK");
}

void loop() {
}

