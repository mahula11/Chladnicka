// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class Config {
private:
	class Constants {
	public:
		//* cas, pocas ktoreho moze nepretrzite bezat kompresor (v milisekundach)
		//* to jest 30 minut = 1800000 milisekund
		//* 30minut = 30minut * 60sekund * 1000milisekund
		//* 1hodina = 60minut * 60sekund * 1000milisekund
		static const unsigned long COMPRESSOR__LONGEST_RUNNING_TIME = 3600000;
		//#define COMPRESSOR__LONGEST_RUNNING_TIME (30 * 60 * 1000)
		//* minimalny cas behu kompresora
		//* pokial tento cas neubehne, kompresor sa nebude moct vypnut
		//#define COMPRESSOR__MINIMUM_RUNNING_TIME 7 * 60 * 1000
		//#define COMPRESSOR__MINIMUM_RUNNING_TIME 300000 //5 * 60 * 1000
		//* oneskorenie zapnutia kompresora po nepretrzitom maximalnom behu (v sekundach)
		//#define COMPRESSOR__DELAY_AFTER_LONGEST_RUNNING_TIME 10

		//* cas, pocas ktoreho nie je mozne nastartovat kompresor (az po uplinuti tohto casu je mozne urobit start) (v milisekundach)
		//* 1 sekunda = 1 sekunda * 1000 milisekund 
		static const unsigned long COMPRESSOR__DELAY_FOR_START = 420000; //420000  //7 * 60 * 1000
		static const unsigned long COMPRESSOR__DELAY_FOR_STOP = 360000; //6 * 60 * 1000

		//* interval blikania svetiel pri alarme
		static const unsigned long LIGHTS__ALARM_INTERVAL = 300;

		//* cas, za ktory musia byt otvorene dvere, aby sa spustil alarm
		static const unsigned long DOORS__ALARM_START = 60000;

		//* typy alarmov pre bzuciak
		//static const unsigned long BUZZER__ALARM_OPEN_DOOR = 0;
		//static const unsigned long BUZZER__ALARM_LOW_TEMP_REFRIG = 1;
		//static const unsigned long BUZZER__ALARM_LOW_TEMP_FREEZER = 2;
		//* intervaly pre jednotlive alarmy
		static const unsigned long BUZZER__ALARM_OPEN_DOOR__INTERVAL = 300;
		static const unsigned long BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL = 20;
		static const unsigned long BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL = 30;

		//* ventilacia chladnicky
		static const unsigned long VENTILATOR__RUNNING_TIME = 780000; //* 600000 = 10min, 780000 = 13, 900000 = 15min, 1200000 = 20min
		static const unsigned long VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR = 120000; //* 120000 = 2min, 300000 = 5min

		//* chlad
		static const unsigned long FRIDGE__LOWER_TEMPERATURE_LIMIT = 3;
		static const unsigned long FRIDGE__UPPER_TEMPERATURE_LIMIT = 7;
		static const long FREEZER__LOWER_TEMPERATURE_LIMIT = -24;
		static const long FREEZER__UPPER_TEMPERATURE_LIMIT = -17;
		static const unsigned long FRIDGE__CHECK_TEMPERATURE_INTERVAL = 5000;
		static const unsigned long FRIDGE__CHECK_VENTILATOR_INTERVAL = 5000;
		static const unsigned long FRIDGE__PRINT_INTERVAL = 5000;

		//* cas impulzu zopnutia pre ventil (po tento cas bude drzane napatie na prepnutie, potom sa vypne)
		static const unsigned long VALVE__IMPULSE_TIME = 400;

		static const unsigned long SENSORS_TABLE_SIZE = 16;

		//* pocet poloziek z ktorych sa vyratava arith.priemer nameranych hodnot teplotnych senzorov
		static const unsigned long NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE = 20;

		static const byte pinCompressor = 6;
		static const byte pinVentilator = 2;
		static const byte pinBuzzer = 7;
		static const byte pinLights = 3;
		static const byte pinSolenoidValveFridge = 4;
		static const byte pinSolenoidValveFreezer = 8;
		static const int pinDoorsSwitch = 5;
		static const int pinSensorFridge = A5;
		static const int pinSensorFreezer = A4;
	};

private:
	//static Config * s_pConfig;
public:
	Config();

	//static Config * getInstance();
	
	const byte getPinCompressor();
	const byte getPinVentilator();
	const byte getPinBuzzer();
	const byte getPinLights();
	const byte getPinSolenoidValveFridge();
	const byte getPinSolenoidValveFreezer();
	const int getPinDoorsSwitch();
	const int getPinTemperatureFridge();
	const int getPinTemperatureFreezer();
	
	unsigned long getVentilatorRunningTime(); //Config::VENTILATOR__RUNNING_TIME;
	unsigned long getVentilatorRunningTimeAfterOpenedDoor(); //Config::VENTILATOR__RUNNIG_TIME_AFTER_OPENED_DOOR
	unsigned long getNumberOfSensorValuesForArithAverage(); //Config::NUMBER_OF_SENSOR_VALUES_FOR_ARITH_AVERAGE
	unsigned long getSensorsTableSize();  // SENSORS_TABLE_SIZE
	unsigned long getCheckTemperatureInterval(); //Config::FRIDGE__CHECK_TEMPERATURE_INTERVAL
	unsigned long getCheckVentilatorInterval(); //Config::FRIDGE__PRINT_INTERVAL;
	unsigned long getPrintInterval(); //Config::FRIDGE__PRINT_INTERVAL;
	float getFridgeLowerTemperatureLimit(); //Config::FRIDGE__LOWER_TEMPERATURE_LIMIT
	float getFreezerLowerTemperatureLimit(); //Config::FREEZER__LOWER_TEMPERATURE_LIMIT;
	//const int getBuzzerAlarmOpenDoor(); // Config::BUZZER__ALARM_OPEN_DOOR
	unsigned long getFridgeCheckVentilatorInterval(); //Config::FRIDGE__CHECK_VENTILATOR_INTERVAL;
	unsigned long getFridgeCheckTemperatureInterval(); //Config::FRIDGE__CHECK_TEMPERATURE_INTERVAL;
	float getFridgeUpperTemperatureLimit(); //Config::FRIDGE__UPPER_TEMPERATURE_LIMIT
	//float getFreezerLowerTemperatureLimit(); //Config::FREEZER__LOWER_TEMPERATURE_LIMIT;
	float getFreezerUpperTemperatureLimit(); // Config::FREEZER__UPPER_TEMPERATURE_LIMIT;
	unsigned long getFridgePrintInterval(); // Config::FRIDGE__PRINT_INTERVAL;
	unsigned long getBuzzerAlarmOpenDoorInterval(); //Config::BUZZER__ALARM_OPEN_DOOR__INTERVAL
	unsigned long getBuzzerAlarmLowTempRefrigInterval(); //Config::BUZZER__ALARM_LOW_TEMP_REFRIG__INTERVAL
	unsigned long getBuzzerAlarmLowTempFreezerInterval(); // Config::BUZZER__ALARM_LOW_TEMP_FREEZER_INTERVAL
	unsigned long getCompressorDelayForStart(); //Config::COMPRESSOR__DELAY_FOR_START
	unsigned long getCompressorDelayForStop(); //Config::COMPRESSOR__DELAY_FOR_STOP
	unsigned long getCompressorLongestRunningTime(); //Config::COMPRESSOR__LONGEST_RUNNING_TIME
	unsigned long getDoorsAlarmStart(); //Config::DOORS__ALARM_START
	unsigned long getLightsAlarmInterval(); //Config::LIGHTS__ALARM_INTERVAL
	unsigned long getValveImpulseTime(); // Config::VALVE__IMPULSE_TIME
};

//extern const Config & g_config = Config();
Config * g_pConfig;

#endif

