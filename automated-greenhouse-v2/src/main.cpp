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

//----CONFIG----
int startMillis;
enum DisplayMode { Temperature, SoilMoisture };
DisplayMode displayMode = DisplayMode::Temperature;

void setup()
{
	//serial for debugging
	Serial.begin(9600);

	//initialize system
	// lcd_system.Initialize();
	// motor.Initialize();
	// clock.Initialize();
	// water.TurnOff();
	// window.SetWindow(true);

	startMillis = millis();
}

void loop()
{
	Serial.println(digitalRead(4));

	//manual override buttons
	// auto windowCallback = [](){ Serial.println("window pressed"); };
	// auto waterCallback = [](){ Serial.println("water pressed"); };
	// Debounce(windowOverridePin, windowDebounceTime, prevWindowState, windowCallback);
	// Debounce(waterOverridePin, waterDebounceTime, prevWaterState, waterCallback);

	// int currentMillis = millis();
	// if(currentMillis - startMillis >= refreshInterval)
	// {
	// 	if(displayMode == DisplayMode::Temperature)
	// 	{
	// 		//display to lcd
	// 		temperature.ProcessTemperature();
	// 		temperature.PrintTemperature();
	//
	// 		//switch mode
	// 		displayMode = DisplayMode::SoilMoisture;
	// 	}
	// 	else
	// 	{
	// 		//display to lcd
	// 		soilMoisture.PrintSoilMoisture();
	//
	// 		//switch mode
	// 		displayMode = DisplayMode::Temperature;
	// 	}
	// }
}

//returns state of the pin after debouncing
void Debounce(int pin, int debounceTime, int prevState, void (*callback)())
{
	//read current state of given pin
	int state = digitalRead(pin);

	//check if the state has changed since last frame
	if(state != prevState)
	{
		//get current time and state
		debounceTime = millis();
		prevState = state;
	}

	//check if enough time has passed to consider the next input
	if((millis() - debounceTime) > debounceDuration)
	{
		//check if it is currently pressed
		if(state == HIGH)
		{
			//callback();
		}
	}
}
