#ifndef PIN_H
#define PIN_H

#include "Arduino.h"

class Pin{

  private:
		int pinNumber;

	public:

	Pin(int pinIN){
		pinNumber = pinIN;
		pinMode(pinNumber,OUTPUT);
	};//..

	void off(){
		digitalWrite(pinNumber,LOW);
	};

	void on(){
		digitalWrite(pinNumber,HIGH);
	};

	void set(int mode){
		digitalWrite(pinNumber,mode);
	};

	int read(){
		return analogRead(pinNumber);
	};

	int getPinNumber(){
		return pinNumber;
	};

	void setPinMode(int mode){
		pinMode(pinNumber,mode);
	};

	void pwm(int val){
		analogWrite(pinNumber,val);
	};

};// end LED

#endif
