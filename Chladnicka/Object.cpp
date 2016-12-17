#include "Object.h"


CObject::CObject(byte pin, byte mode) : _pin(pin), _mode(mode) {
	pinMode(pin, mode);
	pinOnLOW();
};

CObject::CObject(byte pin, byte pin2, byte mode) : _pin(pin), _pin2(pin2), _mode(mode) {
	pinMode(pin, mode);
	pinMode(pin2, mode);
	pinOnLOW();
	pin2OnLOW();
};

CObject::~CObject() {
}

void CObject::loop(unsigned long currentMillis) {
	_currentMillis = currentMillis;
}

void CObject::pinWriteD(bool value) {
	digitalWrite(_pin, value);
}

void CObject::pinWriteA(int value) {
	analogWrite(_pin, value);
}

void CObject::pin2Write(bool value) {
	if (_pin2 != -1) {
		digitalWrite(_pin2, value);
	} else {
		Serial.println(F("pin2Write error!"));
	}
}

void CObject::pinOnHIGH() {
	digitalWrite(_pin, HIGH);
}

void CObject::pin2OnHIGH() {
	if (_pin2 != -1) {
		digitalWrite(_pin2, HIGH);
	} else {
		Serial.println(F("pin2OnHIGH error!"));
	}
}

void CObject::pinOnLOW() {
	digitalWrite(_pin, LOW);
}

void CObject::pin2OnLOW() {
	if (_pin2 != -1) {
		digitalWrite(_pin2, LOW);
	} else {
		Serial.println(F("pin2OnLOW error!"));
	}
}

bool CObject::getDigitalPinStatus() {
	return digitalRead(_pin) ? true : false;
}

bool CObject::getDigitalPin2Status() {
	if (_pin2 != -1) {
		return digitalRead(_pin2) ? true : false;
	} else {
		Serial.println(F("getDigitalPin2Status error!"));
		return false;
	}
}

int CObject::getAnalogPinStatus() {
	return analogRead(_pin);
}

int CObject::getAnalogPin2Status() {
	if (_pin2 = !- 1) {
		return analogRead(_pin2);
	} else {
		Serial.println(F("getAnalogPin2Status error!"));
	}
}