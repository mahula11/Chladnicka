//#include "types.h"

//typedef unsigned long ulong;

//* cas, pocas ktoreho moze nepretrzite bezat kompresor (v milisekundach)
//* to jest 30 minut = 1800000 milisekund
//* 30minut = 30minut * 60sekund * 1000milisekund
//* 1hodina = 60minut * 60sekund * 1000milisekund
#define COMPRESSOR__LONGEST_RUNNING_TIME 3600000
//#define COMPRESSOR__LONGEST_RUNNING_TIME (30 * 60 * 1000)
//* minimalny cas behu kompresora
//* pokial tento cas neubehne, kompresor sa nebude moct vypnut
//#define COMPRESSOR__MINIMUM_RUNNING_TIME 7 * 60 * 1000
#define COMPRESSOR__MINIMUM_RUNNING_TIME 300000 //5 * 60 * 1000
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
#define FREEZER__LOWER_TEMPERATURE_LIMIT -20
#define FREEZER__UPPER_TEMPERATURE_LIMIT -17

#define SENSORS_TABLE_SIZE 16

//* pocet poloziek z ktorych sa vyratava arith.priemer nameranych hodnot teplotnych senzorov
#define NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE 10

const byte pinCompressor = 6;
const byte pinVentilator = 2;
const byte pinBuzzer = 7;
const byte pinLights = 3;
const byte pinSolenoidValve = 4;
const int pinDoorsSwitch = 5;
const int pinSensorFridge = A5;
const int pinSensorFreezer = A4;

enum ONOFF {
	ON,
	OFF
};
//const int ON = 1;
//const int OFF = 0;

enum YESNO {
	YES,
	NO
};
//const int NO = 0;
//const int YES = 1;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class CObject {
private:
	byte _pin;
	byte _mode;
	byte _value;
public:
	CObject(byte pin, byte mode) : _pin(pin), _mode(mode) {
		pinMode(pin, mode);
		pinOnLOW();
	};

	void pinWrite(bool value) {
		_value = value;
		digitalWrite(_pin, value);
	}

	void pinOnHIGH() {
		_value = HIGH;
		digitalWrite(_pin, HIGH);
	}

	void pinOnLOW() {
		_value = LOW;
		digitalWrite(_pin, LOW);
	}

	bool getDigitalPinStatus() {
		return digitalRead(_pin) ? true : false;
	}

	int getAnalogPinStatus() {
		return analogRead(_pin);
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
		//* umozni zapnut kompresor hned ako sa bude dat
		if (_startASAP == true) {
			//* tato podmienka zisti, ci je nastaveny startovaci cas a ci uz je vhodny cas na start (ked je aktualny cas vacsi alebo rovny startovaciemu casu)
			if (_earliestStartingTime > 0 && currentMillis >= _earliestStartingTime) {
				Serial.println("CCompressor: start");
				start(currentMillis);
				_earliestStartingTime = 0;
				_earliestStopingTime = currentMillis + COMPRESSOR__DELAY_FOR_STOP;
				Serial.print("CCompressor: _earliestStopingTime: ");
				Serial.println(_earliestStopingTime);
				_startASAP = false;
			}
		}

		//* 
		if (_started == true && _stopASAP == true) {
			if (_timeStarted > 0 && ((_timeStarted + COMPRESSOR__MINIMUM_RUNNING_TIME) <= currentMillis)) {
				_stopASAP = false;
				Serial.println("CCompressor: stop");
				stop(currentMillis);
				setDelayForStart();
			}
		}

		//Serial.print("COMPRESSOR__LONGEST_RUNNING_TIME: ");
		//Serial.println(COMPRESSOR__LONGEST_RUNNING_TIME);

		//* zabezpeci aby kompresor nebezal dlhsie ako povoleny cas, aby nedoslo k poskodeniu kompresoru
		if (_started == true) {
			if (_timeStarted > 0 && ((_timeStarted + COMPRESSOR__LONGEST_RUNNING_TIME) <= currentMillis)) {
				Serial.println("CCompressor: bezi prilis dlho, vypiname ho");
				stop(currentMillis);
				setDelayForStart();
			}
		}

	}

protected:
	//* tato metoda nemoze byt volana priamo z vonku, kedze zapnutie kompresoru v nevhodny cas by mohlo nicit kompresor
	//* zapne kompresor
	void start(unsigned long currentMillis) {
		_started = true;
		_timeStarted = currentMillis;
		pinOnHIGH();
	}

	void stop(unsigned long currentMillis) {
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
	void setAlarm(unsigned long currentMillis) {
		if (_isAlarm == false) {
			_isAlarm = true;
			_alarmStart = currentMillis;
		}
	}

	//* nastavi priznak vypnutia alarmu
	void resetAlarm() {
		_isAlarm = false;
	}

	void loop(unsigned long currentMillis) {
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
	float _sensorValues[NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE] = { 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999 };
	byte _index = 0;
public:
	//* metoda prepisuje 10 prvkov
	void addValueForArithAverage(float newValue) {
		_sensorValues[_index] = newValue;
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
		Serial.print("Pole hodnot: ");
		for (int i = 0; i < NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE; i++) {
			if (((int)_sensorValues[i]) != 9999) {
				val += _sensorValues[i];
				Serial.print(_sensorValues[i]);
				Serial.print(", ");
				ii++;
			}
		}
		val = val / ii;
		Serial.print("-> ");
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
			//Serial.print("Vout: ");
			//Serial.println(_vOut);
			//Serial.print("rSensor: ");
			//Serial.println(_rSensor);
			return _rSensor;
		} else {
			return -1;
		}
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
	CTempSensorFreezer() : CTempSensor(pinSensorFreezer, 100000) {}
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
	void setAlarm(unsigned long currentMillis, int alarmType) {
		//* pokial nie je alarm, tak ho nastavime aj s casom zacatia a typom alarmu
		if (_isAlarm == false) {
			_isAlarm = true;
			_alarmStart = currentMillis;
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
public:
	CSolenoidValve() : CObject(pinSolenoidValve, OUTPUT) {}

	void switchValveOnFridge() {
		_switchOnFridge = true;
		pinWrite(true);
	}

	void switchValveOnFreezer() {
		_switchOnFridge = false;
		pinWrite(false);
	}

	bool isSwitchOnFridge() {
		return _switchOnFridge;
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

public:
	//* default constructor
	CRefrigerator() {
		//* protect compressor against repeated starts
		setDelayForStart();
		_fridgeLowerTemperatureLimit = FRIDGE__LOWER_TEMPERATURE_LIMIT;
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


		//if (1 == 0) {
		//	_compressor.startASAP();
		//}

		//* zistime ci su dvere otvorene dlho (pre alarm)
		//* pokial nie, tak len zapneme osvetlenie chladnicky
		if (_door.isAlarm()) {
			_lights.setAlarm(currentMillis);
			_buzzer.setAlarm(currentMillis, BUZZER__ALARM_OPEN_DOOR);
		} else {
			_lights.resetAlarm();
			_buzzer.resetAlarm();
			_lights.switchLights(_door.areDoorsOpen());
		}

		float temperatureFridge = _sensorFridge.getSensorCelsius();
		float temperatureFreezer = _sensorFreezer.getSensorCelsius();

		Serial.print("Teplota v chladnicke: ");
		Serial.println(temperatureFridge);
		Serial.print("Limit pre zapnutie chladnicky: ");
		Serial.println(_fridgeLowerTemperatureLimit);
		Serial.print("Teplota v mraznicke");
		Serial.println(temperatureFreezer);
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
				Serial.print("Kompresor je nastartovany od: ");
				Serial.println(_compressor.getStartedTime());
			} else {
				_compressor.startASAP();
				Serial.print("Kompresor bude nastartovany co najskor, po case: ");
				Serial.println(_compressor.getStartingTime());
			}
		} else if (_freezerLowerTemperatureLimit < temperatureFreezer) {
		} else {
			_fridgeLowerTemperatureLimit = FRIDGE__UPPER_TEMPERATURE_LIMIT;
			_freezerLowerTemperatureLimit = FREEZER__UPPER_TEMPERATURE_LIMIT;
			if (_compressor.isStarted()) {
				_compressor.stopASAP();
				Serial.print("Kompresor bude stopnuty co najskor, po case: ");
				Serial.println(_compressor.getStopingTime());
			} else {
				_valve.switchValveOnFreezer();
				Serial.print("Kompresor je stopnuty. Najskor moze by zapnuty: ");
				Serial.println(_compressor.getStartingTime());
			}
		}

	}
	//*-----------------------------------------

	void setDelayForStart() {
		_compressor.setDelayForStart();
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