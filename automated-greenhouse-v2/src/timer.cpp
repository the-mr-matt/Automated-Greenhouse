//----LIBRARIES----
// #include <Arduino.h>
#include "timer.h"

//----IMPLEMENTATIONS----

String SecondsToMinutes(unsigned long totalSeconds)
{
	//calculate minutes
	unsigned long minutes = totalSeconds / 60;
	unsigned long seconds = totalSeconds % 60;

	String output;

	//if the input was less than one minute, don't return with 0 minutes, just seconds
	if(minutes == 0)
	{
		output.concat(seconds);
		output.concat("s");
		return output;
	}

	//return minutes and seconds
	output.concat(minutes);
	output.concat("m ");
	output.concat(seconds);
	output.concat("s  ");

	return output;
};
