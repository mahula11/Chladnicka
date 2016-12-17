// ArithAverage.h

#ifndef _ARITHAVERAGE_h
#define _ARITHAVERAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"

class CArithAverage {
private:
	float _sensorValues[Config::NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE];
	byte _index = 0;
public:
	CArithAverage();
	//* metoda prepisuje NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE prvkov
	void addValueForArithAverage(float newValue);
	//* vrati aritmeticky priemer
	float getArithAverage();
};


#endif

