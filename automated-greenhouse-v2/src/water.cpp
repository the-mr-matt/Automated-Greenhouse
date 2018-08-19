//----LIBRARIES----
#include <Arduino.h>
#include "clock.h"
#include "lcd.h"
#include "pins.h"
#include "timer.h"
#include "water.h"

//----IMPLEMENTATIONS----
Water::Water(){}

//Initializes water
void Water::Initialize()
{
	digitalWrite(solenoidPin, LOW);
}

//Turn on/off the water
void Water::StartWatering()
{
	//if water is to be turned on at night, schedule it for the next morning
	if(!clock.IsDay())
	{
		lcd.clear();
		lcd.print("Cannot use water");
		lcd.setCursor(0, 1);
		lcd.print("at night");

		delay(3000);

		lcd.clear();
		lcd.print("Moisture: ");

		//stop the function - we don't want to turn on the water yet
		return;
	}

	//set water
	digitalWrite(solenoidPin, HIGH);

	//message
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Watering...");

	//wait for duration and then turn off
	unsigned long startTime = millis();

	while(true)
	{
		unsigned long elapsedTime = (millis() - startTime);
		unsigned long seconds = (waterDuration - elapsedTime) / 1000;
		String niceTime = SecondsToMinutes(seconds);

		//timer done
		if(elapsedTime > waterDuration)
		{
			//stop water
			digitalWrite(solenoidPin, LOW);

			//debug
			Serial.println("water finished");

			//inform user that watering is finished
			lcd.clear();
			lcd.print("Water Finished");

			//wait for user to read message
			delay(3000);

			return;
		}

		//debug
		Serial.println(niceTime);

		//show user time remaining
		lcd.setCursor(0, 1);
		lcd.print(niceTime);
		lcd.print("     ");

		delay(50);
	}
}
