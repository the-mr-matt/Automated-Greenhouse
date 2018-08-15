#ifndef window_h
#define window_h

class Window
{
public:
    //constructor
    Window();

    //declarations
    void SetWindow(bool open);
    static bool isOpen;
};

//global
extern Window window;

#endif //window_h
