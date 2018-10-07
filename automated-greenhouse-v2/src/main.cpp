//----LIBRARIES----
#include <Arduino.h>
#include "clock.h"
#include "debounce.h"
#include "header.h"
#include "lcd.h"
#include "motor.h"
#include "pins.h"
#include "soilmoisture.h"
#include "temperature.h"
#include "water.h"
#include "window.h"

//----CONFIG----
long startMillis;
long currentMillis;
enum DisplayMode { Temperature, SoilMoisture };
DisplayMode displayMode = DisplayMode::SoilMoisture;

void setup()
{
	//serial for debugging
	Serial.begin(9600);
	Serial.println("Hello");

	//initialize system
	lcd_system.Initialize();
	clock.Initialize();
	lcd.clear();
	lcd.print("Initializing");
	lcd.setCursor(0, 1);
	lcd.print("Temperature...");
	temperature.Initialize();
	delay(2000);
	temperature.ProcessTemperature();
	water.Initialize();

	currentMillis = refreshInterval + millis();
}

void loop()
{
	//manual override buttons
	auto waterCallback = [](){ Serial.println("water override"); displayMode = DisplayMode::SoilMoisture; water.StartWatering(true); };
	Debounce(waterOverridePin, &waterDebounceTime, &prevWaterState, waterCallback);

	//wait for refresh interval - process values and switch modes
	long timeSinceLastDisplayModeChange = currentMillis - startMillis;
	if(timeSinceLastDisplayModeChange >= refreshInterval)
	{
		//switch modes
		if(displayMode == DisplayMode::Temperature)
		{
			//switch mode
			displayMode = DisplayMode::SoilMoisture;
			Serial.println("soil moisture mode");
		}
		else
		{
			//switch mode
			displayMode = DisplayMode::Temperature;
			Serial.println("temperature mode");

			//display to lcd, only once at the start - not live because the sensor can only be read at a maximum of every two seconds, more than this and it starts returning NAN
			temperature.PrintTemperature();
		}

		//process
		if(displayMode == DisplayMode::Temperature)
		{
			//read temperature and humidity from sensor
			temperature.ProcessTemperature();
		}
		else
		{
			//if the moisture is below the threshold the water will be turned on
			soilMoisture.ProcessSoilMoisture();
		}

		startMillis = millis();
	}

	//display current values, outside of the switching block to show a live reading - it can change
	if(displayMode == DisplayMode::SoilMoisture)
	{
		//display to lcd
		soilMoisture.PrintSoilMoisture();
	}

	//get the new current time
	currentMillis = millis();

	delay(50);
}
