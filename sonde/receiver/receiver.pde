#include <SoftwareSerial.h>

SoftwareSerial tarjeta(2, 3);
void setup() {
  Serial.begin( 38400 );
  tarjeta.begin( 9600 );
  Serial.println("Init...");
}

void loop() {
  Serial.write( tarjeta.read() );
}

