#ifndef header_h
#define header_h

//declarations
void Debounce(int pin, int debounceTime, int prevState, void (*callback)());
const int refreshInterval = 60000;	//1000ms * 60seconds = 1 minute
const int debounceDuration = 50;	//ms
int windowDebounceTime;
int waterDebounceTime;
int prevWindowState;
int prevWaterState;

#endif //header_h
