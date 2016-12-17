// 
// 
// 

#include "Door.h"

bool CDoor::isDoorOpen() {
	return _isDoorOpen;
}

bool CDoor::isAlarm() {
	return _isAlarm;
}

unsigned long CDoor::getLastOpenTime() {
	return _lastOpenTime;
}

void CDoor::resetLastOpenTime() {
	_lastOpenTime = 0;
}

void CDoor::loop(unsigned long currentMillis) {
	CObject::loop(currentMillis);
	//* if door is open, pinStatus return false
	if (getDigitalPinStatus() == false) {
		if (_isDoorOpen == false) {
			_isDoorOpen = true;
			_openDoorTime = currentMillis;
		}
		_lastOpenTime = currentMillis - _openDoorTime;
	} else {
		_isDoorOpen = false;
	}

	//* set alarm flag, while door is open more than time in DOORS__ALARM_START
	if (_isDoorOpen && _openDoorTime + Config::DOORS__ALARM_START <= currentMillis) {
		_isAlarm = true;
	} else {
		_isAlarm = false;
	}
}
