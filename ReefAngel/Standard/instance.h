// Standard

ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 32;
	PCICR |= 1;
}

#include <ReefAngel_1.5_LCD.h>
