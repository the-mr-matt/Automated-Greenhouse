//----LIBRARIES----
#include <dht.h>

//----CONFIG----
#define DHTPIN 2

//----STATE----
dht DHT;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int chk = DHT.read11(DHTPIN);
  if(chk == DHTLIB_OK)
  {
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature, 1);
    Serial.print(" Humidity: " );
    Serial.println(DHT.temperature, 1);
  }
  
  delay(2000);
}
