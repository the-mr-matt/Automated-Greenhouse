//----LIBRARIES----
#include <Arduino.h>
#include "debounce.h"

//----IMPLEMENTATIONS----
//Returns state of the pin after debouncing
void Debounce(int pin, unsigned long* debounceTime, int* prevState, void (*callback)())
{
	//read current state of given pin
	int state = digitalRead(pin);

	// Serial.print("state: ");
	// Serial.print(state);
	//
	// Serial.print(", prevState: ");
	// Serial.print(*prevState);
	//
	// Serial.print(", debounceTime: ");
	// Serial.print(*debounceTime);
	//
	// Serial.print(", millis: ");
	// Serial.println(millis());

	//check if the state has changed since last frame
	if(state != *prevState && state == HIGH)
	{
		Serial.println("----CHANGED----");
		//get current time and state
		*debounceTime = millis();
		*prevState = state;
	}

	//check if enough time has passed to consider the next input
	if((millis() - *debounceTime) > debounceDuration)
	{
		//check if it is currently pressed
		if(state == HIGH)
		{
			callback();
		}
	}
}
