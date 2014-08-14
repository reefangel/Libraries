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

void ReefAngelClass::BuzzerOn(byte style)
{
	switch (style)
	{
	case 1:
		digitalWrite(BuzzerPin,now()%2==0);
		break;
	case 2:
		digitalWrite(BuzzerPin,millis()%1500<300);
		break;
	case 3:
		digitalWrite(BuzzerPin,millis()%5000<300);
		break;
	case 4:
		digitalWrite(BuzzerPin,millis()%30000<300);
		break;
	case 5:
		digitalWrite(BuzzerPin,millis()%3000<200 || (millis()+400)%3000<200);
		break;
	case 6:
		if (millis()%6000<3000)
			if (millis()%3000<1500)
				digitalWrite(BuzzerPin,millis()%600<300);
			else
				digitalWrite(BuzzerPin,HIGH);
		else
			digitalWrite(BuzzerPin,LOW);
		break;
	default:
		digitalWrite(BuzzerPin,HIGH);
		break;
	}
}

void ReefAngelClass::BuzzerOff()
{
	digitalWrite(BuzzerPin,LOW);
}

#include <ReefAngel_1.5_LCD.h>
