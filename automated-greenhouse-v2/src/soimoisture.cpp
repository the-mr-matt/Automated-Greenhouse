//----LIBRARIES----
#include "soilmoisture.h"
#include "pins.h"
#include "lcd.h"
#include <Arduino.h>

//----IMPLEMENTATIONS
SoilMoisture::SoilMoisture(){}

//Prints the soil moisture to the LCD
void SoilMoisture::PrintSoilMoisture()
{
	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("SoilMoisture: ");
	lcd.print(GetSoilMoisture());
	lcd.print("%");
}

//Returns the average soil moisture
int SoilMoisture::GetSoilMoisture()
{
	//read all sensors
	int soil1 = analogRead(soilPin1);
	int soil2 = analogRead(soilPin2);
	int soil3 = analogRead(soilPin3);
	int soil4 = analogRead(soilPin4);

	//get average
	float avg = (float)soil1 + (float)soil2 + (float)soil3 + (float)soil4;
	avg /= 4.0;

	float normalized = avg / 1024.0;
	int percentage = (int)(normalized * 100.0);

	return percentage;
}
