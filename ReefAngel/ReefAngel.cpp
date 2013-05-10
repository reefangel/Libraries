/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#include <Globals.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include "ReefAngel.h"
#include <RA_Wifi.h>

byte ButtonPress = 0;

#if defined DisplayLEDPWM && ! defined RemoveAllLights
	boolean LightsOverride=true;
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
	
SIGNAL(PCINT0_vect) {
	if (millis()-ButtonDebounce>600)
	{
		ButtonDebounce=millis();
		ButtonPress++;
	}
}

// Menu Headings
const prog_char Menu_0_label[] PROGMEM = "Main:";
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
const prog_char Menu_1_label[] PROGMEM = "Setup:";
#ifndef RemoveAllLights
const prog_char Menu_2_label[] PROGMEM = "Lights:";
#endif  // RemoveAllLights
const prog_char Menu_3_label[] PROGMEM = "Temp:";
#if defined SetupExtras || defined ATOSetup
const prog_char Menu_4_label[] PROGMEM = "Timeouts:";
#endif  // defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
PROGMEM const char * const menulabel_items[] = {
	Menu_0_label,
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
	Menu_1_label,
#ifndef RemoveAllLights
	Menu_2_label,
#endif  // RemoveAllLights
	Menu_3_label,
#if defined SetupExtras || defined ATOSetup
	Menu_4_label,
#endif  // defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
};

// Return menu entries
const prog_char Return_0_label[] PROGMEM = "<- Prev Menu";
const prog_char Return_1_label[] PROGMEM = "Exit";
//const prog_char Return_2_label[] PROGMEM = "Press to exit...";
PROGMEM const char * const return_items[] = { Return_0_label, Return_1_label/*, Return_2_label*/ };

// NOTE for nested menus
// NOTE Menu labels can be a max of 20 characters
// Associate a menu name to a numeric value
// the total number must match the max number of menus

enum Menus {
    MainMenu,
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
    SetupMenu,
#ifndef RemoveAllLights
    LightsMenu,
#endif  // RemoveAllLights
    TempsMenu,
#if defined SetupExtras || defined ATOSetup
    TimeoutsMenu,
#endif  // if defined SetupExtras || defined ATOSetup
//	PreviousMenu,
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
//	ExitMenu,
};

#ifdef CUSTOM_MENU
enum CustomMenuItem {
	CustomMenu_1,
#if CUSTOM_MENU_ENTRIES >= 2
	CustomMenu_2,
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
	CustomMenu_3,
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
	CustomMenu_4,
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
	CustomMenu_5,
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
	CustomMenu_6,
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
	CustomMenu_7,
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
	CustomMenu_8,
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
	CustomMenu_9,
#endif  // CUSTOM_MENU_ENTRIES >= 9
};
#else  // CUSTOM_MENU
#ifdef SIMPLE_MENU
// This is the simplified menu
// Main Menu
const prog_char mainmenu_0_label[] PROGMEM = "Feeding";
const prog_char mainmenu_1_label[] PROGMEM = "Water Change";
const prog_char mainmenu_2_label[] PROGMEM = "ATO Clear";
const prog_char mainmenu_3_label[] PROGMEM = "Overheat Clear";
const prog_char mainmenu_4_label[] PROGMEM = "PH Calibration";
#ifdef SALINITYEXPANSION
const prog_char mainmenu_5_label[] PROGMEM = "Sal Calibration";
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
const prog_char mainmenu_6_label[] PROGMEM = "ORP Calibration";
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
const prog_char mainmenu_7_label[] PROGMEM = "PH Exp Calibration";
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
const prog_char mainmenu_8_label[] PROGMEM = "Water Calibration";
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
const prog_char mainmenu_9_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
#ifdef VersionMenu
const prog_char mainmenu_10_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char * const mainmenu_items[] = {
                    mainmenu_0_label,
                    mainmenu_1_label,
                    mainmenu_2_label,
                    mainmenu_3_label,
                    mainmenu_4_label,
#ifdef SALINITYEXPANSION
					mainmenu_5_label,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
                    mainmenu_6_label,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
					mainmenu_7_label,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
					mainmenu_8_label,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
                    mainmenu_9_label,
#endif  // DateTimeSetup
#ifdef VersionMenu
                    mainmenu_10_label
#endif  // VersionMenu
                    };
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
    MainMenu_ATOClear,
    MainMenu_OverheatClear,
    MainMenu_PHCalibration,
#ifdef SALINITYEXPANSION
	MainMenu_SalinityCalibration,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
    MainMenu_ORPCalibration,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	MainMenu_PHExpCalibration,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	MainMenu_WaterCalibration,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
    MainMenu_DateTime,
#endif  // DateTimeSetup
#ifdef VersionMenu
    MainMenu_Version
#endif  // VersionMenu
};

#else  // SIMPLE_MENU
// This is the standard menu

// Main Menu
const prog_char mainmenu_0_label[] PROGMEM = "Feeding";
const prog_char mainmenu_1_label[] PROGMEM = "Water Change";
#ifndef RemoveAllLights
const prog_char mainmenu_2_label[] PROGMEM = "Lights ->";
#endif  // RemoveAllLights
const prog_char mainmenu_3_label[] PROGMEM = "Temps ->";
#if defined SetupExtras || defined ATOSetup
const prog_char mainmenu_4_label[] PROGMEM = "Timeouts ->";
#endif  // if defined SetupExtras || defined ATOSetup
const prog_char mainmenu_5_label[] PROGMEM = "Setup ->";
#ifdef VersionMenu
const prog_char mainmenu_6_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char * const mainmenu_items[] = {
                    mainmenu_0_label,
                    mainmenu_1_label,
#ifndef RemoveAllLights
                    mainmenu_2_label,
#endif  // RemoveAllLights
                    mainmenu_3_label,
#if defined SetupExtras || defined ATOSetup
                    mainmenu_4_label,
#endif  // if defined SetupExtras || defined ATOSetup
                    mainmenu_5_label,
#ifdef VersionMenu
                    mainmenu_6_label
#endif  // VersionMenu
                    };
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
#ifndef RemoveAllLights
    MainMenu_Lights,
#endif  // RemoveAllLights
    MainMenu_Temps,
#if defined SetupExtras || defined ATOSetup
    MainMenu_Timeouts,
#endif  // if defined SetupExtras || defined ATOSetup
    MainMenu_Setup,
#ifdef VersionMenu
    MainMenu_Version
#endif  // VersionMenu
};


// Setup MenuWavemakerSetup
#ifdef WavemakerSetup
const prog_char setupmenu_0_label[] PROGMEM = "Wavemaker";
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
const prog_char setupmenu_1_label[] PROGMEM = "Single Dose";
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
const prog_char setupmenu_2_label[] PROGMEM = "Multi Dose";
#endif  // DosingPumpIntervalSetup
const prog_char setupmenu_3_label[] PROGMEM = "Calibrate pH";
#ifdef SALINITYEXPANSION
const prog_char setupmenu_4_label[] PROGMEM = "Calibrate Sal";
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
const prog_char setupmenu_5_label[] PROGMEM = "Calibrate ORP";
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
const prog_char setupmenu_6_label[] PROGMEM = "Calibrate PH Exp";
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
const prog_char setupmenu_7_label[] PROGMEM = "Calibrate Water";
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
const prog_char setupmenu_8_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
PROGMEM const char * const setupmenu_items[] = {
#ifdef WavemakerSetup
                    setupmenu_0_label,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
                    setupmenu_1_label,
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
					setupmenu_2_label,
#endif  // DosingPumpIntervalSetup
                    setupmenu_3_label,
#ifdef SALINITYEXPANSION
					setupmenu_4_label,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
                    setupmenu_5_label,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
					setupmenu_6_label,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
					setupmenu_7_label,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
                    setupmenu_8_label
#endif  // DateTimeSetup
                    };
enum SetupMenuItem {
#ifdef WavemakerSetup
    SetupMenu_Wavemaker,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
    SetupMenu_DosingPump,
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
	SetupMenu_DosingPumpInterval,
#endif  // DosingPumpIntervalSetup
    SetupMenu_CalibratePH,
#ifdef SALINITYEXPANSION
	SetupMenu_CalibrateSalinity,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
    SetupMenu_CalibrateORP,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	SetupMenu_PHExpCalibration,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	SetupMenu_WaterCalibration,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
    SetupMenu_DateTime
#endif  // DateTimeSetup
};

#ifndef RemoveAllLights
// Lights Menu
const prog_char lightsmenu_0_label[] PROGMEM = "Lights On";
const prog_char lightsmenu_1_label[] PROGMEM = "Lights Off";
#ifdef MetalHalideSetup
const prog_char lightsmenu_2_label[] PROGMEM = "Metal Halides";
const prog_char lightsmenu_3_label[] PROGMEM = "MH On Delay";
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
const prog_char lightsmenu_4_label[] PROGMEM = "Standard Lights";
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
const prog_char lightsmenu_5_label[] PROGMEM = "LED PWM";
#endif  // DisplayLEDPWM
PROGMEM const char * const lightsmenu_items[] = {
                            lightsmenu_0_label, lightsmenu_1_label,
#ifdef MetalHalideSetup
                            lightsmenu_2_label,
                            lightsmenu_3_label,
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
                            lightsmenu_4_label,
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
                            lightsmenu_5_label
#endif  // DisplayLEDPWM
                            };
enum LightsMenuItem {
    LightsMenu_On,
    LightsMenu_Off,
#ifdef MetalHalideSetup
    LightsMenu_MetalHalides,
    LightsMenu_MetalHalideDelay,
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
    LightsMenu_StandardLights,
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
    LightsMenu_LEDPWM
#endif  // DisplayLEDPWM
};
#endif  // RemoveAllLights

// Temps Menu
const prog_char tempsmenu_0_label[] PROGMEM = "Heater";
const prog_char tempsmenu_1_label[] PROGMEM = "Chiller";
#ifdef OverheatSetup
const prog_char tempsmenu_2_label[] PROGMEM = "Overheat Set";
#endif  // OverheatSetup
const prog_char tempsmenu_3_label[] PROGMEM = "Overheat Clear";
PROGMEM const char * const tempsmenu_items[] = {
                        tempsmenu_0_label,
                        tempsmenu_1_label,
#ifdef OverheatSetup
                        tempsmenu_2_label,
#endif  // OverheatSetup
                        tempsmenu_3_label};
enum TempsMenuItem {
    TempsMenu_Heater,
    TempsMenu_Chiller,
#ifdef OverheatSetup
    TempsMenu_Overheat,
#endif  // OverheatSetup
    TempsMenu_OverheatClr
};

// Timeouts Menu
#if defined SetupExtras || defined ATOSetup
#ifdef ATOSetup
const prog_char timeoutsmenu_0_label[] PROGMEM = "ATO Set";
#ifdef SingleATOSetup
const prog_char timeoutsmenu_1_label[] PROGMEM = "ATO Interval";
#endif  // SingleATOSetup
const prog_char timeoutsmenu_2_label[] PROGMEM = "ATO Clear";
#endif  // ATOSetup
#ifdef SetupExtras
const prog_char timeoutsmenu_3_label[] PROGMEM = "Feeding";
const prog_char timeoutsmenu_4_label[] PROGMEM = "LCD";
#endif  // SetupExtras
PROGMEM const char * const timeoutsmenu_items[] = {
#ifdef ATOSetup
                        timeoutsmenu_0_label,
#ifdef SingleATOSetup
                        timeoutsmenu_1_label,
#endif  // SingleATOSetup
                        timeoutsmenu_2_label,
#endif  // ATOSetup
#ifdef SetupExtras
                        timeoutsmenu_3_label,
                        timeoutsmenu_4_label
#endif  // SetupExtras
                        };
enum TimeoutsMenuItem {
#ifdef ATOSetup
    TimeoutsMenu_ATOSet,
#ifdef SingleATOSetup
    TimeoutsMenu_ATOHrInterval,
#endif  // SingleATOSetup
    TimeoutsMenu_ATOClear,
#endif  // ATOSetup
#ifdef SetupExtras
    TimeoutsMenu_Feeding,
    TimeoutsMenu_LCD
#endif  // SetupExtras
};
#endif // if defined SetupExtras || defined ATOSetup

#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU


ReefAngelClass::ReefAngelClass()
{
	SPCR=0x53;
#if defined(__AVR_ATmega2560__)
	PCMSK0 |= 128;
	DDRJ&=(0<<7); //PJ7 (Input) - Bus Lock
	PORTJ|=(1<<7); //PJ7 pull up	
#else  // __AVR_ATmega2560__
	PCMSK0 |= 32;
#endif  // __AVR_ATmega2560__
	PCICR |= 1;
}

void ReefAngelClass::Init()
{
	LCD.BacklightOff();
#if defined WDT || defined WDT_FORCE
	// enable watchdog timer for 1 second.  consider allowing this option to be configured.
	if ( wdtenabled ) wdt_enable(WDTO_1S);
#if defined(__AVR_ATmega2560__)
	wdt_enable(WDTO_1S);
#endif  // __AVR_ATmega2560__
#endif  // defined WDT || defined WDT_FORCE

#ifdef REEFTOUCHDISPLAY
	Wire.onReceive(receiveEvent);
	Wire.onRequest(NULL);
	Wire.begin(I2CRA_TouchDisplay);
#elif defined I2CMASTER 
	Wire.onReceive(receiveEventMaster);
	Wire.onRequest(NULL);
	Wire.begin(I2CRA_Master);
	I2CCommand=0;
#else // REEFTOUCHDISPLAY
	Wire.onReceive(NULL);
	Wire.onRequest(NULL);
	Wire.begin();
#endif // REEFTOUCHDISPLAY
	Serial.begin(57600);
#ifdef __PLUS_SPECIAL_WIFI__
	Serial1.begin(57600);
#endif // __PLUS_SPECIAL_WIFI__
// issue #2 - Piezo Not needed anymore
//	pinMode(Piezo, OUTPUT);
	digitalWrite(lowATOPin,HIGH); //pull up resistor on lowATOPin
	digitalWrite(highATOPin,HIGH); //pull up resistor on highATOPin
	TempSensor.Init();
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
	RAStart=now();
	LastStart = RAStart;  // Set the time normal mode is started
	BusLocked=false;  // Bus is not locked
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	orientation=1;
	LastOrientation=0;
	MilitaryTime=false;
	NeedsRedraw=true;
	SDFound=false;
	Sleeping=false;
	SPI.begin();
	TouchLCD.Init();
	SmallFont.SetFont(f8x8);
	Font.SetFont(f12x12);
	LargeFont.SetFont(ArialBold20);
	TS.Init();
	OkButton.Create(RGB565(0xA0, 0xFF, 0xA0),COLOR_RED,"Ok");
	CancelButton.Create(RGB565(0xA0, 0xFF, 0xA0),COLOR_RED,"Cancel");
	Slider.Create(COLOR_ROYALBLUE,COLOR_RED,"");
	Slider.SetPosition(0,20);
	Slider.Refresh();
	for(int a=0;a<6;a++)
		PB[a].Create(COLOR_BLACK,COLOR_WHITE,COLOR_BLACK,"");
	MenuFunctionPtr=&ReefAngelClass::Touch;
	menu_button_functions1[0] = &ReefAngelClass::FeedingModeStart;
	menu_button_functions1[1] = &ReefAngelClass::WaterChangeModeStart;
	menu_button_functions1[2] = &ReefAngelClass::ATOClear;
	menu_button_functions1[3] = &ReefAngelClass::OverheatClear;
	menu_button_functions1[4] = &ReefAngelClass::LightsOn;
	menu_button_functions1[5] = &ReefAngelClass::MainScreen;
			
#if not defined NOTILT
	Tilt.Init();
	Tilt.Refresh();
	SetOrientation(Tilt.GetOrientation());
#endif // NOTILT 

	// make sure that the default chip select pin is set to
	// output, even if you don't use it:
#if not defined NOSD
	SDFound=SD.begin(SDPin);
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
#else //  defined REEFTOUCH || defined REEFTOUCHDISPLAY
	Joystick.Init();
	LCD.LCDID=InternalMemory.LCDID_read();
	LCD.Init();
	LCD.BacklightOn();
#endif //  defined REEFTOUCH || defined REEFTOUCHDISPLAY
	ChangeMode=0;
	Flags = 0;
	Relay.AllOff();
	OverheatProbe = T2_PROBE;
	TempProbe = T1_PROBE;

	//0x5241494D
	//0xCF06A31E
	if (InternalMemory.IMCheck_read()!=0xCF06A31E)
	{
		char temptext[25];
		while(1)
		{
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
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
#else // defined REEFTOUCH || defined REEFTOUCHDISPLAY
			strcpy_P(temptext, NoIMCheck);
			LCD.DrawText(ModeScreenColor,DefaultBGColor,13,50,temptext);
			strcpy_P(temptext, NoIMCheck1);
			LCD.DrawText(ModeScreenColor,DefaultBGColor,50,75,temptext);
#endif //  REEFTOUCH
		}
	}
#ifdef ENABLE_ATO_LOGGING
	AtoEventCount = 0;
#endif  // ENABLE_ATO_LOGGING
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(Overheat_Exceed_Flag, 0);
	InternalMemory.write(ATO_Exceed_Flag, 0);
	InternalMemory.write(ATO_Single_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS


    PHMin = InternalMemory.PHMin_read();
    PHMax = InternalMemory.PHMax_read();
#ifdef ORPEXPANSION
    ORPMin = InternalMemory.ORPMin_read();
    ORPMax = InternalMemory.ORPMax_read();
#endif  // ORPEXPANSION
#ifdef SALINITYEXPANSION
    SalMax = InternalMemory.SalMax_read();
#endif  // SALINITYEXPANSION
#ifdef PHEXPANSION
    PHExpMin = InternalMemory.PHExpMin_read();
    PHExpMax = InternalMemory.PHExpMax_read();
#endif  // PHEXPANSION
    taddr = InternalMemory.T1Pointer_read();
	Params.Salinity=0;
	Params.ORP=0;
	Params.PHExp=0;

	if ((taddr>120) || (taddr<0))
	{
		InternalMemory.T1Pointer_write(0);
		taddr = 0;
	}

	Timer[FEEDING_TIMER].SetInterval(InternalMemory.FeedingTimer_read());  // Default Feeding timer
	if ( InternalMemory.LCDTimer_read() < 60 ) InternalMemory.LCDTimer_write(60);  // if it's less than 60, force it to 60
	Timer[LCD_TIMER].SetInterval(InternalMemory.LCDTimer_read());  // LCD Sleep Mode timer
	Timer[LCD_TIMER].Start();  // start timer
	Timer[PORTAL_TIMER].SetInterval(300);	// Portal
	Timer[PORTAL_TIMER].Start();  // start timer
	Timer[STORE_PARAMS_TIMER].SetInterval(720);  // Store Params
	Timer[STORE_PARAMS_TIMER].ForceTrigger();



#if defined DisplayLEDPWM && ! defined RemoveAllLights && ! defined REEFANGEL_MINI
    // Restore PWM values
    PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
    PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

#ifdef REEFANGEL_MINI
    LED.RGB(0,0,0);
#endif //REEFANGEL_MINI
    
    // Set the default ports to be turned on & off during the 2 modes
    // To enable a port to be toggled, place a 1 in the appropriate position
    // Default to have ports 4, 5, & 8 toggled
    // Override in Setup function of PDE
    //           Port   87654321
    FeedingModePorts = B10110000;
    WaterChangePorts = B10110000;

    // Set the ports that get shutoff when the overheat value is reached
    // Default to have port 3 shutoff
    //                 Port 87654321
    OverheatShutoffPorts = B00000100;

#ifdef OVERRIDE_PORTS
    // Override all relay masks for the following ports
    OverridePorts = 0;
#endif // OVERRIDE_PORTS

    // DelayedOn ports, do not manually modify this variable, let the DelayedOn function modify it
    DelayedOnPorts = 0;

#ifdef RelayExp
	// Expansion Module ports to toggle, defaults to not toggle any ports
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		FeedingModePortsE[i] = 0;
		WaterChangePortsE[i] = 0;
		OverheatShutoffPortsE[i] = 0;
		DelayedOnPortsE[i] = 0;
#ifdef OVERRIDE_PORTS
		// Override all relay masks for the following ports
		OverridePortsE[i] = 0;
#endif // OVERRIDE_PORTS
#ifndef RemoveAllLights
		LightsOnPortsE[i] = 0;
#endif  // RemoveAllLights
	}
#endif  // RelayExp

#ifndef RemoveAllLights
    // Set the ports that get turned on when you select the Lights On
    // Default to have ports 2 & 3 turn on
    //          Port 87654321
    LightsOnPorts = B00000110;
#endif  // RemoveAllLights

#ifndef CUSTOM_MENU
    // Initialize the Nested Menus
    InitMenus();
#endif  // CUSTOM_MENU

#if defined wifi || defined I2CMASTER
	EM = PWMEbit + RFEbit + AIbit + Salbit + ORPbit + IObit + PHbit + WLbit;
#ifdef wifi
	portalusername="";
#endif // wifi
#ifdef RelayExp
	for (byte a=0;a<InstalledRelayExpansionModules;a++)
	{
		REM+=1<<a;
	}
#else  // RelayExp
    REM = 0;
#endif  // RelayExp
#endif  // wifi
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		CustomVar[EID]=0;
	}
#endif //CUSTOM_VARIABLES
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	EM=0;
	REM=0;
#endif //  REEFTOUCHDISPLAY	
}

void ReefAngelClass::Refresh()
{
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
	if (ChangeMode==FEEDING_MODE)
		FeedingModeStart();
	if (ChangeMode==WATERCHANGE_MODE)
		WaterChangeModeStart();
	ChangeMode=0;
	boolean LightRelayOn=false;
	for (int l=0;l<8;l++)
	{
		if (LightsOnPorts & 1<<l)
			if (ReefAngel.Relay.RelayMaskOn & 1<<l) LightRelayOn=true;
	}

#if defined DisplayLEDPWM && !defined REEFANGEL_MINI
	if (LightRelayOn && LightsOverride)
	{
		PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
		PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
	}
// issue #3: Redundant code
// issue #12: Revert back
	analogWrite(actinicPWMPin, PWM.GetActinicValue()*2.55);
    analogWrite(daylightPWMPin, PWM.GetDaylightValue()*2.55);
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    if (!Splash)
    {
#if not defined NOTILT
		Tilt.Refresh();
		SetOrientation(Tilt.GetOrientation());
#endif // NOTILT 
    }
	if ((millis()>SplashDuration) && Splash)
	{
		Splash=false;
		if (TS.IsCalibrationNeeded())
		{
			BL1;
			CalibrateTouchScreen();
			SaveInitialSettings();
		}
		if(SDFound)	TouchLCD.FullClear(BKCOLOR);
	}
	if (PINJ&(1<<7)) // Check for bus lock
		bitClear(Flags,BusLockFlag);
	else
		bitSet(Flags,BusLockFlag);
		
#endif //  REEFTOUCH
	
#if not defined REEFTOUCHDISPLAY
#ifdef RFEXPANSION
	byte RFRecv=0;
	RFRecv=RF.RFCheck();
	if (RFRecv==1)
	{
		ClearScreen(DefaultBGColor);
		FeedingModeStart();
	}
	if (RFRecv==2)
	{
		Timer[FEEDING_TIMER].ForceTrigger();
	}
	if (DisplayedMenu!=FEEDING_MODE && RF.UseMemory) RF.SetMode(InternalMemory.RFMode_read(),InternalMemory.RFSpeed_read(),InternalMemory.RFDuration_read());
	if (LightRelayOn)
	{
		for (byte a=0; a<RF_CHANNELS; a++)
			RF.SetChannel(a,InternalMemory.read(Mem_B_RadionSlopeEndW+(3*a)));
	}	
	RF.RadionWrite();
#endif  // RFEXPANSION
#ifdef AI_LED
	if (LightRelayOn)
	{
		for (byte a=0; a<AI_CHANNELS; a++)
			AI.SetChannel(a,InternalMemory.read(Mem_B_AISlopeEndW+(3*a)));
	}	
    if (millis()-AI.AImillis>AI.StreamDelay)
    {
      AI.Send();
      AI.AImillis=millis();
    }
#endif  // AI_LED
#if defined PWMEXPANSION && defined DisplayLEDPWM
	PWM.ExpansionWrite();
#endif  // PWMEXPANSION
#ifdef IOEXPANSION
	IO.GetChannel();
#endif  // IOEXPANSION
#endif  // REEFTOUCHDISPLAY	

#ifdef OVERRIDE_PORTS
	// Reset relay masks for ports we want always in their programmed states.
	ReefAngel.Relay.RelayMaskOn &= ~OverridePorts;
	ReefAngel.Relay.RelayMaskOff |= OverridePorts;
#ifdef RelayExp
        byte i;
        for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
        {
                Relay.RelayMaskOnE[i] &= ~OverridePortsE[i];
                Relay.RelayMaskOffE[i] |= OverridePortsE[i];
        }
#endif  // RelayExp  
#endif  // OVERRRIDE_PORTS	
	
	Relay.Write();
	if (ds.read_bit()==0) return;  // ds for OneWire TempSensor
	now();
#ifdef DirectTempSensor
	RefreshScreen();
	Params.Temp[T1_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT1);
	RefreshScreen();
	Params.Temp[T2_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT2);
	RefreshScreen();
	Params.Temp[T3_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT3);
	RefreshScreen();
#else  // DirectTempSensor
    int x = TempSensor.ReadTemperature(TempSensor.addrT1);
    RefreshScreen();
    int y;
    y = x - Params.Temp[T1_PROBE];
    // check to make sure the temp readings aren't beyond max allowed
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T1_PROBE] == 0 || ~x) Params.Temp[T1_PROBE] = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT2);
    RefreshScreen();
    y = x - Params.Temp[T2_PROBE];
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T2_PROBE] == 0 || ~x) Params.Temp[T2_PROBE] = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT3);
    RefreshScreen();
    y = x - Params.Temp[T3_PROBE];
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T3_PROBE] == 0 || ~x) Params.Temp[T3_PROBE] = x;
#endif  // DirectTempSensor
    Params.PH=0;
    for (int a=0;a<20;a++)
    {
    	Params.PH+=analogRead(PHPin);
    }
    Params.PH/=20;
    RefreshScreen();
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	Params.PH=constrain(Params.PH,100,1400);
	RefreshScreen();
	TempSensor.RequestConversion();
	RefreshScreen();
#if defined SALINITYEXPANSION
	unsigned long tempsal=0;
    for (int a=0;a<20;a++)
    {
    	tempsal+=Salinity.Read();
    }
	Params.Salinity=tempsal/20;
	ApplySalinityCompensation();
	Params.Salinity=map(Params.Salinity, 0, SalMax, 60, 350); // apply the calibration to the sensor reading
	RefreshScreen();
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
	unsigned long temporp=0;
    for (int a=0;a<20;a++)
    {
    	temporp+=ORP.Read();
    }
	Params.ORP=temporp/20;
	if (Params.ORP!=0)
	{
		Params.ORP=map(Params.ORP, ORPMin, ORPMax, 0, 470); // apply the calibration to the sensor reading
		Params.ORP=constrain(Params.ORP,0,550);
	}
	RefreshScreen();
#endif  // defined ORPEXPANSION
#if defined PHEXPANSION
	unsigned long tempph=0;
    for (int a=0;a<20;a++)
    {
    	tempph+=PH.Read();
    }
	Params.PHExp=tempph/20;
	if (Params.PHExp!=0)
	{
		Params.PHExp=map(Params.PHExp, PHExpMin, PHExpMax, 700, 1000); // apply the calibration to the sensor reading
		Params.PHExp=constrain(Params.PHExp,100,1400);
	}
	RefreshScreen();
#endif  // defined PHEXPANSION
#if defined WATERLEVELEXPANSION
	WaterLevel.Convert();
	RefreshScreen();
#endif  // defined WATERLEVELEXPANSION
	OverheatCheck();
#ifdef BUSCHECK
	Wire.beginTransmission(0x68);
	Wire.write(0);
	int a=Wire.endTransmission();
	if (a==5)
	{
	  LED.On();
	  delay(20);
	  LED.Off();
	  BusLocked=true;  // Bus is locked
	}
	else
	{
	  BusLocked=false;  // Bus is not locked
	}
#endif
}

void ReefAngelClass::SetTemperatureUnit(byte unit)
{
    // 0 (or DEGREE_F) for farenheit
    // 1 (or DEGREE_C) for celsius
    TempSensor.unit = unit;
}

void ReefAngelClass::ConvertTempUnit()
{
    // check to see if the internal memory values are set correctly
    // if they are not, convert them from F to C or vice versa
    int x;
    if ( TempSensor.unit )
    {
    	// C
    	// if the values are larger than the highest temp, then we know we have F stored
    	bool fConvert = false;
    	x = InternalMemory.HeaterTempOn_read();
    	if ( x > DEGREE_C_HIGH_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_C(x);
    		InternalMemory.HeaterTempOn_write(x);
    	}
    	x = InternalMemory.HeaterTempOff_read();
    	if ( x > DEGREE_C_HIGH_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_C(x);
    		InternalMemory.HeaterTempOff_write(x);
    	}
    	x = InternalMemory.ChillerTempOn_read();
    	if ( x > DEGREE_C_HIGH_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_C(x);
    		InternalMemory.ChillerTempOn_write(x);
    	}
    	x = InternalMemory.ChillerTempOff_read();
    	if ( x > DEGREE_C_HIGH_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_C(x);
    		InternalMemory.ChillerTempOff_write(x);
    	}
    	x = InternalMemory.OverheatTemp_read();
    	if ( (x > DEGREE_C_OVERHEAT_HIGH_TEMP) || fConvert )
    	{
    		x = CONVERT_TO_C(x);
    		InternalMemory.OverheatTemp_write(x);
    	}
    }
    else
    {
    	// F
    	// if the values are smaller than lowest temp, then we know we have C stored
    	bool fConvert = false;
    	x = InternalMemory.HeaterTempOn_read();
    	if ( x < DEGREE_F_LOW_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_F(x);
    		InternalMemory.HeaterTempOn_write(x);
    	}
    	x = InternalMemory.HeaterTempOff_read();
    	if ( x < DEGREE_F_LOW_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_F(x);
    		InternalMemory.HeaterTempOff_write(x);
    	}
    	x = InternalMemory.ChillerTempOn_read();
    	if ( x < DEGREE_F_LOW_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_F(x);
    		InternalMemory.ChillerTempOn_write(x);
    	}
    	x = InternalMemory.ChillerTempOff_read();
    	if ( x < DEGREE_F_LOW_TEMP )
    	{
    		fConvert = true;
    		x = CONVERT_TO_F(x);
    		InternalMemory.ChillerTempOff_write(x);
    	}
    	x = InternalMemory.OverheatTemp_read();
    	if ( (x < DEGREE_F_OVERHEAT_LOW_TEMP) || fConvert )
    	{
    		x = CONVERT_TO_F(x);
    		InternalMemory.OverheatTemp_write(x);
    	}
    }
}

void ReefAngelClass::StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute)
{
	int NumMinsToday=NumMins(hour(),minute());
	if (NumMins(OffHour,OffMinute) > NumMins(OnHour,OnMinute))
	{
		if (NumMinsToday >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay); else Relay.Off(LightsRelay);
		if (NumMinsToday >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay);
	}
	else
	{
		if (NumMinsToday >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay); else Relay.On(LightsRelay);
		if (NumMinsToday >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay);
	}
}

void ReefAngelClass::MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay)
{
    unsigned int MHTimer = MHDelay;
    MHTimer *= SECS_PER_MIN;
    if ( now()-RAStart > MHTimer )
        StandardLights(LightsRelay, OnHour, OnMinute, OffHour, OffMinute);
}

void ReefAngelClass::StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp)
{
    if (Params.Temp[TempProbe] == 0) return;  // Don't turn the heater on if the temp is reading 0
    if (Params.Temp[TempProbe] <= LowTemp && Params.Temp[TempProbe] > 0) Relay.On(HeaterRelay);  // If sensor 1 temperature <= LowTemp - turn on heater
    if (Params.Temp[TempProbe] >= HighTemp) Relay.Off(HeaterRelay);  // If sensor 1 temperature >= HighTemp - turn off heater
}

void ReefAngelClass::StandardFan(byte FanRelay, int LowTemp, int HighTemp)
{
	if (Params.Temp[TempProbe] == 0) return;  // Don't turn the fan/chiller on if the temp is reading 0
	if (Params.Temp[TempProbe] >= HighTemp) Relay.On(FanRelay);  // If sensor 1 temperature >= HighTemp - turn on fan
	if (Params.Temp[TempProbe] <= LowTemp) Relay.Off(FanRelay);  // If sensor 1 temperature <= LowTemp - turn off fan
}

void ReefAngelClass::CO2Control(byte CO2Relay, int LowPH, int HighPH)
{
    if (Params.PH <= LowPH) Relay.Off(CO2Relay);  // If PH <= LowPH - turn on CO2
    if (Params.PH >= HighPH) Relay.On(CO2Relay);  // If sensor 1 PH >= HighPH - turn off CO2
}

void ReefAngelClass::PHControl(byte PHControlRelay, int LowPH, int HighPH)
{
    if (Params.PH <= LowPH) Relay.On(PHControlRelay);  // If PH <= LowPH - turn on PHControlRelay
    if (Params.PH >= HighPH) Relay.Off(PHControlRelay);  // If sensor 1 PH >= HighPH - turn off PHControlRelay
}

void ReefAngelClass::StandardATO(byte ATORelay, int ATOTimeout)
{
    // Input:  Relay port and timeout value (max number of seconds that ATO pump is allowed to run)
	unsigned long TempTimeout = ATOTimeout;
	TempTimeout *= 1000;

	/*
	Is the low switch active (meaning we need to top off) and are we not currently topping off
	Then we set the timer to be now and start the topping pump
	*/
    if ( LowATO.IsActive() && ( !LowATO.IsTopping()) )
    {
        LowATO.Timer = millis();
        LowATO.StartTopping();
        Relay.On(ATORelay);
    }

    // If the high switch is activated, this is a safeguard to prevent over running of the top off pump
    if ( HighATO.IsActive() )
    {
		LowATO.StopTopping();  // stop the low ato timer
		Relay.Off(ATORelay);
    }

    /*
    If the current time minus the start time of the ATO pump is greater than the specified timeout value
    AND the ATO pump is currently running:
    We turn on the status LED and shut off the ATO pump
    This prevents the ATO pump from contniously running.
    */
	if ( (millis()-LowATO.Timer > TempTimeout) && LowATO.IsTopping() )
	{
		LED.On();
		bitSet(Flags,ATOTimeOutFlag);
#ifdef ENABLE_EXCEED_FLAGS
		if (InternalMemory.read(ATO_Exceed_Flag)==0)
			InternalMemory.write(ATO_Exceed_Flag, 1);
#endif  // ENABLE_EXCEED_FLAGS
		Relay.Off(ATORelay);
#ifdef ENABLE_ATO_LOGGING
		// bump the counter if a timeout occurs
		AtoEventCount++;
		if ( AtoEventCount >= MAX_ATO_LOG_EVENTS ) { AtoEventCount = 0; }
#endif  // ENABLE_ATO_LOGGING
	}
}

#ifdef WATERLEVELEXPANSION	
void ReefAngelClass::WaterLevelATO(byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel)
{
    // Input:  Relay port and timeout value (max number of seconds that ATO pump is allowed to run)
	// Input:  Low and High Water Level to start and stop ATO pump
	unsigned long TempTimeout = ATOTimeout;
	TempTimeout *= 1000;

	/*
	Is the low level is reached (meaning we need to top off) and are we not currently topping off
	Then we set the timer to be now and start the topping pump
	*/
    if ( WaterLevel.GetLevel()<LowLevel && ( !WLATO.IsTopping()) )
    {
    	WLATO.Timer = millis();
    	WLATO.StartTopping();
        Relay.On(ATORelay);
    }

    // If the high level is reached, this is a safeguard to prevent over running of the top off pump
    if ( WaterLevel.GetLevel()>HighLevel )
    {
    	WLATO.StopTopping();  // stop the low ato timer
		Relay.Off(ATORelay);
    }

    /*
    If the current time minus the start time of the ATO pump is greater than the specified timeout value
    AND the ATO pump is currently running:
    We turn on the status LED and shut off the ATO pump
    This prevents the ATO pump from contniously running.
    */
	if ( (millis()-WLATO.Timer > TempTimeout) && WLATO.IsTopping() )
	{
		LED.On();
		bitSet(Flags,ATOTimeOutFlag);
#ifdef ENABLE_EXCEED_FLAGS
		if (InternalMemory.read(ATO_Exceed_Flag)==0)
			InternalMemory.write(ATO_Exceed_Flag, 1);
#endif  // ENABLE_EXCEED_FLAGS
		Relay.Off(ATORelay);
#ifdef ENABLE_ATO_LOGGING
		// bump the counter if a timeout occurs
		AtoEventCount++;
		if ( AtoEventCount >= MAX_ATO_LOG_EVENTS ) { AtoEventCount = 0; }
#endif  // ENABLE_ATO_LOGGING
	}
}
#endif  // WATERLEVELEXPANSION	

void ReefAngelClass::SingleATO(bool bLow, byte ATORelay, int intTimeout, byte byteHrInterval)
{
	/*
	If the switch is active, the float is opposite of the 2 wires,
		Check if we are not currently topping, if we are not check if we can run
		If we have an hour interval, check if we can run
		If we can run, activate the pump because we need water
	Otherwise the switch is not active, we need to see if we are currently topping
		If we are topping, then we need to stop the pump because we are topped off
	*/
    bool bCanRun = true;
    static int iLastTop = -1;
    if ( byteHrInterval )
    {
        int iSafeTop = NumMins(hour(), minute()) - iLastTop;
        if ( iSafeTop < 0 )
        {
            iSafeTop += 1440;
        }
        if ( (iSafeTop < (byteHrInterval * 60)) && (iLastTop >= 0) )
        {
            bCanRun = false;
        }
    }
    RA_ATOClass *ato;
    if ( bLow )
    {
        ato = &LowATO;
    }
    else
    {
        ato = &HighATO;
    }
    unsigned long t = intTimeout;
    t *= 1000;
    if ( ato->IsActive() )
    {
        if ( (! ato->IsTopping()) && bCanRun )
        {
            ato->Timer = millis();
            ato->StartTopping();
            Relay.On(ATORelay);
        }
    }
    else
    {
    	// not active
		if ( ato->IsTopping() )
		{
			iLastTop = NumMins(hour(), minute());
			ato->StopTopping();
			Relay.Off(ATORelay);
		}
    }

    if ( ((millis() - ato->Timer) > t) && ato->IsTopping() )
    {
        LED.On();
		bitSet(Flags,ATOTimeOutFlag);
#ifdef ENABLE_EXCEED_FLAGS
		if (InternalMemory.read(ATO_Single_Exceed_Flag)==0)
			InternalMemory.write(ATO_Single_Exceed_Flag, 1);
#endif  // ENABLE_EXCEED_FLAGS
        Relay.Off(ATORelay);
#ifdef ENABLE_ATO_LOGGING
		// bump the counter if a timeout occurs
		AtoEventCount++;
		if ( AtoEventCount >= MAX_ATO_LOG_EVENTS ) { AtoEventCount = 0; }
#endif  // ENABLE_ATO_LOGGING
    }
}

void ReefAngelClass::DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, int RunTime)
{
    /*
    This function configures and sets up the dosing pump and turns it on at the appropriate time
    Once the timer has expired for the dosing pump, it shuts it off

    DPRelay - relay dosing pump is plugged into (0-8)
    Timer - timer to control dosing pump
    OnHour & OnMinute - time to turn on the dosing pump (in 24hr based time)
    RunTime - duration to run the pump
    */

    // Let's see if it's supposed to start running the timer now
    if ( (NumMins(hour(), minute()) == NumMins(OnHour, OnMinute)) && (second() == 0) )
    {
        Relay.On(DPRelay);
        //LED.On();
        Timer[DPTimer].SetInterval(RunTime);
        Timer[DPTimer].Start();
    }

    // is the timer expired?
    if ( Timer[DPTimer].IsTriggered() )
    {
        // timer expired, so let's shut off the pump
        Relay.Off(DPRelay);
        //LED.Off();
    }

}

void ReefAngelClass::DosingPumpRepeat(byte DPRelay, int OffsetMinute, int RepeatMinute, int RunTime)
{
	// Old code has been replaced with dedvalson (Don) - 01/06/2012
	/*
	This function runs the specified relay for the RunTime seconds every RepeatMinute minutes.
	So you can run the relay for 10 seconds every 60 minutes (1 hour)

	This function bases the RepeatMinute off of Midnight (00:00) of the current day.  It uses midnight to
	compute when the pump will run.

	DPRelay - Relay that contains the dosing pump
	Timer - number of the timer in the timer array to use
	RepeatMinute - number of minutes to wait before running the pump again
	RunTime - duration (in seconds) to run the pump
	*/

	// if the current minutes since midnight are divisible by the repeat interval and the current seconds
	// are zero (top of the minute), then we can run the pump
	/*
	time_t t = now();
	uint8_t h = hour(t);
	if ( (h == 0) && (minute(t) == 0) )
	{
		// if we are at midnight, change hours to 24 so we can get the correct minutes for computation
		h = 24;
	}
	int current_min = NumMins(h, minute(t));
	int r = current_min % RepeatMinute;
	if ( (r == 0) && (second(t) == 0) )
	{
		Relay.On(DPRelay);
		Timer[DPTimer].SetInterval(RunTime);
		Timer[DPTimer].Start();
	}

	// Should change the timer to be a Dosing Pump Timer
	// is the timer expired?
	if ( Timer[DPTimer].IsTriggered() )
	{
		Relay.Off(DPRelay);
	}
	*/
	signed long t=(elapsedSecsToday(now())-((long)OffsetMinute*60));
	Relay.Set(DPRelay,(t%((long)RepeatMinute*60))<RunTime && t>=0);
}

void ReefAngelClass::Wavemaker(byte WMRelay, int WMTimer)
{
	// Old code has been replaced with dedvalson (Don) - 01/06/2012
	Relay.Set(WMRelay,(now()%(WMTimer*2))<WMTimer);
}

void ReefAngelClass::WavemakerRandom(byte WMRelay, int MinWMTimer, int MaxWMTimer)
{
	static time_t WMRTimer=now()+MinWMTimer;
	if (now()>WMRTimer)
	{
		WMRTimer=now()+random(MinWMTimer, MaxWMTimer);
		Relay.Toggle(WMRelay);
	}
}

void ReefAngelClass::WavemakerRandom1(byte WMRelay, int MinWMTimer, int MaxWMTimer)
{
	static time_t WMRTimer1=now()+MinWMTimer;
	if (now()>WMRTimer1)
	{
		WMRTimer1=now()+random(MinWMTimer, MaxWMTimer);
		Relay.Toggle(WMRelay);
	}
}

void ReefAngelClass::WavemakerRandom2(byte WMRelay, int MinWMTimer, int MaxWMTimer)
{
	static time_t WMRTimer2=now()+MinWMTimer;
	if (now()>WMRTimer2)
	{
		WMRTimer2=now()+random(MinWMTimer, MaxWMTimer);
		Relay.Toggle(WMRelay);
	}
}

void ReefAngelClass::WavemakerToggle(byte WMRelay1, byte WMRelay2, int WMTimer)
{
	if ( (now()%(WMTimer*2))<WMTimer )
	{
		Relay.On(WMRelay1);
		Relay.Off(WMRelay2);
	}
	else
	{
		Relay.Off(WMRelay1);
		Relay.On(WMRelay2);
	}
}

// Simplified for PDE file
void ReefAngelClass::StandardLights(byte Relay)
{
    StandardLights(Relay,
                   InternalMemory.StdLightsOnHour_read(),
                   InternalMemory.StdLightsOnMinute_read(),
                   InternalMemory.StdLightsOffHour_read(),
                   InternalMemory.StdLightsOffMinute_read());
}

void ReefAngelClass::StandardLights(byte Relay, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	StandardLights(Relay,
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60
			);
}

void ReefAngelClass::DayLights(byte Relay)
{
	StandardLights(Relay);
}

void ReefAngelClass::ActinicLights(byte Relay)
{
	StandardLights(Relay, InternalMemory.ActinicOffset_read());
}

void ReefAngelClass::DelayedStartLights(byte Relay)
{
    MHLights(Relay,
           InternalMemory.StdLightsOnHour_read(),
           InternalMemory.StdLightsOnMinute_read(),
           InternalMemory.StdLightsOffHour_read(),
           InternalMemory.StdLightsOffMinute_read(),
     	   InternalMemory.MHDelay_read());
}

void ReefAngelClass::MoonLights(byte Relay)
{
    StandardLights(Relay,
                   InternalMemory.StdLightsOffHour_read(),
                   InternalMemory.StdLightsOffMinute_read(),
                   InternalMemory.StdLightsOnHour_read(),
                   InternalMemory.StdLightsOnMinute_read());
}


void ReefAngelClass::MHLights(byte Relay)
{
    MHLights(Relay,
             InternalMemory.MHOnHour_read(),
             InternalMemory.MHOnMinute_read(),
             InternalMemory.MHOffHour_read(),
             InternalMemory.MHOffMinute_read(),
             InternalMemory.MHDelay_read());
}

void ReefAngelClass::StandardHeater(byte Relay)
{
    StandardHeater(Relay,
                   InternalMemory.HeaterTempOn_read(),
                   InternalMemory.HeaterTempOff_read());
}

void ReefAngelClass::StandardFan(byte Relay)
{
    StandardFan(Relay,
                InternalMemory.ChillerTempOff_read(),
                InternalMemory.ChillerTempOn_read());
}

void ReefAngelClass::CO2Control(byte Relay)
{
    CO2Control(Relay,
                InternalMemory.CO2ControlOff_read(),
                InternalMemory.CO2ControlOn_read());
}

void ReefAngelClass::PHControl(byte Relay)
{
	PHControl(Relay,
                InternalMemory.PHControlOn_read(),
                InternalMemory.PHControlOff_read());
}

void ReefAngelClass::StandardATO(byte Relay)
{
    StandardATO(Relay, InternalMemory.ATOExtendedTimeout_read());
}

void ReefAngelClass::SingleATOLow(byte Relay)
{
    SingleATO(true, Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.ATOHourInterval_read());
}

void ReefAngelClass::SingleATOHigh(byte Relay)
{
    SingleATO(false, Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.ATOHourInterval_read());
}

void ReefAngelClass::StandardATOExtended(byte Relay)
{
    StandardATO(Relay);
}

void ReefAngelClass::SingleATOLowExtended(byte Relay)
{
	SingleATOLow(Relay);
}

void ReefAngelClass::SingleATOHighExtended(byte Relay)
{
	SingleATOHigh(Relay);
}

#ifdef WATERLEVELEXPANSION
void ReefAngelClass::WaterLevelATO(byte Relay)
{
	WaterLevelATO(Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.WaterLevelLow_read(), InternalMemory.WaterLevelHigh_read());
}
#endif  // WATERLEVELEXPANSION

void ReefAngelClass::DosingPump1(byte Relay)
{
    DosingPump(Relay, 1,
               InternalMemory.DP1OnHour_read(),
               InternalMemory.DP1OnMinute_read(),
               InternalMemory.DP1Timer_read());
}

void ReefAngelClass::DosingPump2(byte Relay)
{
    DosingPump(Relay, 2,
               InternalMemory.DP2OnHour_read(),
               InternalMemory.DP2OnMinute_read(),
               InternalMemory.DP2Timer_read());
}

void ReefAngelClass::DosingPumpRepeat1(byte Relay)
{
	// 0 minute offset
	DosingPumpRepeat(Relay, 0,
					InternalMemory.DP1RepeatInterval_read(),
					InternalMemory.DP1Timer_read());
}

void ReefAngelClass::DosingPumpRepeat2(byte Relay)
{
	// 5 minute offset
	DosingPumpRepeat(Relay, 5,
					InternalMemory.DP2RepeatInterval_read(),
					InternalMemory.DP2Timer_read());
}

void ReefAngelClass::DosingPumpRepeat3(byte Relay)
{
	// 5 minute offset
	DosingPumpRepeat(Relay, 10,
					InternalMemory.DP3RepeatInterval_read(),
					InternalMemory.DP3Timer_read());
}

void ReefAngelClass::Wavemaker1(byte WMRelay)
{
	Wavemaker(WMRelay,InternalMemory.WM1Timer_read());
//#ifdef WavemakerSetup
//	WM1Port = WMRelay; deprecated by issue #47
//#endif
}

void ReefAngelClass::Wavemaker2(byte WMRelay)
{
	Wavemaker(WMRelay,InternalMemory.WM2Timer_read());
//#ifdef WavemakerSetup
//	WM2Port = WMRelay; deprecated by issue #47
//#endif
}

#ifdef VersionMenu
void ReefAngelClass::DisplayVersion()
{
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	LargeFont.DrawText(WARNING_TEXT,BKCOLOR,20,20,"Reef Angel");
	LargeFont.DrawText(WARNING_TEXT,BKCOLOR,20,20,"v"ReefAngel_Version);
#else  // REEFTOUCH
    // Display the Software Version
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,10,"Reef Angel");
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,20,"v"ReefAngel_Version);
#ifdef wifi
    // Display wifi related information
    // Place holder information currently, need wifi module
    // to be able to write functions to retrieve actual information
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,30,"Wifi");
#endif  // wifi
#if defined WDT || defined WDT_FORCE
	LCD.DrawText(ModeScreenColor,DefaultBGColor,40,30,"WDT");
#endif
#ifdef RelayExp
	LCD.DrawText(ModeScreenColor,DefaultBGColor,10,40,InstalledRelayExpansionModules);
#endif  // RelayExp
#endif  // REEFTOUCH
}
#endif  // VersionMenu

void ReefAngelClass::ClearScreen(byte Color)
{
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	TouchLCD.FullClear(BKCOLOR);
#else  // REEFTOUCH
    // clears the entire screen
    LCD.Clear(Color, 0, 0, 131, 131);
#endif  // REEFTOUCH
}

#if defined DisplayLEDPWM && ! defined RemoveAllLights
void ReefAngelClass::MoonlightPWM(byte RelayID, bool ShowPWM)
{
	int m,d,y;
	int yy,mm;
	long K1,K2,K3,J,V;
	byte PWMvalue;
	m = month();
	d = day();
	y = year();
	yy = y - ((12-m)/10);
	mm = m + 9;
	if (mm>=12) mm -= 12;
	K1 = 365.25 * (yy+4712);
	K2 = 30.6 * mm+.5;
	K3 = int(int((yy/100)+49)*.75)-38;
	J = K1+K2+d+59-K3;
	V = (J-2451550.1)/0.29530588853;
	V -= int(V/100)*100;
	V = abs(V-50);
	PWMvalue = 4*abs(50-V);  // 5.12=100%    4=~80%
	pinMode(lowATOPin,OUTPUT);
	if (RelayID && (bitRead(Relay.RelayData,RelayID-1)==0)) PWMvalue=0;
	analogWrite(lowATOPin,PWMvalue);
	if (ShowPWM) PWM.SetActinic((PWMvalue*100)/255);
}
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

#ifdef wifi
void ReefAngelClass::LoadWebBanner(int pointer, byte qty)
{
//	webbannerpointer = pointer;
//	webbannerqty = qty;
}

void ReefAngelClass::Portal(char *username)
{
	/*
	static byte LastRelayData;
    byte TempRelay = Relay.RelayData;
    TempRelay &= Relay.RelayMaskOff;
    TempRelay |= Relay.RelayMaskOn;
    if (TempRelay!=LastRelayData)
    {
    	Timer[PORTAL_TIMER].ForceTrigger();
    	LastRelayData=TempRelay;
    }
#ifdef RelayExp
	static byte LastRelayDataE[MAX_RELAY_EXPANSION_MODULES];

    for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		TempRelay = Relay.RelayDataE[EID];
		TempRelay &= Relay.RelayMaskOffE[EID];
		TempRelay |= Relay.RelayMaskOnE[EID];
	    if (TempRelay!=LastRelayDataE[EID])
	    {
	    	Timer[PORTAL_TIMER].ForceTrigger();
	    	LastRelayDataE[EID]=TempRelay;
	    }
	}
#endif  // RelayExp
	 */
	if (Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,"");
	portalusername=username;
}

void ReefAngelClass::Portal(char *username, char *key)
{
	if (Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,key);
	portalusername=username;
}

void ReefAngelClass::SendPortal(char *username, char*key)
{
	Timer[PORTAL_TIMER].Start();
	PROGMEMprint(BannerGET);
	WIFI_SERIAL.print(Params.Temp[T1_PROBE], DEC);
	PROGMEMprint(BannerT2);
	WIFI_SERIAL.print(Params.Temp[T2_PROBE], DEC);
	PROGMEMprint(BannerT3);
	WIFI_SERIAL.print(Params.Temp[T3_PROBE], DEC);
	PROGMEMprint(BannerPH);
	WIFI_SERIAL.print(Params.PH, DEC);
	PROGMEMprint(BannerID);
	WIFI_SERIAL.print(username);
	PROGMEMprint(BannerEM);
	WIFI_SERIAL.print(EM, DEC);
	PROGMEMprint(BannerREM);
	WIFI_SERIAL.print(REM, DEC);
	PROGMEMprint(BannerKey);
	WIFI_SERIAL.print(key);
	PROGMEMprint(BannerATOHIGH);
	WIFI_SERIAL.print(HighATO.IsActive(), DEC);
	PROGMEMprint(BannerATOLOW);
	WIFI_SERIAL.print(LowATO.IsActive(), DEC);
	PROGMEMprint(BannerRelayData);
	WIFI_SERIAL.print("=");
	WIFI_SERIAL.print(Relay.RelayData, DEC);
	PROGMEMprint(BannerRelayMaskOn);
	WIFI_SERIAL.print("=");
	WIFI_SERIAL.print(Relay.RelayMaskOn, DEC);
	PROGMEMprint(BannerRelayMaskOff);
	WIFI_SERIAL.print("=");
	WIFI_SERIAL.print(Relay.RelayMaskOff, DEC);

#ifdef RelayExp
	for ( byte x = 0; x < InstalledRelayExpansionModules && x < MAX_RELAY_EXPANSION_MODULES; x++ )
	{
		PROGMEMprint(BannerRelayData);
		WIFI_SERIAL.print(x+1, DEC);
		WIFI_SERIAL.print("=");
		WIFI_SERIAL.print(Relay.RelayDataE[x], DEC);
		PROGMEMprint(BannerRelayMaskOn);
		WIFI_SERIAL.print(x+1, DEC);
		WIFI_SERIAL.print("=");
		WIFI_SERIAL.print(Relay.RelayMaskOnE[x], DEC);
		PROGMEMprint(BannerRelayMaskOff);
		WIFI_SERIAL.print(x+1, DEC);
		WIFI_SERIAL.print("=");
		WIFI_SERIAL.print(Relay.RelayMaskOffE[x], DEC);
	}  // for x
#endif  // RelayExp
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	PROGMEMprint(BannerPWMA);
	WIFI_SERIAL.print(PWM.GetActinicValue(), DEC);
	PROGMEMprint(BannerPWMD);
	WIFI_SERIAL.print(PWM.GetDaylightValue(), DEC);
#endif  // DisplayLEDPWM && ! defined RemoveAllLights
#ifdef PWMEXPANSION
	for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
	{
		PROGMEMprint(BannerPWME);
		WIFI_SERIAL.print(EID, DEC);
		WIFI_SERIAL.print("=");
		WIFI_SERIAL.print(PWM.GetChannelValue(EID), DEC);
	}
#endif  // PWMEXPANSION
#ifdef RFEXPANSION
	PROGMEMprint(BannerRFM);
	WIFI_SERIAL.print(RF.Mode, DEC);
	PROGMEMprint(BannerRFS);
	WIFI_SERIAL.print(RF.Speed, DEC);
	PROGMEMprint(BannerRFD);
	WIFI_SERIAL.print(RF.Duration, DEC);
	PROGMEMprint(BannerRFW);
	WIFI_SERIAL.print(RF.GetChannel(0), DEC);
	PROGMEMprint(BannerRFRB);
	WIFI_SERIAL.print(RF.GetChannel(1), DEC);
	PROGMEMprint(BannerRFR);
	WIFI_SERIAL.print(RF.GetChannel(2), DEC);
	PROGMEMprint(BannerRFG);
	WIFI_SERIAL.print(RF.GetChannel(3), DEC);
	PROGMEMprint(BannerRFB);
	WIFI_SERIAL.print(RF.GetChannel(4), DEC);
	PROGMEMprint(BannerRFI);
	WIFI_SERIAL.print(RF.GetChannel(5), DEC);
#endif  // RFEXPANSION
#ifdef AI_LED
	PROGMEMprint(BannerAIW);
	WIFI_SERIAL.print(AI.GetChannel(0), DEC);
	PROGMEMprint(BannerAIB);
	WIFI_SERIAL.print(AI.GetChannel(1), DEC);
	PROGMEMprint(BannerAIRB);
	WIFI_SERIAL.print(AI.GetChannel(2), DEC);
#endif  // AI_LED
#ifdef SALINITYEXPANSION
	PROGMEMprint(BannerSal);
	WIFI_SERIAL.print(Params.Salinity, DEC);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
	PROGMEMprint(BannerORP);
	WIFI_SERIAL.print(Params.ORP, DEC);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	PROGMEMprint(BannerPHE);
	WIFI_SERIAL.print(Params.PHExp, DEC);
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	PROGMEMprint(BannerWL);
	WIFI_SERIAL.print(WaterLevel.GetLevel(), DEC);
#endif  // WATERLEVELEXPANSION
#ifdef IOEXPANSION
	PROGMEMprint(BannerIO);
	WIFI_SERIAL.print(IO.GetChannel(), DEC);
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		PROGMEMprint(BannerCustomVar);
		WIFI_SERIAL.print(EID, DEC);
		WIFI_SERIAL.print("=");
		WIFI_SERIAL.print(CustomVar[EID], DEC);
	}
#endif  // CUSTOM_VARIABLES
	WIFI_SERIAL.println("\n\n");
}
#endif  // wifi

void ReefAngelClass::FeedingModeStart()
{
	// turn off ports
#ifdef SaveRelayState
	// TODO Test SaveRelayState
	CurrentRelayState = Relay.RelayData;
#endif  // SaveRelayState
	Relay.RelayMaskOff &= ~FeedingModePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] &= ~FeedingModePortsE[i];
	}
#endif  // RelayExp
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY	
	NeedsRedraw=true;
#else  // REEFTOUCH
	ClearScreen(DefaultBGColor);
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 30, 10, "Feeding Mode");
#ifdef DisplayImages
	LCD.DrawEEPromImage(40,50, 40, 30, I2CEEPROM2, I2CEEPROM2_Feeding);
#endif  // DisplayImages
#endif  // REEFTOUCH
#ifdef RFEXPANSION
	RF.SetMode(Feeding_Start,0,0);
#endif  // RFEXPANSION
	Timer[FEEDING_TIMER].Start();  //Start Feeding Mode timer
	// Tell controller what mode we are in
	SetDisplayedMenu(FEEDING_MODE);
}

void ReefAngelClass::WaterChangeModeStart()
{
	// turn off ports
#ifdef SaveRelayState
	// TODO Test SaveRelayState
	CurrentRelayState = Relay.RelayData;
#endif  // SaveRelayState
	Relay.RelayMaskOff &= ~WaterChangePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] &= ~WaterChangePortsE[i];
	}
#endif  // RelayExp
//	Relay.Write();
	ClearScreen(DefaultBGColor);
	// Display the water change mode
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	NeedsRedraw=true;
#else  // REEFTOUCH
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 20, 10, "Water Change Mode");
#ifdef DisplayImages
	LCD.DrawEEPromImage(51,55, 40, 30, I2CEEPROM2, I2CEEPROM2_Water_Change);
#endif  // DisplayImages
#endif  // REEFTOUCH
	// Tell controller what mode we are in
	SetDisplayedMenu(WATERCHANGE_MODE);
}

void ReefAngelClass::ATOClear()
{
	LED.Off();
	bitClear(Flags,ATOTimeOutFlag);
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(ATO_Single_Exceed_Flag, 0);
	InternalMemory.write(ATO_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS
	LowATO.StopTopping();
	HighATO.StopTopping();
#ifdef WATERLEVELEXPANSION
	WLATO.StopTopping();
#endif // WATERLEVELEXPANSION
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // REEFTOUCH
#ifdef REEFTOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_CLEAR_ATO,0);
#endif // REEFTOUCHDISPLAY

}

void ReefAngelClass::OverheatCheck()
{
	// if overheat probe exceeds the temp
	if ( Params.Temp[OverheatProbe] < InternalMemory.OverheatTemp_read() )
		Overheatmillis=millis();
	if (millis()-Overheatmillis>3000) // Only flag overheat if we have overheat for 3 seconds
	{
		LED.On();
		bitSet(Flags,OverheatFlag);
#ifdef ENABLE_EXCEED_FLAGS
		InternalMemory.write(Overheat_Exceed_Flag, 1);
#endif  // ENABLE_EXCEED_FLAGS
		// invert the ports that are activated
		Relay.RelayMaskOff &= ~OverheatShutoffPorts;
#ifdef RelayExp
		for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
		{
			Relay.RelayMaskOffE[i] &= ~OverheatShutoffPortsE[i];
		}
#endif  // RelayExp
	}
}

void ReefAngelClass::OverheatClear()
{
	LED.Off();
	bitClear(Flags,OverheatFlag);
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(Overheat_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS
	Relay.RelayMaskOff |= OverheatShutoffPorts;
#ifdef RelayExp
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] |= OverheatShutoffPortsE[i];
	}
#endif  // RelayExp
	Relay.Write();
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // REEFTOUCH
#ifdef REEFTOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_CLEAR_OVERHEAT,0);
#endif // REEFTOUCHDISPLAY
}

void ReefAngelClass::LightsOn()
{
    // turn on ports
    Relay.RelayMaskOn |= LightsOnPorts;
#ifdef RelayExp
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOnE[i] |= LightsOnPortsE[i];
	}
#endif  // RelayExp
    Relay.Write();
	bitSet(Flags,LightsOnFlag);
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	menu_button_functions1[4] = &ReefAngelClass::LightsOff;
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // REEFTOUCH
#ifdef REEFTOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_LIGHTS_ON,0);
#endif // REEFTOUCHDISPLAY
}

void ReefAngelClass::LightsOff()
{
    // reset ports
    Relay.RelayMaskOn &= ~LightsOnPorts;
#ifdef RelayExp
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOnE[i] &= ~LightsOnPortsE[i];
	}
#endif  // RelayExp
#if defined DisplayLEDPWM && !defined REEFANGEL_MINI
	// TODO should possibly store the PWM value to be reset instead of turning off completely
    // sets PWM to 0%
    PWM.SetActinic(0);
    PWM.SetDaylight(0);
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI
    Relay.Write();
	bitClear(Flags,LightsOnFlag);
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	menu_button_functions1[4] = &ReefAngelClass::LightsOn;
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // REEFTOUCH
#ifdef REEFTOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_LIGHTS_OFF,0);
#endif // REEFTOUCHDISPLAY
}

void ReefAngelClass::RefreshScreen()
{
#if not defined REEFTOUCH && not defined REEFTOUCHDISPLAY
    LCD.PutPixel(DefaultBGColor,1,1);
#endif  // REEFTOUCH
}

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
void ReefAngelClass::SetOrientation(byte o)
{
	if (orientation!=o) NeedsRedraw=true;
	orientation=o;
	TouchLCD.SetOrientation(o);
	TS.SetOrientation(o);
}

void ReefAngelClass::CalibrateTouchScreen()
{
	boolean calibrated=false;
	long CalibrationPoints[4]={24,32,213,287};
	long TouchedPoints[4];

	SetOrientation(1);
	while (!calibrated)
	{
		wdt_reset();
		TouchLCD.FullClear(COLOR_WHITE);
		LargeFont.SetColor(COLOR_GOLD,COLOR_WHITE,true);
		LargeFont.DrawCenterTextP(120,45,CALI1);
		LargeFont.DrawCenterTextP(120,75,CALI2);
		Font.SetColor(COLOR_BLACK,COLOR_WHITE,true);
		Font.DrawCenterTextP(120,190,CALI3);
		Font.DrawCenterTextP(120,205,CALI4);
		for(byte a = 0; a<2; a++)
		{
			TouchLCD.DrawCircle(COLOR_RED,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 5, false);
			do wdt_reset(); while (!TS.IsTouched());
			TS.GetTouch();
//			Serial.println(TS.uX,DEC);
//			Serial.println(TS.uY,DEC);
			if (a==0)
			{
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,40,25,TS.uX);			
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,40,35,TS.uY);
			}
			else
			{
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,175,280,TS.uX);			
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,175,290,TS.uY);
			}
			TouchedPoints[a*2] = TS.uX;
			TouchedPoints[(a*2)+1] = TS.uY;
			TouchLCD.DrawCircle(COLOR_BLUE,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 4, true);
			delay(500);
		}
		TS.calibration.XMin=map(0,CalibrationPoints[0],CalibrationPoints[2],TouchedPoints[0],TouchedPoints[2]);
		TS.calibration.XMax=map(239,CalibrationPoints[0],CalibrationPoints[2],TouchedPoints[0],TouchedPoints[2]);
		TS.calibration.YMin=map(0,CalibrationPoints[1],CalibrationPoints[3],TouchedPoints[1],TouchedPoints[3]);
		TS.calibration.YMax=map(319,CalibrationPoints[1],CalibrationPoints[3],TouchedPoints[1],TouchedPoints[3]);
		if (
			TS.calibration.XMin > TS_CALIBRATION_XMIN - TS_CALIBRATION_DELTA && 
			TS.calibration.XMin < TS_CALIBRATION_XMIN + TS_CALIBRATION_DELTA &&
			TS.calibration.XMax > TS_CALIBRATION_XMAX - TS_CALIBRATION_DELTA && 
			TS.calibration.XMax < TS_CALIBRATION_XMAX + TS_CALIBRATION_DELTA &&
			TS.calibration.YMin > TS_CALIBRATION_YMIN - TS_CALIBRATION_DELTA &&
			TS.calibration.YMin < TS_CALIBRATION_YMIN + TS_CALIBRATION_DELTA &&
			TS.calibration.YMax > TS_CALIBRATION_YMAX - TS_CALIBRATION_DELTA &&
			TS.calibration.YMax < TS_CALIBRATION_YMAX + TS_CALIBRATION_DELTA
			) calibrated=true;
//		calibrated=true;
	}
    TS.SaveCalibration();
    TouchLCD.Clear(COLOR_WHITE,0,180,239,270);
	OkButton.SetPosition(78,200);
	OkButton.Show();
	do
	{
		wdt_reset();
		TS.GetTouch();
	}
	while(!OkButton.IsPressed());
	OkButton.Hide();
	TouchLCD.FullClear(COLOR_WHITE);
}

void ReefAngelClass::SaveInitialSettings()
{
	// Initialize EEPROM strings
	char tempname[15];
	char temp[3];
	int eindex=0;
	
	// Temperature Probes
	for (int a=0; a<3; a++)
	{
		strcpy_P(tempname,LABEL_TEMP);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(Probe1Name+(a*0x10)), sizeof(tempname));
	}
	
	// Relays
	for (int a=0; a<=8; a++)
	{
		for (int b=0; b<8; b++)
		{
			strcpy_P(tempname,LABEL_RELAY);
			if (a>0)
			{
				itoa(a,temp,10);
				strcat(tempname,temp);
			}
			itoa(b+1,temp,10);
			strcat(tempname,temp);
			eeprom_write_block((void*)&tempname, (void*)(R1Name+(eindex*0x10)), sizeof(tempname));
			eindex++;
		}
	}

	//PWM Expansion Channels
	for (int a=0; a<6; a++)
	{
		strcpy_P(tempname,LABEL_CHANNEL);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(PWMChannel1+(a*0x10)), sizeof(tempname));
	}

	//IO Expansion Channels
	for (int a=0; a<6; a++)
	{
		strcpy_P(tempname,LABEL_IOPORT);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(IOChannel1+(a*0x10)), sizeof(tempname));
	}		
}

void ReefAngelClass::ChangeDisplayedScreen(signed char index)
{
	NeedsRedraw=true;
	DisplayedScreen+=index;
	if (DisplayedScreen<0) DisplayedScreen=MAX_SCREENS-1;
	if (DisplayedScreen>RELAY_BOX)
		if (index>0)
		{
			for (int a=DisplayedScreen;a<MAX_SCREENS;a++)
			{
				if (a<10)
				{
					if (bitRead(REM,a-2)) break;
				}
				else
				{
					if (a==10 && bitRead(EM,0)) break;
					if (a==11 && bitRead(EM,1)) break;
					if (a==12 && bitRead(EM,1)) break;
					if (a==13 && bitRead(EM,2)) break;
					if (a==14 && bitRead(EM,5)) break;
				}
				DisplayedScreen++;
			}
		}
		else
		{
			for (int a=DisplayedScreen;a>=2;a--)
			{
				if (a<10)
				{
					if (bitRead(REM,a-2)) break;
				}
				else
				{
					if (a==10 && bitRead(EM,0)) break;
					if (a==11 && bitRead(EM,1)) break;
					if (a==12 && bitRead(EM,1)) break;
					if (a==13 && bitRead(EM,2)) break;
					if (a==14 && bitRead(EM,5)) break;
//					if (bitRead(EM,a-10)) break;
				}
				DisplayedScreen--;
			}
		}
	if (DisplayedScreen>=MAX_SCREENS) DisplayedScreen=0;
}

void ReefAngelClass::MainScreen()
{
	ExitMenu();
	NeedsRedraw=true;	
}

#endif  // REEFTOUCH

#ifdef CUSTOM_MENU
void ReefAngelClass::InitMenu(int ptr, byte qty)
{
    // loads all the menus
    menusptr[MainMenu] = ptr;
    menuqtysptr[MainMenu] = qty;
    // initialize menus
    PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(DEFAULT_MENU);
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}
#else
void ReefAngelClass::InitMenus()
{
    // loads all the menus
    menusptr[MainMenu] = pgm_read_word(&(mainmenu_items[0]));
    menuqtysptr[MainMenu] = SIZE(mainmenu_items);

#ifndef SIMPLE_MENU
    menusptr[SetupMenu] = pgm_read_word(&(setupmenu_items[0]));
    menuqtysptr[SetupMenu] = SIZE(setupmenu_items);
#ifndef RemoveAllLights
    menusptr[LightsMenu] = pgm_read_word(&(lightsmenu_items[0]));
    menuqtysptr[LightsMenu] = SIZE(lightsmenu_items);
#endif  // RemoveAllLights
    menusptr[TempsMenu] = pgm_read_word(&(tempsmenu_items[0]));
    menuqtysptr[TempsMenu] = SIZE(tempsmenu_items);
#if defined SetupExtras || defined ATOSetup
    menusptr[TimeoutsMenu] = pgm_read_word(&(timeoutsmenu_items[0]));
    menuqtysptr[TimeoutsMenu] = SIZE(timeoutsmenu_items);
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // SIMPLE_MENU

    // initialize menus
    PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(DEFAULT_MENU);
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}
#endif  // CUSTOM_MENU

#ifdef I2CMASTER
void ReefAngelClass::UpdateTouchDisplay()
{
	// ID 0 - T1, T2, T3 and pH
	Wire.beginTransmission(I2CRA_TouchDisplay);
	Wire.write(0);
	Wire.write(Params.Temp[T1_PROBE]%256);
	Wire.write(Params.Temp[T1_PROBE]/256);
	Wire.write(Params.Temp[T2_PROBE]%256);
	Wire.write(Params.Temp[T2_PROBE]/256);
	Wire.write(Params.Temp[T3_PROBE]%256);
	Wire.write(Params.Temp[T3_PROBE]/256);
	Wire.write(Params.PH%256);
	Wire.write(Params.PH/256);
	Wire.endTransmission();
	delay(10);
	wdt_reset();

	// ID 1 - R, RON, ROFF, ATO, EM, REM, DisplayedMenu and Flags
	byte atostatus=0;
	Wire.beginTransmission(I2CRA_TouchDisplay);
	Wire.write(1);
	Wire.write(Relay.RelayData);
	Wire.write(Relay.RelayMaskOn);
	Wire.write(Relay.RelayMaskOff);
	if (ReefAngel.LowATO.IsActive())
		bitSet(atostatus,0);
	else
		bitClear(atostatus,0);
	if (ReefAngel.HighATO.IsActive())
		bitSet(atostatus,1);
	else
		bitClear(atostatus,1);
	Wire.write(atostatus);
	Wire.write(EM);
	Wire.write(REM);
	Wire.write(DisplayedMenu);
	Wire.write(Flags);
	Wire.endTransmission();
	delay(10);
	wdt_reset();

	// ID 2 - PWM Daylight, Actinic and Exp. Channels
	Wire.beginTransmission(I2CRA_TouchDisplay);
	Wire.write(2);
#ifdef DisplayLEDPWM
	Wire.write(PWM.GetDaylightValue());
	Wire.write(PWM.GetActinicValue());
#else
	Wire.write(0);
	Wire.write(0);
#endif  // DisplayLEDPWM

#ifdef PWMEXPANSION
	for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
		Wire.write(PWM.GetChannelValue(a));
#else
	for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
		Wire.write(0);
#endif //  PWMEXPANSION
	Wire.endTransmission();
	delay(10);
	wdt_reset();

	// ID 3 - RF Mode, Speed and Duration - AI Channels
#ifdef RFEXPANSION
	Wire.beginTransmission(I2CRA_TouchDisplay);
	Wire.write(3);
	Wire.write(RF.Mode);
	Wire.write(RF.Speed);
	Wire.write(RF.Duration);
#ifdef AI_LED
	Wire.write(AI.GetChannel(0));
	Wire.write(AI.GetChannel(1));
	Wire.write(AI.GetChannel(2));
#else
	Wire.write(0);
	Wire.write(0);
	Wire.write(0);
#endif // AI_LED
#ifdef IOEXPANSION
	Wire.write(IO.IOPorts);
#else
	Wire.write(0);
#endif // IOEXPANSION
	Wire.write(0);
	Wire.endTransmission();
	delay(10);
	wdt_reset();
#endif //  RFEXPANSION

	// ID 4 - RF Radion Channels
#ifdef RFEXPANSION
	Wire.beginTransmission(I2CRA_TouchDisplay);
	Wire.write(4);
	for (int a=0;a<RF_CHANNELS;a++)
		Wire.write(RF.RadionChannels[a]);
	Wire.write(0);
	Wire.write(0);
	Wire.endTransmission();
	delay(10);
	wdt_reset();
#endif //  RFEXPANSION

	if (DisplayedMenu==FEEDING_MODE)
	{
		// ID 0 - Feeding Timer
		int t=Timer[FEEDING_TIMER].Trigger-now();
		Wire.beginTransmission(I2CRA_TouchDisplay);
		Wire.write(0);
		Wire.write(t%256);
		Wire.write(t/256);
		Wire.endTransmission();
		delay(10);
		wdt_reset();
	}
	if (I2CCommand==COMMAND_CLEAR_ATO)
		ATOClear();
	if (I2CCommand==COMMAND_CLEAR_OVERHEAT)
		OverheatClear();
	if (I2CCommand==COMMAND_LIGHTS_ON)
		LightsOn();
	if (I2CCommand==COMMAND_LIGHTS_OFF)
		LightsOff();
	I2CCommand=0;
}
#endif // I2CMASTER

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
void ReefAngelClass::ShowTouchInterface()
{
	Refresh();
	int twidth=TouchLCD.GetWidth();
	int theight=TouchLCD.GetHeight();
	if (LastOrientation != orientation)
	{
		LastOrientation = orientation;
		twidth=TouchLCD.GetWidth();
		theight=TouchLCD.GetHeight();
		NeedsRedraw=true;
	}
	byte numexp=0;
	if ((EM&(1<<3))!=0) numexp++;
	if ((EM&(1<<4))!=0) numexp++;
	if ((EM&(1<<6))!=0) numexp++;
	if ((EM&(1<<7))!=0) numexp++;

	switch ( DisplayedMenu )
	{
		case MAIN_MENU:
		case DEFAULT_MENU:  // This is the home screen
		{
			// process screensaver timeout
			if ( Timer[LCD_TIMER].IsTriggered() )
			{
				// Screensaver timeout expired
				TouchLCD.SetBacklight(0);
				Sleeping=true;
				DisplayedScreen==MAIN_SCREEN;
				NeedsRedraw=true;
			}
#ifdef CUSTOM_MAIN
			DrawCustomMain();
#else  // CUSTOM_MAIN
			if (!Splash)
			{
				int i,j;
				char tempname[15];
				byte TempRelay,TempRelayOn,TempRelayOff;

				if (!TS.IsTouched())
				{
					LongTouch=0;
					if (!Sleeping)
					{
						TouchEnabled=true;
						//Draw Top Bar
						if (orientation%2==0) i=0; else i=12;
						if (NeedsRedraw)
						{
							//Top Bar
							TouchLCD.DrawRectangle(TOPBAR_BC,0,0,twidth,27,true);
							for (int a=0;a<=5;a++)
							{
								TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,28+a,twidth,28+a);
							}
							//Bottom Bar
							TouchLCD.DrawRectangle(BOTTOMBAR_BC,0,theight-27,twidth,theight,true);
							for (int a=0;a<=5;a++)
							{
								TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,theight-28-a,twidth,theight-28-a);
							}
							//Logo
							TouchLCD.DrawBMP(5,2,ICONLOGO);
							//Arrows
							TouchLCD.DrawBMP(10,theight-25,ARROWLEFT);
							TouchLCD.DrawBMP(twidth-10-23,theight-25,ARROWRIGHT);
							//Menu Dividers and arrow
							TouchLCD.DrawBMP(twidth/2-50,theight-25,DIVIDER);
							TouchLCD.DrawBMP(twidth/2+50,theight-25,DIVIDER);
							TouchLCD.DrawBMP(twidth/2-3,theight-25,ARROWMENU);
							Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
							Font.DrawCenterText(twidth/2,theight-15,"Menu");
						}
						TouchLCD.DrawDateTime(38,9,MilitaryTime,Font);
						TouchLCD.DrawFlags(Flags);
						if (DisplayedScreen==MAIN_SCREEN)
						{
							int x=0;

							if (NeedsRedraw)
							{
								NeedsRedraw=false;

								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
								if (i==12 || i==4) // Orientation is portrait
								{
									x=twidth*3/16;
									if (numexp>2) i=4;
									//Temperature
									j=60+i;
									Font.SetColor(COLOR_GOLD,BKCOLOR,true);
									eeprom_read_block((void*)&tempname, (void*)Probe1Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/16;
									eeprom_read_block((void*)&tempname, (void*)Probe2Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/16;
									eeprom_read_block((void*)&tempname, (void*)Probe3Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);

									//pH
									x=twidth*3/16;
									j+=45+i;
									Font.DrawCenterTextP(x,j,LABEL_PH);
									x+=twidth*5/16;
									//Salinity
									if ((EM&(1<<3))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_SALINITY);
										x+=twidth*5/16;
									}
									//ORP
									if ((EM&(1<<4))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_ORP);
										x+=twidth*5/16;
									}
									//pH Exp
									if ((EM&(1<<6))!=0)
									{
										if (x>twidth*14/16)
										{
											x=twidth*3/16;
											j+=45+i;
										}
										Font.DrawCenterTextP(x,j,LABEL_PHE);
										x+=twidth*5/16;
									}
									//Water Level
									if ((EM&(1<<7))!=0)
									{
										if (x>twidth*14/16)
										{
											x=twidth*3/16;
											j+=45+i;
										}
										Font.DrawCenterTextP(x,j,LABEL_WL);
									}
								}
								else // Orientation is landscape
								{
									//Temperature, pH
									x=twidth*3/21;
									if (numexp==0) i=7;
									j=64+i;
									Font.SetColor(COLOR_GOLD,BKCOLOR,true);
									eeprom_read_block((void*)&tempname, (void*)Probe1Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									eeprom_read_block((void*)&tempname, (void*)Probe2Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									eeprom_read_block((void*)&tempname, (void*)Probe3Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									Font.DrawCenterTextP(x,j,LABEL_PH);

									x=twidth*3/21;
									if (numexp>0) j+=43+i;
									//Salinity
									if ((EM&(1<<3))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_SALINITY);
										x+=twidth*5/21;
									}
									//ORP
									if ((EM&(1<<4))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_ORP);
										x+=twidth*5/21;
									}
									//pH Exp
									if ((EM&(1<<6))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_PHE);
										x+=twidth*5/21;
									}
									//Water Level
									if ((EM&(1<<7))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_WL);
									}
								}

								j+=18+i;

								//Division
								TouchLCD.DrawLine(DIVISION,0,j,twidth,j);

								j+=5+i;
								PB[0].NeedsRedraw=true;
								j+=30+(i/2);
								PB[1].NeedsRedraw=true;
								j+=25+i;
								//Division
								TouchLCD.DrawLine(DIVISION,0,j,twidth,j);

								j+=8+(i*3/4);

								//ATO Ports
								Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,(twidth/10)+25,j,LABEL_ATOHIGHPORT);
								Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,(twidth*6/10)+25,j,LABEL_ATOLOWPORT);
							}

							// Draw Parameter values
							if (i==12 || i==4) // Orientation is portrait
							{
								if (numexp>2) i=4;
								//Temperature
								x=twidth*3/16;
								j=27+i;
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								x+=twidth*5/16;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								x+=twidth*5/16;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);

								//pH
								x=twidth*3/16;
								j+=43+i;
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);
								x+=twidth*5/16;
#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									x+=twidth*5/16;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,1);
									x+=twidth*5/16;
								}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
								//pH Exp
								if ((EM&(1<<6))!=0)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									x+=twidth*5/16;
								}
#endif // PHEXPANSION
#ifdef WATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(),0);
									x+=twidth*5/16;
								}
#endif // WATERLEVELEXPANSION
							}
							else // Orientation is landscape
							{
								x=twidth*3/21;
								if (numexp==0) i=7;
								//Temperature
								j=32+i;
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);

								x=twidth*3/21;
								if (numexp>0) j+=43+i;								
#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									x+=twidth*5/21;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,10);
									x+=twidth*5/21;
								}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
								//pH Exp
								if ((EM&(1<<6))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									x+=twidth*5/21;
								}
#endif // PHEXPANSION
#ifdef WATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(),0);
								}
#endif // WATERLEVELEXPANSION
							}
							
							// Progress Bars
							j+=55+(i*2);
							PB[0].SetPosition(10,j);
							PB[0].SetColor(COLOR_ORANGE);
							PB[0].SetLabel("Daylight");
							PB[0].SetCurrent(PWM.GetDaylightValue());
							PB[0].Show();
							j+=29+(i/2);
							PB[1].SetPosition(10,j);
							PB[1].SetColor(COLOR_ROYALBLUE);
							PB[1].SetLabel("Actinic");
							PB[1].SetCurrent(PWM.GetActinicValue());
							PB[1].Show();
							
							j+=19+i;
							if (i==4) // Orientation is portrait
							{
								j+=3;
							}
							
							// ATO Buttons
							j+=11+(i*3/4);
							if (HighATO.IsActive())
								TouchLCD.DrawBMP(twidth/10,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(twidth/10,j,REDBUTTON);
							if (LowATO.IsActive())
								TouchLCD.DrawBMP(twidth*6/10,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(twidth*6/10,j,REDBUTTON);
							if (i==4) i=12;
							if (i==7) i=0;
							
						}
						else if (DisplayedScreen>=RELAY_BOX && DisplayedScreen<=EXP_BOX_8)
						{
							int l=twidth;
							byte k;							
							int rx;

							if (DisplayedScreen==RELAY_BOX)
							{
								TempRelay=Relay.RelayData;
								TempRelayOn=Relay.RelayMaskOn;
								TempRelayOff=Relay.RelayMaskOff;
							}
							else
							{
#ifdef RelayExp
								TempRelay=Relay.RelayDataE[DisplayedScreen-2];
								TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-2];
								TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-2];
#endif // RelayExp
							}
							TempRelay&=TempRelayOff;
							TempRelay|=TempRelayOn;

							if (NeedsRedraw)
							{
								NeedsRedraw=false;

								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RELAYBOXLABELBAR/DisplayedScreen,a*3),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
							}

							// Relay Ports Status
							j=36;
							k=l*5/16;
							rx=10;
							if (i==0) // Orientation is landscape
							{
								j=42;
								k=l/4;
							}
							for (int a=1;a<9;a++)
							{
								if (a==5)
								{
									j=36;
									k=l*11/16;
									if (i==0) // Orientation is landscape
									{
										j=42;
										k=l*3/4;
									}
									rx=l-40;
								}
								if (i==12) j+=52; else j+=34;
								eeprom_read_block((void*)&tempname, (void*)(R1Name+((a-1)*0x10)+((DisplayedScreen-1)*0x80)), sizeof(tempname));
								Font.SetColor(COLOR_WHITE,BKCOLOR,true);
								Font.DrawCenterText(k,j+3,tempname);
								SmallFont.SetColor(COLOR_YELLOW,BKCOLOR,true);
								if (bitRead(TempRelayOn,a-1) || !bitRead(TempRelayOff,a-1))
									SmallFont.DrawCenterTextP(k,j+18,LABEL_OVERRIDE);
								else
									TouchLCD.Clear(BKCOLOR,k-23,j+18,k+23,j+26);
								TouchLCD.DrawRelayStatus(rx,j,bitRead(TempRelay,a-1));
							}				
						}
#ifdef PWMEXPANSION
						else if(DisplayedScreen==PWM_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(PWMLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));

								for (int a=0;a<6;a++)
									PB[a].NeedsRedraw=true;
							}	
							
							// Progress Bars
							j=50+(i/2);
							Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								eeprom_read_block((void*)&tempname, (void*)(PWMChannel1+((a)*0x10)), sizeof(tempname));
								PB[a].SetPosition(10,j);
								PB[a].SetColor(COLOR_GREEN);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(PWM.GetChannelValue(a));
								PB[a].Show();
							}
						}
#endif // PWMEXPANSION
#ifdef RFEXPANSION
						else if(DisplayedScreen==RF_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RFLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));					
								j=60;
								Font.SetColor(COLOR_GOLD,BKCOLOR,false);
								j+=40+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_MODE);
								j+=45+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_SPEED);
								j+=45+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_DURATION);
							}
							j=27;
							j+=40+(i*2);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							char temp[25];
							char rf_mode[35];
							strcpy_P(temp,(char * )pgm_read_word(&(rf_items[RF.Mode])));
							strcpy(rf_mode,"   ");
							strcat(rf_mode,temp);
							strcat(rf_mode,"   ");
							LargeFont.DrawCenterText((twidth/2),j,rf_mode);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Speed,0);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Duration,0);
						}
						else if(DisplayedScreen==RF_SCREEN1)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RFLABELBAR1,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));					
								for (int a=0;a<6;a++)
									PB[a].NeedsRedraw=true;
							}
							// Progress Bars
							j=50+(i/2);
							int rfcolor[] = {COLOR_ORANGE,COLOR_ROYALBLUE,COLOR_RED,COLOR_GREEN,COLOR_LIGHTBLUE,COLOR_MAGENTA};
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								int ptr = pgm_read_word(&(LABEL_RF[a]));								
								strcpy_P(tempname, (char *)ptr);
								PB[a].SetPosition(10,j);
								PB[a].SetColor(rfcolor[a]);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(RF.GetChannel(a));
								PB[a].Show();
							}							

						}	
#endif // RFEXPANSION
#ifdef AI_LED
						else if(DisplayedScreen==AI_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(AILABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								for (int a=0;a<3;a++)
									PB[a].NeedsRedraw=true;
							}	
							
							// Progress Bars
							j=35;
							int aicolor[] = {COLOR_ORANGE,COLOR_LIGHTBLUE,COLOR_ROYALBLUE};
							for (int a=0;a<3;a++)
							{
								j+=40+(i*2);
								int ptr = pgm_read_word(&(LABEL_AI[a]));								
								strcpy_P(tempname, (char *)ptr);
								PB[a].SetPosition(10,j);
								PB[a].SetColor(aicolor[a]);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(AI.GetChannel(a));
								PB[a].Show();							
							}
						}
#endif //  AI_LED
#ifdef IOEXPANSION
						else if(DisplayedScreen==IO_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(IOLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));	
								Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
								j=50+(i/2);
								for (int a=0;a<6;a++)
								{
									j+=22+i;
									Font.DrawCenterTextP((twidth/2)-10,j+5,LABEL_IOPORT);
									Font.DrawText(a);
								}								
							}
							j=50+(i/2);
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								int iocolor;
								if (IO.GetChannel(a))
									TouchLCD.DrawBMP(twidth/6,j+3,GREENBUTTON);
								else
									TouchLCD.DrawBMP(twidth/6,j+3,REDBUTTON);
							}								

						}
#endif //  IOEXPANSION
						else if(DisplayedScreen==DIMMING_OVERRIDE)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_WHITE,0,28,twidth,theight);	
								Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
								Font.DrawTextP(10,105,PWM_OVERRIDE_LABEL1);
								Font.DrawTextP(10,117,PWM_OVERRIDE_LABEL2);
								OkButton.SetPosition(twidth/2-40,theight*5/9);
								OkButton.Show();
								CancelButton.SetPosition(twidth/2-65,theight*7/9);
								CancelButton.Show();
								Slider.Show();
//									Slider.Refresh();
							}
						}
					}
				}
				else // if touched
				{
					// Check for Soft Reset of screen
					// If top left corner is long touched, we reinitialize the LCD. 
					if (TS.X<50 && TS.Y<50) // top left corner
					{
						if (LongTouch++==255) // 255 count is about 6 seconds
						{	
							LongTouch=0;
							NeedsRedraw=true;
							TouchLCD.Init();
						}
					}
					if (TouchEnabled && Sleeping)
					{
						TouchEnabled=false;
						Sleeping=false;
					}
					else
					{
						if (TouchEnabled && DisplayedScreen<MAX_SCREENS)
						{
							int j;
							TouchEnabled=false;
							if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
								ChangeDisplayedScreen(-1);
							if (TS.X>twidth-50 && TS.Y>theight-30)
								ChangeDisplayedScreen(1);
							if (TS.X<twidth-80 && TS.X>80 && TS.Y>theight-30)
							{
								SetDisplayedMenu(TOUCH_MENU);
								DisplayedScreen=MAIN_MENU_SCREEN;
								NeedsRedraw=true;
							}
							if(DisplayedScreen==MAIN_SCREEN)
							{
								if (orientation%2==0)
								{
									//landscape
									if (numexp>0)
										j=127;
									else
										j=105;
								}
								else
								{
									//portrait
									if (numexp>2)
										j=187;
									else
										j=172;
										
								}
								if (TS.IsTouchedInside(0,j-5,twidth,j+25))
								{
									RecallScreen=DisplayedScreen;
									DisplayedScreen=DIMMING_OVERRIDE;
									NeedsRedraw=true;
									Slider.SetColor(COLOR_ORANGE);							
									Slider.SetCurrent(PWM.GetDaylightValue());
									Slider.SetOverrideID(OVERRIDE_DAYLIGHT);
									Slider.SetLabel("Daylight");
								}
								j+=35;
								if (TS.IsTouchedInside(0,j-5,twidth,j+25))
								{
									RecallScreen=DisplayedScreen;
									NeedsRedraw=true;
									DisplayedScreen=DIMMING_OVERRIDE;
									Slider.SetColor(COLOR_ROYALBLUE);
									Slider.SetCurrent(PWM.GetActinicValue());
									Slider.SetOverrideID(OVERRIDE_ACTINIC);
									Slider.SetLabel("Actinic");
								}
							}
							else if (DisplayedScreen>=RELAY_BOX && DisplayedScreen<=EXP_BOX_8)
							{
								int k,l,x,x1,h;
								
								if (DisplayedScreen==RELAY_BOX)
								{
									TempRelay=Relay.RelayData;
									TempRelayOn=Relay.RelayMaskOn;
									TempRelayOff=Relay.RelayMaskOff;
								}
								else
								{
#ifdef RelayExp
									TempRelay=Relay.RelayDataE[DisplayedScreen-2];
									TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-2];
									TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-2];
#endif // RelayExp
								}
								TempRelay&=TempRelayOff;
								TempRelay|=TempRelayOn;
								
								if (orientation%2==0) // Orientation is landscape
								{
									j=53;
									l=34;
									h=16;
								}
								else
								{
									j=43;
									l=52;
									h=24;
								}
								k=j;
								x=0;
								x1=45;
								for (int a=1;a<9;a++)
								{

									if (a==5)
									{
										k=j;
										x=twidth-45;
										x1=twidth-118;
									}
									k+=l;
									if (TS.IsTouchedInside(x,k-h,x+45,k+h))
									{
										if (bitRead(TempRelayOn,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,0);
#endif // REEFTOUCHDISPLAY
											Relay.Override(a+(DisplayedScreen-1)*10,0);
										}
										else if (!bitRead(TempRelayOff,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,1);
#endif // REEFTOUCHDISPLAY
											Relay.Override(a+(DisplayedScreen-1)*10,1);
										}
										else if (!bitRead(TempRelayOn,a-1) && bitRead(TempRelayOff,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,!bitRead(TempRelay,a-1));
#endif // REEFTOUCHDISPLAY
											if (bitRead(TempRelay,a-1))
											{
												Relay.Override(a+(DisplayedScreen-1)*10,0);
											}
											else
											{
												Relay.Override(a+(DisplayedScreen-1)*10,1);
											}
										}
									}
									if (TS.IsTouchedInside(x1,k-h,x1+74,k+h))
									{
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,2);
#endif // REEFTOUCHDISPLAY
										Relay.Override(a+(DisplayedScreen-1)*10,2);
									}
								}										
							}
#ifdef PWMEXPANSION
							else if (DisplayedScreen==PWM_SCREEN)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=60;
									h=10;
									k=22;
								}
								else
								{
									//portrait
									j=65;
									h=15;
									k=34;
								}									
								for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(COLOR_GREEN);
										Slider.SetCurrent(PWM.GetChannelValue(a));
										Slider.SetOverrideID(OVERRIDE_CHANNEL0+a);
										eeprom_read_block((void*)&tempname, (void*)(PWMChannel1+((a)*0x10)), sizeof(tempname));
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // PWMEXPANSION
#ifdef AI_LED
							else if (DisplayedScreen==AI_SCREEN)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=45;
									h=17;
									k=40;
								}
								else
								{
									//portrait
									j=45;
									h=20;
									k=64;
								}									
								int aicolor[] = {COLOR_ORANGE,COLOR_LIGHTBLUE,COLOR_ROYALBLUE};
								for (int a=0;a<AI_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(aicolor[a]);
										Slider.SetCurrent(AI.GetChannel(a));
										Slider.SetOverrideID(OVERRIDE_AI_WHITE+a);
										int ptr = pgm_read_word(&(LABEL_AI[a]));								
										strcpy_P(tempname, (char *)ptr);
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // AI_LED
#ifdef RFEXPANSION
							else if (DisplayedScreen==RF_SCREEN1)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=60;
									h=10;
									k=22;
								}
								else
								{
									//portrait
									j=65;
									h=15;
									k=34;
								}									
								int rfcolor[] = {COLOR_ORANGE,COLOR_ROYALBLUE,COLOR_RED,COLOR_GREEN,COLOR_LIGHTBLUE,COLOR_MAGENTA};
								for (int a=0;a<RF_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(rfcolor[a]);
										Slider.SetCurrent(RF.GetChannel(a));
										Slider.SetOverrideID(OVERRIDE_RF_WHITE+a);
										int ptr = pgm_read_word(&(LABEL_RF[a]));								
										strcpy_P(tempname, (char *)ptr);
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // RFEXPANSION
						}
						else
						{
							// if we are displaying overrides, refresh sliders
							if(DisplayedScreen==DIMMING_OVERRIDE)
							{
								if (TouchEnabled)
								{
									TouchEnabled=false;
									bool bDone=false;
									TS.GetTouch();
									byte oid=Slider.GetOverrideID();
									byte ovalue=Slider.GetCurrent();
									if (OkButton.IsPressed()) 
									{
										bDone=true;
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,ovalue); 	// Send Override Request
#endif // REEFTOUCHDISPLAY
										if (oid<=OVERRIDE_CHANNEL5)
											ReefAngel.PWM.Override(oid,ovalue);
#ifdef AI_LED
										if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_BLUE)
											ReefAngel.AI.Override(oid-OVERRIDE_AI_WHITE,ovalue);
#endif // AI_LED
#ifdef RFEXPANSION
										if (oid>=OVERRIDE_RF_WHITE && oid<=OVERRIDE_RF_INTENSITY)
											ReefAngel.RF.Override(oid-OVERRIDE_RF_WHITE,ovalue);
#endif // RFEXPANSION
									}
									if (CancelButton.IsPressed()) 
									{
										bDone=true;
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,255); 	// Send Cancel Override Request
#endif // REEFTOUCHDISPLAY
										if (oid<=OVERRIDE_CHANNEL5)
											ReefAngel.PWM.Override(oid,255);
#ifdef AI_LED
										if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_BLUE)
											ReefAngel.AI.Override(oid-OVERRIDE_AI_WHITE,255);
#endif // AI_LED
#ifdef RFEXPANSION
										if (oid>=OVERRIDE_RF_WHITE && oid<=OVERRIDE_RF_INTENSITY)
											ReefAngel.RF.Override(oid-OVERRIDE_RF_WHITE,255);
#endif // RFEXPANSION
									}
									if (bDone)
									{
										TouchEnabled=false;
										DisplayedScreen=RecallScreen;
										NeedsRedraw=true;
									}
								}
								Slider.Refresh();
							}
						}
					}
				}
			}	
#endif //  CUSTOM_MAIN
			
			wdt_reset();
			if (TS.IsTouched())
			{
				// turn the backlight on
				TouchLCD.SetBacklight(100);
				Timer[LCD_TIMER].SetInterval(InternalMemory.LCDTimer_read());  // LCD Sleep Mode timer
				Timer[LCD_TIMER].Start();  // start timer
			}			
			break;
		}  // DEFAULT_MENU
		case FEEDING_MODE:
		{
			int t;
			byte y;
			bool bDone = false;
			t = Timer[FEEDING_TIMER].Trigger - now();
			if ( (t >= 0) && ! Timer[FEEDING_TIMER].IsTriggered() )
			{
				if (orientation%2==0) y=0; else y=40;
				wdt_reset();
				if (NeedsRedraw)
				{
					delay(50);
					TouchLCD.FullClear(BKCOLOR);
					TouchLCD.SetBacklight(0);
					if (orientation%2==0)
					{
						if (SDFound) TouchLCD.DrawSDRawImage("feed_l.raw",0,80,320,160);
					}
					else
					{
						if (SDFound) TouchLCD.DrawSDRawImage("feed_p.raw",0,160,240,160);
					}
					TouchLCD.SetBacklight(100);
					LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
					LargeFont.DrawCenterTextP(twidth/2, 10+y, FEEDING_LABEL);	
					NeedsRedraw=false;
				}
				LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
				LargeFont.DrawCenterNumber(twidth/2, 40+y, t, 0);
			}
			else
			{
				bDone = true;
			}

			LastStart = now();  // Set the time normal mode is started
			if ( TS.IsTouched() )
			{
				// screen was touched, so we stop the feeding mode
				bDone = true;
				TouchEnabled=false;
			}
			if ( bDone )
			{
				// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
				Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

				// turn on ports
				Relay.RelayMaskOff |= FeedingModePorts;
				// Compare the delayed on ports with the previous port states
				Relay.RelayData &= ~(FeedingModePorts & DelayedOnPorts);
#ifdef RelayExp
				for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
				{
					Relay.RelayMaskOffE[i] |= FeedingModePortsE[i];
					Relay.RelayDataE[i] &= ~(FeedingModePortsE[i] & DelayedOnPortsE[i]);
				}
#endif  // RelayExp
#ifdef RFEXPANSION
				RF.SetMode(Feeding_Stop,0,0);
#endif  // RFEXPANSION
				NeedsRedraw=true;
#ifdef REEFTOUCHDISPLAY
				SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // REEFTOUCHDISPLAY
				ExitMenu();
			}
//				Relay.Write();
			break;
		}
		case WATERCHANGE_MODE:
		{
			byte y;
			bool bDone = false;
			if (orientation%2==0) y=0; else y=40;
			wdt_reset();
			if (NeedsRedraw)
			{
				TouchLCD.FullClear(BKCOLOR);
				TouchLCD.SetBacklight(0);
				if (orientation%2==0)
				{
					if (SDFound) TouchLCD.DrawSDRawImage("water_l.raw",0,40,320,200);
				}
				else
				{
					if (SDFound) TouchLCD.DrawSDRawImage("water_p.raw",0,170,240,150);
				}
				TouchLCD.SetBacklight(100);
				NeedsRedraw=false;
			}
			LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
			LargeFont.DrawCenterTextP(twidth/2, 10+y, WATER_CHANGE_LABEL);
			LastStart = now();  // Set the time normal mode is started
			if ( TS.IsTouched() )
			{
				// screen was touched, so we stop the feeding mode
				bDone = true;
				TouchEnabled=false;
			}
			if ( bDone )
			{
				// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
				Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

				// turn on ports
				Relay.RelayMaskOff |= WaterChangePorts;
				// Compare the delayed on ports with the previous port states
				Relay.RelayData &= ~(WaterChangePorts & DelayedOnPorts);
#ifdef RelayExp
				for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
				{
					Relay.RelayMaskOffE[i] |= WaterChangePortsE[i];
					Relay.RelayDataE[i] &= ~(WaterChangePortsE[i] & DelayedOnPortsE[i]);
				}
#endif  // RelayExp
				NeedsRedraw=true;
#ifdef REEFTOUCHDISPLAY
				SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // REEFTOUCHDISPLAY
				ExitMenu();
			}
//				Relay.Write();
			break;
		}
		case TOUCH_MENU:
		{
			int ch=44;
			wdt_reset();
			if (!TS.IsTouched())
			{
				TouchEnabled=true;
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					TouchLCD.Clear(COLOR_WHITE,0,0,twidth,theight); // Clear screen
					//Arrows
					TouchLCD.DrawBMP(10,theight-25,ARROWLEFT);
					TouchLCD.DrawBMP(twidth-10-23,theight-25,ARROWRIGHT);
					Font.SetColor(RGB565(0xBD, 0x13, 0x00),COLOR_WHITE,true);
					switch ( DisplayedScreen )
					{
						case MAIN_MENU_SCREEN:
						{
							for (int a=0;a<6;a++)
							{
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),5,5+(47*a),(twidth/2)-6,5+ch+(47*a),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,7,7+(47*a),(twidth/2)-8,3+ch+(47*a),4,false);
								if (a==4)
								{
									Serial.print(Relay.RelayMaskOn);
									Serial.print("\t");
									Serial.print(LightsOnPorts);
									Serial.print("\t");
									Serial.print(Relay.RelayMaskOn & LightsOnPorts);
									Serial.println();
									if (bitRead(Flags,LightsOnFlag))
										Font.DrawCenterTextP(twidth/4,14+(47*a),MENU_BUTTON_CANCEL);
									else
										Font.DrawCenterTextP(twidth/4,14+(47*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
								}
								else
								{
									Font.DrawCenterTextP(twidth/4,14+(47*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
								}
								Font.DrawCenterTextP(twidth/4,30+(47*a),(char * )pgm_read_word(&(menu_button_items1[(a*2)+1])));
			
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,5+(47*a),twidth-6,5+ch+(47*a),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+7,7+(47*a),twidth-8,3+ch+(47*a),4,false);
								Font.DrawCenterTextP(twidth/4*3,14+(47*a),(char * )pgm_read_word(&(menu_button_items2[a*2])));
								Font.DrawCenterTextP(twidth/4*3,30+(47*a),(char * )pgm_read_word(&(menu_button_items2[(a*2)+1])));
							}
							break;
						}
						case SETUP_MENU_SCREEN:
						{
							for (int a=0;a<6;a++)
							{
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),5,5+(47*a),(twidth/2)-6,5+ch+(47*a),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,7,7+(47*a),(twidth/2)-8,3+ch+(47*a),4,false);
								Font.DrawCenterTextP(twidth/4,14+(47*a),(char * )pgm_read_word(&(menu_button_items3[a*2])));
								Font.DrawCenterTextP(twidth/4,30+(47*a),(char * )pgm_read_word(&(menu_button_items3[(a*2)+1])));
			
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,5+(47*a),twidth-6,5+ch+(47*a),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+7,7+(47*a),twidth-8,3+ch+(47*a),4,false);
								Font.DrawCenterTextP(twidth/4*3,14+(47*a),(char * )pgm_read_word(&(menu_button_items4[a*2])));
								Font.DrawCenterTextP(twidth/4*3,30+(47*a),(char * )pgm_read_word(&(menu_button_items4[(a*2)+1])));
							}
							break;							
						}
					}
				}
				if (MenuFunctionPtr!=&ReefAngelClass::Touch)
				{
					(this->*MenuFunctionPtr)(); 
					NeedsRedraw=true;
					MenuFunctionPtr=&ReefAngelClass::Touch;
				}
			}
			else
			{
				menutimeout=now();
				if (TouchEnabled && DisplayedScreen<MAX_SCREENS)
				{
					TouchEnabled=false;
					if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
						ChangeDisplayedScreen(-1);
					if (TS.X>twidth-50 && TS.Y>theight-30)
						ChangeDisplayedScreen(1);
					for (int a=0;a<6;a++)
					{
//						ReefAngel.TouchLCD.Clear(COLOR_RED,0,5+(47*a),twidth/2,5+ch+(47*a));
						if (TS.IsTouchedInside(0,5+(47*a),twidth/2,5+ch+(47*a)))
							MenuFunctionPtr=menu_button_functions1[a];
					}
				}
			}
			if ( (now() - menutimeout) > MENU_TIMEOUT )
			{
				SetDisplayedMenu(DEFAULT_MENU);
				DisplayedScreen=MAIN_SCREEN;
				NeedsRedraw=true;
#ifdef REEFTOUCHDISPLAY
				SendMaster(MESSAGE_MENU,DEFAULT_MENU,DEFAULT_MENU); 	// Change Menu
#endif // REEFTOUCHDISPLAY
			}
			break;
		}
#ifdef CUSTOM_MENU
		case ALT_SCREEN_MODE:
		{
			/*
			for Custom Menus if displaying an alternate screen, to force the menu to be
			redrawn when exiting the screen.  Used primarily for SetupCalibratePH
			and with SetupDateTime.
			Or used when you draw your own screen and stay inside the function and want to
			return to the menu when your function exits
			If not used, then you will have to press the button 2 times to return to the
			main menu.
			*/
			redrawmenu = true;
			showmenu = true;
			SetDisplayedMenu(MAIN_MENU);
			break;
		}
		case RETURN_MAIN_MODE:
		{
			ExitMenu();
			break;
		}
#endif  // CUSTOM_MENU
		default:
		{
			// we are viewing another screen, primarily the DisplayMenuEntry screen
			if ( TS.IsTouched() )
			{
				// button is pressed, so we gotta exit out, show the menu & redraw it too
				redrawmenu = true;
				showmenu = true;
				Timer[FEEDING_TIMER].ForceTrigger();
				Timer[LCD_TIMER].Start();
			}
			// if displayed screen is less than 156, it means we need to redraw because we just came from a I2C receive interrupt
			if (DisplayedMenu<156)
			{
				DisplayedMenu+=100;
				NeedsRedraw=true;
			}
			break;
		}
	}  // switch DisplayedMenu
}
#else // REEFTOUCH
void ReefAngelClass::ShowInterface()
{
    Refresh();
    // are we displaying the menu or not??
    if ( showmenu )
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        DisplayMenuHeading();
        DisplayMenu();
    }
    else
    {
        // not displaying the menu, so we're gonna show the appropriate screen
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        switch ( DisplayedMenu )
        {
        	case TOUCH_MENU:
			case DEFAULT_MENU:  // This is the home screen
			{
				// process screensaver timeout
				if ( Timer[LCD_TIMER].IsTriggered() )
				{
					// Screensaver timeout expired
					LCD.BacklightOff();
				}

				if ( Joystick.IsButtonPressed() )
				{
					// turn the backlight on
					LCD.BacklightOn();

					// TODO check Timer[LCD_TIMER] code
					if ( Timer[LCD_TIMER].Trigger == 0 )
					{
						Timer[LCD_TIMER].Start();
						return;
					}
					PrepMenuScreen();
					// get out of this function and display the menu
					return;
				}

				if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
				{
					// Turn backlight on
					LCD.BacklightOn();
					Timer[LCD_TIMER].Start();
				}

#ifdef CUSTOM_MAIN
				DrawCustomMain();
#else
				// display everything on the home screen except the graph
				// the graph is drawn/updated when we exit the main menu & when the parameters are saved
#ifdef DATETIME24
				LCD.DrawDateTimeISO8601(6, 112);
#else
				LCD.DrawDate(6, 112);
#endif // DATETIME24
#if defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params, PWM.GetDaylightValue(), PWM.GetActinicValue());
#else  // defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params);
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
#if defined WDT || defined WDT_FORCE
				wdt_reset();
#endif  // defined WDT || defined WDT_FORCE

				byte TempRelay = Relay.RelayData;
				TempRelay &= Relay.RelayMaskOff;
				TempRelay |= Relay.RelayMaskOn;
				LCD.DrawOutletBox(12, 93, TempRelay);
#endif  // CUSTOM_MAIN
				// Process any checks/tests/events that can happen while displaying the home screen
				// This can be the timers for wavemakers or any overheat temperatures

				// process timers
				// If bus is locked, it will trigger wdt when drawing graph
				if ( Timer[STORE_PARAMS_TIMER].IsTriggered() && !BusLocked) // Only access eeprom if bus is not locked
				{
					int CurTemp;

					// Values are stored in the I2CEEPROM1
					taddr++;
					if ( taddr >= 120 ) taddr = 0;
					Timer[STORE_PARAMS_TIMER].Start();
					CurTemp = map(Params.Temp[T1_PROBE], T1LOW, T1HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.Temp[T2_PROBE], T2LOW, T2HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+120, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.Temp[T3_PROBE], T3LOW, T3HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+240, CurTemp);
//					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.PH, PHLOW, PHHIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+360, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					if ((taddr%10)==0) InternalMemory.T1Pointer_write(taddr);
#if defined WDT || defined WDT_FORCE
					wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
#ifdef CUSTOM_MAIN
					DrawCustomGraph();
#else
					LCD.DrawGraph(5, 5);
#endif  // CUSTOM_MAIN
				}
				break;
			}  // DEFAULT_MENU
			case FEEDING_MODE:
			{
				int t;
				bool bDone = false;
				t = Timer[FEEDING_TIMER].Trigger - now();
				if ( (t >= 0) && ! Timer[FEEDING_TIMER].IsTriggered() )
				{
					LCD.Clear(DefaultBGColor,60+(intlength(t)*5),100,100,108);
					LCD.DrawText(DefaultFGColor,DefaultBGColor,60,100,t);
					delay(200);  // to keep from redraw flicker on timer
				}
				else
				{
					bDone = true;
				}

				LastStart = now();  // Set the time normal mode is started
				if ( Joystick.IsButtonPressed() )
				{
					// joystick button pressed, so we stop the feeding mode
					bDone = true;
				}
				if ( bDone )
				{
					// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
					Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

					// turn on ports
					Relay.RelayMaskOff |= FeedingModePorts;
					// Compare the delayed on ports with the previous port states
					Relay.RelayData &= ~(FeedingModePorts & DelayedOnPorts);
#ifdef RelayExp
					for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
					{
						Relay.RelayMaskOffE[i] |= FeedingModePortsE[i];
						Relay.RelayDataE[i] &= ~(FeedingModePortsE[i] & DelayedOnPortsE[i]);
					}
#endif  // RelayExp
#ifdef RFEXPANSION
					RF.SetMode(Feeding_Stop,0,0);
#endif  // RFEXPANSION
					ExitMenu();
				}
//				Relay.Write();
				break;
			}
			case WATERCHANGE_MODE:
			{
				LastStart = now();  // Set the time normal mode is started
				if ( Joystick.IsButtonPressed() )
				{
					// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
					Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

					// turn on ports
					Relay.RelayMaskOff |= WaterChangePorts;
					// Compare the delayed on ports with the previous port states
					Relay.RelayData &= ~(WaterChangePorts & DelayedOnPorts);
#ifdef RelayExp
					for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
					{
						Relay.RelayMaskOffE[i] |= WaterChangePortsE[i];
						Relay.RelayDataE[i] &= ~(WaterChangePortsE[i] & DelayedOnPortsE[i]);
					}
#endif  // RelayExp
					ExitMenu();
				}
//				Relay.Write();
				break;
			}
#ifdef CUSTOM_MENU
			case ALT_SCREEN_MODE:
			{
				/*
				for Custom Menus if displaying an alternate screen, to force the menu to be
				redrawn when exiting the screen.  Used primarily for SetupCalibratePH
				and with SetupDateTime.
				Or used when you draw your own screen and stay inside the function and want to
				return to the menu when your function exits
				If not used, then you will have to press the button 2 times to return to the
				main menu.
				*/
				redrawmenu = true;
				showmenu = true;
				SetDisplayedMenu(MAIN_MENU);
				break;
			}
			case RETURN_MAIN_MODE:
			{
				ExitMenu();
				break;
			}
#endif  // CUSTOM_MENU
			default:
			{
				// we are viewing another screen, primarily the DisplayMenuEntry screen
				if ( Joystick.IsButtonPressed() )
				{
					// button is pressed, so we gotta exit out, show the menu & redraw it too
					redrawmenu = true;
					showmenu = true;
					Timer[FEEDING_TIMER].ForceTrigger();
					Timer[LCD_TIMER].Start();
				}
				break;
			}
        }  // switch DisplayedMenu
    }  // if showmenu
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
}
#endif // REEFTOUCH

void ReefAngelClass::PrepMenuScreen()
{
	// Clears the screen to draw the menu
	// Displays main menu, select first item, save existing menu
	ClearScreen(DefaultBGColor);
	SelectedMenuItem = DEFAULT_MENU_ITEM;
	PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(MAIN_MENU);
	showmenu = true;
	redrawmenu = true;
	menutimeout = now();
}

void ReefAngelClass::DisplayMenu()
{
#if not defined REEFTOUCH && not defined REEFTOUCHDISPLAY
    // redrawmenu should only get set from within this function when we move the joystick or press the button
    byte qty = menuqtysptr[DisplayedMenu];
    int ptr = menusptr[DisplayedMenu];

#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE

    if ( Joystick.IsUp() )
    {
        // process UP press
        if ( (--SelectedMenuItem) > qty )
        {
            // we're moving up and we hit the top of the list
            // gotta wrap down to the bottom of the list
            // qty - 1 gives us the last item in our list
            //SelectedMenuItem = qty - 1;
            // This allows us to add in our last item
            SelectedMenuItem = qty;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    if ( Joystick.IsDown() )
    {
        // process DOWN press
        // > allows for selection of last item, >= skips it
        if ( (++SelectedMenuItem) > qty )
        {
            // we've hit the bottom of the list
            // wrap around to the top of the list
            SelectedMenuItem = DEFAULT_MENU_ITEM;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    // TODO Have ability to customize menu timeout delay
    if ( (now() - menutimeout) > MENU_TIMEOUT )
    {
        // menu timeout returns to the main screen
        // skip all the other menu checks
        SelectedMenuItem = EXCEED_TIMEOUT_MENU;
		SetDisplayedMenu(EXCEED_TIMEOUT_MENU);
        ButtonPress++;
    }

    if ( Joystick.IsButtonPressed() )
    {
        // button gets pressed, so we need to handle the button press
        ProcessButtonPress();
        redrawmenu = true;
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        // Don't finish processing the rest of the menu
        return;
    }

    // don't redraw the menu if we don't have to
    if ( ! redrawmenu )
        return;

    byte i;
    byte bcolor, fcolor;
    char buffer[22];
    for ( i = 0; i <= qty; i++ )
    {
        bcolor = DefaultBGColor;
        fcolor = DefaultFGColor;
        if ( i < qty )
        {
            strcpy_P(buffer, (char *)ptr++);
        }
        else
        {
            // the last item in the list is either Exit or Prev Menu
            int ptr2 = pgm_read_word(&(return_items[0]));
            if ( DisplayedMenu == MainMenu )
            {
                ptr2 += strlen(Return_0_label) + 1;
            }
            strcpy_P(buffer, (char *)ptr2);
        }
        ptr += strlen(buffer);

        // change the background color on the selected menu entry
        if ( i == SelectedMenuItem )
        {
            bcolor = SelectionBGColor;
            fcolor = SelectionFGColor;
        }
        LCD.Clear(bcolor, MENU_START_COL-3,
                         (i*MENU_START_ROW)+MENU_HEADING_SIZE-1,
                          MENU_END_COL,
                         (i*MENU_START_ROW)+(MENU_HEADING_SIZE+MENU_ITEM_HEIGHT-1));
        LCD.DrawText(fcolor, bcolor, MENU_START_COL, (i*MENU_START_ROW)+MENU_HEADING_SIZE, buffer);
    }  // for i
    // once drawn, no need to redraw yet
    redrawmenu = false;
#endif //  REEFTOUCH    
}

void ReefAngelClass::DisplayMenuHeading()
{
#if not defined REEFTOUCH && not defined REEFTOUCHDISPLAY
    // NOTE do we redraw the menu heading or not?  use same logic as with the menu
    if ( ! redrawmenu )
        return;

    char buffer[10];
    int ptr = pgm_read_word(&(menulabel_items[0]));

    switch ( DisplayedMenu )
    {
        default:
//            {
//                //strcpy(buffer, "Menu:");
//                sprintf(buffer, "Menu (%d):", MenuNum);
//            }
//            break;
        case MainMenu:
            {
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
        case SetupMenu:
            {
                ptr += strlen(Menu_0_label) + 1;
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#ifndef RemoveAllLights
        case LightsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // RemoveAllLights
        case TempsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + strlen(Menu_3_label) + 3;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
    }  // switch MenuNum
    strcpy_P(buffer, (char*)ptr);

    // clear the line that has the menu heading on it
    LCD.Clear(DefaultBGColor, MENU_START_COL, MENU_START_ROW, MAX_X, MAX_Y);
    // Display the menu heading
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, buffer);
#endif //  REEFTOUCH    
}

void ReefAngelClass::DisplayMenuEntry(char *text)
{
#if not defined REEFTOUCH && not defined REEFTOUCHDISPLAY
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, text);
    /*
    char buffer[18];
	int ptr = pgm_read_word(&(return_items[0]));
	ptr += strlen(Return_0_label) + strlen(Return_1_label) + 2;
	strcpy_P(buffer, (char *)ptr);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, buffer);
    */
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "Press to exit...");
#endif //  REEFTOUCH
}

void ReefAngelClass::ExitMenu()
{
	// Handles the cleanup to return to the main screen
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
	ClearScreen(DefaultBGColor);
	Timer[LCD_TIMER].Start();
	SetDisplayedMenu(DEFAULT_MENU);
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
	NeedsRedraw=true;
#else
	// Handles the cleanup to return to the main screen
	ClearScreen(DefaultBGColor);
	// If bus is locked, it will trigger wdt when drawing graph
	if(!BusLocked) // Only draw if bus is not locked
#ifdef CUSTOM_MAIN
	DrawCustomGraph();
#else
	LCD.DrawGraph(5, 5);
#endif  // CUSTOM_MAIN
#endif //  REEFTOUCH
}

void ReefAngelClass::SetDisplayedMenu(byte value)
{
	DisplayedMenu=value;
#ifdef REEFTOUCHDISPLAY
	SendMaster(MESSAGE_MENU,value,value); 	// Change Menu
#endif // REEFTOUCHDISPLAY
}

void ReefAngelClass::ProcessButtonPress()
{
    bool bResetMenuTimeout = true;
    switch ( DisplayedMenu )
    {
        default:  // DEFAULT_MENU == 255
//        {
//            // Default Screen
            break;
//        }
        case MainMenu:
        {
#ifdef CUSTOM_MENU
			ProcessButtonPressCustom();
            break;
        }
#else
            ProcessButtonPressMain();
            break;
        }

#ifndef SIMPLE_MENU
        case SetupMenu:
        {
            ProcessButtonPressSetup();
            break;
        }
#ifndef RemoveAllLights
        case LightsMenu:
        {
            ProcessButtonPressLights();
            break;
        }
#endif  // RemoveAllLights
        case TempsMenu:
        {
            ProcessButtonPressTemps();
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
        {
            ProcessButtonPressTimeouts();
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU

        case EXCEED_TIMEOUT_MENU:
        {
            // we bypass all the other menus when the timeout has exceeded
            // we need to mimic the default action for the main menu
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            showmenu = false;
            bResetMenuTimeout = false;
            // we are exiting the menu, so draw the graph
			ExitMenu();
            break;
        }
    }
    // if a button was pressed, we have to reset the timeout after processing the press
    if ( bResetMenuTimeout )
    {
        menutimeout = now();
    }
}

#ifdef CUSTOM_MENU
void ReefAngelClass::ProcessButtonPressCustom()
{
	showmenu = false;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case CustomMenu_1:
        {
        	MenuEntry1();
            break;
        }
#if CUSTOM_MENU_ENTRIES >= 2
        case CustomMenu_2:
        {
        	MenuEntry2();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
        case CustomMenu_3:
        {
        	MenuEntry3();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
        case CustomMenu_4:
        {
        	MenuEntry4();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
        case CustomMenu_5:
        {
        	MenuEntry5();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
        case CustomMenu_6:
        {
        	MenuEntry6();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
        case CustomMenu_7:
        {
        	MenuEntry7();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
        case CustomMenu_8:
        {
        	MenuEntry8();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
        case CustomMenu_9:
        {
        	MenuEntry9();
        	break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 9
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
			ExitMenu();
            break;
        }
    }
}

#else  // CUSTOM_MENU
void ReefAngelClass::ProcessButtonPressMain()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case MainMenu_FeedingMode:
        {
        	// run feeding mode
        	FeedingModeStart();
        	showmenu = false;
            break;
        }
        case MainMenu_WaterChangeMode:
        {
        	WaterChangeModeStart();
        	showmenu = false;
            break;
        }


#ifdef SIMPLE_MENU
		// Simplified menu
		case MainMenu_ATOClear:
		{
			ATOClear();
			DisplayMenuEntry("Clear ATO Timeout");
			showmenu = false;
			break;
		}
		case MainMenu_OverheatClear:
		{
			OverheatClear();
			DisplayMenuEntry("Clear Overheat");
			showmenu = false;
			break;
		}
		case MainMenu_PHCalibration:
		{
#if defined SETUP_CALIBRATEPH_CHOICE
			SetupCalibrateChoicePH();
#else
			SetupCalibratePH();
#endif
			break;
		}
#ifdef SALINITYEXPANSION
		case MainMenu_SalinityCalibration:
		{
			SetupCalibrateSalinity();
			break;
		}
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
        case MainMenu_ORPCalibration:
        {
            SetupCalibrateORP();
            break;
        }
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
		case MainMenu_PHExpCalibration:
		{
			SetupCalibratePHExp();
			break;
		}
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
		case MainMenu_WaterCalibration:
		{
			SetupCalibrateWaterLevel();
			break;
		}
#endif  // WATERLEVELEXPANSION
#if defined DateTimeSetup
		case MainMenu_DateTime:
		{
#ifdef DATETIME24
            SetupDateTime24();
#else
			SetupDateTime();
#endif //DATETIME24
			break;
		}
#endif  // DateTimeSetup

#else  // SIMPLE_MENU

		// Standard menus
#ifndef RemoveAllLights
        case MainMenu_Lights:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(LightsMenu);
            break;
        }
#endif  // RemoveAllLights
        case MainMenu_Temps:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(TempsMenu);
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case MainMenu_Timeouts:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(TimeoutsMenu);
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
        case MainMenu_Setup:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(SetupMenu);
            break;
        }
#endif  // SIMPLE_MENU


#ifdef VersionMenu
        case MainMenu_Version:
        {
            DisplayVersion();
            // turn off the menu so we can wait for the button press to exit
            showmenu = false;
            break;
        }
#endif  // VersionMenu
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // disable the menu, display main screen
            showmenu = false;
            // When we exit the main menu, we will redraw the graph
			ExitMenu();
            break;
        }
    }
}

#ifndef SIMPLE_MENU
void ReefAngelClass::ProcessButtonPressSetup()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef WavemakerSetup
        case SetupMenu_Wavemaker:
        {
            int v = InternalMemory.WM1Timer_read();
            int y = InternalMemory.WM2Timer_read();
            if ( SetupOption(v, y, 0, 21600, 5, "s", "", "Setup Wavemakers", "WM1:", "WM2:") )
            {
                InternalMemory.WM1Timer_write(v);
                InternalMemory.WM2Timer_write(y);
//                Relay.On(WM1Port);
//                Relay.On(WM2Port);
//                Relay.Write();
            }
            break;
        }
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
        case SetupMenu_DosingPump:
        {
            SetupDosingPump();
            break;
        }
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
		case SetupMenu_DosingPumpInterval:
		{
            int v = InternalMemory.DP1RepeatInterval_read();
            int y = InternalMemory.DP2RepeatInterval_read();
            if ( SetupOption(v, y, 1, 1440, 4, "m", "", "Repeat Interval", "DP1:", "DP2:") )
            {
            	InternalMemory.DP1RepeatInterval_write(v);
            	InternalMemory.DP2RepeatInterval_write(y);
            }
            v = InternalMemory.DP1Timer_read();
            y = InternalMemory.DP2Timer_read();
            if ( SetupOption(v, y, 1, 255, 3, "s", "", "Run Time", "DP1:", "DP2:") )
            {
            	InternalMemory.DP1Timer_write(v);
            	InternalMemory.DP2Timer_write(y);
            }
			break;
		}
#endif  // DosingPumpIntervalSetup
        case SetupMenu_CalibratePH:
        {
#if defined SETUP_CALIBRATEPH_CHOICE
            SetupCalibrateChoicePH();
#else
            SetupCalibratePH();
#endif
            break;
        }
#ifdef SALINITYEXPANSION
		case SetupMenu_CalibrateSalinity:
		{
			SetupCalibrateSalinity();
			break;
		}
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
        case SetupMenu_CalibrateORP:
        {
            SetupCalibrateORP();
            break;
        }
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
		case SetupMenu_PHExpCalibration:
		{
			SetupCalibratePHExp();
			break;
		}
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
		case SetupMenu_WaterCalibration:
		{
			SetupCalibrateWaterLevel();
			break;
		}
#endif  // WATERLEVELEXPANSION
#if defined DateTimeSetup
        case SetupMenu_DateTime:
        {
#ifdef DATETIME24
            SetupDateTime24();
#else
			SetupDateTime();
#endif
            break;
        }
#endif  // DateTimeSetup
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}

#ifndef RemoveAllLights
void ReefAngelClass::ProcessButtonPressLights()
{
    showmenu = true;  // set to true when displaying setup screens
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case LightsMenu_On:
        {
        	LightsOn();
            DisplayMenuEntry("Lights On");
        	showmenu = false;
            break;
        }
        case LightsMenu_Off:
        {
        	LightsOff();
            DisplayMenuEntry("Restore Lights");	
            showmenu = false;
            break;
        }
#ifdef MetalHalideSetup
        case LightsMenu_MetalHalides:
        {
            SetupLightsOptionDisplay(true);
            break;
        }
        case LightsMenu_MetalHalideDelay:
        {
            int v = InternalMemory.MHDelay_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 255, 3, "m", "", "Setup MH Delay", "", "") )
            {
                InternalMemory.MHDelay_write((uint8_t)v);
            }
            break;
        }
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
        case LightsMenu_StandardLights:
        {
            SetupLightsOptionDisplay(false);
            break;
        }
#endif  // StandardLightSetup
#if defined DisplayLEDPWM && !defined REEFANGEL_MINI
        case LightsMenu_LEDPWM:
        {
            int v = InternalMemory.LEDPWMActinic_read();
            int y = InternalMemory.LEDPWMDaylight_read();
            if ( SetupOption(v, y, 0, 100, 3, "%", "", "Setup LED", "AP:", "DP:") )
            {
                InternalMemory.LEDPWMActinic_write((uint8_t)v);
                InternalMemory.LEDPWMDaylight_write((uint8_t)y);
                // Restore PWM values
                PWM.SetActinic((uint8_t)v);
                PWM.SetDaylight((uint8_t)y);
            }
            break;
        }
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}
#endif  // RemoveAllLights

void ReefAngelClass::ProcessButtonPressTemps()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case TempsMenu_Heater:
        {
            int v = InternalMemory.HeaterTempOn_read();
            int y = InternalMemory.HeaterTempOff_read();
            int min, max;
            char *s = "0";
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_LOW_TEMP;
            	max = DEGREE_C_HIGH_TEMP;
            	s = "C";
            }
            else
            {
            	min = DEGREE_F_LOW_TEMP;
            	max = DEGREE_F_HIGH_TEMP;
            	s = "F";
            }
            if ( SetupOption(v, y, min, max, 3, s, ".", "Setup Heater", "On @", "Off @") )
            {
                InternalMemory.HeaterTempOn_write(v);
                InternalMemory.HeaterTempOff_write(y);
            }
            break;
        }
        case TempsMenu_Chiller:
        {
            int v = InternalMemory.ChillerTempOn_read();
            int y = InternalMemory.ChillerTempOff_read();
            int min, max;
            char *s = "0";
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_LOW_TEMP;
            	max = DEGREE_C_HIGH_TEMP;
            	s = "C";
            }
            else
            {
            	min = DEGREE_F_LOW_TEMP;
            	max = DEGREE_F_HIGH_TEMP;
            	s = "F";
            }
            if ( SetupOption(v, y, min, max, 3, s, ".", "Setup Chiller", "On @", "Off @") )
            {
                InternalMemory.ChillerTempOn_write(v);
                InternalMemory.ChillerTempOff_write(y);
            }
            break;
        }
#ifdef OverheatSetup
        case TempsMenu_Overheat:
        {
            int v = InternalMemory.OverheatTemp_read();
            int y = -1;
            int min, max;
            char *s = "0";
            byte d;
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_OVERHEAT_LOW_TEMP;
            	max = DEGREE_C_OVERHEAT_HIGH_TEMP;
            	s = "C";
            	d = 3;
            }
            else
            {
            	min = DEGREE_F_OVERHEAT_LOW_TEMP;
            	max = DEGREE_F_OVERHEAT_HIGH_TEMP;
            	s = "F";
            	d = 4;
            }
            if ( SetupOption(v, y, min, max, d, s, ".", "Setup Overheat", "", "") )
            {
                InternalMemory.OverheatTemp_write(v);
            }
            break;
        }
#endif  // OverheatSetup
        case TempsMenu_OverheatClr:
        {
        	OverheatClear();
            DisplayMenuEntry("Clear Overheat");
            showmenu = false;
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}

#if defined SetupExtras || defined ATOSetup
void ReefAngelClass::ProcessButtonPressTimeouts()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef ATOSetup
        case TimeoutsMenu_ATOSet:
        {
#ifdef SingleATOSetup
            int v = InternalMemory.ATOTimeout_read();
            int y = InternalMemory.ATOHighTimeout_read();
            if ( SetupOption(v, y, 0, 255, 3, "s", "", "ATO Timeout", "Low:", "High:") )
            {
                InternalMemory.ATOTimeout_write((uint8_t)v);
                InternalMemory.ATOHighTimeout_write((uint8_t)y);
            }
#else  // SingleATOSetup
            int v = InternalMemory.ATOTimeout_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 255, 3, "s", "", "ATO Timeout", "", "") )
            {
                InternalMemory.ATOTimeout_write((uint8_t)v);
            }
#endif  // SingleATOSetup
            break;
        }
#ifdef SingleATOSetup
        case TimeoutsMenu_ATOHrInterval:
        {
            int v = InternalMemory.ATOHourInterval_read();
            int y = InternalMemory.ATOHighHourInterval_read();
            if ( SetupOption(v, y, 0, 24, 2, "h", "", "ATO Interval", "Low:", "High:") )
            {
                InternalMemory.ATOHourInterval_write((uint8_t)v);
                InternalMemory.ATOHighHourInterval_write((uint8_t)y);
            }
            break;
        }
#endif  // SingleATOSetup
        case TimeoutsMenu_ATOClear:
        {
        	ATOClear();
            DisplayMenuEntry("Clear ATO Timeout");
            showmenu = false;
            break;
        }
#endif  // ATOSetup
#ifdef SetupExtras
        case TimeoutsMenu_Feeding:
        {
            int v = InternalMemory.FeedingTimer_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 3600, 4, "s", "", "Feeding Timer", "", "") )
            {
                InternalMemory.FeedingTimer_write(v);
                // update the feeding timer value
                Timer[FEEDING_TIMER].SetInterval(v);
            }
            break;
        }
        case TimeoutsMenu_LCD:
        {
            int v = InternalMemory.LCDTimer_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 3600, 4, "s", "", "Screen Timeout", "", "") )
            {
                InternalMemory.LCDTimer_write(v);
                // update the timer value
                Timer[LCD_TIMER].SetInterval(v);
                Timer[LCD_TIMER].Start();
            }
            break;
        }
#endif  // SetupExtras
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}
#endif  // if defined SetupExtras || defined ATOSetup

// Setup Menu Screens
bool ReefAngelClass::SetupOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                       char* unit, char* subunit, char* title,
                       char* prefix1, char* prefix2)
{
    // return true to save value stored in out in memory
    enum choices {
        OPT1,
        OPT2,
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    bool bSingle = (y < 0) ? true : false;
    byte offset = 50;
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, title);
    // prefix for each option
    if ( ! bSingle )
    {
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, prefix1);
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, prefix2);
    }
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                    {
                        if ( ! bSingle )
                            LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        LCD.DrawOption(v, 1, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OPT2:
                    {
                        LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        LCD.DrawOption(y, 1, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OK:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            if ( ! bSingle )
                                LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(true);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case CANCEL:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            if ( ! bSingle )
                                LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(false);
                            LCD.DrawCancel(true);
                        }
                        break;
                    }
            }
            bRedraw = false;
            bDrawButtons = false;
        } // if bRedraw
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v++;
                if ( v > rangemax || v < rangemin)
                {
                    v = rangemin;
                }
            }
            else if ( sel == OPT2 )
            {
                y++;
                if ( y > rangemax || y < rangemin )
                {
                    y = rangemin;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v--;
                if ( v < rangemin || v > rangemax )
                {
                    v = rangemax;
                }
            }
            else if ( sel == OPT2 )
            {
                y--;
                if ( y < rangemin || y > rangemax )
                {
                    y = rangemax;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( bSingle && sel == OPT2 ) sel++; // advance again if single
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( bSingle && sel == OPT2 ) sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    // return true saves the value, false ignores the value
    return bSave;
}

void ReefAngelClass::SetupLightsOptionDisplay(bool bMetalHalide)
{
    enum choices {
        OPT1, // On Hour
        OPT2, // On Minute
        OPT3, // Off Hour
        OPT4, // Off Minute
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2;
    char msg[20];
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
#ifdef MetalHalideSetup
	strcpy(msg, "Metal Halide Setup");
	h1 = InternalMemory.MHOnHour_read();
	m1 = InternalMemory.MHOnMinute_read();
	h2 = InternalMemory.MHOffHour_read();
	m2 = InternalMemory.MHOffMinute_read();
#endif // MetalHalideSetup
#ifdef StandardLightSetup
	strcpy(msg, "Std Lights Setup");
	h1 = InternalMemory.StdLightsOnHour_read();
	m1 = InternalMemory.StdLightsOnMinute_read();
	h2 = InternalMemory.StdLightsOffHour_read();
	m2 = InternalMemory.StdLightsOffMinute_read();
#endif // StandardLightSetup
    ClearScreen(DefaultBGColor);
    // header / title
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, msg);
    // prefixes, draw in ':' between options
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*4, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "Off:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
        	byte WhichOption=0;
        	boolean OKStatus=false;
        	boolean CancelStatus=false;
            switch ( sel )
            {
                case OPT1:
                {
                	WhichOption=1;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT2:
                {
                	WhichOption=2;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT3:
                {
                	WhichOption=3;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT4:
                {
                	WhichOption=4;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OK:
                {
                	WhichOption=0;
                	OKStatus=true;
                	CancelStatus=false;

                    break;
                }
                case CANCEL:
                {
                	WhichOption=0;
                	OKStatus=false;
                	CancelStatus=true;
                    break;
                }
            }
            LCD.DrawOption(h1, WhichOption==1, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
            LCD.DrawOption(m1, WhichOption==2, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
            LCD.DrawOption(h2, WhichOption==3, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
            LCD.DrawOption(m2, WhichOption==4, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
            if ( bDrawButtons )
            {
                LCD.DrawOK(OKStatus);
                LCD.DrawCancel(CancelStatus);
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT4 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT4 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        if ( bMetalHalide )
        {
#ifdef MetalHalideSetup
            InternalMemory.MHOnHour_write(h1);
            InternalMemory.MHOnMinute_write(m1);
            InternalMemory.MHOffHour_write(h2);
            InternalMemory.MHOffMinute_write(m2);
#endif MetalHalideSetup
        }
        else
        {
#ifdef StandardLightSetup
            InternalMemory.StdLightsOnHour_write(h1);
            InternalMemory.StdLightsOnMinute_write(m1);
            InternalMemory.StdLightsOffHour_write(h2);
            InternalMemory.StdLightsOffMinute_write(m2);
#endif // StandardLightSetup
        }
    }
}
#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU


void ReefAngelClass::SetupCalibratePH()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {7,10};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate pH");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, "pH ");
    	Font.DrawText((int)iCal[b]);    	
#else //  REEFTOUCH
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL + 18, MENU_START_ROW*5, (int)iCal[b]);
#endif //  REEFTOUCH    	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<30;a++)
			{
				iO[b] += analogRead(PHPin);
			}
			iO[b]/=30;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH			
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH    	
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
        // save PHMin & PHMax to memory
        InternalMemory.PHMin_write(iO[0]);
        PHMin = iO[0];
        InternalMemory.PHMax_write(iO[1]);
		PHMax = iO[1];
	}
}

#ifdef SETUP_CALIBRATEPH_CHOICE
void ReefAngelClass::SetupCalibrateChoicePH()
{
	enum choices {
        TARGETPH,
        CANCEL,
        OK
    };
    byte sel = CANCEL;
	
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    unsigned int iStart[2] = {7,10};
    unsigned int iTarget[2] = {0,0};
    unsigned int iValue[2] = {0,0};
    char msg[15];
	unsigned int maxPh = 10;
	unsigned int minPh = 4;
    byte offset = 65;
		
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
		bRedraw = true;
    	bDrawButtons = true;
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate pH");		
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, "pH");
		
		strcpy(msg, b==0 ? "First value\0" : "Second value\0");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, msg);
		
		iTarget[b] = iStart[b];
		if(b==1 && iTarget[0]==iTarget[b])
		{
			iTarget[b]--;
		}
	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iValue[b]=0;
			for (int a=0;a<30;a++)
			{
				iValue[b] += analogRead(PHPin);
			}
			iValue[b]/=30;
			LCD.DrawCalibrate(iValue[b], MENU_START_COL + offset, MENU_START_ROW*6);
			
			
			if ( bRedraw )
			{
				switch ( sel )
				{
					case TARGETPH:
					{
						LCD.DrawOption(iTarget[b], 1, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
						if ( bDrawButtons )
						{
							LCD.DrawOK(false);
							LCD.DrawCancel(false);
						}
						break;
					}
					case OK:
					{
						if ( bDrawButtons )
						{
							LCD.DrawOption(iTarget[b], 0, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
							LCD.DrawOK(true);
							LCD.DrawCancel(false);
						}
						break;
					}
					case CANCEL:
					{
						if ( bDrawButtons )
						{
							LCD.DrawOption(iTarget[b], 0, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
							LCD.DrawOK(false);
							LCD.DrawCancel(true);
						}
						break;
					}
				}
				bRedraw = false;
				bDrawButtons = false;
			}
			
			if ( Joystick.IsUp() )
			{
				if (sel == TARGETPH)
				{
					iTarget[b]++;
					if(b==1 && iTarget[0]==iTarget[b])
					{
						if((iTarget[b] + 1) <= maxPh)
						{
							iTarget[b]++;
						} else {
							iTarget[b]--;
						}
					}
					
					if ( iTarget[b] > maxPh )
					{
						iTarget[b] = maxPh;
					}
					else 
					{
						bRedraw = true;
					}
				}
			}
			if ( Joystick.IsDown() )
			{
				if (sel == TARGETPH)
				{
					iTarget[b]--;
					if(b==1 && iTarget[0]==iTarget[b])
					{
						if((iTarget[b] - 1) >= minPh)
						{
							iTarget[b]--;
						} else {
							iTarget[b]++;
						}
					}
					
					if ( iTarget[b] < minPh )
					{
						iTarget[b] = minPh;
					}
					else 
					{
						bRedraw = true;
					}
				}
			}
			
			if ( Joystick.IsLeft() )
			{
				bRedraw = true;
				bDrawButtons = true;
				sel--;
				if ( sel > OK )
				{
					sel = OK;
				}
			}
			
			if ( Joystick.IsRight() )
			{
				bRedraw = true;
				bDrawButtons = true;
				sel++;
				if ( sel > OK )
				{
					sel = TARGETPH;
				}
			}
			
			if ( Joystick.IsButtonPressed() )
			{
				if ( sel == OK || sel == TARGETPH)
				{
					bDone = true;
					bSave = true;
				}
				else if ( sel == CANCEL )
				{
					bDone = true;
				}
			}
		} while ( ! bDone );
    }
	
    ClearScreen(DefaultBGColor);
	
	if ( bSave )
	{
		PHMin = map(7.0, iTarget[0], iTarget[1], iValue[0], iValue[1]);
		PHMax = map(10.0, iTarget[0], iTarget[1], iValue[0], iValue[1]);
		
        // save PHMin & PHMax to memory
        InternalMemory.PHMin_write(PHMin);
        InternalMemory.PHMax_write(PHMax);
	}
}
#endif // SETUP_CALIBRATEPH_CHOICE

#ifdef SALINITYEXPANSION
void ReefAngelClass::SetupCalibrateSalinity()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iS = 0;
    byte offset = 65;
    // draw labels
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate");
    	LargeFont.DrawText(30, 50, "Salinity");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, "35 PPT");
#else //  REEFTOUCH
        ClearScreen(DefaultBGColor);
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate Salinity");
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "35 PPT");
#endif //  REEFTOUCH     
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
		iS=0;
	    for (int a=0;a<15;a++)
	    {
	    	iS += Salinity.Read();
	    }
	    iS/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iS);
	    	Font.DrawText("   ");
#else //  REEFTOUCH				    
        LCD.DrawCalibrate(iS, MENU_START_COL + offset, MENU_START_ROW*5);
        if (  bDrawButtons )
        {
            if ( bOKSel )
            {
                LCD.DrawOK(true);
                LCD.DrawCancel(false);
            }
            else
            {
                LCD.DrawOK(false);
                LCD.DrawCancel(true);
            }
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
        {
            // toggle the selection
            bOKSel = !bOKSel;
            bDrawButtons = true;
        }
        if ( Joystick.IsButtonPressed() )
        {
            bDone = true;
            if ( bOKSel )
            {
                bSave = true;
            }
        }
#endif //  REEFTOUCH     
    } while ( ! bDone );

    if ( bSave )
    {
        // save SalMax to memory
        InternalMemory.SalMax_write(iS);
        InternalMemory.SalTempComp_write(Params.Temp[TempProbe]);
		SalMax = iS;
    }
}

void ReefAngelClass::ApplySalinityCompensation()
{
	// Salinity Compensation was contributed by ahmedess
	// http://forum.reefangel.com/viewtopic.php?p=7386#p7386
	// Credits to dazza1304
	// http://forum.reefangel.com/viewtopic.php?f=3&t=2670	
	if (Salinity.TemperatureCompensation!=-1 && Params.Temp[TempProbe]>0)
	{
		double SalCompensation;
		double SalConstant=Salinity.TemperatureCompensation;
		if (Salinity.TemperatureCompensation==0)
		{
			if (TempSensor.unit)
				SalConstant=0.0024;
			else
				SalConstant=0.001333;
		}
		SalCompensation=Params.Salinity/(1+((Params.Temp[TempProbe]-InternalMemory.SalTempComp_read())*SalConstant));
		Params.Salinity=round(SalCompensation);
	}	
}

#endif  // SALINITYEXPANSION

#ifdef ORPEXPANSION
void ReefAngelClass::SetupCalibrateORP()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {0,470};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate ORP");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, iCal[b]);
    	Font.DrawText(" mV  ");
#else //  REEFTOUCH
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate ORP");
		char text[10];
		itoa(iCal[b],text,10);
		strcat(text , " mV  ");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, text);
#endif //  REEFTOUCH  
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += ORP.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH     
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save ORPMin & ORPMax to memory
		InternalMemory.ORPMin_write(iO[0]);
		ORPMin = iO[0];
		InternalMemory.ORPMax_write(iO[1]);
		ORPMax = iO[1];
	}
}
#endif  // ORPEXPANSION

#ifdef PHEXPANSION
void ReefAngelClass::SetupCalibratePHExp()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {7,10};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate PH");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, "pH ");
    	Font.DrawText(iCal[b]);
#else //  REEFTOUCH
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate PH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL + 18, MENU_START_ROW*5, (int)iCal[b]);
#endif //  REEFTOUCH      	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += PH.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
	    	LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH 			
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save PHExpMin & PHExpMax to memory
		InternalMemory.PHExpMin_write(iO[0]);
		PHExpMin = iO[0];
		InternalMemory.PHExpMax_write(iO[1]);
		PHExpMax = iO[1];
	}
}
#endif  // PHEXPANSION

#ifdef WATERLEVELEXPANSION
void ReefAngelClass::SetupCalibrateWaterLevel()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {0,100};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate PH");
    	LargeFont.DrawText(30, 50, "Water Level");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, iCal[b]);
    	Font.DrawText(" %  ");
#else //  REEFTOUCH
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate Water Level");
		char text[10];
		itoa(iCal[b],text,10);
		strcat(text , " %  ");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, text);
#endif //  REEFTOUCH    	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += WaterLevel.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH    			
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save WaterLevelMin & WaterLevelMax to memory
		InternalMemory.WaterLevelMin_write(iO[0]);
		InternalMemory.WaterLevelMax_write(iO[1]);
	}
}
#endif  // WATERLEVELEXPANSION

#if defined DateTimeSetup && !defined DATETIME24
void ReefAngelClass::SetupDateTime()
{
    enum choices {
        MONTH,
        DAY,
        YEAR,
        HOUR,
        MINUTE,
        OK,
        CANCEL
    };
    byte sel = MONTH;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte Year, Month, Day, Hour, Minute;
    byte MonthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    byte DateRow = 45, TimeRow = 75;

    Year = year() - 2000;
    Month = month();
    Day = day();
    Hour = hour();
    Minute = minute();

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
#else //  REEFTOUCH
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Set Date & Time");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, DateRow,"Date:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, TimeRow,"Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, TimeRow, ":");

    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    LCD.DrawOption(Month, 1, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case DAY:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 1, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case YEAR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 1, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case HOUR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 1, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case MINUTE:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 1, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }

            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month++;
                    if ( Month > 12 )
                    {
                        Month = 1;
                    }
                    break;
                }
                case DAY:
                {
                    Day++;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day > MonthDays[Month] )
                        {
                            Day = 1;
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day > 29 )
                            {
                                Day = 1;
                            }
                        }
                        else
                        {
                            if ( Day > MonthDays[Month] )
                            {
                                Day = 1;
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year++;
                    if ( Year > 99 )
                    {
                        Year = 0;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour++;
                    if ( Hour > 23 )
                    {
                        Hour = 0;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute++;
                    if ( Minute > 59 )
                    {
                        Minute = 0;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsDown() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month--;
                    if ( Month < 1 || Month > 12 )
                    {
                        Month = 12;
                    }
                    break;
                }
                case DAY:
                {
                    Day--;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day < 1 || Day > MonthDays[Month] )
                        {
                            Day = MonthDays[Month];
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = 29;
                            }
                        }
                        else
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = MonthDays[Month];
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year--;
                    if ( Year > 99 )
                    {
                        Year = 99;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour--;
                    if ( Hour > 23 )
                    {
                        Hour = 23;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute--;
                    if ( Minute > 59 )
                    {
                        Minute = 59;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = MONTH;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
#endif //  REEFTOUCH

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }

}
#endif  // DateTimeSetup


#if defined DateTimeSetup && defined DATETIME24
void ReefAngelClass::SetupDateTime24()
{
    enum choices {
        YEAR,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
        OK,
        CANCEL
    };
    byte sel = YEAR;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte Year, Month, Day, Hour, Minute;
    byte MonthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	uint8_t iTimeformat;
    byte DateRow = 35, TimeRow = 60, FormatRow = 85;

    Year = year() - 2000;
    Month = month();
    Day = day();
    Hour = hour();
    Minute = minute();

    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Set Date & Time");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, DateRow,"Date:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, TimeRow,"Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 57, DateRow, "20");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, DateRow, "-");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 102, DateRow, "-");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, TimeRow, ":");

    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
				case YEAR:
				{
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 1, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
					if ( bDrawButtons )
					{
						LCD.DrawOK(false);
						LCD.DrawCancel(false);
					}
					break;
				}
                case MONTH:
                {
                    LCD.DrawOption(Month, 1, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case DAY:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 1, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case HOUR:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 1, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case MINUTE:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 1, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }

                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }

            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month++;
                    if ( Month > 12 )
                    {
                        Month = 1;
                    }
                    break;
                }
                case DAY:
                {
                    Day++;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day > MonthDays[Month] )
                        {
                            Day = 1;
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day > 29 )
                            {
                                Day = 1;
                            }
                        }
                        else
                        {
                            if ( Day > MonthDays[Month] )
                            {
                                Day = 1;
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year++;
                    if ( Year > 99 )
                    {
                        Year = 0;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour++;
                    if ( Hour > 23 )
                    {
                        Hour = 0;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute++;
                    if ( Minute > 59 )
                    {
                        Minute = 0;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsDown() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month--;
                    if ( Month < 1 || Month > 12 )
                    {
                        Month = 12;
                    }
                    break;
                }
                case DAY:
                {
                    Day--;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day < 1 || Day > MonthDays[Month] )
                        {
                            Day = MonthDays[Month];
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = 29;
                            }
                        }
                        else
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = MonthDays[Month];
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year--;
                    if ( Year > 99 )
                    {
                        Year = 99;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour--;
                    if ( Hour > 23 )
                    {
                        Hour = 23;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute--;
                    if ( Minute > 59 )
                    {
                        Minute = 59;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = YEAR;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }
}

#endif  // DATETIME24

#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
#ifdef DosingPumpSetup
void ReefAngelClass::SetupDosingPump()
{
    enum choices {
        OPT1, // DP1 On Hour
        OPT2, // DP1 On Minute
        OPT3, // DP1 Run Time
        OPT4, // DP2 On Hour
        OPT5, // DP2 On Minute
        OPT6, // DP2 Run Time
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2, r1, r2;
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
    h1 = InternalMemory.DP1OnHour_read();
    m1 = InternalMemory.DP1OnMinute_read();
    r1 = InternalMemory.DP1Timer_read();
    h2 = InternalMemory.DP2OnHour_read();
    m2 = InternalMemory.DP2OnMinute_read();
    r2 = InternalMemory.DP2Timer_read();
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "DP Setup");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*3, "DP1 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*3, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "Run Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "DP2 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*9, "Run Time:");
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 1, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT2:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m1, 1, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT3:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 1, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT4:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 1, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT5:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m2, 1, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT6:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 1, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                r1++;
            }
            else if ( sel == OPT4 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT5 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
            else if ( sel == OPT6 )
            {
                r2++;
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                r1--;
            }
            else if ( sel == OPT4 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT5 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
            else if ( sel == OPT6 )
            {
                r2--;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        InternalMemory.DP1OnHour_write(h1);
        InternalMemory.DP1OnMinute_write(m1);
        InternalMemory.DP1Timer_write(r1);
        InternalMemory.DP2OnHour_write(h2);
        InternalMemory.DP2OnMinute_write(m2);
        InternalMemory.DP2Timer_write(r2);
    }
}
#endif  // DosingPumpSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU

#ifdef REEFTOUCHDISPLAY
void receiveEvent(int howMany) {
	byte d[9];
	wdt_reset();
	if (ReefAngel.Sleeping)
	{
		while(Wire.available())
			Wire.read();
	}
	else
	{
		if (howMany==9)
		{
			for(int a=0;a<9;a++) d[a]=Wire.read();
			switch (d[0])
			{
			case 0:
				ReefAngel.Params.Temp[T1_PROBE]=d[1]+(d[2]<<8);
				ReefAngel.Params.Temp[T2_PROBE]=d[3]+(d[4]<<8);
				ReefAngel.Params.Temp[T3_PROBE]=d[5]+(d[6]<<8);
				ReefAngel.Params.PH=d[7]+(d[8]<<8);
				break;
			case 1:
				ReefAngel.Relay.RelayData=d[1];
				ReefAngel.Relay.RelayMaskOn=d[2];
				ReefAngel.Relay.RelayMaskOff=d[3];
				ReefAngel.LowATO.SetActive(bitRead(d[4],0));
				ReefAngel.HighATO.SetActive(bitRead(d[4],1));
				ReefAngel.EM=d[5];
				ReefAngel.REM=d[6];
				if (ReefAngel.DisplayedMenu!=d[7])
				{
					if (ReefAngel.DisplayedMenu!=TOUCH_MENU)
					{
						// deduct 100 from the value to indicate we are coming from an interrupt.
						ReefAngel.DisplayedMenu=d[7]-100;
					}
				}
				ReefAngel.Flags=d[8];
				break;
			case 2:
				ReefAngel.PWM.SetDaylight(d[1]);
				ReefAngel.PWM.SetActinic(d[2]);
				for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
					ReefAngel.PWM.SetChannel(a,d[a+3]);
				break;
			case 3:
				ReefAngel.RF.Mode=d[1];
				ReefAngel.RF.Speed=d[2];
				ReefAngel.RF.Duration=d[3];
				ReefAngel.AI.SetChannel(0,d[4]);
				ReefAngel.AI.SetChannel(1,d[5]);
				ReefAngel.AI.SetChannel(2,d[6]);
				ReefAngel.IO.IOPorts=d[7];
				break;
			case 4:
				ReefAngel.RF.RadionChannels[0]=d[1];
				ReefAngel.RF.RadionChannels[1]=d[2];
				ReefAngel.RF.RadionChannels[2]=d[3];
				ReefAngel.RF.RadionChannels[3]=d[4];
				ReefAngel.RF.RadionChannels[4]=d[5];
				ReefAngel.RF.RadionChannels[5]=d[6];
				break;
			}
		}
		else if (howMany==3)
		{
			for(int a=0;a<3;a++) d[a]=Wire.read();
			switch (d[0])
			{
			case 0:
				if (abs(ReefAngel.Timer[FEEDING_TIMER].Trigger-(now()+d[1]+(d[2]<<8)))>2)
						ReefAngel.Timer[FEEDING_TIMER].Trigger=now()+d[1]+(d[2]<<8);
				break;
			}
		}
		else
		{
			for (int a=0;a<howMany;a++)
			{
				Wire.read();
			}
		}  
	}
}

void SendMaster(byte ID, byte data1, byte data2)
{
	Wire.beginTransmission(I2CRA_Master);
	Wire.write(ID);
	Wire.write(data1);
	Wire.write(data2);
	Wire.endTransmission();	// Simulate button press
	delay(10);
	
}

#endif //  REEFTOUCHDISPLAY

#ifdef I2CMASTER 
void receiveEventMaster(int howMany)
{
	byte d[9];
	wdt_reset();
	if (howMany==3)
	{
		for(int a=0;a<3;a++) d[a]=Wire.read();
		switch (d[0])
		{
			case MESSAGE_BUTTON: // Simulate button press
			{
				if (d[1]==1 && d[2]==1) ButtonPress++;
				break;
			}
			case MESSAGE_RELAY_OVERRIDE: // Override relay ports
			{
				ReefAngel.Relay.Override(d[1],d[2]);
				break;
			}
			case MESSAGE_CHANNEL_OVERRIDE: // Override Channels
			{	
				if (d[1]<=OVERRIDE_CHANNEL5)
					ReefAngel.PWM.Override(d[1],d[2]);
				if (d[1]>=OVERRIDE_AI_WHITE && d[1]<=OVERRIDE_AI_BLUE)
					ReefAngel.AI.Override(d[1]-OVERRIDE_AI_WHITE,d[2]);
				if (d[1]>=OVERRIDE_RF_WHITE && d[1]<=OVERRIDE_RF_INTENSITY)
					ReefAngel.RF.Override(d[1]-OVERRIDE_RF_WHITE,d[2]);
				break;
			}
			case MESSAGE_MENU: // Change menu screen
			{
				if (d[1]==d[2])
				{
					switch (d[1])
					{
					case FEEDING_MODE:
					case WATERCHANGE_MODE:
						ReefAngel.ChangeMode=d[1];
						break;
					}
				}
				break;
			}
			case MESSAGE_COMMAND: // I2C Commands
			{
				ReefAngel.I2CCommand=d[1];
				break;
			}
		}
	}
	else
	{
		for (int a=0;a<howMany;a++)
		{
			Wire.read();
		}
	}  	
}
#endif // I2CMASTER
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
ButtonClass::ButtonClass()
{
	x1=0;
	y1=0;
}

void ButtonClass::Create(int icolor, int itextcolor, char *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void ButtonClass::Show()
{
	visible=true;
	ReefAngel.LargeFont.SetColor(COLOR_BLACK,COLOR_SILVER,true);
	ReefAngel.LargeFont.DrawText(x1+25,y1+5,str);
	x2=ReefAngel.LargeFont.GetX()+25;
	ReefAngel.TouchLCD.DrawRoundRect(RGB565(0xD0, 0xD0, 0xD0),x1+1,y1+1,x2+1,y1+41,10,false);
	ReefAngel.TouchLCD.DrawRoundRect(COLOR_BLACK,x1,y1,x2,y1+40,10,false);
	ReefAngel.TouchLCD.DrawRoundRect(color,x1+1,y1+1,x2-1,y1+39,10,true);
	ReefAngel.LargeFont.SetColor(COLOR_BLACK,COLOR_SILVER,true);
	ReefAngel.LargeFont.DrawText(x1+26,y1+5,str);
	ReefAngel.LargeFont.SetColor(textcolor,COLOR_SILVER,true);
	ReefAngel.LargeFont.DrawText(x1+25,y1+4,str);
}

void ButtonClass::Hide()
{
	visible=false;
	ReefAngel.TouchLCD.Clear(BKCOLOR,x1,y1,x2+1,y1+41);
}

boolean ButtonClass::IsPressed()
{
	return (ReefAngel.TS.X > x1 && ReefAngel.TS.X < x2 && ReefAngel.TS.Y > y1 && ReefAngel.TS.Y < y1+40);
}

ProgressBarClass::ProgressBarClass()
{
	min=0;
	max=100;
	current=0;
	x1=0;
	y1=0;
	NeedsRedraw=true;
}

void ProgressBarClass::Create(int icolor, int ibkcolor, int itextcolor, char *istr)
{
	color=icolor;
	bkcolor=ibkcolor;
	textcolor=itextcolor;
	str=istr;
}

void ProgressBarClass::SetCurrent(int value)
{
	if (current!=value) NeedsRedraw=true;
	current=value; 
}

void ProgressBarClass::Show()
{
	if (NeedsRedraw)
	{
		visible=true;
		NeedsRedraw=false;
		ReefAngel.TouchLCD.Clear(bkcolor,x1,y1,130,y1+20);
		for (int a=0;a<10;a++) ReefAngel.TouchLCD.DrawPixel(COLOR_RED,x1+68,y1+(a*2));
		ReefAngel.Font.SetColor(textcolor,bkcolor,false);
		ReefAngel.Font.DrawText(x1+5,y1+5,str);
		ReefAngel.Font.DrawText(x1+75,y1+5,current);
		ReefAngel.Font.DrawText("%   ");
		for (int a=0;a<(ReefAngel.TouchLCD.GetWidth()-139)*current/100;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-139)),130+a,y1,130+a,y1+20);
		ReefAngel.TouchLCD.Clear(BKCOLOR,((ReefAngel.TouchLCD.GetWidth()-139)*current/100)+130,y1,ReefAngel.TouchLCD.GetWidth(),y1+20);
	}
}

void ProgressBarClass::Hide()
{
	visible=false;
	ReefAngel.TouchLCD.Clear(BKCOLOR,x1,y1,ReefAngel.TouchLCD.GetWidth()-10,y1+20);
}

boolean ProgressBarClass::IsPressed()
{
	return (ReefAngel.TS.X > x1 && ReefAngel.TS.Y > y1 && ReefAngel.TS.Y < y1+20);
}

SliderClass::SliderClass()
{
	min=0;
	max=100;
	current=0;
	x1=0;
	y1=0;
	NeedsRedraw=true;
}

void SliderClass::Create(int icolor, int itextcolor, char *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void SliderClass::DrawMarker()
{
	int currentX=map(current,min,max,41,ReefAngel.TouchLCD.GetWidth()-41);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX-2,y1+31,currentX-2,y1+32);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX-2,y1+59,currentX-2,y1+60);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX+2,y1+31,currentX+2,y1+32);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX+2,y1+59,currentX+2,y1+60);
//	for (int a=currentX-2;a<=currentX+2;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,BKCOLOR,((a-40)*100)/(ReefAngel.TouchLCD.GetWidth()-80)),a,y1+33,a,y1+58);
	for (int a=0;a<ReefAngel.TouchLCD.GetWidth()-80;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-80)),40+a,y1+33,40+a,y1+58);
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,0,y1+31,ReefAngel.TouchLCD.GetWidth(),y1+32);
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,0,y1+59,ReefAngel.TouchLCD.GetWidth(),y1+60);
	ReefAngel.TouchLCD.Clear(COLOR_GRAY,currentX-1,y1+31,currentX+1,y1+60);
	ReefAngel.TouchLCD.Clear(COLOR_RED,currentX,y1+32,currentX,y1+59);
	ReefAngel.LargeFont.SetColor(textcolor,COLOR_WHITE,false);
	char c[10];
	char temp[10];
	itoa(current,temp,10);
	strcpy(c," ");
	strcat(c,temp);
	strcat(c," ");
	ReefAngel.LargeFont.DrawCenterText(ReefAngel.TouchLCD.GetWidth()/2,y1,c);
}

void SliderClass::Show()
{
	visible=true;
	NeedsRedraw=false;
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,40,y1,ReefAngel.TouchLCD.GetWidth()-40,y1+75);
	ReefAngel.Font.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.Font.DrawCenterText(ReefAngel.TouchLCD.GetWidth()/2,y1+63,str);
	for (int a=0;a<ReefAngel.TouchLCD.GetWidth()-80;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-80)),40+a,y1+33,40+a,y1+58);
	ReefAngel.LargeFont.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.LargeFont.DrawCenterNumber(ReefAngel.TouchLCD.GetWidth()/2,y1,current,0);
	ReefAngel.TouchLCD.DrawBMP(10,y1+33,MINUS);
	ReefAngel.TouchLCD.DrawBMP(ReefAngel.TouchLCD.GetWidth()-35,y1+33,PLUS);
	DrawMarker();
}

void SliderClass::Hide()
{
	visible=false;
	ReefAngel.TouchLCD.Clear(BKCOLOR,0,y1,ReefAngel.TouchLCD.GetWidth(),y1+72);
}

boolean SliderClass::Refresh()
{
    wdt_reset();
	if (IsPlusPressed())
	{
		current++;
		if (current>max) current=max;
		DrawMarker();
		delay(100);
	}
	if (IsMinusPressed())
	{
		current--;
		if (current<min) current=min;
		DrawMarker();
		delay(100);
	}
	if (ReefAngel.TS.X > 40 && ReefAngel.TS.X < (ReefAngel.TouchLCD.GetWidth()-40) && ReefAngel.TS.Y > y1+25 && ReefAngel.TS.Y < y1+60)
	{
		current=map(ReefAngel.TS.X,40,ReefAngel.TouchLCD.GetWidth()-40,min,max);
		DrawMarker();
	}
	if (NeedsRedraw)
	{
		Show();
	}
}

boolean SliderClass::IsPlusPressed()
{
	return (ReefAngel.TS.X > (ReefAngel.TouchLCD.GetWidth()-35) && ReefAngel.TS.Y > y1+28 && ReefAngel.TS.Y < y1+63);
}

boolean SliderClass::IsMinusPressed()
{
	return (ReefAngel.TS.X < 35 && ReefAngel.TS.Y > y1+28 && ReefAngel.TS.Y < y1+63);
}
#endif //  REEFTOUCH


ReefAngelClass ReefAngel = ReefAngelClass() ;
