#ifndef water_h
#define water_h

//----DEFINITIONS----
const unsigned long waterDuration = 300000;    //1000ms * 60s * 5s= 1 minute

class Water
{
public:
  //constructor
  Water();

  //declarations
  void Initialize();
  void StartWatering(bool overrideClock);
};

//global
extern Water water;

#endif //water_h
