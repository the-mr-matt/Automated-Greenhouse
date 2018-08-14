//----LIBRARIES----
#include "temperature.h"
#include "dht.h"
#include "pins.h"
#include "lcd.h"
#include "window.h"

//----CONFIG----
dht tempSensor;
bool isTemperatureOverThreshold;

//----BOOK KEEPING----
bool b_IsTemperatureOverThreshold;

//----IMPLEMENTATIONS
Temperature::Temperature(){}

void Temperature::ProcessTemperature()
{
	//check if the temperature is currently over the threshold
	isTemperatureOverThreshold = temperature.GetTemperature() > temperatureThreshold;

	//temperature has changed
	if(b_IsTemperatureOverThreshold != isTemperatureOverThreshold)
	{
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
	lcd.clear();

	//temperature
	lcd.setCursor(0, 0);
	lcd.print("Temp: ");
	lcd.print(GetTemperature());
	lcd.print("°C");

	//humidity
	lcd.setCursor(0, 1);
	lcd.print("Humidity: ");
	lcd.print((int)GetHumidity());
	lcd.print("%");
}

//Returns the current temperature of the greenhouse
int Temperature::GetTemperature()
{
	int result = tempSensor.read11(tempSensorPin);

	//check the reading is correct
	if(result == DHTLIB_OK)
	{
		return tempSensor.temperature;
	}

	return 0;
}

//Returns the current humidity of the greenhouse
float Temperature::GetHumidity()
{
	int result = tempSensor.read11(tempSensorPin);

	//check the reading is correct
	if(result == DHTLIB_OK)
	{
		return tempSensor.humidity;
	}

	return 0;
}
