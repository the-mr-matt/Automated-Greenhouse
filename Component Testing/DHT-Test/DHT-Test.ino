#include <DHT.h>

DHT dht;

void setup() {
  dht.setup(12);
  Serial.begin(9600);
}

void loop() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  delay(2000);
}
