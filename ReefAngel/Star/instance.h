// Star

ReefAngelClass::ReefAngelClass()
{
	DDRJ=B00000110;
	PORTJ=B11010000;
	PCMSK0 |= 128;
	PCICR |= 1;
}

void ReefAngelClass::WDTReset()
{
	wdt_reset();
}


#include <ReefAngel_1.5_LCD.h>
