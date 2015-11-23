//#include "types.h"

//typedef unsigned long ulong;

//#define TEST

#ifdef TEST
#define COMPRESSOR__LONGEST_RUNNING_TIME 3600000
//#define COMPRESSOR__MINIMUM_RUNNING_TIME 300000 
#define COMPRESSOR__DELAY_FOR_START 420 //420000  //7 * 60 * 1000
#define COMPRESSOR__DELAY_FOR_STOP  360
#define LIGHTS__ALARM_INTERVAL 300
#define DOORS__ALARM_START 1000
#define BUZZER__ALARM_OPEN_DOOR 0
#define BUZZER__ALARM_LOW_TEMP_REFRIG 1
#define BUZZER__ALARM_LOW_TEMP_FREEZER 2
#define BUZZER__ALARM_OPEN_DOOR__INTERVAL 300
#define BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL 20
#define BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL 30
#define FRIDGE__LOWER_TEMPERATURE_LIMIT 4
#define FRIDGE__UPPER_TEMPERATURE_LIMIT 6
#define FREEZER__LOWER_TEMPERATURE_LIMIT -20
#define FREEZER__UPPER_TEMPERATURE_LIMIT -17
#define FRIDGE__CHECK_TEMPERATURE_INTERVAL 500
#define SENSORS_TABLE_SIZE 16
#define NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE 10
#define VALVE__IMPULSE_TIME 100
#else
//* cas, pocas ktoreho moze nepretrzite bezat kompresor (v milisekundach)
//* to jest 30 minut = 1800000 milisekund
//* 30minut = 30minut * 60sekund * 1000milisekund
//* 1hodina = 60minut * 60sekund * 1000milisekund
#define COMPRESSOR__LONGEST_RUNNING_TIME 3600000
//#define COMPRESSOR__LONGEST_RUNNING_TIME (30 * 60 * 1000)
//* minimalny cas behu kompresora
//* pokial tento cas neubehne, kompresor sa nebude moct vypnut
//#define COMPRESSOR__MINIMUM_RUNNING_TIME 7 * 60 * 1000
//#define COMPRESSOR__MINIMUM_RUNNING_TIME 300000 //5 * 60 * 1000
//* oneskorenie zapnutia kompresora po nepretrzitom maximalnom behu (v sekundach)
//#define COMPRESSOR__DELAY_AFTER_LONGEST_RUNNING_TIME 10

//* cas, pocas ktoreho nie je mozne nastartovat kompresor (az po uplinuti tohto casu je mozne urobit start) (v milisekundach)
//* 1 sekunda = 1 sekunda * 1000 milisekund 
#define COMPRESSOR__DELAY_FOR_START 420000 //420000  //7 * 60 * 1000
#define COMPRESSOR__DELAY_FOR_STOP  360000  //6 * 60 * 1000

//* interval blikania svetiel pri alarme
#define LIGHTS__ALARM_INTERVAL 300

//* cas, za ktory musia byt otvorene dvere, aby sa spustil alarm
#define DOORS__ALARM_START 1000

//* typy alarmov pre bzuciak
#define BUZZER__ALARM_OPEN_DOOR 0
#define BUZZER__ALARM_LOW_TEMP_REFRIG 1
#define BUZZER__ALARM_LOW_TEMP_FREEZER 2
//* intervaly pre jednotlive alarmy
#define BUZZER__ALARM_OPEN_DOOR__INTERVAL 300
#define BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL 20
#define BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL 30

//* chlad
#define FRIDGE__LOWER_TEMPERATURE_LIMIT 4
#define FRIDGE__UPPER_TEMPERATURE_LIMIT 6
#define FREEZER__LOWER_TEMPERATURE_LIMIT -22
#define FREEZER__UPPER_TEMPERATURE_LIMIT -20
#define FRIDGE__CHECK_TEMPERATURE_INTERVAL 5000

//* cas impulzu zopnutia pre ventil (po tento cas bude drzane napatie na prepnutie, potom sa vypne)
#define VALVE__IMPULSE_TIME 100

#define SENSORS_TABLE_SIZE 16

//* pocet poloziek z ktorych sa vyratava arith.priemer nameranych hodnot teplotnych senzorov
#define NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE 10

#endif //* ifdef TEST

const byte pinCompressor = 6;
const byte pinVentilator = 2;
const byte pinBuzzer = 7;
const byte pinLights = 3;
const byte pinSolenoidValveFridge = 4;
const byte pinSolenoidValveFreezer = 8;
const int pinDoorsSwitch = 5;
const int pinSensorFridge = A5;
const int pinSensorFreezer = A4;

class CObject {
private:
	byte _pin;
	byte _pin2 = -1;
	byte _mode;
	//byte _value;
	//byte _value2;

protected:
	unsigned long _currentMillis = 0; 
	void loop(unsigned long currentMillis) {
		_currentMillis = currentMillis;
	}

public:
	CObject(byte pin, byte mode) : _pin(pin), _mode(mode) {
		pinMode(pin, mode);
		pinOnLOW();
	};

	CObject(byte pin, byte pin2, byte mode) : _pin(pin), _pin2(pin2), _mode(mode) {
		pinMode(pin, mode);
		pinMode(pin2, mode);
		pinOnLOW();
		pin2OnLOW();
	};

	void pinWrite(bool value) {
		//_value = value;
		digitalWrite(_pin, value);
	}

	void pin2Write(bool value) {
		//_value2 = value;
		if (_pin2 != -1) {
			digitalWrite(_pin2, value);
		} else {
			Serial.println("pin2Write error!");
		}
	}

	void pinOnHIGH() {
		//_value = HIGH;
		digitalWrite(_pin, HIGH);
	}

	void pin2OnHIGH() {
		//_value2 = HIGH;
		if (_pin2 != -1) {
			digitalWrite(_pin2, HIGH);
		} else{
			Serial.println("pin2OnHIGH error!");
		}		
	}

	void pinOnLOW() {
		//_value = LOW;
		digitalWrite(_pin, LOW);
	}

	void pin2OnLOW() {
		//_value2 = LOW;
		if (_pin2 != -1) {
			digitalWrite(_pin2, LOW);
		} else {
			Serial.println("pin2OnLOW error!");
		}		
	}

	bool getDigitalPinStatus() {
		return digitalRead(_pin) ? true : false;
	}

	bool getDigitalPin2Status() {
		if (_pin2 != - 1) {
			return digitalRead(_pin2) ? true : false;
		} else {
			Serial.println("getDigitalPin2Status error!");
			return false;
		}
	}

	int getAnalogPinStatus() {
		return analogRead(_pin);
	}

	int getAnalogPin2Status() {
		if (_pin2 = !- 1) {
			return analogRead(_pin2);
		} else {
			Serial.println("getAnalogPin2Status error!");
		}		
	}
};

class CCompressor : CObject {
private:
	//bool _possibleToStart = false;
	//unsigned long _timeWantedStart = 0;
	bool _started = false;
	//* cas nastartovania kompresora (v milisekundach)
	unsigned long _timeStarted = 0;
	bool _startASAP = false;
	bool _stopASAP = true;

	//* udrziava cas, kedy sa ma nastartovat compressor (v milisekundach)
	unsigned long _earliestStartingTime = 0;
	unsigned long _earliestStopingTime = 0;
	//* cas v milisekundach, oneskorenie startu kompresora
	//unsigned long _delayForStart = 0;

public:
	CCompressor() : CObject(pinCompressor, OUTPUT) {

	}

	bool isStarted() {
		return _started;
	}

	unsigned long getStartedTime() {
		return _timeStarted;
	}

	unsigned long getStartingTime() {
		return _earliestStartingTime;
	}

	unsigned long getStopingTime() {
		return _earliestStopingTime;
	}

	//* nastavi oneskorenie startu kompresora
	void setDelayForStart() {
		if (_earliestStartingTime == 0) {
			_earliestStartingTime = millis() + COMPRESSOR__DELAY_FOR_START;
			Serial.print("setDelayForStart(): set new _earliestStartingTime: ");
			Serial.println(_earliestStartingTime);
		}
	}

	void startASAP() {
		_startASAP = true;
		_stopASAP = false;
	}

	void stopASAP() {
		_stopASAP = true;
		_startASAP = false;
	}

	void loop(unsigned long currentMillis) {
		CObject::loop(currentMillis);
		//* umozni zapnut kompresor hned ako sa bude dat
		if (_startASAP == true) {
			//* tato podmienka zisti, ci je nastaveny startovaci cas a ci uz je vhodny cas na start (ked je aktualny cas vacsi alebo rovny startovaciemu casu)
			if (_earliestStartingTime > 0 && currentMillis >= _earliestStartingTime) {
				Serial.println("CCompressor: start");
				start();
				_earliestStartingTime = 0;
				_earliestStopingTime = currentMillis + COMPRESSOR__DELAY_FOR_STOP;
				Serial.print("CCompressor: _earliestStopingTime: ");
				Serial.println(_earliestStopingTime);
				_startASAP = false;
			}
		}

		//* 
		if (_started == true && _stopASAP == true) {
			if (_earliestStopingTime > 0 && _earliestStopingTime <= currentMillis) {
				_stopASAP = false;
				Serial.println("CCompressor: stop");
				stop();
				setDelayForStart();
			}
		}

		//* zabezpeci aby kompresor nebezal dlhsie ako povoleny cas, aby nedoslo k poskodeniu kompresoru
		if (_started == true) {
			if (_timeStarted > 0 && ((_timeStarted + COMPRESSOR__LONGEST_RUNNING_TIME) <= currentMillis)) {
				Serial.println("CCompressor: bezi prilis dlho, vypiname ho");
				stop();
				setDelayForStart();
			}
		}

	}

protected:
	//* tato metoda nemoze byt volana priamo z vonku, kedze zapnutie kompresoru v nevhodny cas by mohlo nicit kompresor
	//* zapne kompresor
	void start() {
		_started = true;
		_timeStarted = _currentMillis;
		pinOnHIGH();
	}

	void stop() {
		_started = false;
		_timeStarted = 0;
		pinOnLOW();
	}

};

class CVentilator : CObject {
private:
public:
	CVentilator() : CObject(pinVentilator, OUTPUT) {}

	void turnOn() {
		pinOnHIGH();
	}

	void turnOff() {
		pinOnLOW();
	}
};

class CLights : CObject {
private:
	bool _switchLights = false;
	bool _isAlarm = false;
	unsigned long _alarmStart = 0;

public:
	CLights() : CObject(pinLights, OUTPUT) {}

	//* nastavi priznak zapnutia alebo vypnutia osvetlenia
	void switchLights(bool state) {
		_switchLights = state;
	}

	//* nastavi priznak zapnutia alarmu
	//* najblizsim prechodom cez loop, sa osvetlenie prepne
	void setAlarm() {
		if (_isAlarm == false) {
			_isAlarm = true;
			_alarmStart = _currentMillis;
		}
	}

	//* nastavi priznak vypnutia alarmu
	void resetAlarm() {
		_isAlarm = false;
	}

	void loop(unsigned long currentMillis) {
		CObject::loop(currentMillis);
		if (_isAlarm == true) {
			if (_alarmStart + LIGHTS__ALARM_INTERVAL <= currentMillis) {
				pinWrite(!getDigitalPinStatus());
				_isAlarm = false;
			}
		} else {
			pinWrite(_switchLights);
		}
	}
};

class CArithAverage {
private:
	float _sensorValues[NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE];
	byte _index = 0;
public:
	CArithAverage() {
		for (int i = 0; i < NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE; i++) {
			_sensorValues[i] = 9999;
		}
	}

	//* metoda prepisuje NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE prvkov
	void addValueForArithAverage(float newValue) {
		_sensorValues[_index] = newValue;
		//		Serial.print("added: ");
		//		Serial.println(newValue);
		if (_index == NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE - 1) {
			_index = 0;
		} else {
			_index++;
		}
	}

	//* vrati aritmeticky priemer
	float getArithAverage() {
		int ii = 0;
		float val = 0;
		float min = 9999;
		float max = -9999;
		Serial.print("Stat: cur: ");
		Serial.print(_sensorValues[_index - 1]);
		for (int i = 0; i < NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE; i++) {
			if (((int)_sensorValues[i]) != 9999) {
				val += _sensorValues[i];
				//Serial.print(_sensorValues[i]);
				//Serial.print(", ");
				min = _sensorValues[i] < min ? _sensorValues[i] : min;
				max = _sensorValues[i] > max ? _sensorValues[i] : max;
				ii++;
			}
		}
		Serial.print(", min: ");
		Serial.print(min);
		Serial.print(", max: ");
		Serial.print(max);
		val = val / ii;
		Serial.print(", avr: ");
		Serial.println(val);
		return val;
	}
};

class CTempSensor : CObject {
private:
	struct stSensorTempTable {
		float degC;
		float impedance;
	};
	stSensorTempTable _table[SENSORS_TABLE_SIZE] = {
		{ -30, 175199.63 },		//* 0
		{ -25, 129286.93 },		//* 1
		{ -20, 96358.16 },		//* 2
		{ -15, 72500.36 },		//* 3
		{ -10, 55045.94 },		//* 4
		{ -5, 42156.97 },		//* 5
		{ 0, 32554.20 },		//* 6
		{ 5, 25338.55 },		//* 7
		{ 10, 19872.17 },		//* 8
		{ 15, 15698.46 },		//* 9
		{ 20, 12487.74 },		//* 10
		{ 25, 10000.00 },		//* 11
		{ 30, 8059.08 },		//* 12
		{ 35, 6534.72 },		//* 13
		{ 40, 5329.87 },		//* 14
		{ 45, 4371.72 } };		//* 15

	//* 19000 -
	//* 20000 - 10
	//* 24000 - 7
	//* 25000 - 6
	//* 27000 - 4
	//* 28000 - 4
	//* 29000 - 3
	//* 30000 - 2
	//* 50000 -

	int _raw = 0;
	int _vIn = 5;
	float _vOut = 0;
	float _r1 = 0;  //* rezistor, ktoreho hodnota je znama, zapojeny do serii so senzorom
	float _rSensor = 0;
	float _buffer = 0;
	CArithAverage _arithAverage;

public:
	CTempSensor(byte pin, float r1) : CObject(pin, INPUT), _r1(r1) {}

	//*  _r1  _rSensor  vypocitana hodnota
	//* 80000  129286 -> 129514.07    -25°C
	//* 90000  129286 -> 129428.58 *
	//* 95000  129286 -> 129665.13
	//* 100000 129286 -> 129596.42
	//* 80000   42157 -> 42268.66     -5°C
	//* 90000   42157 -> 42223.82
	//* 95000   42157 -> 42207.34  *
	//* 100000  42157 -> 42222.23

	//* 80000  32554  -> 32682.26      0°C
	//* 90000  32554  -> 32716.37
	//* 95000  32554  -> 32664.04
	//* 100000 32554  -> 32642.49  *
	//* 80000  15698  -> 15812.86     15°C
	//* 90000  15698  -> 15809.41 
	//* 95000  15698  -> 15797.27  *
	//* 100000 15698  -> 15837.11
public:
	float getSensorCelsius() {
		float r = getSensorImpedance();
		if (r == -1) {
			Serial.println("CTempSensor: Zo senzoru neprisla ziadna hodnota");
			return 88888;
		}

		int indexOfArr = -1;
		for (int i = 0; i < SENSORS_TABLE_SIZE - 1; i++) {
			if ((_table[i + 1].impedance <= r) && (r <= _table[i].impedance)) {
				indexOfArr = i;
				break;
			}
		}
		if (indexOfArr != -1) {
			//float celsius = map(r, _table[indexOfArr + 1].impedance, _table[indexOfArr].impedance, _table[indexOfArr + 1].degC, _table[indexOfArr].degC);
			//* ziskame mapnutu hodnotu nameraneho odporu zo snimaca do tabulky _table
			float celsius = mapfloat(r, _table[indexOfArr + 1].impedance, _table[indexOfArr].impedance, _table[indexOfArr + 1].degC, _table[indexOfArr].degC);
			//* pridame ju do triedy, ktora vypocita priemer z poslednych 10 merani
			_arithAverage.addValueForArithAverage(celsius);
			//* ziskame aritmeticky priemer poslednych 10 merani (teplot v chladnicke/mraznicke)
			//* odcitame 2 od priemeru - tymto zrealnime odmeranu teplotu (voci meranej teplomerom arduino ukazuje o dva stupne viac)
			return _arithAverage.getArithAverage() - 2;
		} else {
			Serial.println("CTempSensor: Zo senzora prisla hodnota, ktoru nemame v tabulke hodnot impedancie");
			return 99999;
		}
	}

	//* meranie hodnoty rezistoru
	//* http://www.circuitbasics.com/arduino-ohm-meter/
	float getSensorImpedance() {
		_raw = getAnalogPinStatus();
		if (_raw) {
			_buffer = _raw * _vIn;
			_vOut = (_buffer) / 1024.0;
			_buffer = (_vIn / _vOut) - 1;
			_rSensor = _r1 * _buffer;
			return _rSensor;
		} else {
			return -1;
		}
	}

private:
	float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};

class CTempSensorFridge : public CTempSensor {
private:
public:
	CTempSensorFridge() : CTempSensor(pinSensorFridge, 80000) {}
};

class CTempSensorFreezer : public CTempSensor {
private:
public:
	CTempSensorFreezer() : CTempSensor(pinSensorFreezer, 80000) {}
};

class CBuzzer : CObject {
private:
	bool _isAlarm = false;
	unsigned long _alarmStart = 0;
	unsigned long _beepInterval = 0;

public:
	CBuzzer() : CObject(pinBuzzer, OUTPUT) {}

	//* nastavi priznak zapnutia pipaca
	//* dalsim prechodom cez loop() sa pipac zapne/vypne
	void setAlarm(int alarmType) {
		//* pokial nie je alarm, tak ho nastavime aj s casom zacatia a typom alarmu
		if (_isAlarm == false) {
			_isAlarm = true;
			_alarmStart = _currentMillis;
			switch (alarmType) {
				case BUZZER__ALARM_OPEN_DOOR:
					_beepInterval = BUZZER__ALARM_OPEN_DOOR__INTERVAL;
					break;
				case BUZZER__ALARM_LOW_TEMP_REFRIG:
					_beepInterval = BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL;
					break;
				case BUZZER__ALARM_LOW_TEMP_FREEZER:
					_beepInterval = BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL;
					break;
			}
		}
	}

	//* nastavi priznak zrusenia alarmu
	void resetAlarm() {
		_isAlarm = false;
	}

	void loop(unsigned long currentMillis) {
		CObject::loop(currentMillis);
		if (_isAlarm == true) {
			if (_alarmStart + _beepInterval <= currentMillis) {
				pinWrite(!getDigitalPinStatus());
				_isAlarm = false;
			}
		} else {
			pinWrite(false);
		}
	}
};

class CSolenoidValve : CObject {
private:
	bool _switchOnFridge = false;
	unsigned long _switched = -1;
public:
	CSolenoidValve() : CObject(pinSolenoidValveFridge, pinSolenoidValveFreezer, OUTPUT) {
		switchValveOnFridge();
	}

	void switchValveOnFridge() {
		if (_switchOnFridge == false) {
			_switchOnFridge = true;
			pinOnHIGH();
			pin2OnLOW();
			_switched = _currentMillis;
			Serial.print("Ventil sa prepina na chladnicku ");
			Serial.println(_switched);
		}
	}

	void switchValveOnFreezer() {
		if (_switchOnFridge == true) {
			_switchOnFridge = false;
			pinOnLOW();
			pin2OnHIGH();
			_switched = _currentMillis;
			Serial.println("Ventil sa prepina na mraznicku");
		}
	}

	bool isSwitchOnFridge() {
		//Serial.print("Ventil je prepnuty na ");
		//Serial.println(_switchOnFridge ? "chladnicku/mraznicku" : "mraznicku");
		return _switchOnFridge;
	}

	void loop(unsigned long currentMillis) {
		CObject::loop(currentMillis);
		Serial.print("_switched = ");
		Serial.println(_switched);
		Serial.print("_switched + VALVE__IMPULSE_TIME = ");
		Serial.println(_switched + VALVE__IMPULSE_TIME);
		if (_switched != -1 && currentMillis >= _switched + VALVE__IMPULSE_TIME) {
			//* ked ubehne stanoveny cas treba vypnut zopnutie ventilu
			//* ventil sa prepina polaritou (jednosmernym napatim) a staci ho prepnut len impulzom (potom treba vypnut prud do ventilu)
			pinOnLOW();
			pin2OnLOW();
			_switched = -1;
			Serial.println("Napatie na ventil odpojene");
		}
	}
};

class CDoor : CObject {
private:
	unsigned long _openDoorTime = 0;
	bool _areDoorOpen;
	bool _isAlarm = false;

public:
	CDoor() : CObject(pinDoorsSwitch, INPUT) {}

	bool areDoorsOpen() {
		return _areDoorOpen;
	}

	bool isAlarm() {
		return _isAlarm;
	}

	void loop(unsigned long currentMillis) {
		CObject::loop(currentMillis);
		//* ked su dvere otvorene, tak pinStatus vrati false
		if (getDigitalPinStatus() == false) {
			if (_areDoorOpen == false) {
				_areDoorOpen = true;
				_openDoorTime = currentMillis;
			}
		} else {
			_areDoorOpen = false;
		}

		//* nastavime priznak alarmu, pokial su dvere otvorene dlhsie ako je nastaveny cas
		if (_areDoorOpen && _openDoorTime + DOORS__ALARM_START <= currentMillis) {
			_isAlarm = true;
		} else {
			_isAlarm = false;
		}
	}
};

class CRefrigerator {
private:
	CCompressor _compressor;
	CVentilator _ventilator;
	CLights _lights;
	CTempSensorFridge _sensorFridge;
	CTempSensorFreezer _sensorFreezer;
	CBuzzer _buzzer;
	CSolenoidValve _valve;
	CDoor _door;

	float _fridgeLowerTemperatureLimit = 0;
	float _freezerLowerTemperatureLimit = 0;

	unsigned long _checkTemperatureInterval = FRIDGE__CHECK_TEMPERATURE_INTERVAL;

public:
	//* default constructor
	CRefrigerator() {
		//* protect compressor against repeated starts
		_fridgeLowerTemperatureLimit = FRIDGE__LOWER_TEMPERATURE_LIMIT;
		_freezerLowerTemperatureLimit = FREEZER__LOWER_TEMPERATURE_LIMIT;
		//_valve.switchValveOnFridge();
		_compressor.setDelayForStart();
	}

	void loop() {
		unsigned long currentMillis = millis();
		Serial.println("");
		Serial.print("currentMillis: ");
		Serial.println(currentMillis);

		_compressor.loop(currentMillis);
		_door.loop(currentMillis);
		_lights.loop(currentMillis);
		_buzzer.loop(currentMillis);
		_valve.loop(currentMillis);

		if (Serial.available()) {
			int incomingByte = Serial.read();
			Serial.print("I received: ");
			Serial.println(incomingByte, DEC);
			if (incomingByte == 'c') {
				_valve.switchValveOnFridge();
			} else if (incomingByte == 'm') {
				_valve.switchValveOnFreezer();
			}
		}

		//* zistime ci su dvere otvorene dlho (pre alarm)
		//* pokial nie, tak len zapneme osvetlenie chladnicky
		if (_door.isAlarm()) {
			_lights.setAlarm();
			_buzzer.setAlarm(BUZZER__ALARM_OPEN_DOOR);
		} else {
			_lights.resetAlarm();
			_buzzer.resetAlarm();
			_lights.switchLights(_door.areDoorsOpen());
		}

		//* kontrolujeme teplotu a ovladame v urceny interval (kazdych 5 sekund)
		//if (currentMillis >= _checkTemperatureInterval) 
		{
			_checkTemperatureInterval = currentMillis + FRIDGE__CHECK_TEMPERATURE_INTERVAL;

			float temperatureFridge = _sensorFridge.getSensorCelsius();
			float temperatureFreezer = _sensorFreezer.getSensorCelsius();

			Serial.print("Teplota v chladnicke: ");
			Serial.println(temperatureFridge);
			Serial.print("Teplota v mraznicke: ");
			Serial.println(temperatureFreezer);
			Serial.print("Limit pre zapnutie chladnicky: ");
			Serial.println(_fridgeLowerTemperatureLimit);
			Serial.print("Limit pre zapnutie mraznicky: ");
			Serial.println(_freezerLowerTemperatureLimit);
			Serial.print("Ventil je prepnuty na: ");
			Serial.println(_valve.isSwitchOnFridge() ? "Chladnicku/Mraznicku" : "Mraznicku");

			//* 5°C - pre chladnicku
			//* -18 * pre mraznicku
			if (_fridgeLowerTemperatureLimit < temperatureFridge) {
				_fridgeLowerTemperatureLimit = FRIDGE__LOWER_TEMPERATURE_LIMIT;
				if (_compressor.isStarted()) {
					_valve.switchValveOnFridge();
					Serial.print("Kompresor ide pre chladnicku od: ");
					Serial.println(_compressor.getStartedTime());
				} else {
					_compressor.startASAP();
					Serial.print("Kompresor bude nastartovany pre chladnicku po: ");
					Serial.println(_compressor.getStartingTime());
				}
			} else if (_freezerLowerTemperatureLimit < temperatureFreezer) {
				_fridgeLowerTemperatureLimit = FRIDGE__UPPER_TEMPERATURE_LIMIT;
				_freezerLowerTemperatureLimit = FREEZER__LOWER_TEMPERATURE_LIMIT;
				if (_compressor.isStarted()) {
					_valve.switchValveOnFreezer();
					Serial.print("Kompresor ide pre mraznicku od: ");
					Serial.println(_compressor.getStartedTime());
				} else {
					_compressor.startASAP();
					Serial.print("Kompresor bude nastartovany pre mraznicku po: ");
					Serial.println(_compressor.getStartingTime());
				}
			} else {
				_fridgeLowerTemperatureLimit = FRIDGE__UPPER_TEMPERATURE_LIMIT;
				_freezerLowerTemperatureLimit = FREEZER__UPPER_TEMPERATURE_LIMIT;
				if (_compressor.isStarted()) {
					_compressor.stopASAP();
					Serial.print("Kompresor bude stopnuty co najskor, po case: ");
					Serial.println(_compressor.getStopingTime());
				} else {
					//_valve.switchValveOnFreezer();
					Serial.print("Kompresor je stopnuty. Najskor moze by zapnuty: ");
					Serial.println(_compressor.getStartingTime());
				}
			}
		}
	}
};

CRefrigerator * g_pRefrigerator;

//* -----------------------------------------------------------
void setup() {
	Serial.begin(9600);
	g_pRefrigerator = new CRefrigerator();
}

//* -----------------------------------------------------------
void loop() {
	g_pRefrigerator->loop();
	delay(5000);
}



//* TODO
//* po presiahnuti limitov vypnut chladnicku celu a dat alarm!