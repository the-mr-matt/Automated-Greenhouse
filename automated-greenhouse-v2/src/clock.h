#ifndef clock_h
#define clock_h

class Clock
{
public:
  //constructor
  Clock();

  //declarations
  void Initialize();
  void OnStartDay(void(*callback)());
  bool IsDay();
  static bool schedule;
};

extern Clock clock;

#endif //clock_h
