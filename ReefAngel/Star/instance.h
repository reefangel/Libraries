// Star

ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 128;
	PCICR |= 1;
}

void ReefAngelClass::WDTReset()
{
	wdt_reset();
}


#include <ReefAngel_1.5_LCD.h>
