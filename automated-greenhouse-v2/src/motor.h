#ifndef motor_h
#define motor_h

//----DECLARATIONS----
const int motorSteps = 200;

enum Direction { Clockwise, Anticlockwise };

void InitializeMotor();
void Step(float degrees, Direction direction);

#endif //motor_h
