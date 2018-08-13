#ifndef lcd_h
#define lcd_h

//----LIBRARIES----
#include <LiquidCrystalSerial.h>

class LCD_System
{
public:
  //constructor
  LCD_System();

  //declarations
  void Initialize();
};

//global
extern LiquidCrystalSerial lcd;
extern LCD_System lcd_system;

#endif //lcd_h
