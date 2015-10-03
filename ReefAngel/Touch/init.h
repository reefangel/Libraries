// Touch

wdt_enable(WDTO_1S);
EM=0;
EM1=0;
REM=0;
orientation=1;
LastOrientation=0;
MilitaryTime=false;
NeedsRedraw=true;
SDFound=false;
Sleeping=false;
pinMode(53,OUTPUT);
digitalWrite(53,HIGH); // Pull up resistor on hardware SS SPI
SPI.begin();
Serial.println("SPI Begin");
wdt_reset();
TouchLCD.Init();
Serial.println("Touch Init");
SmallFont.SetFont(f8x8);
Font.SetFont(f12x12);
LargeFont.SetFont(ArialBold20);
setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
TS.Init();
Serial.println("TS Init");
wdt_reset();
OkButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,"Ok",OKBUTTON);
CancelButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,"Cancel",CANCELBUTTON);
Slider.Create(COLOR_ROYALBLUE,COLOR_RED,"");
Slider.SetPosition(0,50);
Slider.Refresh();
wdt_reset();
Serial.println("Slider Init");
InitCustomLabels();
Serial.println("Labels Init");
for(int a=0;a<6;a++)
	PB[a].Create(COLOR_BLACK,COLOR_WHITE,COLOR_BLACK,"");
Serial.println("Buttons Init");
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

Serial.println("Skipping Tilt");

#if not defined NOTILT
Tilt.Init();
Tilt.Refresh();
SetOrientation(Tilt.GetOrientation());
#endif // NOTILT 

// make sure that the default chip select pin is set to
// output, even if you don't use it:
#if not defined NOSD
wdt_reset();
Serial.println("SD Check");
SDFound=(PINJ & (1<<PJ3))==0;
Serial.println(SDFound);
#endif // NOSD
Splash=true;
if (SDFound)
{
	wdt_reset();
	SD.begin(SDPin);
	wdt_reset();
	Serial.println("SD Init");
	if (orientation%2==0)
		TouchLCD.DrawSDRawImage("splash_l.raw",0,0,320,240);
	else
		TouchLCD.DrawSDRawImage("splash_p.raw",0,0,240,320);
}
else
{
	TouchLCD.FullClear(BKCOLOR);	
}
TouchLCD.SetBacklight(10);
//0x5241494D
//0xCF06A31E
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
	char temptext[25];
	while(1)
	{
		digitalWrite(ledPin,millis()%2000>100);
		SetOrientation(2);
		LargeFont.SetColor(WARNING_TEXT,BKCOLOR,true);
		LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2,20,NoIMCheck);
		LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2,70,NoIMCheck1);
		Font.SetColor(COLOR_BLACK,BKCOLOR,true);
		Font.DrawTextP(10,120,NoIMLine1);
		Font.DrawTextP(10,150,NoIMLine2);
		Font.DrawTextP(10,165,NoIMLine3);
		Font.DrawTextP(10,180,NoIMLine4);
		Font.DrawTextP(10,195,NoIMLine5);
		wdt_reset();
		Serial.println("No Internal Memory");
	}
}
#ifdef RA_TOUCHDISPLAY
Board=RATouchDisplay;
Wire.onReceive(receiveEvent);
Wire.onRequest(NULL);
Wire.begin(I2CRA_TouchDisplay);
#elif defined RA_TOUCH
Board=RATouch;
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
#endif // RA_TOUCHDISPLAY
