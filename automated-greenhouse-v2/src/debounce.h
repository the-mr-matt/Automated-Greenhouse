#ifndef debounce_h
#define debounce_h

//----DECLARATIONS----
void Debounce(int pin, int debounceTime, int prevState, void (*callback)());
const int debounceDuration = 50;	//ms

#endif //debounce_h
