// 
// 
// 

#include "Buzzer.h"

void CBuzzer::beep() {
	Serial.println(F("beep"));
	tone(Config::pinBuzzer, 200, 100);
}

//* nastavi priznak zapnutia pipaca
//* dalsim prechodom cez loop() sa pipac zapne/vypne
void CBuzzer::setAlarm(int alarmType) {
	//* pokial nie je alarm, tak ho nastavime aj s casom zacatia a typom alarmu
	if (_isAlarm == false) {
		_isAlarm = true;
		_alarmStart = _currentMillis;
		switch (alarmType) {
			case Config::BUZZER__ALARM_OPEN_DOOR:
				_beepInterval = Config::BUZZER__ALARM_OPEN_DOOR__INTERVAL;
				break;
			case Config::BUZZER__ALARM_LOW_TEMP_REFRIG:
				_beepInterval = Config::BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL;
				break;
			case Config::BUZZER__ALARM_LOW_TEMP_FREEZER:
				_beepInterval = Config::BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL;
				break;
		}
	}
}

//* nastavi priznak zrusenia alarmu
void CBuzzer::resetAlarm() {
	_isAlarm = false;
}

void CBuzzer::loop(unsigned long currentMillis) {
	CObject::loop(currentMillis);
	if (_isAlarm == true) {
		if (_alarmStart + _beepInterval <= currentMillis) {
			pinWriteD(!getDigitalPinStatus());
			_isAlarm = false;
		}
	} else {
		pinWriteD(false);
	}
}
