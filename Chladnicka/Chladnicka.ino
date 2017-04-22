#include <avr/wdt.h>

#include "Config.h"
#include "Compressor.h"
#include "Ventilator.h"
#include "Lights.h"
#include "ArithAverage.h"
#include "TempSensor.h"
#include "Buzzer.h"
#include "SolenoidValve.h"
#include "Door.h"
#include "Refrigerator.h" 


//* pre testnutie kompilacie, kolko pamate zabera projekt, ked je trieda CRefrigerator vytvorena na halde
//* toto testnutie ukaze cislo, kolko by to mohlo zaberat v pamati, ked je trieda CRefrigerator vytvorena na heape
//Config conf;
//CRefrigerator ref;

//* -----------------------------------------------------------
void setup() {
	new Config();
	watchdogSetup();
	Serial.begin(115200);
	new CRefrigerator();
}

//* -----------------------------------------------------------
void loop() {
	//* vynuteny restart chladnicky
	//* pokial je millis() mensi ako nastaveny cas (4 hodiny), dovtedy sa watchdog bude resetovat. 
	//* pokial tento cas presiahne, tak nedovolime reset watchdogu a tym bude vynuteny reset procesoru
	if (millis() < g_pConfig->getForcedResetTime()) {
		//* resetuje watchdog
		wdt_reset();
	}
	CRefrigerator::getInstance()->loop();
	delay(50);
}

void watchdogSetup(void) {
	cli(); // disable all interrupts
	wdt_reset(); // reset the WDT timer
	/*
	Bit Name
	7	WDIF
	6	WDIE
	5	WDP3
	4	WDCE
	3	WDE
	2	WDP2
	1	WDP1
	0	WDP0

	WDP3 WDP2 WDP1 WDP0 Time-out(ms)
	0	  0    0    0    16
	0	  0    0    1    32
	0	  0    1    0    64
	0 	  0    1    1    125
	0 	  1    0    0    250
	0 	  1    0    1    500
	0 	  1    1    0    1000
	0 	  1    1    1    2000
	1 	  0    0    0    4000
	1 	  0    0    1    8000

	WDTCSR configuration:
	WDIE = 1: Interrupt Enable
	WDE = 1 :Reset Enable
	WDP3 = 0 :For 2000ms Time-out
	WDP2 = 1 :For 2000ms Time-out
	WDP1 = 1 :For 2000ms Time-out
	WDP0 = 1 :For 2000ms Time-out
	*/
	// Enter Watchdog Configuration mode:
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	// Set Watchdog settings:
	WDTCSR = (1 << WDIE) | (1 << WDE) | (0 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
	sei();
}

ISR(WDT_vect) // Watchdog timer interrupt.
{
	// Include your code here - be careful not to use functions they may cause the interrupt to hang and
	// prevent a reset.
	//* nedavat sem Serial.print(), lebo vyvolava prerusenie
}



//* TODO
//* po presiahnuti limitov vypnut chladnicku celu a dat alarm!