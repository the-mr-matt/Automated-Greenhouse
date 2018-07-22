//----LIBRARIES----
//#include <SPI.h>
#include <LiquidCrystal.h>

//----CONFIG----
LiquidCrystal lcd(9);

void setup()
{
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
}

void loop()
{
  lcd.setCursor(0, 1);
  lcd.print(millis());
}
