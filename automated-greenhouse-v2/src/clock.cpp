//----LIBRARIES----
#include "clock.h"
#include <Wire.h>
#include "RTClib.h"

//----CONFIG----
RTC_DS1307 rtc;
bool b_IsDay;

//----IMPLEMENTATIONS----
Clock::Clock(){}

bool Clock::schedule = false;

void Clock::Initialize()
{
  Wire.begin();
  rtc.begin();
}

void Clock::OnStartDay(void (*callback)())
{
    //if watering was scheduled
    if(schedule)
    {
        //if day status changed
        if(b_IsDay != IsDay() && IsDay())
        {
            callback();
            schedule = false;
        }

        b_IsDay = IsDay();
    }
}

bool Clock::IsDay() {
  //get current time
  DateTime now = rtc.now();

  //day time is considered to be between 8AM and 5PM for this project
  return (now.hour() >= 8 && now.hour() < 17);
}
