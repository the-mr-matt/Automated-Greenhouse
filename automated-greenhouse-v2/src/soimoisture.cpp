//----LIBRARIES----
#include <Arduino.h>
#include "clamp.h"
#include "clock.h"
#include "lcd.h"
#include "pins.h"
#include "soilmoisture.h"
#include "water.h"

//----IMPLEMENTATIONS
SoilMoisture::SoilMoisture(){}

//Determines if the water needs to be turned on or not based on the soil moisture
void SoilMoisture::ProcessSoilMoisture()
{
	Serial.println("process soil moisture");

	bool waterState = GetSoilMoisture() < soilMoistureThreshold;
	if(waterState)
	{
		water.StartWatering(false);
	}
}

//Prints the soil moisture to the LCD
void SoilMoisture::PrintSoilMoisture()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Moisture: ");
	lcd.print(GetSoilMoisture());
	lcd.print("%");
	lcd.setCursor(0, 1);

	/*
	Debug the clock

	lcd.print("hour ");
	lcd.print(clock.Hour());
	Serial.print("hour");
	Serial.println(clock.Hour());
	*/
}

//Returns the average soil moisture
int SoilMoisture::GetSoilMoisture()
{
	//read all sensors
	int soil3 = analogRead(soilPin3);
	int soil4 = analogRead(soilPin4);

	//get average
	float avg = (float)soil3 + (float)soil4;
	avg /= 2.0;

	float normalized = avg / 1024.0;
	float flipped = 1.0 - normalized;
	int percentage = (int)(flipped * 100.0);

	Serial.print("moisture: ");
	Serial.println(ClampInt(percentage, 0, 100));

	return ClampInt(percentage, 0, 100);
}
