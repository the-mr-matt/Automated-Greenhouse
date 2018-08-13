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
	isTemperatureOverThreshold = temperature.GetTemperature() > temperatureThreshold;

	//temperature has changed
	if(b_IsTemperatureOverThreshold != isTemperatureOverThreshold)
	{
		if(isTemperatureOverThreshold)
		{
			window.SetWindow(true);
		}
		else
		{
			window.SetWindow(false);
		}
	}

	b_IsTemperatureOverThreshold = isTemperatureOverThreshold;
}

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

int Temperature::GetTemperature()
{
	int result = tempSensor.read11(tempSensorPin);

	if(result == DHTLIB_OK)
	{
		return tempSensor.temperature;
	}

	return 0;
}

float Temperature::GetHumidity()
{
	int result = tempSensor.read11(tempSensorPin);

	if(result == DHTLIB_OK)
	{
		return tempSensor.humidity;
	}

	return 0;
}
