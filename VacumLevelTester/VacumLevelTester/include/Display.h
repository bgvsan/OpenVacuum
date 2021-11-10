#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <Adafruit_SSD1306.h>

class Display{

public:
Display();
void Setup();

protected:
Adafruit_SSD1306 SSD1306_display;

private:

};

#endif // _DISPLAY_H_
