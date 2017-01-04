// 
// 
// 

#include "TempSensor.h"

CTempSensor::CTempSensor(byte pin, float r1) : CObject(pin, INPUT), _r1(r1) {
	_table = new stSensorTempTable[g_pConfig->getSensorsTableSize()];
	_table[0] = { -30, 175199.63 };
	_table[1] = { -25, 129286.93 };
	_table[2] = { -20, 96358.16 };
	_table[3] = { -15, 72500.36 };
	_table[4] = { -10, 55045.94 };
	_table[5] = {  -5, 42156.97 };
	_table[6] = {   0, 32554.20 };
	_table[7] = {   5, 25338.55 };
	_table[8] = {  10, 19872.17 };
	_table[9] = {  15, 15698.46 };
	_table[10] = { 20, 12487.74 };
	_table[11] = { 25, 10000.00 };
	_table[12] = { 30, 8059.08 };
	_table[13] = { 35, 6534.72 };
	_table[14] = { 40, 5329.87 };
	_table[15] = { 45, 4371.72 };
}

float CTempSensor::getSensorCelsius() {
	float r = getSensorImpedance();
	if (r == -1) {
		Serial.println(F("CTempSensor: Zo senzoru neprisla ziadna hodnota"));
		return 88888;
	}

	int indexOfArr = -1;
	for (int i = 0; i < g_pConfig->getSensorsTableSize() - 1; i++) {
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