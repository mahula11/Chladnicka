#ifndef _CONFIG_h
#define _CONFIG_h


#include <arduino.h>

namespace Config {

	//* cas, pocas ktoreho moze nepretrzite bezat kompresor (v milisekundach)
	//* to jest 30 minut = 1800000 milisekund
	//* 30minut = 30minut * 60sekund * 1000milisekund
	//* 1hodina = 60minut * 60sekund * 1000milisekund
	const unsigned long COMPRESSOR__LONGEST_RUNNING_TIME = 3600000;
	//#define COMPRESSOR__LONGEST_RUNNING_TIME (30 * 60 * 1000)
	//* minimalny cas behu kompresora
	//* pokial tento cas neubehne, kompresor sa nebude moct vypnut
	//#define COMPRESSOR__MINIMUM_RUNNING_TIME 7 * 60 * 1000
	//#define COMPRESSOR__MINIMUM_RUNNING_TIME 300000 //5 * 60 * 1000
	//* oneskorenie zapnutia kompresora po nepretrzitom maximalnom behu (v sekundach)
	//#define COMPRESSOR__DELAY_AFTER_LONGEST_RUNNING_TIME 10

	//* cas, pocas ktoreho nie je mozne nastartovat kompresor (az po uplinuti tohto casu je mozne urobit start) (v milisekundach)
	//* 1 sekunda = 1 sekunda * 1000 milisekund 
	const unsigned long COMPRESSOR__DELAY_FOR_START = 420000; //420000  //7 * 60 * 1000
	const unsigned long COMPRESSOR__DELAY_FOR_STOP = 360000; //6 * 60 * 1000

	//* interval blikania svetiel pri alarme
	const unsigned long LIGHTS__ALARM_INTERVAL = 300;

	//* cas, za ktory musia byt otvorene dvere, aby sa spustil alarm
	const unsigned long DOORS__ALARM_START = 60000;

	//* typy alarmov pre bzuciak
	const unsigned long BUZZER__ALARM_OPEN_DOOR = 0;
	const unsigned long BUZZER__ALARM_LOW_TEMP_REFRIG = 1;
	const unsigned long BUZZER__ALARM_LOW_TEMP_FREEZER = 2;
	//* intervaly pre jednotlive alarmy
	const unsigned long BUZZER__ALARM_OPEN_DOOR__INTERVAL = 300;
	const unsigned long BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL = 20;
	const unsigned long BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL = 30;

	//* ventilacia chladnicky
	const unsigned long VENTILATOR__RUNNING_TIME = 780000; //* 600000 = 10min, 780000 = 13, 900000 = 15min, 1200000 = 20min
	const unsigned long VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR = 120000; //* 120000 = 2min, 300000 = 5min

	//* chlad
	const unsigned long FRIDGE__LOWER_TEMPERATURE_LIMIT = 3;
	const unsigned long FRIDGE__UPPER_TEMPERATURE_LIMIT = 7;
	const long FREEZER__LOWER_TEMPERATURE_LIMIT = -24;
	const long FREEZER__UPPER_TEMPERATURE_LIMIT = -17;
	const unsigned long FRIDGE__CHECK_TEMPERATURE_INTERVAL = 5000;
	const unsigned long FRIDGE__CHECK_VENTILATOR_INTERVAL = 5000;
	const unsigned long FRIDGE__PRINT_INTERVAL = 5000;

	//* cas impulzu zopnutia pre ventil (po tento cas bude drzane napatie na prepnutie, potom sa vypne)
	const unsigned long VALVE__IMPULSE_TIME = 400;

	const unsigned long SENSORS_TABLE_SIZE = 16;

	//* pocet poloziek z ktorych sa vyratava arith.priemer nameranych hodnot teplotnych senzorov
	const unsigned long NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE = 20;


	const byte pinCompressor = 6;
	const byte pinVentilator = 2;
	const byte pinBuzzer = 7;
	const byte pinLights = 3;
	const byte pinSolenoidValveFridge = 4;
	const byte pinSolenoidValveFreezer = 8;
	const int pinDoorsSwitch = 5;
	const int pinSensorFridge = A5;
	const int pinSensorFreezer = A4;

};

#endif