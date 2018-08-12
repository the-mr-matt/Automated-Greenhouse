//----LIBRARIES----
#include "lcd.h"
#include "pins.h"

//----IMPLEMENTATIONS----
extern LiquidCrystalSerial lcd(lcdPin);

void InitializeLCD()
{
	//initialize lcd for 16 columns and 2 rows
	lcd.begin(16, 2);

	//inform user that the system is Initializing
	lcd.setCursor(0, 0);
	lcd.print("Initializing");

	delay(3000);

	lcd.clear();
}
