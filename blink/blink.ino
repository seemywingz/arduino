
#include <Pin.h>

Pin *p = new Pin(LED_BUILTIN);
int mills = 100;

void setup()
{
};

void loop()
{
    p->on();
    delay(mills);           // wait for a second
    p->off(); 
    delay(mills);
};
