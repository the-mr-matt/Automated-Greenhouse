//----LIBRARIES----
#include "temperature.h"
#include "dht.h"
#include "pins.h"

//----CONFIG----
dht tempSensor;

//----IMPLEMENTATIONS
int GetTemperature()
{
	return tempSensor.read11(tempSensorPin);
}
