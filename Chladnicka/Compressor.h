
#ifndef _COMPRESSOR_h
#define _COMPRESSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Object.h"

class CCompressor :	public CObject {
private:
	//bool _possibleToStart = false;
	//unsigned long _timeWantedStart = 0;
	bool _started = false;
	//* cas nastartovania kompresora (v milisekundach)
	unsigned long _timeStarted = 0;
	bool _startASAP = false;
	bool _stopASAP = true;

	//* udrziava cas, kedy sa ma nastartovat compressor (v milisekundach)
	unsigned long _earliestStartingTime = 0;
	unsigned long _earliestStopingTime = 0;
	//* cas v milisekundach, oneskorenie startu kompresora
	//unsigned long _delayForStart = 0;

public:
	CCompressor() : CObject(g_pConfig->getPinCompressor(), OUTPUT) {

	}

	bool isStarted();
	unsigned long getStartedTime();
	unsigned long getStartingTime();
	unsigned long getStopingTime();
	void setEarliestStartingTime(unsigned long time);

	//* nastavi oneskorenie startu kompresora
	void setDelayForStart();
	void startASAP();
	void stopASAP();
	void loop(unsigned long currentMillis);
protected:
	//* tato metoda nemoze byt volana priamo z vonku, kedze zapnutie kompresoru v nevhodny cas by mohlo nicit kompresor
	//* zapne kompresor
	void start();
	void stop();
};

#endif