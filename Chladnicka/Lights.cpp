// 
// 
// 

#include "Lights.h"


//* nastavi priznak zapnutia alebo vypnutia osvetlenia
void CLights::switchLights(bool state) {
	_switchLights = state;
}

//* nastavi priznak zapnutia alarmu
//* najblizsim prechodom cez loop, sa osvetlenie prepne
void CLights::setAlarm() {
	if (_isAlarm == false) {
		_isAlarm = true;
		_alarmStart = _currentMillis;
	}
}

//* nastavi priznak vypnutia alarmu
void CLights::resetAlarm() {
	_isAlarm = false;
}

void CLights::loop(unsigned long currentMillis) {
	CObject::loop(currentMillis);
	if (_isAlarm == true) {
		if (_alarmStart + g_pConfig->getLightsAlarmInterval()/*Config::LIGHTS__ALARM_INTERVAL*/ <= currentMillis) {
			pinWriteD(!getDigitalPinStatus());
			_isAlarm = false;
		}
	} else {
		pinWriteD(_switchLights);
	}
}