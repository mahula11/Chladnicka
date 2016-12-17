// Ventilator.h

#ifndef _VENTILATOR_h
#define _VENTILATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CVentilator : CObject {
private:
	unsigned long _timeStart = 0;
	unsigned long _timeStop = 0;
	unsigned long _runnigTime = Config::VENTILATOR__RUNNING_TIME;
	bool _statusVentilator = false;
	bool _wasTemporaryStoped = false;
public:
	CVentilator() : CObject(Config::pinVentilator, OUTPUT) {}

	void startVentilator();
	void stopVentilator();
	//* should by je pouzite preto, ze sice by mal byt nastartovany ale docasne je stopnuty (pretoze su dvere otvorene)
	bool shouldByStarted();
	void loop(unsigned long currentMillis, bool temporaryStopVentilator, bool compressorIsNotRunnigForFridge);
};

#endif

