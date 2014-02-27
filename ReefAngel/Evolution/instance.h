// Evolution

ReefAngelClass::ReefAngelClass()
{
}

void ReefAngelClass::WDTReset()
{
	// WDT reset is handled by main.cpp in the core files.
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
