// TempSensor.h

#ifndef _TEMPSENSOR_h
#define _TEMPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Object.h"
#include "ArithAverage.h"

class CTempSensor : CObject {
private:
	struct stSensorTempTable {
		float degC;
		float impedance;
	};

	stSensorTempTable _table[Config::SENSORS_TABLE_SIZE] = {
		{ -30, 175199.63 },		//* 0
		{ -25, 129286.93 },		//* 1
		{ -20, 96358.16 },		//* 2
		{ -15, 72500.36 },		//* 3
		{ -10, 55045.94 },		//* 4
		{ -5, 42156.97 },		//* 5
		{ 0, 32554.20 },		//* 6
		{ 5, 25338.55 },		//* 7
		{ 10, 19872.17 },		//* 8
		{ 15, 15698.46 },		//* 9
		{ 20, 12487.74 },		//* 10
		{ 25, 10000.00 },		//* 11
		{ 30, 8059.08 },		//* 12
		{ 35, 6534.72 },		//* 13
		{ 40, 5329.87 },		//* 14
		{ 45, 4371.72 } };		//* 15

	//* 19000 -
	//* 20000 - 10
	//* 24000 - 7
	//* 25000 - 6
	//* 27000 - 4
	//* 28000 - 4
	//* 29000 - 3
	//* 30000 - 2
	//* 50000 -

	int _raw = 0;
	int _vIn = 5;
	float _vOut = 0;
	float _r1 = 0;  //* rezistor, ktoreho hodnota je znama, zapojeny do serii so senzorom
	float _rSensor = 0;
	float _buffer = 0;
	CArithAverage _arithAverage;

public:
	CTempSensor(byte pin, float r1) : CObject(pin, INPUT), _r1(r1) {}

	//*  _r1  _rSensor  vypocitana hodnota
	//* 80000  129286 -> 129514.07    -25°C
	//* 90000  129286 -> 129428.58 *
	//* 95000  129286 -> 129665.13
	//* 100000 129286 -> 129596.42
	//* 80000   42157 -> 42268.66     -5°C
	//* 90000   42157 -> 42223.82
	//* 95000   42157 -> 42207.34  *
	//* 100000  42157 -> 42222.23

	//* 80000  32554  -> 32682.26      0°C
	//* 90000  32554  -> 32716.37
	//* 95000  32554  -> 32664.04
	//* 100000 32554  -> 32642.49  *
	//* 80000  15698  -> 15812.86     15°C
	//* 90000  15698  -> 15809.41 
	//* 95000  15698  -> 15797.27  *
	//* 100000 15698  -> 15837.11
public:
	float getSensorCelsius();
	//* meranie hodnoty rezistoru
	//* http://www.circuitbasics.com/arduino-ohm-meter/
	float getSensorImpedance();
private:
	float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
};

class CTempSensorFridge : public CTempSensor {
private:
public:
	CTempSensorFridge() : CTempSensor(Config::pinSensorFridge, 80000) {}
};

class CTempSensorFreezer : public CTempSensor {
private:
public:
	CTempSensorFreezer() : CTempSensor(Config::pinSensorFreezer, 80000) {}
};


#endif

