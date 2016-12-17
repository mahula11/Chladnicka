// 
// 
// 

#include "TempSensor.h"

float CTempSensor::getSensorCelsius() {
	float r = getSensorImpedance();
	if (r == -1) {
		Serial.println(F("CTempSensor: Zo senzoru neprisla ziadna hodnota"));
		return 88888;
	}

	int indexOfArr = -1;
	for (int i = 0; i < Config::SENSORS_TABLE_SIZE - 1; i++) {
		if ((_table[i + 1].impedance <= r) && (r <= _table[i].impedance)) {
			indexOfArr = i;
			break;
		}
	}
	if (indexOfArr != -1) {
		//float celsius = map(r, _table[indexOfArr + 1].impedance, _table[indexOfArr].impedance, _table[indexOfArr + 1].degC, _table[indexOfArr].degC);
		//* ziskame mapnutu hodnotu nameraneho odporu zo snimaca do tabulky _table
		float celsius = mapfloat(r, _table[indexOfArr + 1].impedance, _table[indexOfArr].impedance, _table[indexOfArr + 1].degC, _table[indexOfArr].degC);
		//* pridame ju do triedy, ktora vypocita priemer z poslednych 10 merani
		_arithAverage.addValueForArithAverage(celsius);
		//* ziskame aritmeticky priemer poslednych 10 merani (teplot v chladnicke/mraznicke)
		//* odcitame 2 od priemeru - tymto zrealnime odmeranu teplotu (voci meranej teplomerom arduino ukazuje o dva stupne viac)
		return _arithAverage.getArithAverage() - 2;
	} else {
		Serial.println(F("CTempSensor: Zo senzora prisla hodnota, ktoru nemame v tabulke hodnot impedancie"));
		return 99999;
	}
}

//* meranie hodnoty rezistoru
//* http://www.circuitbasics.com/arduino-ohm-meter/
float CTempSensor::getSensorImpedance() {
	_raw = getAnalogPinStatus();
	if (_raw) {
		_buffer = _raw * _vIn;
		_vOut = (_buffer) / 1024.0;
		_buffer = (_vIn / _vOut) - 1;
		_rSensor = _r1 * _buffer;
		return _rSensor;
	} else {
		return -1;
	}
}

float CTempSensor::mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}