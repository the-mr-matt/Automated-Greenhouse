#ifndef soilmoisture_h
#define soilmoisture_h

const int soilMoistureThreshold = 30;	//%

class SoilMoisture
{
public:
    //constructor
    SoilMoisture();

    //declarations
    void ProcessSoilMoisture();
    void PrintSoilMoisture();

private:
    int GetSoilMoisture();
};

//global
extern SoilMoisture soilMoisture;

#endif //soilmoisture_h
