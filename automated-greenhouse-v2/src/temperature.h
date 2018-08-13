#ifndef temperature_h
#define temperature_h

const int temperatureThreshold = 30;

class Temperature
{
public:
    //constructor
    Temperature();

    //declarations
    void ProcessTemperature();
    void PrintTemperature();
    int GetTemperature();
    float GetHumidity();
};

//global
extern Temperature temperature;

#endif //temperature_h
