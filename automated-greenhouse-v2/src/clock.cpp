//----LIBRARIES----
#include <Wire.h>
#include "clock.h"
#include "RTClib.h"

//----CONFIG----
RTC_DS1307 rtc;
bool b_IsDay;

//----IMPLEMENTATIONS----
Clock::Clock(){}

bool Clock::schedule = false;

//Initialize the RTC module
void Clock::Initialize()
{
  Wire.begin();
  rtc.begin();

  //override rtc time
  //rtc.adjust(DateTime(2018, 7, 7, 10, 0, 0));

  //set rtc to time at the time of compiling
  //rtc.adjust(DateTime(__DATE__, __TIME__));
}

//Called at the start of each day
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

//Returns true within the allowed hours of day
bool Clock::IsDay() {
  //get current time
  DateTime now = rtc.now();

  //day time is considered to be between 8AM and 5PM for this project
  return (now.hour() >= 8 && now.hour() < 17);
}
