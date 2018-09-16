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
  void Test();
  int Hour();
};

extern Clock clock;

#endif //clock_h
