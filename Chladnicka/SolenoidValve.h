// SolenoidValve.h

#ifndef _SOLENOIDVALVE_h
#define _SOLENOIDVALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CSolenoidValve : CObject {
private:
	bool _switchOnFridge = false;
	unsigned long _switched = -1;
public:
	CSolenoidValve();
	void switchValveOnFridge();
	void switchValveOnFreezer();
	bool isSwitchOnFridge();
	void loop(unsigned long currentMillis);
};


#endif

