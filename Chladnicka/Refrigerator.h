// Refrigerator.h

#ifndef _REFRIGERATOR_h
#define _REFRIGERATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Compressor.h"
#include "Ventilator.h"
#include "Lights.h"
#include "ArithAverage.h"
#include "TempSensor.h"
#include "Buzzer.h"
#include "SolenoidValve.h"
#include "Door.h"

class CRefrigerator {
private:
	CCompressor _compressor;
	bool _compressorStartedForFridge = false;
	CVentilator _ventilator;
	CLights _lights;
	CTempSensorFridge _sensorFridge;
	CTempSensorFreezer _sensorFreezer;
	CBuzzer _buzzer;
	CSolenoidValve _valve;
	CDoor _door;

	float _fridgeLowerTemperatureLimit = 0;
	float _freezerLowerTemperatureLimit = 0;

	float _temperatureFridge = 0;
	float _temperatureFreezer = 0;

	bool _tryPutDownLimitsBeforeStop = false;

	unsigned long _checkTemperatureInterval = Config::FRIDGE__CHECK_TEMPERATURE_INTERVAL;
	unsigned long _checkVentilatorInterval = Config::FRIDGE__CHECK_VENTILATOR_INTERVAL;
	unsigned long _printInterval = Config::FRIDGE__PRINT_INTERVAL;

	static CRefrigerator * _pRefrigerator;

public:
	//* default constructor
	CRefrigerator();
	void loop();
	static CRefrigerator * getInstance();
};

#endif

