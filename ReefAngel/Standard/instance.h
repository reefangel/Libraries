// Standard

ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 32;
	PCICR |= 1;
}

void ReefAngelClass::WDTReset()
{
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
}

#include <ReefAngel_1.5_LCD.h>
