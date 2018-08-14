// #include "DHT.h"
// #include <Arduino.h>
// #include "temperature.h"
//
// DHT temp;
//
// Temperature::Temperature(){}
//
// void Temperature::Initialize()
// {
// 	temp.setup(12);
// }
//
// int Temperature::GetTemperature()
// {
// Serial.print(temp.getStatusString());
// Serial.print(", ");
// Serial.print(temp.getTemperature());
// Serial.print(", ");
// Serial.println(temp.getHumidity());
//
// 	return 0;
// }





//----LIBRARIES----
#include <Arduino.h>
#include "temperature.h"
#include "DHT.h"
#include "pins.h"
#include "lcd.h"
#include "window.h"

//----CONFIG----
DHT tempSensor;
bool isTemperatureOverThreshold;
TempAndHumidity values;

//----BOOK KEEPING----
bool b_IsTemperatureOverThreshold;

//----IMPLEMENTATIONS
Temperature::Temperature(){}

void Temperature::Initialize()
{
	tempSensor.setup(12);
}

void Temperature::ProcessTemperature()
{
	Serial.println("process temperature");

	//get current values
	values = GetValues();

	//check if the temperature is currently over the threshold
	isTemperatureOverThreshold = values.temperature > temperatureThreshold;

	//temperature has changed
	if(b_IsTemperatureOverThreshold != isTemperatureOverThreshold)
	{
		Serial.println("temp over threshold changed");

		//set window
		if(isTemperatureOverThreshold)
		{
			window.SetWindow(true);
		}
		else
		{
			window.SetWindow(false);
		}
	}

	//save for next frame
	b_IsTemperatureOverThreshold = isTemperatureOverThreshold;
}

//Prints the temperature and humidity to the LCD
void Temperature::PrintTemperature()
{
	//debug
	Serial.print("temperature: ");
	Serial.print(values.temperature);
	Serial.println("*C");

	Serial.print("humidity: ");
	Serial.print(values.humidity);
	Serial.println("%");

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

TempAndHumidity Temperature::GetValues()
{
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
	tempAndHumidity.humidity = humidity;

	return tempAndHumidity;
}
