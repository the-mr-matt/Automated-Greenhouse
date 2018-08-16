//----LIBRARIES----
#include <A4988.h>
#include "motor.h"
#include "pins.h"

//----CONFIG----
A4988 stepper(motorSteps, motorDirPin, motorStepPin);

//----IMPLEMENTATIONS----
Motor::Motor(){}

//Initialize the motor
void Motor::Initialize()
{
	//30 RPM, no microstepping
	stepper.begin(30, 1);
}

//Turn the motor by the given angle and in the given direction
void Motor::Step (float degrees, Direction direction)
{
	degrees *= direction == Direction::Clockwise ? 1 : -1;
	stepper.rotate(degrees);
}
