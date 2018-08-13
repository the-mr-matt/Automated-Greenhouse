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

//----CONFIG----
int startMillis;
enum DisplayMode { Temperature, SoilMoisture };
DisplayMode displayMode = DisplayMode::Temperature;

void setup()
{
	lcd_system.Initialize();
	motor.Initialize();
	clock.Initialize();
	water.TurnOff();
	window.SetWindow(true);

	startMillis = millis();
}

void loop()
{


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

//returns state of the pin after debouncing
bool Debounce(int pin)
{

}
