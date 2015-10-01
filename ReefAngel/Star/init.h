// Star
Serial.println(F("RA Init"));
TouchLCD.SetBacklight(0);
Board=RAStar;
wdt_enable(WDTO_2S);
EM=0;
EM1=0;
REM=0;
orientation=InternalMemory.Touch_Orientation_read();
LastOrientation=orientation;
MilitaryTime=false;
NeedsRedraw=true;
SDFound=false;
Sleeping=false;
pinMode(53,OUTPUT);
digitalWrite(53,HIGH); // Pull up resistor on hardware SS SPI
Serial1.begin(57600);
Serial2.begin(57600);
pinMode(SDPin,OUTPUT);
digitalWrite(SDPin,HIGH);
pinMode(BuzzerPin,OUTPUT);
pinMode(actinic2PWMPin,OUTPUT);
pinMode(daylight2PWMPin,OUTPUT);
digitalWrite(actinic2PWMPin,LOW); //pull down resistor on actinicPWMPin
digitalWrite(daylight2PWMPin,LOW); //pull down resistor on daylightPWMPin
DDRJ&=(0<<3); //PJ3 as input (SD card detect pin)
PORTJ|=(1<<3); //PJ3 pull up
DDRJ&=(0<<4); //PJ4 as input (Alarm pin)
PORTJ|=(1<<4); //PJ4 pull up
DDRH|=(1<<2); // Port PH2 output (Exp Bus Power)
cbi(PORTH,2); // Turn on exp bus power
pinMode(i2cMuxEnable,OUTPUT);
digitalWrite(i2cMuxEnable,LOW);
delay(10);
digitalWrite(i2cMuxEnable,HIGH);
SPI.begin();
Serial.println(F("SPI Init"));
TouchLCD.Init();
Serial.println(F("LCD Init"));
SmallFont.SetFont(f8x8);
Font.SetFont(f12x12);
LargeFont.SetFont(ArialBold20);
setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
//EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
//NetServer.begin();
Network.Init();
Serial.println(F("Network Init"));
TS.Init();
Serial.println(F("Touch Init"));
SetOrientation(orientation);
Splash=true;
TouchEnabled=true;
lastRedraw=millis();
lastDisplayChange=millis();
RANetSeq=0;
RANetlastmillis=millis();
OkButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,CUSTOMLABELOKBUTTON,OKBUTTON);
CancelButton.Create(COLOR_WHITE,COLOR_MIDNIGHTBLUE,CUSTOMLABELCANCELBUTTON,CANCELBUTTON);
Slider.Create(COLOR_ROYALBLUE,COLOR_RED,"");
Slider.SetPosition(0,50);
Slider.Refresh();
wdt_reset();
for(int a=0;a<6;a++)
	PB[a].Create(COLOR_BLACK,COLOR_WHITE,COLOR_BLACK,"");
MenuFunctionPtr=&ReefAngelClass::Touch; // default pointer
menu_button_functions1[0] = &ReefAngelClass::FeedingModeStart;
menu_button_functions1[1] = &ReefAngelClass::WaterChangeModeStart;
menu_button_functions1[2] = &ReefAngelClass::LightsOn;
menu_button_functions1[3] = &ReefAngelClass::ChangeOrientation;
menu_button_functions1[4] = &ReefAngelClass::MainScreen;

menu_button_functions2[0] = &ReefAngelClass::Reboot;
menu_button_functions2[1] = &ReefAngelClass::SetupTouchDateTime;
menu_button_functions2[2] = &ReefAngelClass::ATOClear;
menu_button_functions2[3] = &ReefAngelClass::OverheatClear;
menu_button_functions2[4] = &ReefAngelClass::LeakClear;

menu_button_functions3[0] = &ReefAngelClass::SetupTouchCalibratePH;
menu_button_functions3[1] = &ReefAngelClass::SetupTouchCalibrateSal;
menu_button_functions3[2] = &ReefAngelClass::SetupTouchCalibrateORP;
menu_button_functions3[3] = &ReefAngelClass::SetupTouchCalibratePHExp;
menu_button_functions3[4] = &ReefAngelClass::MainScreen;

menu_button_functions4[0] = &ReefAngelClass::SetupTouchCalibrateWL0;
menu_button_functions4[1] = &ReefAngelClass::SetupTouchCalibrateWL1;
menu_button_functions4[2] = &ReefAngelClass::SetupTouchCalibrateWL2;
menu_button_functions4[3] = &ReefAngelClass::SetupTouchCalibrateWL3;
menu_button_functions4[4] = &ReefAngelClass::SetupTouchCalibrateWL4;

menu_button_functions5[0] = &ReefAngelClass::SetupTouchCalibrateCustom1;
menu_button_functions5[1] = &ReefAngelClass::SetupTouchCalibrateCustom2;
menu_button_functions5[2] = &ReefAngelClass::SetupTouchCalibrateCustom3;
menu_button_functions5[3] = &ReefAngelClass::SetupTouchCalibrateCustom4;
menu_button_functions5[4] = &ReefAngelClass::MainScreen;

menu_button_functions6[0] = &ReefAngelClass::SetupTouchCalibrateCustom5;
menu_button_functions6[1] = &ReefAngelClass::SetupTouchCalibrateCustom6;
menu_button_functions6[2] = &ReefAngelClass::SetupTouchCalibrateCustom7;
menu_button_functions6[3] = &ReefAngelClass::SetupTouchCalibrateCustom8;
menu_button_functions6[4] = &ReefAngelClass::SetupTouchCalibrateCustom1;

#if not defined NOSD
SDFound=(PINJ & (1<<PJ3))==0;
Serial.print(F("SD Found: "));
Serial.println(SDFound);
#endif // NOSD
if (SDFound)
{
	wdt_reset();
	SD.begin(SDPin);
	wdt_reset();
	Serial.println(F("SD Init"));
	if (orientation%2==0)
		TouchLCD.DrawSDRawImage("splash_l.raw",0,0,320,240);
	else
		TouchLCD.DrawSDRawImage("splash_p.raw",0,0,240,320);
}
else
{
	TouchLCD.FullClear(BKCOLOR);	
	Font.DrawCenterTextP(TouchLCD.GetWidth()/2,TouchLCD.GetHeight()/2,LABEL_REEFANGELCONTROLLER);
}
BuzzerOn(0);
delay(100);
BuzzerOff();
TouchLCD.SetBacklight(100);
for (int a=0;a<8;a++)
{
	CustomExpansionValue[a]=0;
	CustomExpansionLastValue[a]=0;
	
}
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
	Serial.println(F("No Internal Memory"));
	TouchLCD.FullClear(BKCOLOR);
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
	}
}
#ifdef I2CMASTER 
Wire.onReceive(receiveEventMaster);
Wire.onRequest(NULL);
Wire.begin(I2CRA_Master);
I2CCommand=0;
#else
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
Wire.beginTransmission(i2cMux);
Wire.write(0x6);
Wire.endTransmission();
#endif
