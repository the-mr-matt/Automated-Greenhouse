//----LIBRARIES----
#include <Arduino.h>
#include <Wire.h>
#include "clock.h"
#include "RTClib.h"

//----CONFIG----
RTC_DS1307 rtc;
bool b_IsDay;

//----IMPLEMENTATIONS----
Clock::Clock(){}

//Initialize the RTC module
void Clock::Initialize()
{
  Wire.begin();
  rtc.begin();

  //override rtc time
  //rtc.adjust(DateTime(2018, 7, 7, 7, 59, 30));

  //set rtc to time at the time of compiling
  // rtc.adjust(DateTime(__DATE__, __TIME__));
}

//Returns true within the allowed hours of day
bool Clock::IsDay()
{
  //get current time
  DateTime now = rtc.now();

  //day time is considered to be at 8AM or 7PM for this project
  return (now.hour() == 8 || now.hour() == 19);
}

int Clock::Hour()
{
    DateTime now = rtc.now();
    return now.hour();
}
