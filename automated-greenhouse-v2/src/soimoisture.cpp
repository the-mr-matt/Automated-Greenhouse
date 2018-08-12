//----LIBRARIES----
#include "soilmoisture.h"
#include "pins.h"
#include <Arduino.h>

//----IMPLEMENTATIONS
float GetSoilMoisture()
{
	//read all sensors
	int soil1 = analogRead(soilPin1);
	int soil2 = analogRead(soilPin2);
	int soil3 = analogRead(soilPin3);
	int soil4 = analogRead(soilPin4);

	//get average
	float avg = (float)soil1 + (float)soil2 + (float)soil3 + (float)soil4;
	avg /= 4.0;

	return avg;
}
