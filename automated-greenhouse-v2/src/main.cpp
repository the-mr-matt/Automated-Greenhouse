#include <Arduino.h>
#include "lcd.h"
#include "window.h"

void setup()
{
	Serial.begin(9600);

	InitializeLCD();

	lcd.clear();
	lcd.print("opening window");

	OpenWindow();

	lcd.clear();
	lcd.print("finished");
}

void loop()
{
}
