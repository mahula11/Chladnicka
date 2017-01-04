// 
// 
// 

#include "Refrigerator.h"

CRefrigerator * CRefrigerator::_pRefrigerator = nullptr;

CRefrigerator::CRefrigerator() {
	_pRefrigerator = this;
	_checkTemperatureInterval = g_pConfig->getCheckTemperatureInterval();
	_checkVentilatorInterval = g_pConfig->getCheckVentilatorInterval();
	_printInterval = g_pConfig->getPrintInterval();
	//* protect compressor against repeated starts
	_fridgeLowerTemperatureLimit = g_pConfig->getFridgeLowerTemperatureLimit(); //Config::FRIDGE__LOWER_TEMPERATURE_LIMIT;
	_freezerLowerTemperatureLimit = g_pConfig->getFreezerLowerTemperatureLimit(); // Config::FREEZER__LOWER_TEMPERATURE_LIMIT;
	//_valve.switchValveOnFridge();
	_compressor.setDelayForStart();
	_buzzer.beep();
}

CRefrigerator * CRefrigerator::getInstance() {
	if (_pRefrigerator == nullptr) {
		_pRefrigerator = new CRefrigerator();
	} 
	return _pRefrigerator;
}

void CRefrigerator::loop() {
	unsigned long currentMillis = millis();
	if (currentMillis >= _printInterval) {
		Serial.println(F(""));
		Serial.print(F("currentMillis: "));
		Serial.println(currentMillis);
	}

	_compressor.loop(currentMillis);
	_door.loop(currentMillis);
	_lights.loop(currentMillis);
	_buzzer.loop(currentMillis);
	_valve.loop(currentMillis);
	_ventilator.loop(currentMillis, _door.isDoorOpen(), ((_compressor.isStarted() == false) || (_valve.isSwitchOnFridge() == false)));

	if (Serial.available()) {
		int incomingByte = Serial.read();
		Serial.print(F("I received: "));
		Serial.println(incomingByte, DEC);
		if (incomingByte == 'r') {
			_valve.switchValveOnFridge();
		} else if (incomingByte == 'f') {
			_valve.switchValveOnFreezer();
		} else if (incomingByte == 'z') {
			_compressor.setEarliestStartingTime(currentMillis);
		} else if (incomingByte == 'v') {
			_ventilator.startVentilator();
		} else if (incomingByte == 'b') {
			_ventilator.stopVentilator();
		}
	}

	//* zistime ci su dvere otvorene dlho (pre alarm)
	//* pokial nie, tak len zapneme osvetlenie chladnicky
	if (_door.isAlarm()) {
		if (currentMillis >= _printInterval) {
			Serial.println(F("Door is in alarm!"));
		}
		_lights.setAlarm();
		_buzzer.setAlarm(AlarmType::openDoor); // Config::BUZZER__ALARM_OPEN_DOOR);
	} else {
		_lights.resetAlarm();
		_buzzer.resetAlarm();
		if (currentMillis >= _printInterval) {
			Serial.print(F("Door is "));
			Serial.println(_door.isDoorOpen() ? F("opened") : F("closed"));
		}
		_lights.switchLights(_door.isDoorOpen());
	}

	//* start/stop ventilator, kontrolujeme v intervale FRIDGE__CHECK_VENTILATOR_INTERVAL
	if (currentMillis >= _checkVentilatorInterval) {
		Serial.println(F("Check ventilator"));
		_checkVentilatorInterval = currentMillis + g_pConfig->getFridgeCheckVentilatorInterval(); // Config::FRIDGE__CHECK_VENTILATOR_INTERVAL;

		//* pokial je startnuty kompresor a ventil je prepnuty na chladnicku, tak nastavime priznak, 
		//* ze kompresor isiel pre chladnicku, aby sme potom vedeli zapnut ventilator.
		//* ventilator zapiname IBA ked neide kompresor pre chladnicku, pretoze ked je pusteny ventilator, 
		//* tak teplota v chladnicke je ina, ako pri vypnutom ventilatory

		if (_compressor.isStarted() && _valve.isSwitchOnFridge()) {
			Serial.println(F("--- Compressor started for fridge!!!!!!"));
			_compressorStartedForFridge = true;
			_ventilator.stopVentilator();
		}
		if (_compressorStartedForFridge && ((_compressor.isStarted() == false) || (_valve.isSwitchOnFridge() == false))) {
			Serial.println(F("--- Start ventilator"));
			_ventilator.startVentilator();
			_compressorStartedForFridge = false;
		}
	}

	//* kontrolujeme teplotu a ovladame v urceny interval (kazdych 5 sekund)
	if (currentMillis >= _checkTemperatureInterval) {
		_checkTemperatureInterval = currentMillis + g_pConfig->getFridgeCheckTemperatureInterval(); // Config::FRIDGE__CHECK_TEMPERATURE_INTERVAL;
		_temperatureFridge = _sensorFridge.getSensorCelsius();
		_temperatureFreezer = _sensorFreezer.getSensorCelsius();
	}

	float fridgeUpperTemperatureLimit = g_pConfig->getFridgeUpperTemperatureLimit();
	float fridgeLowerTemperatureLimit = g_pConfig->getFridgeLowerTemperatureLimit();
	float freezerLowerTemperatureLimit = g_pConfig->getFreezerLowerTemperatureLimit();
	float freezerUpperTemperatureLimit = g_pConfig->getFreezerUpperTemperatureLimit();

	//* 5°C - pre chladnicku
	//* -18 * pre mraznicku
	if (_fridgeLowerTemperatureLimit < _temperatureFridge) {
		_fridgeLowerTemperatureLimit = fridgeLowerTemperatureLimit; // Config::FRIDGE__LOWER_TEMPERATURE_LIMIT;
		_tryPutDownLimitsBeforeStop = false;
		if (_compressor.isStarted()) {
			_valve.switchValveOnFridge();
			//Serial.print("Kompresor ide pre chladnicku od: ");
			//Serial.println(_compressor.getStartedTime());
		} else {
			_compressor.startASAP();
			//Serial.print("Kompresor bude nastartovany pre chladnicku po: ");
			//Serial.println(_compressor.getStartingTime());
		}
	} else if (_freezerLowerTemperatureLimit < _temperatureFreezer) {
		_fridgeLowerTemperatureLimit = fridgeUpperTemperatureLimit; // Config::FRIDGE__UPPER_TEMPERATURE_LIMIT;
		_freezerLowerTemperatureLimit = freezerLowerTemperatureLimit; // Config::FREEZER__LOWER_TEMPERATURE_LIMIT;
		_tryPutDownLimitsBeforeStop = false;
		if (_compressor.isStarted()) {
			_valve.switchValveOnFreezer();
			//Serial.print("Kompresor ide pre mraznicku od: ");
			//Serial.println(_compressor.getStartedTime());
		} else {
			_compressor.startASAP();
			//Serial.print("Kompresor bude nastartovany pre mraznicku po: ");
			//Serial.println(_compressor.getStartingTime());
		}
	} else {
		_fridgeLowerTemperatureLimit = fridgeUpperTemperatureLimit; // Config::FRIDGE__UPPER_TEMPERATURE_LIMIT;
		_freezerLowerTemperatureLimit = freezerUpperTemperatureLimit; // Config::FREEZER__UPPER_TEMPERATURE_LIMIT;
		if (_compressor.isStarted()) {
			if (_tryPutDownLimitsBeforeStop == false) {
				_fridgeLowerTemperatureLimit = fridgeUpperTemperatureLimit/*Config::FRIDGE__UPPER_TEMPERATURE_LIMIT*/ - ((float)(fridgeUpperTemperatureLimit/*Config::FRIDGE__UPPER_TEMPERATURE_LIMIT*/ - fridgeLowerTemperatureLimit/*Config::FRIDGE__LOWER_TEMPERATURE_LIMIT*/) / (float)2) - 1;
				_freezerLowerTemperatureLimit = freezerUpperTemperatureLimit/*Config::FREEZER__UPPER_TEMPERATURE_LIMIT*/ - ((float)(freezerUpperTemperatureLimit/*Config::FREEZER__UPPER_TEMPERATURE_LIMIT*/ - freezerLowerTemperatureLimit/*Config::FREEZER__LOWER_TEMPERATURE_LIMIT*/) / (float)2) - 1;
				_tryPutDownLimitsBeforeStop = true;
			} else {
				_compressor.stopASAP();
				_tryPutDownLimitsBeforeStop = false;
			}
			//Serial.print("Kompresor bude stopnuty co najskor, po case: ");
			//Serial.println(_compressor.getStopingTime());
		} else {
			//_valve.switchValveOnFreezer();
			//Serial.print("Kompresor je stopnuty. Najskor moze by zapnuty: ");
			//Serial.println(_compressor.getStartingTime());
		}
	}

	if (currentMillis >= _printInterval) {
		Serial.print(F("Teplota v chladnicke: "));
		Serial.println(_temperatureFridge);
		Serial.print(F("Teplota v mraznicke: "));
		Serial.println(_temperatureFreezer);
		Serial.print(F("Limit pre zapnutie chladnicky: "));
		Serial.println(_fridgeLowerTemperatureLimit);
		Serial.print(F("Limit pre zapnutie mraznicky: "));
		Serial.println(_freezerLowerTemperatureLimit);
		Serial.print(F("Ventil je prepnuty na: "));
		Serial.println(_valve.isSwitchOnFridge() ? F("Chladnicku/Mraznicku") : F("Mraznicku"));

		if (_fridgeLowerTemperatureLimit < _temperatureFridge) {
			if (_compressor.isStarted()) {
				Serial.print(F("Kompresor ide pre chladnicku od: "));
				Serial.println(_compressor.getStartedTime());
			} else {
				Serial.print(F("Kompresor bude nastartovany pre chladnicku po: "));
				Serial.println(_compressor.getStartingTime());
			}
		} else if (_freezerLowerTemperatureLimit < _temperatureFreezer) {
			if (_compressor.isStarted()) {
				Serial.print(F("Kompresor ide pre mraznicku od: "));
				Serial.println(_compressor.getStartedTime());
			} else {
				Serial.print(F("Kompresor bude nastartovany pre mraznicku po: "));
				Serial.println(_compressor.getStartingTime());
			}
		} else {
			if (_compressor.isStarted()) {
				if (_tryPutDownLimitsBeforeStop == true) {
					Serial.print(F("Try put down limits before stop compressor."));
				} else {
					Serial.print(F("Kompresor bude stopnuty co najskor, po case: "));
					Serial.println(_compressor.getStopingTime());
				}
			} else {
				Serial.print(F("Kompresor je stopnuty. Najskor moze by zapnuty: "));
				Serial.println(_compressor.getStartingTime());
			}
		}
	}

	//* set timers
	if (currentMillis >= _printInterval) {
		_printInterval = currentMillis + g_pConfig->getFridgePrintInterval();
	}
}