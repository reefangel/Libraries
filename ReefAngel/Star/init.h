// Star

wdt_enable(WDTO_1S);
Serial1.begin(57600);
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
EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
NetServer.begin();
Joystick.Init();
LCD.LCDID=InternalMemory.LCDID_read();
LCD.Init();
LCD.DrawImage(98,38,15,50,RA_LOGO);
LCD.BacklightOn();
Splash=true;
//0x5241494D
//0xCF06A31E
pinMode(actinic2PWMPin,OUTPUT);
pinMode(daylight2PWMPin,OUTPUT);
digitalWrite(actinic2PWMPin,LOW); //pull down resistor on actinicPWMPin
digitalWrite(daylight2PWMPin,LOW); //pull down resistor on daylightPWMPin

char temptext[25];
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
	while(1)
	{
		digitalWrite(ledPin,millis()%2000>100);
		strcpy_P(temptext, NoIMCheck);
		LCD.DrawText(ModeScreenColor,DefaultBGColor,13,50,temptext);
		strcpy_P(temptext, NoIMCheck1);
		LCD.DrawText(ModeScreenColor,DefaultBGColor,50,75,temptext);
	}
}

#ifndef CUSTOM_MENU
    // Initialize the Nested Menus
    InitMenus();
#endif  // CUSTOM_MENU
