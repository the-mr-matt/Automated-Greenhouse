//----LIBRARIES----
#include "motor.h"
#include "pins.h"
#include <A4988.h>

//----CONFIG----
A4988 stepper(motorSteps, motorDirPin, motorStepPin);

//----IMPLEMENTATIONS----
Motor::Motor(){}

void Motor::Initialize()
{
	//30 RPM, no microstepping
	stepper.begin(30, 1);
}

void Motor::Step (float degrees, Direction direction)
{
	degrees *= direction == 0;
	stepper.rotate(degrees);
}
