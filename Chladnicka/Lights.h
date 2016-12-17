// Lights.h

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CLights : CObject {
private:
	bool _switchLights = false;
	bool _isAlarm = false;
	unsigned long _alarmStart = 0;

public:
	CLights() : CObject(Config::pinLights, OUTPUT) {}

	//* nastavi priznak zapnutia alebo vypnutia osvetlenia
	void switchLights(bool state);
	//* nastavi priznak zapnutia alarmu
	//* najblizsim prechodom cez loop, sa osvetlenie prepne
	void setAlarm();
	//* nastavi priznak vypnutia alarmu
	void resetAlarm();
	void loop(unsigned long currentMillis);
};



#endif

