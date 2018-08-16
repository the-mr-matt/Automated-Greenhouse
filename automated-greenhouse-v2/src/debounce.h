#ifndef debounce_h
#define debounce_h

//----DECLARATIONS----
const unsigned long debounceDuration = 50;	//ms
void Debounce(int pin, unsigned long* debounceTime, int* prevState, void (*callback)());

#endif //debounce_h
