#ifndef _OBJECT_h
#define _OBJECT_h


#include <arduino.h>
#include "Config.h"

//Config * g_pConfig;

class CObject {
private:
	byte _pin;
	byte _pin2 = -1;
	byte _mode;

protected:
	//Config * _pConfig;
	unsigned long _currentMillis;
	void loop(unsigned long currentMillis);

public:
	CObject(const byte pin, byte mode);
	CObject(const byte pin, const byte pin2, byte mode);
	~CObject();

	void pinWriteD(bool value);
	void pinWriteA(int value);
	void pin2Write(bool value);
	void pinOnHIGH();
	void pin2OnHIGH();
	void pinOnLOW();
	void pin2OnLOW();
	bool getDigitalPinStatus();
	bool getDigitalPin2Status();
	int getAnalogPinStatus();
	int getAnalogPin2Status();
};

#endif