// Evolution

Serial.println("Evolution Reboot");
Serial1.begin(57600);
Serial2.begin(57600);
EM=0;
EM1=0;
REM=0;
orientation=1;
LastOrientation=0;
MilitaryTime=false;
NeedsRedraw=true;
Sleeping=false;
SPI.begin();
TouchLCD.Init();
SmallFont.SetFont(f8x8);
Font.SetFont(f12x12);
LargeFont.SetFont(ArialBold20);
setSyncProvider(I2CRTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
SPIEEPROM.Init();
TS.Init();
OkButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,"Ok",OKBUTTON);
CancelButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,"Cancel",CANCELBUTTON);
Slider.Create(COLOR_ROYALBLUE,COLOR_RED,"");
Slider.SetPosition(0,50);
Slider.Refresh();
InitCustomLabels();
for(int a=0;a<6;a++)
	PB[a].Create(COLOR_BLACK,COLOR_WHITE,COLOR_BLACK,"");
MenuFunctionPtr=&ReefAngelClass::Touch; // default pointer
menu_button_functions1[0] = &ReefAngelClass::FeedingModeStart;
menu_button_functions1[1] = &ReefAngelClass::WaterChangeModeStart;
menu_button_functions1[2] = &ReefAngelClass::ATOClear;
menu_button_functions1[3] = &ReefAngelClass::OverheatClear;
menu_button_functions1[4] = &ReefAngelClass::LightsOn;
menu_button_functions1[5] = &ReefAngelClass::MainScreen;
menu_button_functions2[0] = &ReefAngelClass::SetupTouchDateTime;
menu_button_functions2[1] = &ReefAngelClass::SetupTouchCalibratePH;
menu_button_functions2[2] = &ReefAngelClass::SetupTouchCalibrateSal;
menu_button_functions2[3] = &ReefAngelClass::SetupTouchCalibrateORP;
menu_button_functions2[4] = &ReefAngelClass::SetupTouchCalibratePHExp;
menu_button_functions2[5] = &ReefAngelClass::SetupTouchCalibrateWL;

// make sure that the default chip select pin is set to
// output, even if you don't use it:
#if not defined NOSD
SDFound=(analogRead(4)<100);
Serial.println("SD Detected");
if (SDFound) SD.begin(SDPin);
#endif // NOSD
Splash=true;
if (SDFound)
{
	if (orientation%2==0)
		TouchLCD.DrawSDRawImage("splash_l.raw",0,0,320,240);
	else
		TouchLCD.DrawSDRawImage("splash_p.raw",0,0,240,320);
}
else
{
	TouchLCD.FullClear(BKCOLOR);	
}
TouchLCD.SetBacklight(100);
//0x5241494D
//0xCF06A31E
//if (InternalMemory.IMCheck_read()!=0xCF06A31E)
//{
//	char temptext[25];
//	while(1)
//	{
//		digitalWrite(ledPin,millis()%2000>100);
//		SetOrientation(2);
//		LargeFont.SetColor(WARNING_TEXT,BKCOLOR,true);
//		LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2,20,NoIMCheck);
//		LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2,70,NoIMCheck1);
//		Font.SetColor(COLOR_BLACK,BKCOLOR,true);
//		Font.DrawTextP(10,120,NoIMLine1);
//		Font.DrawTextP(10,150,NoIMLine2);
//		Font.DrawTextP(10,165,NoIMLine3);
//		Font.DrawTextP(10,180,NoIMLine4);
//		Font.DrawTextP(10,195,NoIMLine5);
//		WDT_Restart( WDT );
//		Serial.println("No Internal Memory");
//		delay(300);
//	}
//}
Board=RAEvolution;
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
pinMode(ExpBusPin,OUTPUT);
digitalWrite(ExpBusPin,HIGH);
WDT_Restart( WDT );
