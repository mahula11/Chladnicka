// Door.h

#ifndef _DOOR_h
#define _DOOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CDoor : CObject {
private:
	unsigned long _openDoorTime = 0;
	unsigned long _lastOpenTime = 0;
	bool _isDoorOpen = false;
	bool _isAlarm = false;

public:
	CDoor() : CObject(Config::pinDoorsSwitch, INPUT) {}

	bool isDoorOpen();
	bool isAlarm();
	unsigned long getLastOpenTime();
	void resetLastOpenTime();
	void loop(unsigned long currentMillis);
};


#endif

