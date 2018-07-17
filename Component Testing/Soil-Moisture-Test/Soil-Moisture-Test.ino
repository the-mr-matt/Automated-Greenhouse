//----CONFIG----
#define PIN1 A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int moisture = analogRead(PIN1);
  Serial.println(moisture);
}
