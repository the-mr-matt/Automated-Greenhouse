//----LIBRARIES----
#include "clock.h"
#include <Wire.h>
#include "RTClib.h"

//----CONFIG----
RTC_DS1307 rtc;

//----IMPLEMENTATIONS----
void InitializeClock()
{
  Wire.begin();
  rtc.begin();
}

bool IsDay() {
  //get current time
  DateTime now = rtc.now();

  //day time is considered to be between 8AM and 5PM for this project
  return (now.hour() >= 8 && now.hour() < 17);
}
