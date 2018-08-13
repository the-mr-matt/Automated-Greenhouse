#ifndef soilmoisture_h
#define soilmoisture_h

const int soilThreshold = 30;	//%

class SoilMoisture
{
public:
    //constructor
    SoilMoisture();

    //declarations
    void PrintSoilMoisture();
    int GetSoilMoisture();
};

//global
extern SoilMoisture soilMoisture;

#endif //soilmoisture_h
