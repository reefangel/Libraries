// Plus

Board=RAPlus;
wdt_enable(WDTO_1S);
LCD.BacklightOff();
#ifdef I2CMASTER 
Wire.onReceive(receiveEventMaster);
Wire.onRequest(NULL);
Wire.begin(I2CRA_Master);
I2CCommand=0;
#else
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
#endif
setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
Joystick.Init();
LCD.LCDID=InternalMemory.LCDID_read();
LCD.Init();
relaytest=(InternalMemory.read(Mem_B_TestMode)==0);
#ifdef RANET
RANET_SERIAL.begin(57600);
#endif // RANET

#ifdef MAIN_2014
LCD.DrawImage(98,38,15,50,RA_LOGO);
#endif // MAIN_2014
LCD.BacklightOn();
//0x5241494D
//0xCF06A31E
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
	char temptext[25];
	while(1)
	{
		digitalWrite(ledPin,millis()%2000<100);
		strcpy_P(temptext, NoIMCheck);
		Serial.println(temptext);
		LCD.DrawText(ModeScreenColor,DefaultBGColor,13,50,temptext);
		strcpy_P(temptext, NoIMCheck1);
		LCD.DrawText(ModeScreenColor,DefaultBGColor,50,75,temptext);
	}
}
#ifndef CUSTOM_MENU
    // Initialize the Nested Menus
    InitMenus();
#endif  // CUSTOM_MENU

#ifdef MAIN_2014
    MenuItem_2014=PARAMS_2014;
//    InitCustomLabels();
#endif // MAIN_2014
