// Star
Board=RAStar;
wdt_enable(WDTO_2S);
Serial1.begin(57600);
Serial2.begin(57600);
LCD.BacklightOff();
pinMode(SDPin,OUTPUT);
digitalWrite(SDPin,HIGH);
pinMode(HW_SPI_Pin,OUTPUT);
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
SPI.begin();
SDFound=(PINJ & (1<<PJ3))==0;
if (SDFound) SD.begin(SDPin);
wdt_reset();

setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
//EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
//NetServer.begin();
Network.Init();
Joystick.Init();
LCD.LCDID=InternalMemory.LCDID_read();
LCD.Init();
LCD.DrawImage(98,38,15,50,RA_LOGO);
LCD.BacklightOn();
Splash=true;
RANetSeq=0;
RANetlastmillis=millis();
//0x5241494D
//0xCF06A31E
char temptext[25];
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
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

#ifndef CUSTOM_MENU
    // Initialize the Nested Menus
    InitMenus();
#endif  // CUSTOM_MENU

#ifdef MAIN_2014
    MenuItem_2014=PARAMS_2014;
    InitCustomLabels();
#endif // MAIN_2014
