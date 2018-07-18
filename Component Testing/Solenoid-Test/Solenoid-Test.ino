//----CONFIG----
#define PIN 9

void setup()
{
  pinMode(PIN, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(PIN, HIGH);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(PIN, LOW);
  digitalWrite(13, LOW);
  delay(2000);
}
