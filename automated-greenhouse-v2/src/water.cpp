//----LIBRARIES----
#include "water.h"
#include "pins.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
Water::Water(){}

void Water::TurnOn()
{
	digitalWrite(solenoidPin, HIGH);
}

void Water::TurnOff()
{
	digitalWrite(solenoidPin, LOW);
}
