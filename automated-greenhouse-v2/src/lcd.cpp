//----LIBRARIES----
#include "lcd.h"
#include "pins.h"

//----IMPLEMENTATIONS----
LCD_System::LCD_System(){}

void LCD_System::Initialize()
{
		//initialize lcd for 16 columns and 2 rows
		lcd.begin(16, 2);

		//inform user that the system is Initializing
		lcd.setCursor(0, 0);
		lcd.print("Initializing...");

		//wait for user to read message
		delay(3000);
}

//global
extern LiquidCrystalSerial lcd(lcdPin);
