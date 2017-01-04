// 
// 
// 

#include "ArithAverage.h"

CArithAverage::CArithAverage() {
	_sensorValues = new float[g_pConfig->getNumberOfSensorValuesForArithAverage()];
	for (int i = 0; i < g_pConfig->getNumberOfSensorValuesForArithAverage(); i++) {
		_sensorValues[i] = 9999;
	}
}

//* metoda prepisuje NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE prvkov
void CArithAverage::addValueForArithAverage(float newValue) {
	_sensorValues[_index] = newValue;
	//		Serial.print("added: ");
	//		Serial.println(newValue);
	if (_index == g_pConfig->getNumberOfSensorValuesForArithAverage() - 1) {
		_index = 0;
	} else {
		_index++;
	}
}

//* vrati aritmeticky priemer
float CArithAverage::getArithAverage() {
	int ii = 0;
	float val = 0;
	float min = 9999;
	float max = -9999;
	//Serial.print(F("Stat: cur: "));
	//Serial.print(_sensorValues[_index - 1]);
	for (int i = 0; i < g_pConfig->getNumberOfSensorValuesForArithAverage(); i++) {
		if (((int)_sensorValues[i]) != 9999) {
			val += _sensorValues[i];
			//Serial.print(_sensorValues[i]);
			//Serial.print(", ");
			min = _sensorValues[i] < min ? _sensorValues[i] : min;
			max = _sensorValues[i] > max ? _sensorValues[i] : max;
			ii++;
		}
	}
	//Serial.print(F(", min: "));
	//Serial.print(min);
	//Serial.print(F(", max: "));
	//Serial.print(max);
	val = val / ii;
	//Serial.print(F(", avr: "));
	//Serial.println(val);
	return val;
}
