//----LIBRARIES----
#include <Arduino.h>
#include "clamp.h"

//----IMPLEMENTATIONS----
//Returns an integer clamped within the given range
int ClampInt(int x, int lower, int upper)
{
	return max(lower, min(x, upper));
}
