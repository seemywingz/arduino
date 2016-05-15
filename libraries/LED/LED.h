#ifndef LED_h
#define LED_h

#include "Arduino.h"
#include "Pin.h"

class LED{
	public:
		boolean ison;

		LED(){};

		LED(int p){
			hot = new Pin(p);
		};//..

		LED(int p,int g){
			hot = new Pin(p);
			gnd = new Pin(g);
			gnd->set(LOW);
		};//..


		void on(){
			hot->on();
			ison = true;
		};//..

		void off(){
			hot->off();
			ison = false;
		};//..

		void setHot(int s){
			hot->set(s);
		};

		Pin* getHot(){
			return hot;
		};

		boolean isOn(){
			return ison;
		};

	protected:
		Pin *hot, *gnd;
};
#endif
