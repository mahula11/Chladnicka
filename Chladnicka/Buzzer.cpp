// 
// 
// 

#include "Buzzer.h"

void CBuzzer::beep() {
	Serial.println(F("beep"));
	tone(g_pConfig->getPinBuzzer()/*Config::pinBuzzer*/, 200, 100);
}

//* nastavi priznak zapnutia pipaca
//* dalsim prechodom cez loop() sa pipac zapne/vypne
void CBuzzer::setAlarm(AlarmType alarmType) {
	//* pokial nie je alarm, tak ho nastavime aj s casom zacatia a typom alarmu
	if (_isAlarm == false) {
		_isAlarm = true;
		_alarmStart = _currentMillis;
		switch (alarmType) {
			case openDoor/*Config::BUZZER__ALARM_OPEN_DOOR*/:
				_beepInterval = g_pConfig->getBuzzerAlarmOpenDoorInterval(); // Config::BUZZER__ALARM_OPEN_DOOR__INTERVAL;
				break;
			case lowTempRefrig/*Config::BUZZER__ALARM_LOW_TEMP_REFRIG*/:
				_beepInterval = g_pConfig->getBuzzerAlarmLowTempRefrigInterval(); // Config::BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL;
				break;
			case lowTempFreezer/*Config::BUZZER__ALARM_LOW_TEMP_FREEZER*/:
				_beepInterval = g_pConfig->getBuzzerAlarmLowTempFreezerInterval(); //Config::BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL;
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
