// #include <DHT.h>
// #include <SPI.h>
// #include <LiquidCrystalSerial.h>
//
// DHT dht;
// // LiquidCrystalSerial LCD(9);
//
// void setup() {
//   dht.setup(12);
//   Serial.begin(9600);
//
//   // LCD.begin(16, 2);
//   // LCD.print("hello");
// }
//
// void loop() {
//   // LCD.setCursor(0, 1);
//   // LCD.print(millis() / 1000);
//
//   SPI.end();
//
//   float humidity = dht.getHumidity();
//   float temperature = dht.getTemperature();
//
//   Serial.print(dht.getStatusString());
//   Serial.print("\t");
//   Serial.print(humidity, 1);
//   Serial.print("\t\t");
//   Serial.print(temperature, 1);
//   Serial.print("\t\t");
//   Serial.println(dht.toFahrenheit(temperature), 1);
//
//   SPI.begin();
//
//   delay(2000);
// }


// #include <SPI.h>
// #include <LiquidCrystalSerial.h>
// #include <DHT.h>
//
// LiquidCrystalSerial LCD(9);
// DHT dht;
//
// void setup()
// {
//     Serial.begin(9600);
//
//     dht.setup(12);
//     LCD.begin(16, 2);
//     LCD.print("hello, world!");
//     SPI.end();
// }
//
// void loop()
// {
//     Serial.print("temp: ");
//     Serial.println(dht.getTemperature());
//
//     // SPI.begin();
//     // Serial.println("spi on");
//
//     // LCD.setCursor(0, 1);
//     // LCD.print(millis()/1000);
//
//     // SPI.end();
//     // Serial.println("spi off");
//
//     delay(2000);
// }

// #include <DHT.h>
//
// DHT dht;
//
// void setup() {
//   dht.setup(12);
//   Serial.begin(9600);
// }
//
// void loop() {
//   float humidity = dht.getHumidity();
//   float temperature = dht.getTemperature();
//
//   Serial.print(dht.getStatusString());
//   Serial.print("\t");
//   Serial.print(humidity, 1);
//   Serial.print("\t\t");
//   Serial.print(temperature, 1);
//   Serial.print("\t\t");
//   Serial.println(dht.toFahrenheit(temperature), 1);
//
//   delay(2000);
// }

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
	water.SetWater(false);
	window.SetWindow(true);

	//make the system start with a refresh
	currentMillis = refreshInterval;
}

void loop()
{
	//manual override buttons
	// auto windowCallback = [](){ Serial.println("window pressed"); };
	// auto waterCallback = [](){ Serial.println("water pressed"); };
	// Debounce(windowOverridePin, windowDebounceTime, prevWindowState, windowCallback);
	// Debounce(waterOverridePin, waterDebounceTime, prevWaterState, waterCallback);

	//process scheduled water, water is turned on at the start of day if something was scheduled
	auto startDay = [](){ Serial.println("starting scheduled water"); water.SetWater(true); };
	clock.OnStartDay(startDay);

	//wait for refresh interval
	long timeSinceLastDisplayModeChange = currentMillis - startMillis;
	if(timeSinceLastDisplayModeChange >= refreshInterval)
	{
		Serial.println("tick");

		if(displayMode == DisplayMode::Temperature)
		{
			Serial.println("temperature mode");

			//display to lcd
			temperature.ProcessTemperature();
			temperature.PrintTemperature();

			//switch mode
			displayMode = DisplayMode::SoilMoisture;
		}
		else
		{
			Serial.println("soil moisture mode");

			//display to lcd
			soilMoisture.ProcessSoilMoisture();
			soilMoisture.PrintSoilMoisture();

			//switch mode
			displayMode = DisplayMode::Temperature;
		}

		startMillis = millis();
	}

	//get the new current time
	currentMillis = millis();

	delay(50);
}
