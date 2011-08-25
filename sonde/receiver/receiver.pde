#include <SoftwareSerial.h>

SoftwareSerial tarjeta(2, 3);
void setup() {
  Serial.begin( 38400 );
  tarjeta.begin( 1200 );
  Serial.println("Init...");
}

void loop() {
  Serial.write( tarjeta.read() );
}

