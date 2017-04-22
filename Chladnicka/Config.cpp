// 
// 
// 

#include "Config.h"

//Config * Config::s_pConfig = nullptr;

Config * g_pConfig = nullptr; // = Config();

Config::Config() {
	//s_pConfig = this;
	g_pConfig = this;
	loadFromConstants();
}

void Config::loadFromConstants() {
	_pinCompresor = Constants::pinCompressor;
	_pinVentilator = Constants::pinVentilator;
	_pinBuzzer = Constants::pinBuzzer;
	_pinLights = Constants::pinLights;
	_pinSolenoidValveFridge = Constants::pinSolenoidValveFridge;
	_pinSolenoidValveFreezer = Constants::pinSolenoidValveFreezer;
	_pinDoorSwitch = Constants::pinDoorsSwitch;
	_pinTemperatureFridge = Constants::pinSensorFridge;
	_pinTemperatureFreezer = Constants::pinSensorFreezer;
	_ventilatorRunningTime = Constants::VENTILATOR__RUNNING_TIME;
	_ventilatorRunningTimeAfterOpenedDoor = Constants::VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR;
	_numberOfSensorValuesForArithAverage = Constants::NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE;
	_sensorsTableSize = Constants::SENSORS_TABLE_SIZE;
	_checkTemperatureInterval = Constants::FRIDGE__CHECK_TEMPERATURE_INTERVAL;
	_checkVentilatorInterval = Constants::FRIDGE__CHECK_VENTILATOR_INTERVAL;
	_printInterval = Constants::FRIDGE__PRINT_INTERVAL;
	_fridgeLowerTemperatureLimit = Constants::FRIDGE__LOWER_TEMPERATURE_LIMIT;
	_freezerLowerTemperatureLimit = Constants::FREEZER__LOWER_TEMPERATURE_LIMIT;
	_fridgeCheckVentilatorInterval = Constants::FRIDGE__CHECK_VENTILATOR_INTERVAL;
	_fridgeCheckTemperatureInterval = Constants::FRIDGE__CHECK_TEMPERATURE_INTERVAL;
	_fridgeUpperTemperatureLimit = Constants::FRIDGE__UPPER_TEMPERATURE_LIMIT;
	_freezerUpperTemperatureLimit = Constants::FREEZER__UPPER_TEMPERATURE_LIMIT;
	_fridgePrintInterval = Constants::FRIDGE__PRINT_INTERVAL;
	_buzzerAlarmOpenDoorInterval = Constants::BUZZER__ALARM_OPEN_DOOR__INTERVAL;
	_buzzerAlarmLowTempRefrigInterval = Constants::BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL;
	_buzzerAlarmLowTempFreezerInterval = Constants::BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL;
	_compressorDelayForStart = Constants::COMPRESSOR__DELAY_FOR_START;
	_compressorDelayForStop = Constants::COMPRESSOR__DELAY_FOR_STOP;
	_compressorLongestRunningTime = Constants::COMPRESSOR__LONGEST_RUNNING_TIME;
	_doorsAlarmStart = Constants::DOORS__ALARM_START;
	_lightAlarmInterval = Constants::LIGHTS__ALARM_INTERVAL;
	_valveImpulseTime = Constants::VALVE__IMPULSE_TIME;
	_forcedResetTime = Constants::forcedResetTime;
}

//Config * Config::getInstance() {
//	if (s_pConfig == nullptr) {
//		s_pConfig = new Config();
//	}
//	return s_pConfig;
//}

unsigned long Config::getForcedResetTime() {
	return _forcedResetTime;
}

byte Config::getPinCompressor() {
	return _pinCompresor;
}

byte Config::getPinVentilator() {
	return _pinVentilator;
}

byte Config::getPinBuzzer() {
	return _pinBuzzer;
}

byte Config::getPinLights() {
	return _pinLights;
}

byte Config::getPinSolenoidValveFridge() {
	return _pinSolenoidValveFridge;
}

byte Config::getPinSolenoidValveFreezer() {
	return _pinSolenoidValveFreezer;
}

byte Config::getPinDoorsSwitch() {
	return _pinDoorSwitch;
}

byte Config::getPinTemperatureFridge() {
	return _pinTemperatureFridge;
}

byte Config::getPinTemperatureFreezer() {
	return _pinTemperatureFreezer;
}

unsigned long Config::getVentilatorRunningTime() {
	return _ventilatorRunningTime;
}

unsigned long Config::getVentilatorRunningTimeAfterOpenedDoor() {
	return _ventilatorRunningTimeAfterOpenedDoor;
}

unsigned long Config::getNumberOfSensorValuesForArithAverage() {
	return _numberOfSensorValuesForArithAverage;
}

unsigned long Config::getSensorsTableSize() {
	return _sensorsTableSize;
}

unsigned long Config::getCheckTemperatureInterval() {
	return _checkTemperatureInterval;
}

unsigned long Config::getCheckVentilatorInterval() {
	return _checkVentilatorInterval;
}

unsigned long Config::getPrintInterval() {
	return _printInterval;
}

float Config::getFridgeLowerTemperatureLimit() {
	return _fridgeLowerTemperatureLimit;
} 

float Config::getFreezerLowerTemperatureLimit() {
	return _freezerLowerTemperatureLimit;
}

unsigned long Config::getFridgeCheckVentilatorInterval() {
	return _fridgeCheckVentilatorInterval;
}

unsigned long Config::getFridgeCheckTemperatureInterval() {
	return _fridgeCheckTemperatureInterval;
}

float Config::getFridgeUpperTemperatureLimit() {
	return _fridgeUpperTemperatureLimit;
}

float Config::getFreezerUpperTemperatureLimit() {
	return _freezerUpperTemperatureLimit;
}

unsigned long Config::getFridgePrintInterval() {
	return _fridgePrintInterval;
}

unsigned long Config::getBuzzerAlarmOpenDoorInterval() {
	return _buzzerAlarmOpenDoorInterval;
}

unsigned long Config::getBuzzerAlarmLowTempRefrigInterval() {
	return _buzzerAlarmLowTempRefrigInterval;
}

unsigned long Config::getBuzzerAlarmLowTempFreezerInterval() {
	return _buzzerAlarmLowTempFreezerInterval;
}

unsigned long Config::getCompressorDelayForStart() {
	return _compressorDelayForStart;
}

unsigned long Config::getCompressorDelayForStop() {
	return _compressorDelayForStop;
}

unsigned long Config::getCompressorLongestRunningTime() {
	return _compressorLongestRunningTime;
}

unsigned long Config::getDoorsAlarmStart() {
	return	_doorsAlarmStart;
}

unsigned long Config::getLightsAlarmInterval() {
	return _lightAlarmInterval;
}

unsigned long Config::getValveImpulseTime() {
	return _valveImpulseTime;
}


