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
PROGMEM const char *menulabel_items[] = {
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
PROGMEM const char *return_items[] = { Return_0_label, Return_1_label/*, Return_2_label*/ };

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
#ifdef DateTimeSetup
const prog_char mainmenu_7_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
#ifdef VersionMenu
const prog_char mainmenu_8_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char *mainmenu_items[] = {
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
#ifdef DateTimeSetup
                    mainmenu_7_label,
#endif  // DateTimeSetup
#ifdef VersionMenu
                    mainmenu_8_label
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
PROGMEM const char *mainmenu_items[] = {
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
#ifdef DateTimeSetup
const prog_char setupmenu_6_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
PROGMEM const char *setupmenu_items[] = {
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
#ifdef DateTimeSetup
                    setupmenu_6_label
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
PROGMEM const char *lightsmenu_items[] = {
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
PROGMEM const char *tempsmenu_items[] = {
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
PROGMEM const char *timeoutsmenu_items[] = {
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
#if defined(__AVR_ATmega2560__)
	PCMSK0 |= 128;
#else  // __AVR_ATmega2560__
	PCMSK0 |= 32;
#endif  // __AVR_ATmega2560__
	PCICR |= 1;
}

void ReefAngelClass::Init()
{
	Wire.begin();
	Serial.begin(57600);
#ifdef __PLUS_SPECIAL_WIFI__
	Serial1.begin(57600);
#endif // __PLUS_SPECIAL_WIFI__
	pinMode(Piezo, OUTPUT);
	digitalWrite(lowATOPin,HIGH); //pull up resistor on lowATOPin
	digitalWrite(highATOPin,HIGH); //pull up resistor on highATOPin
	LCD.Init();
	Joystick.Init();
	TempSensor.Init();
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	setSyncInterval(SECS_PER_HOUR);  // Changed to sync every hour.
	RAStart=now();
	LastStart = RAStart;  // Set the time normal mode is started
	LCD.BacklightOn();
	Relay.AllOff();
	OverheatProbe = T2_PROBE;
	TempProbe = T1_PROBE;
	if (InternalMemory.IMCheck_read()!=0x5241494D) //0x5241494D
	{
		char temptext[25];
		while(1)
		{
			strcpy_P(temptext, NoIMCheck);
			LCD.DrawText(ModeScreenColor,DefaultBGColor,13,50,temptext);
			strcpy_P(temptext, NoIMCheck1);
			LCD.DrawText(ModeScreenColor,DefaultBGColor,50,75,temptext);
		}
	}
#ifdef ENABLE_ATO_LOGGING
	AtoEventCount = 0;
#endif  // ENABLE_ATO_LOGGING
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(Overheat_Exceed_Flag, 0);
	InternalMemory.write(ATO_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS


    PHMin = InternalMemory.PHMin_read();
    PHMax = InternalMemory.PHMax_read();
#ifdef SALINITYEXPANSION
    SalMax = InternalMemory.SalMax_read();
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
    ORPMin = InternalMemory.ORPMin_read();
    ORPMax = InternalMemory.ORPMax_read();
#endif  // ORPEXPANSION
	taddr = InternalMemory.T1Pointer_read();
	Params.Salinity=0;
	Params.ORP=0;

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



#if defined DisplayLEDPWM && ! defined RemoveAllLights
    // Restore PWM values
    PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
    PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

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

#if defined WDT || defined WDT_FORCE
	// enable watchdog timer for 1 second.  consider allowing this option to be configured.
	if ( wdtenabled ) wdt_enable(WDTO_1S);
#if defined(__AVR_ATmega2560__)
	wdt_enable(WDTO_1S);
#endif  // __AVR_ATmega2560__
#endif  // defined WDT || defined WDT_FORCE

#ifdef wifi
	EM = PWMEbit + RFEbit + AIbit + Salbit + ORPbit + IObit;
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
}

void ReefAngelClass::Refresh()
{
    pingSerial();
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
#ifdef DisplayLEDPWM
	boolean LightRelayOn=false;
	for (int l=0;l<8;l++)
	{
		if (LightsOnPorts & 1<<l)
			if (ReefAngel.Relay.RelayMaskOn & 1<<l) LightRelayOn=true;
	}
	if (PWM.LightsOverride) LightRelayOn=true;
	if (LightRelayOn)
	{
		PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
		PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
	}
	analogWrite(actinicPWMPin, PWM.GetActinicValue()*2.55);
    analogWrite(daylightPWMPin, PWM.GetDaylightValue()*2.55);
#endif  // DisplayLEDPWM

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
#endif  // RFEXPANSION
#ifdef AI_LED
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
	if (ds.read_bit()==0) return;  // ds for OneWire TempSensor
	now();
#ifdef DirectTempSensor
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp[T1_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT1);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp[T2_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT2);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp[T3_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT3);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.PH=analogRead(PHPin);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	Params.PH=constrain(Params.PH,100,1400);
	LCD.Clear(DefaultBGColor,0,0,1,1);
#if defined SALINITYEXPANSION
	Params.Salinity=Salinity.Read();
	Params.Salinity=map(Params.Salinity, 0, SalMax, 60, 350); // apply the calibration to the sensor reading
	// Salinity Compensation was contributed by ahmedess
	// http://forum.reefangel.com/viewtopic.php?p=7386#p7386
	if (Salinity.TemperatureCompensation)
	{
		double SalCompensation;
		if (TempSensor.unit && Params.Temp[T1_PROBE])
		SalCompensation=Params.Salinity/(1+((Params.Temp[T1_PROBE]-250)*0.0024));
		else
		SalCompensation=Params.Salinity/(1+((Params.Temp[T1_PROBE]-770)*0.001333));
		Params.Salinity=round(SalCompensation);
	}
	LCD.Clear(DefaultBGColor,0,0,1,1);
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
	Params.ORP=ORP.Read();
	if (Params.ORP!=0)
	{
		Params.ORP=map(Params.ORP, ORPMin, ORPMax, 0, 470); // apply the calibration to the sensor reading
		Params.ORP=constrain(Params.ORP,0,550);
	}
	LCD.Clear(DefaultBGColor,0,0,1,1);
#endif  // defined ORPEXPANSION
	TempSensor.RequestConversion();
	LCD.Clear(DefaultBGColor,0,0,1,1);
#else  // DirectTempSensor
    int x = TempSensor.ReadTemperature(TempSensor.addrT1);
    LCD.Clear(DefaultBGColor,0,0,1,1);
    int y;
    y = x - Params.Temp[T1_PROBE];
    // check to make sure the temp readings aren't beyond max allowed
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T1_PROBE] == 0 || ~x) Params.Temp[T1_PROBE] = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT2);
    LCD.Clear(DefaultBGColor,0,0,1,1);
    y = x - Params.Temp[T2_PROBE];
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T2_PROBE] == 0 || ~x) Params.Temp[T2_PROBE] = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT3);
    LCD.Clear(DefaultBGColor,0,0,1,1);
    y = x - Params.Temp[T3_PROBE];
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T3_PROBE] == 0 || ~x) Params.Temp[T3_PROBE] = x;
    Params.PH=0;
    for (int a=0;a<20;a++)
    {
    	Params.PH+=analogRead(PHPin);
    }
    Params.PH/=20;
    LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	Params.PH=constrain(Params.PH,100,1400);

#if defined SALINITYEXPANSION
	Params.Salinity=Salinity.Read();
	Params.Salinity=map(Params.Salinity, 0, SalMax, 60, 350); // apply the calibration to the sensor reading
	LCD.Clear(DefaultBGColor,0,0,1,1);
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
	Params.ORP=ORP.Read();
	if (Params.ORP!=0)
	{
		Params.ORP=map(Params.ORP, ORPMin, ORPMax, 0, 470); // apply the calibration to the sensor reading
		Params.ORP=constrain(Params.ORP,0,550);
	}
	LCD.Clear(DefaultBGColor,0,0,1,1);
#endif  // defined ORPEXPANSION
	TempSensor.RequestConversion();
#endif  // DirectTempSensor
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
	if (NumMins(OffHour,OffMinute) > NumMins(OnHour,OnMinute))
	{
		if (NumMins(hour(),minute()) >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay); else Relay.Off(LightsRelay);
		if (NumMins(hour(),minute()) >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay);
	}
	else
	{
		if (NumMins(hour(),minute()) >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay); else Relay.On(LightsRelay);
		if (NumMins(hour(),minute()) >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay);
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
#ifdef ENABLE_EXCEED_FLAGS
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
#ifdef ENABLE_EXCEED_FLAGS
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
	Relay.Set(DPRelay,((elapsedSecsToday(now())-(OffsetMinute*60))%(RepeatMinute*60))<RunTime);
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
    StandardATO(Relay, InternalMemory.ATOTimeout_read());
}

void ReefAngelClass::SingleATOLow(byte Relay)
{
    SingleATO(true, Relay, InternalMemory.ATOTimeout_read(), InternalMemory.ATOHourInterval_read());
}

void ReefAngelClass::SingleATOHigh(byte Relay)
{
    SingleATO(false, Relay, InternalMemory.ATOHighTimeout_read(), InternalMemory.ATOHighHourInterval_read());
}

void ReefAngelClass::StandardATOExtended(byte Relay)
{
    StandardATO(Relay, InternalMemory.ATOExtendedTimeout_read());
}

void ReefAngelClass::SingleATOLowExtended(byte Relay)
{
    SingleATO(true, Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.ATOHourInterval_read());
}

void ReefAngelClass::SingleATOHighExtended(byte Relay)
{
    SingleATO(false, Relay, InternalMemory.ATOHighExtendedTimeout_read(), InternalMemory.ATOHighHourInterval_read());
}

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

void ReefAngelClass::Wavemaker1(byte WMRelay)
{
	Wavemaker(WMRelay,InternalMemory.WM1Timer_read());
#ifdef WavemakerSetup
	WM1Port = WMRelay;
#endif
}

void ReefAngelClass::Wavemaker2(byte WMRelay)
{
	Wavemaker(WMRelay,InternalMemory.WM2Timer_read());
#ifdef WavemakerSetup
	WM2Port = WMRelay;
#endif
}

#ifdef VersionMenu
void ReefAngelClass::DisplayVersion()
{
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
}
#endif  // VersionMenu

void ReefAngelClass::ClearScreen(byte Color)
{
    // clears the entire screen
    LCD.Clear(Color, 0, 0, 131, 131);
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
}

void ReefAngelClass::Portal(char *username, char *key)
{
	if (Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,key);
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
	Relay.RelayMaskOff = ~FeedingModePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] = ~FeedingModePortsE[i];
	}
#endif  // RelayExp
	Relay.Write();
	ClearScreen(DefaultBGColor);
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 30, 10, "Feeding Mode");
	Timer[FEEDING_TIMER].Start();  //Start Feeding Mode timer
#ifdef DisplayImages
	LCD.DrawEEPromImage(40,50, 40, 30, I2CEEPROM2, I2CEEPROM2_Feeding);
#endif  // DisplayImages
#ifdef RFEXPANSION
	RF.SetMode(Feeding_Start,0,0);
#endif  // RFEXPANSION
	// Tell controller what mode we are in
	DisplayedMenu = FEEDING_MODE;
}

void ReefAngelClass::WaterChangeModeStart()
{
	// turn off ports
#ifdef SaveRelayState
	// TODO Test SaveRelayState
	CurrentRelayState = Relay.RelayData;
#endif  // SaveRelayState
	Relay.RelayMaskOff = ~WaterChangePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] = ~WaterChangePortsE[i];
	}
#endif  // RelayExp
	Relay.Write();
	ClearScreen(DefaultBGColor);
	// Display the water change mode
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 20, 10, "Water Change Mode");
#ifdef DisplayImages
	LCD.DrawEEPromImage(51,55, 40, 30, I2CEEPROM2, I2CEEPROM2_Water_Change);
#endif  // DisplayImages
	// Tell controller what mode we are in
	DisplayedMenu = WATERCHANGE_MODE;
}

void ReefAngelClass::ATOClear()
{
	LED.Off();
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(ATO_Single_Exceed_Flag, 0);
	InternalMemory.write(ATO_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS
	LowATO.StopTopping();
	HighATO.StopTopping();
}

void ReefAngelClass::OverheatClear()
{
	LED.Off();
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(Overheat_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS
	Relay.RelayMaskOff = B11111111;
#ifdef RelayExp
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] = B11111111;
	}
#endif  // RelayExp
	Relay.Write();
}

#ifdef CUSTOM_MENU
void ReefAngelClass::InitMenu(int ptr, byte qty)
{
    // loads all the menus
    menusptr[MainMenu] = ptr;
    menuqtysptr[MainMenu] = qty;
    // initialize menus
    PreviousMenu = DEFAULT_MENU;
    DisplayedMenu = DEFAULT_MENU;  // default menu to display
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
    DisplayedMenu = DEFAULT_MENU;  // default menu to display
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}
#endif  // CUSTOM_MENU

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

					// Clears the screen to draw the menu
					// Displays main menu, select first item, save existing menu
					ClearScreen(DefaultBGColor);
					SelectedMenuItem = DEFAULT_MENU_ITEM;
					PreviousMenu = DEFAULT_MENU;
					DisplayedMenu = MAIN_MENU;
					showmenu = true;
					redrawmenu = true;
					menutimeout = now();
#if defined WDT || defined WDT_FORCE
					wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
					// get out of this function and display the menu
					return;
				}

				if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
				{
					// Turn backlight on
					LCD.BacklightOn();
					Timer[LCD_TIMER].Start();
				}

				pingSerial();
#ifdef CUSTOM_MAIN
				DrawCustomMain();
#else
				// display everything on the home screen except the graph
				// the graph is drawn/updated when we exit the main menu & when the parameters are saved
				LCD.DrawDate(6, 112);
#if defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params, PWM.GetDaylightValue(), PWM.GetActinicValue());
#else  // defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params);
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
				byte TempRelay = Relay.RelayData;
				TempRelay &= Relay.RelayMaskOff;
				TempRelay |= Relay.RelayMaskOn;
				LCD.DrawOutletBox(12, 93, TempRelay);
#endif  // CUSTOM_MAIN

				pingSerial();

				// Process any checks/tests/events that can happen while displaying the home screen
				// This can be the timers for wavemakers or any overheat temperatures

				// process timers
				if ( Timer[STORE_PARAMS_TIMER].IsTriggered() )
				{
					int CurTemp;

					// Values are stored in the I2CEEPROM1
					taddr++;
					if ( taddr >= 120 ) taddr = 0;
					Timer[STORE_PARAMS_TIMER].Start();
					CurTemp = map(Params.Temp[T1_PROBE], T1LOW, T1HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.Clear(DefaultBGColor,0,0,1,1);
					Memory.Write(taddr, CurTemp);
					pingSerial();
					LCD.Clear(DefaultBGColor,0,0,1,1);
					CurTemp = map(Params.Temp[T2_PROBE], T2LOW, T2HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.Clear(DefaultBGColor,0,0,1,1);
					Memory.Write(taddr+120, CurTemp);
					pingSerial();
					LCD.Clear(DefaultBGColor,0,0,1,1);
					CurTemp = map(Params.Temp[T3_PROBE], T3LOW, T3HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.Clear(DefaultBGColor,0,0,1,1);
					Memory.Write(taddr+240, CurTemp);
					pingSerial();
					LCD.Clear(DefaultBGColor,0,0,1,1);
					CurTemp = map(Params.PH, PHLOW, PHHIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.Clear(DefaultBGColor,0,0,1,1);
					Memory.Write(taddr+360, CurTemp);
					pingSerial();
					LCD.Clear(DefaultBGColor,0,0,1,1);
					if ((taddr%10)==0) InternalMemory.T1Pointer_write(taddr);
#ifdef CUSTOM_MAIN
					DrawCustomGraph();
#else
					LCD.DrawGraph(5, 5);
#endif  // CUSTOM_MAIN
				}

				// if overheat probe exceeds the temp
				if ( Params.Temp[OverheatProbe] >= InternalMemory.OverheatTemp_read() )
				{
					LED.On();
#ifdef ENABLE_EXCEED_FLAGS
					InternalMemory.write(Overheat_Exceed_Flag, 1);
#endif  // ENABLE_EXCEED_FLAGS
					// invert the ports that are activated
					Relay.RelayMaskOff = ~OverheatShutoffPorts;
#ifdef RelayExp
					for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
					{
						Relay.RelayMaskOffE[i] = ~OverheatShutoffPortsE[i];
					}
#endif  // RelayExp
				}
				// commit relay changes
				Relay.Write();
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
					Relay.RelayMaskOff = B11111111;
					// Compare the delayed on ports with the previous port states
					Relay.RelayData &= ~(FeedingModePorts & DelayedOnPorts);
#ifdef RelayExp
					for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
					{
						Relay.RelayMaskOffE[i] = B11111111;
						Relay.RelayDataE[i] &= ~(FeedingModePortsE[i] & DelayedOnPortsE[i]);
					}
#endif  // RelayExp
#ifdef RFEXPANSION
					RF.SetMode(Feeding_Stop,0,0);
#endif  // RFEXPANSION
					ExitMenu();
				}
				Relay.Write();
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
					Relay.RelayMaskOff = B11111111;
					// Compare the delayed on ports with the previous port states
					Relay.RelayData &= ~(WaterChangePorts & DelayedOnPorts);
#ifdef RelayExp
					for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
					{
						Relay.RelayMaskOffE[i] = B11111111;
						Relay.RelayDataE[i] &= ~(WaterChangePortsE[i] & DelayedOnPortsE[i]);
					}
#endif  // RelayExp
					ExitMenu();
				}
				Relay.Write();
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
				DisplayedMenu = MAIN_MENU;
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

void ReefAngelClass::DisplayMenu()
{
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
        DisplayedMenu = EXCEED_TIMEOUT_MENU;
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
}

void ReefAngelClass::DisplayMenuHeading()
{
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
                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
        case SetupMenu:
            {
                ptr += strlen(Menu_0_label) + 1;
                strcpy_P(buffer, (char*)ptr);
            }
            break;
#ifndef RemoveAllLights
        case LightsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // RemoveAllLights
        case TempsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + strlen(Menu_3_label) + 3;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
    }  // switch MenuNum

    // clear the line that has the menu heading on it
    LCD.Clear(DefaultBGColor, MENU_START_COL, MENU_START_ROW, MAX_X, MAX_Y);
    // Display the menu heading
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, buffer);
}

void ReefAngelClass::DisplayMenuEntry(char *text)
{
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
}

void ReefAngelClass::ExitMenu()
{
	// Handles the cleanup to return to the main screen
	ClearScreen(DefaultBGColor);
	Timer[LCD_TIMER].Start();
	DisplayedMenu = DEFAULT_MENU;
#ifdef CUSTOM_MAIN
	DrawCustomGraph();
#else
	LCD.DrawGraph(5, 5);
#endif  // CUSTOM_MAIN
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
			SetupCalibratePH();
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
#ifdef DateTimeSetup
		case MainMenu_DateTime:
		{
			SetupDateTime();
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
            DisplayedMenu = LightsMenu;
            break;
        }
#endif  // RemoveAllLights
        case MainMenu_Temps:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = TempsMenu;
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case MainMenu_Timeouts:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = TimeoutsMenu;
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
        case MainMenu_Setup:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = SetupMenu;
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
                Relay.On(WM1Port);
                Relay.On(WM2Port);
                Relay.Write();
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
            SetupCalibratePH();
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
#ifdef DateTimeSetup
        case SetupMenu_DateTime:
        {
            SetupDateTime();
            break;
        }
#endif  // DateTimeSetup
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
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
            // turn on ports
            Relay.RelayMaskOn = LightsOnPorts;
#ifdef RelayExp
			for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
			{
				Relay.RelayMaskOnE[i] = LightsOnPortsE[i];
			}
#endif  // RelayExp
#ifdef DisplayLEDPWM
            PWM.LightsOverride=true;
#endif  // DisplayLEDPWM
            Relay.Write();
            DisplayMenuEntry("Lights On");
            showmenu = false;
            break;
        }
        case LightsMenu_Off:
        {
            // reset ports
            Relay.RelayMaskOn = B00000000;
#ifdef RelayExp
			for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
			{
				Relay.RelayMaskOnE[i] = B00000000;
			}
#endif  // RelayExp
#ifdef DisplayLEDPWM
			// TODO should possibly store the PWM value to be reset instead of turning off completely
            // sets PWM to 0%
            PWM.SetActinic(0);
            PWM.SetDaylight(0);
            PWM.LightsOverride=false;
#endif  // DisplayLEDPWM
            Relay.Write();
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
#ifdef DisplayLEDPWM
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
#endif  // DisplayLEDPWM
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
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
            DisplayedMenu = PreviousMenu;
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
            DisplayedMenu = PreviousMenu;
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
                if ( v > rangemax )
                {
                    v = rangemin;
                }
            }
            else if ( sel == OPT2 )
            {
                y++;
                if ( y > rangemax )
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
                if ( v < rangemin )
                {
                    v = rangemax;
                }
            }
            else if ( sel == OPT2 )
            {
                y--;
                if ( y < rangemin )
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
    if ( bMetalHalide )
    {
        strcpy(msg, "Metal Halide Setup");
        h1 = InternalMemory.MHOnHour_read();
        m1 = InternalMemory.MHOnMinute_read();
        h2 = InternalMemory.MHOffHour_read();
        m2 = InternalMemory.MHOffMinute_read();
    }
    else
    {
        strcpy(msg, "Std Lights Setup");
        h1 = InternalMemory.StdLightsOnHour_read();
        m1 = InternalMemory.StdLightsOnMinute_read();
        h2 = InternalMemory.StdLightsOffHour_read();
        m2 = InternalMemory.StdLightsOffMinute_read();
    }
    ClearScreen(DefaultBGColor);
    // header / title
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, msg);
    // prefixes, draw in ':' between options
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*4, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, "Off:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*6, ":");
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
                    // draw the second line items before the first line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(h1, 1, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT2:
                {
                    // draw the second line items before the first line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(m1, 1, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT3:
                {
                    // draw the first line items before the second line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(h2, 1, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT4:
                {
                    // draw the first line items before the second line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                    LCD.DrawOption(m2, 1, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
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
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*6, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*6, "", "");
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
            InternalMemory.MHOnHour_write(h1);
            InternalMemory.MHOnMinute_write(m1);
            InternalMemory.MHOffHour_write(h2);
            InternalMemory.MHOffMinute_write(m2);
        }
        else
        {
            InternalMemory.StdLightsOnHour_write(h1);
            InternalMemory.StdLightsOnMinute_write(m1);
            InternalMemory.StdLightsOffHour_write(h2);
            InternalMemory.StdLightsOffMinute_write(m2);
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
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL + 18, MENU_START_ROW*5, (int)iCal[b]);
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
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate Salinity");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "35 PPT");
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
    } while ( ! bDone );

    if ( bSave )
    {
        // save SalMax to memory
        InternalMemory.SalMax_write(iS);
		SalMax = iS;
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
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate ORP");
		char text[10];
		itoa(iCal[b],text,10);
		strcat(text , " mV  ");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, text);
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

#ifdef DateTimeSetup
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

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }
}
#endif  // DateTimeSetup

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


ReefAngelClass ReefAngel = ReefAngelClass() ;
