//----LIBRARIES----
#include <Arduino.h>
#include "header.h"
#include "lcd.h"
#include "motor.h"
#include "clock.h"
#include "window.h"
#include "water.h"
#include "temperature.h"
#include "soilmoisture.h"
#include "pins.h"
#include "debounce.h"

//----CONFIG----
int startMillis;
enum DisplayMode { Temperature, SoilMoisture };
DisplayMode displayMode = DisplayMode::Temperature;

void setup()
{
	//serial for debugging
	Serial.begin(9600);

	//initialize system
	lcd_system.Initialize();
	motor.Initialize();
	clock.Initialize();
	water.SetWater(false);
	window.SetWindow(true);

	startMillis = millis();
}

void loop()
{
	//manual override buttons
	auto windowCallback = [](){ Serial.println("window pressed"); };
	auto waterCallback = [](){ Serial.println("water pressed"); };
	Debounce(windowOverridePin, windowDebounceTime, prevWindowState, windowCallback);
	Debounce(waterOverridePin, waterDebounceTime, prevWaterState, waterCallback);

	//process scheduled water, water is turned on at the start of day if something was scheduled
	auto startDay = [](){ water.SetWater(true); };
	clock.OnStartDay(startDay);

	//wait for refresh interval
	int currentMillis = millis();
	if(currentMillis - startMillis >= refreshInterval)
	{
		if(displayMode == DisplayMode::Temperature)
		{
			//display to lcd
			temperature.ProcessTemperature();
			temperature.PrintTemperature();

			//switch mode
			displayMode = DisplayMode::SoilMoisture;
		}
		else
		{
			//display to lcd
			soilMoisture.PrintSoilMoisture();

			//switch mode
			displayMode = DisplayMode::Temperature;
		}
	}
}
