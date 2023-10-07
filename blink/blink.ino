
#include <Pin.h>

Pin *p = new Pin(LED_BUILTIN);
int mills = 1000;

void setup(){};

void loop()
{
    p->on();
    delay(mills);
    p->off();
    delay(mills);
};
