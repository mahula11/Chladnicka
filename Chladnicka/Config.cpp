// 
// 
// 

#include "Config.h"

//Config * Config::s_pConfig = nullptr;

extern Config * g_pConfig; // = Config();

Config::Config() {
	//s_pConfig = this;
	g_pConfig = this;
}

//Config * Config::getInstance() {
//	if (s_pConfig == nullptr) {
//		s_pConfig = new Config();
//	}
//	return s_pConfig;
//}

const byte Config::getPinCompressor() {

}

const byte Config::getPinVentilator() {

}

const byte Config::getPinBuzzer() {

}

const byte Config::getPinLights() {

}

const byte Config::getPinSolenoidValveFridge() {

}

const byte Config::getPinSolenoidValveFreezer() {

}

const int Config::getPinDoorsSwitch() {

}

const int Config::getPinTemperatureFridge() {

}

const int Config::getPinTemperatureFreezer() {

}

unsigned long Config::getVentilatorRunningTime() {
}

unsigned long Config::getVentilatorRunningTimeAfterOpenedDoor() {

}

unsigned long Config::getNumberOfSensorValuesForArithAverage() {

}

unsigned long Config::getSensorsTableSize() {

}

unsigned long Config::getCheckTemperatureInterval() {

}

unsigned long Config::getCheckVentilatorInterval() {

}

unsigned long Config::getPrintInterval() {

}

float Config::getFridgeLowerTemperatureLimit() {

} 

float Config::getFreezerLowerTemperatureLimit() {

}

unsigned long Config::getFridgeCheckVentilatorInterval() {

}

unsigned long Config::getFridgeCheckTemperatureInterval() {

}

float Config::getFridgeUpperTemperatureLimit() {

}

float Config::getFreezerUpperTemperatureLimit() {

}

unsigned long Config::getFridgePrintInterval() {

}

unsigned long Config::getBuzzerAlarmOpenDoorInterval() {

}

unsigned long Config::getBuzzerAlarmLowTempRefrigInterval() {

}

unsigned long Config::getBuzzerAlarmLowTempFreezerInterval() {

}

unsigned long Config::getCompressorDelayForStart() {

}

unsigned long Config::getCompressorDelayForStop() {

}

unsigned long Config::getCompressorLongestRunningTime() {

}

unsigned long Config::getDoorsAlarmStart() {

}

unsigned long Config::getLightsAlarmInterval() {

}

unsigned long Config::getValveImpulseTime() {

}