// Buzzer.h

#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

enum AlarmType {
	openDoor,
	lowTempRefrig,
	lowTempFreezer
};

class CBuzzer : CObject {
private:
	bool _isAlarm = false;
	unsigned long _alarmStart = 0;
	unsigned long _beepInterval = 0;

public:
	CBuzzer() : CObject(g_pConfig->getPinBuzzer(), OUTPUT) {}

	void beep();
	//* nastavi priznak zapnutia pipaca
	//* dalsim prechodom cez loop() sa pipac zapne/vypne
	void setAlarm(AlarmType alarmType);
	//* nastavi priznak zrusenia alarmu
	void resetAlarm();
	void loop(unsigned long currentMillis);
};


#endif

