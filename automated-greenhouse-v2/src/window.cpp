//----LIBRARIES----
#include <Arduino.h>
#include "lcd.h"
#include "motor.h"
#include "pins.h"
#include "timer.h"
#include "window.h"

//----IMPLEMENTATIONS----
Window::Window(){}

bool Window::isOpen = false;

//Opens/closes the window
void Window::SetWindow(bool open)
{
	Serial.print("set window to: ");
	Serial.println(open);

	//init timer
	unsigned long startTime = millis();
	Serial.print("start time: ");
	Serial.println(startTime);

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
		unsigned long elapsedTime = ((millis() - startTime) / 1000);
		String niceTime = SecondsToMinutes(elapsedTime);
		lcd.setCursor(0, 1);
		lcd.print(niceTime);
		lcd.print("s");
		Serial.print("current: ");
		Serial.print(millis());
		Serial.print(", elapsed: ");
		Serial.print(niceTime);
		Serial.println("s");

		//check if the window is now open
		int windowState = digitalRead(open ? windowOpenPin : windowClosedPin);
		if(windowState == HIGH)
		{
			Serial.print("window ");
			Serial.println(open ? "open" : "closed");

			//window is open
			lcd.clear();
			String message = open ? "Window Open" : "Window Closed";
			lcd.print(message);

			isOpen = open;

			//wait for user to read message
			delay(2000);

			//break loop
			return;
		}

		//wait a bit
		delay(25);
	}
}
