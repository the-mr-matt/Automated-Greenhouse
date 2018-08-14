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
	//if water is to be turned on at night, schedule it for the next morning
	if(state)
	{
		if(!clock.IsDay())
		{
			clock.schedule = true;
		}
	}

	digitalWrite(solenoidPin, state ? HIGH : LOW);
}
