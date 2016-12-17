// Buzzer.h

#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CBuzzer : CObject {
private:
	bool _isAlarm = false;
	unsigned long _alarmStart = 0;
	unsigned long _beepInterval = 0;

public:
	CBuzzer() : CObject(Config::pinBuzzer, OUTPUT) {}

	void beep();
	//* nastavi priznak zapnutia pipaca
	//* dalsim prechodom cez loop() sa pipac zapne/vypne
	void setAlarm(int alarmType);
	//* nastavi priznak zrusenia alarmu
	void resetAlarm();
	void loop(unsigned long currentMillis);
};


#endif

