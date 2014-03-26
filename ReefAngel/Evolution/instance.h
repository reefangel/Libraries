// Evolution

ReefAngelClass::ReefAngelClass()
{
	PIOC->PIO_OER |= 0x001FFFFE;
	PIOC->PIO_OWDR = 0xFFFE0001;
}

void ReefAngelClass::WDTReset()
{
	WDT_Restart( WDT );
	// WDT reset is handled by main.cpp in the core files.
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

#include <ReefAngel_2.8_LCD.h>

// The functions below are just to compile without a screen.
// We need to eventually make a screen for Evolution.

//void ReefAngelClass::ShowInterface()
//{
//	Refresh();
//}
//
//void ReefAngelClass::CheckFeedingDrawing()
//{
//}
//
//void ReefAngelClass::CheckWaterChangeDrawing()
//{
//}
//
//void ReefAngelClass::RefreshScreen()
//{
//}
//
//void ReefAngelClass::SetupCalibratePH()
//{
//}
//
