// 
// 
// 

#include "Ventilator.h"
#include "Ventilator.h"

void CVentilator::startVentilator() {
	if (_statusVentilator == false && _timeStart == 0) {
		_timeStart = _currentMillis;
		Serial.println(F("Fridge ventilator start"));
		_runnigTime = g_pConfig->getVentilatorRunningTime(); // Config::VENTILATOR__RUNNING_TIME;
	}
}

void CVentilator::stopVentilator() {
	if (_statusVentilator == true && _timeStop == 0) {
		_timeStop = _currentMillis;
		Serial.println(F("Fridge ventilator stop"));
	}
}

//* should by je pouzite preto, ze sice by mal byt nastartovany ale docasne je stopnuty (pretoze su dvere otvorene)
bool CVentilator::shouldByStarted() {
	return _statusVentilator;
}

void CVentilator::loop(unsigned long currentMillis, bool temporaryStopVentilator, bool compressorIsNotRunnigForFridge) {
	CObject::loop(currentMillis);

	if (_timeStop && _timeStart) {
		//Serial.println(F("Fridge ventilator realy stop"));
		_timeStop = 0;
		_timeStart = 0;
		_statusVentilator = false;
		pinOnLOW();
	}

	if (temporaryStopVentilator) {
		//Serial.println(F("Temporary turn OFF"));
		//* vypneme ventilator ked su otvorene dvere alebo ide kompresor pre chladnicku alebo ...
		pinOnLOW();
		if (compressorIsNotRunnigForFridge) {
			_wasTemporaryStoped = true;
		}
	} else if (_wasTemporaryStoped && (_statusVentilator == false)) {
		_wasTemporaryStoped = false;
		_runnigTime = g_pConfig->getVentilatorRunningTimeAfterOpenedDoor(); //Config::VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR;
		_timeStart = _currentMillis;
		//Serial.println(F("Fridge ventilator start after opened door"));
	} else {
		if (_wasTemporaryStoped) {
			_wasTemporaryStoped = false;
			_runnigTime += g_pConfig->getVentilatorRunningTimeAfterOpenedDoor(); //Config::VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR;
			Serial.println(F("Ventilator - runnig time was increased"));
		}
		//* ak aktualny cas je mensi ako cas zapnutia plus cas na bezanie ventilatora, tak zapneme ventilator
		if (_timeStart && _currentMillis < _timeStart + _runnigTime) {
			//Serial.println(F("Fridge ventilator is ON"));
			_statusVentilator = true;
			pinOnHIGH();
			//* ak je ventilator zapnuty a aktualny cas je uz vacsi ako cas zanutia s casom na bezanie ventilatora, tak ventilator zastavime
		} else if (_timeStart && _statusVentilator && _currentMillis >= _timeStart + _runnigTime) {
			//Serial.println(F("Fridge ventilator is OFF for short time"));
			_statusVentilator = false;
			pinOnLOW();
			//* pokial je aktualny cas vacsi alebo rovny casu zapnutia plus casu nabeh a plus casu na vypnuty ventilator, 
			//* tak nastavime cas zapnutia na aktulany cas (v dalsom behu sa ventilator zapne, kedze cas zapnutia s casom behu ventilatora bude mensi ako aktualny cas)
		} //else if (_timeStart && _statusVentilator == false && _currentMillis >= _timeStart + VENTILATOR__RUNNING_TIME + VENTILATOR__OFF_TIME) {
		//Serial.println(F("Fridge ventilator reset"));
		//_timeStart = _currentMillis;
		//}
	}
}