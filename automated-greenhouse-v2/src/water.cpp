//----LIBRARIES----
#include "water.h"
#include "pins.h"
#include "clock.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
Water::Water(){}

//Turn on/off the water
void Water::SetWater(bool state)
{
	Serial.print("set water to: ");
	Serial.println(state);

	//if water is to be turned on at night, schedule it for the next morning
	if(state && !clock.IsDay())
	{
		Serial.println("scheduling water for next day");

		//schedule for next day
		clock.schedule = true;

		//stop the function - we don't want to turn on the water yet
		return;
	}

	digitalWrite(solenoidPin, state ? HIGH : LOW);
}
