#ifndef motor_h
#define motor_h

const int motorSteps = 200;
enum Direction { Clockwise, Anticlockwise };

class Motor
{
public:
    //constructor
    Motor();

    //declarations
    void Initialize();
    void Step(float degrees, Direction direction);
};

//global
extern Motor motor;

#endif //motor_h
