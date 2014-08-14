// Standard

Board=RA;
#if defined WDT || defined WDT_FORCE
// enable watchdog timer for 1 second.  consider allowing this option to be configured.
if ( wdtenabled ) wdt_enable(WDTO_1S);
#endif  // defined WDT || defined WDT_FORCE
LCD.BacklightOff();
#ifdef I2CMASTER 
Wire.onReceive(receiveEventMaster);
Wire.onRequest(NULL);
Wire.begin(I2CRA_Master);
I2CCommand=0;
#else // I2CMASTER
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
#endif // I2CMASTER
setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
Joystick.Init();
LCD.LCDID=InternalMemory.LCDID_read();
LCD.Init();
LCD.BacklightOn();
#ifdef REEFANGEL_MINI
    LED.RGB(0,0,0);
#endif //REEFANGEL_MINI
    
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
