
#include "Compressor.h"



bool CCompressor::isStarted() {
	return _started;
}

unsigned long CCompressor::getStartedTime() {
	return _timeStarted;
}

unsigned long CCompressor::getStartingTime() {
	return _earliestStartingTime;
}

unsigned long CCompressor::getStopingTime() {
	return _earliestStopingTime;
}

void CCompressor::setEarliestStartingTime(unsigned long time) {
	_earliestStartingTime = time;
}

//* nastavi oneskorenie startu kompresora
void CCompressor::setDelayForStart() {
	if (_earliestStartingTime == 0) {
		_earliestStartingTime = millis() + Config::COMPRESSOR__DELAY_FOR_START;
		Serial.print(F("setDelayForStart(): set new _earliestStartingTime: "));
		Serial.println(_earliestStartingTime);
	}
}

void CCompressor::startASAP() {
	_startASAP = true;
	_stopASAP = false;
}

void CCompressor::stopASAP() {
	_stopASAP = true;
	_startASAP = false;
}

void CCompressor::loop(unsigned long currentMillis) {
	CObject::loop(currentMillis);
	//* umozni zapnut kompresor hned ako sa bude dat
	if (_startASAP == true) {
		//* tato podmienka zisti, ci je nastaveny startovaci cas a ci uz je vhodny cas na start 
		//* (ked je aktualny cas vacsi alebo rovny startovaciemu casu)
		if (_earliestStartingTime > 0 && currentMillis >= _earliestStartingTime) {
			Serial.println(F("CCompressor: start"));
			start();
			_earliestStartingTime = 0;
			_earliestStopingTime = currentMillis + Config::COMPRESSOR__DELAY_FOR_STOP;
			Serial.print(F("CCompressor: _earliestStopingTime: "));
			Serial.println(_earliestStopingTime);
			_startASAP = false;
		}
	}

	//* 
	if (_started == true && _stopASAP == true) {
		if (_earliestStopingTime > 0 && _earliestStopingTime <= currentMillis) {
			_stopASAP = false;
			Serial.println(F("CCompressor: stop"));
			stop();
			setDelayForStart();
		}
	}

	//* zabezpeci aby kompresor nebezal dlhsie ako povoleny cas, aby nedoslo k poskodeniu kompresoru
	if (_started == true) {
		if (_timeStarted > 0 && ((_timeStarted + Config::COMPRESSOR__LONGEST_RUNNING_TIME) <= currentMillis)) {
			Serial.println(F("CCompressor: bezi prilis dlho, vypiname ho"));
			stop();
			setDelayForStart();
		}
	}

}

	//* tato metoda nemoze byt volana priamo z vonku, kedze zapnutie kompresoru v nevhodny cas by mohlo nicit kompresor
	//* zapne kompresor
void CCompressor::start() {
	_started = true;
	_timeStarted = _currentMillis;
	pinOnHIGH();
}

void CCompressor::stop() {
	_started = false;
	_timeStarted = 0;
	pinOnLOW();
}

