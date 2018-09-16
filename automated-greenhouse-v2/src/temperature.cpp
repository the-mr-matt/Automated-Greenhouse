//----LIBRARIES----
#include <Arduino.h>
#include <SPI.h>
#include "clamp.h"
#include "DHT.h"
#include "lcd.h"
#include "pins.h"
#include "temperature.h"
#include "window.h"

//----CONFIG----
DHT tempSensor;
bool isTemperatureOverThreshold;
TempAndHumidity values;

//----BOOK KEEPING----
bool b_IsTemperatureOverThreshold;

//----IMPLEMENTATIONS
Temperature::Temperature(){}

//Initialize the temperature sensor
void Temperature::Initialize()
{
	tempSensor.setup(12);
	//ProcessTemperature();
}

//Determines if the windows need to be opened/closed based on the temperature
void Temperature::ProcessTemperature()
{
	Serial.println("process temperature");
	
	//get current values
	values = GetValues();
	//
	// //check if the temperature is currently over the threshold
	// isTemperatureOverThreshold = values.temperature > temperatureThreshold;
	//
	// //temperature has changed
	// if(b_IsTemperatureOverThreshold != isTemperatureOverThreshold)
	// {
	// 	Serial.println("temp over threshold changed");
	//
	// 	//set window
	// 	if(isTemperatureOverThreshold)
	// 	{
	// 		window.SetWindow(true);
	// 	}
	// 	else
	// 	{
	// 		window.SetWindow(false);
	// 	}
	// }
	//
	// //save for next frame
	// b_IsTemperatureOverThreshold = isTemperatureOverThreshold;
}

//Prints the temperature and humidity to the LCD
void Temperature::PrintTemperature()
{
	lcd.clear();

	//temperature
	lcd.setCursor(0, 0);
	lcd.print("Temp: ");
	lcd.print(values.temperature);
	lcd.print("*C");

	//humidity
	lcd.setCursor(0, 1);
	lcd.print("Humidity: ");
	lcd.print(values.humidity);
	lcd.print("%");
}

//Get the current temperature and humidity from the sensor
TempAndHumidity Temperature::GetValues()
{
	//terminate SPI to allow communication on pin 12
	SPI.end();

	TempAndHumidity tempAndHumidity;

	//get current values
	int temperature = (int)tempSensor.getTemperature();
	int humidity = (int)tempSensor.getHumidity();

	//check for valid numbers
	if(isnan(temperature) || isnan(humidity))
	{
		tempAndHumidity.temperature = defaultTemperature;
		tempAndHumidity.humidity = defaultHumidity;
		return tempAndHumidity;
	}

	//values are as expected
	tempAndHumidity.temperature = temperature;
	tempAndHumidity.humidity = ClampInt(humidity, 0, 100);

	//resume normal SPI behaviour
	SPI.begin();

	return tempAndHumidity;
}
