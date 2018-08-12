//----LIBRARIES----
#include "water.h"
#include "pins.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
void TurnOnWater()
{
	digitalWrite(solenoidPin, HIGH);
}

void TurnOffWater()
{
	digitalWrite(solenoidPin, LOW);
}
