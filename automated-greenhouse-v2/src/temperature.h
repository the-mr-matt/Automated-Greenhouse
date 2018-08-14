#ifndef temperature_h
#define temperature_h

const int temperatureThreshold = 30;
const int defaultTemperature = 25;
const int defaultHumidity = 30;

struct TempAndHumidity
{
    int temperature;
    int humidity;
};

class Temperature
{
public:
    //constructor
    Temperature();

    //declarations
    void Initialize();
    void ProcessTemperature();
    void PrintTemperature();

private:
    TempAndHumidity GetValues();
};

//global
extern Temperature temperature;

#endif //temperature_h
