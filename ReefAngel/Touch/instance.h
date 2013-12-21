// Touch

ReefAngelClass::ReefAngelClass()
{
	DDRJ=B00000110;
	PORTJ=B11010000;
}

void ReefAngelClass::WDTReset()
{
	wdt_reset();
}

#include <ReefAngel_2.8_LCD.h>
