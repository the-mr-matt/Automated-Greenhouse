//----LIBRARIES----
#include "window.h"
#include "pins.h"
#include "motor.h"
#include "lcd.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
extern bool isWindowOpen = false;

void OpenWindow()
{
	//init timer
	int startTime = millis();

	//clear LCD
	lcd.clear();

	//loop until window is fully open
	while(!windowOpenPin)
	{
		//turn motor slightly
		// Step(1.8, Direction::Anticlockwise);

		//display time to user
		lcd.setCursor(0, 0);
		lcd.print(millis() - startTime);

		//check if the window is now open
		int windowState = digitalRead(windowOpenPin);
		Serial.println(windowState);
		if(windowState == HIGH)
		{
			//window is open
			lcd.clear();
			lcd.print("Window Open");

			isWindowOpen = true;
		}

		//wait a bit
		delay(25);
	}
}

void CloseWindow()
{

}
