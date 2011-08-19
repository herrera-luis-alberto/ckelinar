
int analogPin = 3;
int val = 0;

void setup() {
  Serial.begin( 38400 );
  Serial.println("Init...");
}

void loop() {
  Serial.println("val: ");
  val = analogRead(analogPin);
  Serial.println(val);
  delay(1000);

}

