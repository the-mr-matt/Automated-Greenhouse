#ifndef clock_h
#define clock_h

class Clock
{
public:
  //constructor
  Clock();

  //declarations
  void Initialize();
  bool IsDay();
};

extern Clock clock;

#endif //clock_h
