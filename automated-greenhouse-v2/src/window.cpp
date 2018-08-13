//----LIBRARIES----
#include "window.h"
#include "pins.h"
#include "motor.h"
#include "lcd.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
// extern bool isWindowOpen = false;

Window::Window(){}

void Window::SetWindow(bool open)
{
	//init timer
	int startTime = millis();

	lcd.clear();

	//loop until window is fully open
	while(true)
	{
		//turn motor slightly
		motor.Step(1.8, open ? Direction::Anticlockwise : Direction::Clockwise);

		//give message to user
		lcd.setCursor(0, 0);
		String message = open ? "Opening Window" : "Closing Window";
		lcd.print(message);

		//show elapsed time
		lcd.setCursor(0, 1);
		lcd.print((millis() - startTime) / 1000);

		//check if the window is now open
		int windowState = digitalRead(windowOpenPin);
		if(windowState == HIGH)
		{
			//window is open
			lcd.clear();
			String message = open ? "Window Open" : "Window Closed";
			lcd.print(message);

			//wait for user to read message
			delay(2000);

			//break loop
			return;
		}

		//wait a bit
		delay(25);
	}
}
