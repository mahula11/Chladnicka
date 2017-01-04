// 
// 
// 

#include "SolenoidValve.h"

CSolenoidValve::CSolenoidValve() : CObject(g_pConfig->getPinSolenoidValveFridge()/*Config::pinSolenoidValveFridge*/, g_pConfig->getPinSolenoidValveFreezer()/*Config::pinSolenoidValveFreezer*/, OUTPUT) {
	switchValveOnFridge();
}

void CSolenoidValve::switchValveOnFridge() {
	if (_switchOnFridge == false) {
		_switchOnFridge = true;
		pinOnHIGH();
		pin2OnLOW();
		_switched = _currentMillis;
		Serial.print(F("Ventil sa prepina na chladnicku "));
		Serial.println(_switched);
	}
}

void CSolenoidValve::switchValveOnFreezer() {
	if (_switchOnFridge == true) {
		_switchOnFridge = false;
		pinOnLOW();
		pin2OnHIGH();
		_switched = _currentMillis;
		Serial.println(F("Ventil sa prepina na mraznicku"));
	}
}

bool CSolenoidValve::isSwitchOnFridge() {
	return _switchOnFridge;
}

void CSolenoidValve::loop(unsigned long currentMillis) {
	CObject::loop(currentMillis);
	if (_switched != -1 && currentMillis >= _switched + g_pConfig->getValveImpulseTime()/*Config::VALVE__IMPULSE_TIME*/) {
		//* ked ubehne stanoveny cas treba vypnut zopnutie ventilu
		//* ventil sa prepina polaritou (jednosmernym napatim) a staci ho prepnut len impulzom (potom treba vypnut prud do ventilu)
		pinOnLOW();
		pin2OnLOW();
		_switched = -1;
		Serial.println(F("Napatie na ventil odpojene"));
	}
}
