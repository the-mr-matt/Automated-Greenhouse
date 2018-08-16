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
DisplayMode displayMode = DisplayMode::Temperature;

void setup()
{
	//serial for debugging
	Serial.begin(9600);
	Serial.println("initializing");

	//initialize system
	lcd_system.Initialize();
	motor.Initialize();
	clock.Initialize();
	temperature.Initialize();
	water.Initialize();
	window.SetWindow(true);
}

void loop()
{
	//manual override buttons
	auto windowCallback = [](){ Serial.println("window override"); window.SetWindow(!window.isOpen); };
	auto waterCallback = [](){ Serial.println("water override"); displayMode = DisplayMode::SoilMoisture; water.StartWatering(); };
	Debounce(windowOverridePin, &windowDebounceTime, &prevWindowState, windowCallback);
	Debounce(waterOverridePin, &waterDebounceTime, &prevWaterState, waterCallback);

	//wait for refresh interval - process values and switch modes
	long timeSinceLastDisplayModeChange = currentMillis - startMillis;
	if(timeSinceLastDisplayModeChange >= refreshInterval)
	{
		Serial.println("tick");

		//switch modes
		if(displayMode == DisplayMode::Temperature)
		{
			//switch mode
			displayMode = DisplayMode::SoilMoisture;
			Serial.println("soil moisture mode");

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Moisture: ");
		}
		else
		{
			//switch mode
			displayMode = DisplayMode::Temperature;
			Serial.println("temperature mode");

			//display to lcd
			temperature.PrintTemperature();
		}

		//process
		if(displayMode == DisplayMode::Temperature)
		{
			//display to lcd
			temperature.ProcessTemperature();
		}
		else
		{
			//display to lcd
			soilMoisture.ProcessSoilMoisture();
		}

		startMillis = millis();
	}

	//display current values
	if(displayMode == DisplayMode::SoilMoisture)
	{
		//display to lcd
		soilMoisture.PrintSoilMoisture();
	}

	//get the new current time
	currentMillis = millis();

	// delay(50);
}
