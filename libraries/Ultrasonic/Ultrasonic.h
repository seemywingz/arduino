#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h";
#include "Pin.h";

class Ultrasonic{

	public:

	Ultrasonic(int t, int e){
		trigger = new Pin(t);
		echo = new Pin(e);
		echo->setPinMode(INPUT);	
	}
	
	double getDistanceCM(){
		trigger->on();  
		delayMicroseconds(10); 
		trigger->off(); 
		duration = pulseIn(echo->getPinNumber(), HIGH); 
		cm = duration/58;
		delayMicroseconds(500);
		return cm;
	}//..

	protected:
		Pin * trigger;
		Pin * echo;
		double cm;
		double duration;
};
#endif
