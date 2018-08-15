//----LIBRARIES----
#include "window.h"
#include "pins.h"
#include "motor.h"
// #include "lcd.h"
#include <Arduino.h>

//----IMPLEMENTATIONS----
Window::Window(){}

//Opens/closes the window
void Window::SetWindow(bool open)
{
	Serial.print("set window to: ");
	Serial.println(open);

	//init timer
	int startTime = millis();

	// lcd.clear();

	//loop until window is fully open
	while(true)
	{
		//turn motor slightly
		motor.Step(1.8, open ? Direction::Anticlockwise : Direction::Clockwise);

		//give message to user
		// lcd.setCursor(0, 0);
		// String message = open ? "Opening Window" : "Closing Window";
		// lcd.print(message);

		//show elapsed time
		int elapsedTime = (millis() - startTime) / 1000;
		// lcd.setCursor(0, 1);
		// lcd.print(elapsedTime);
		// lcd.print("s");
		Serial.print(elapsedTime);
		Serial.println("s");

		//check if the window is now open
		int windowState = digitalRead(open ? windowOpenPin : windowClosedPin);
		if(windowState == HIGH)
		{
			Serial.print("window ");
			Serial.println(open ? "open" : "closed");

			//window is open
			// lcd.clear();
			// String message = open ? "Window Open" : "Window Closed";
			// lcd.print(message);

			//wait for user to read message
			delay(2000);

			//break loop
			return;
		}

		//wait a bit
		delay(25);
	}
}
