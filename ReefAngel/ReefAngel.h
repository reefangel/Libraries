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

#ifndef	__REEFANGEL_H__
#define __REEFANGEL_H__

#define ReefAngel_Version "0.9.7"

#include <Globals.h>
#include <InternalEEPROM.h>  // NOTE read/write internal memory
#include <Time.h>
#include <RA_NokiaLCD.h>
#include <RA_ATO.h>
#include <RA_Joystick.h>
#include <LED.h>
#include <RA_TempSensor.h>
#include <Relay.h>
#include <RA_PWM.h>
#include <Timer.h>
#include <Memory.h>

#if defined SALINITYEXPANSION
	#include <Salinity.h>
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
	#include <ORP.h>
#endif  // defined ORPEXPANSION
#if defined RFEXPANSION
	#include <RF.h>
#endif  // defined RFEXPANSION
#if defined AI_LED
	#include <AI.h>
#endif  // defined AI_LED
#if defined IOEXPANSION
	#include <IO.h>
#endif  // defined IOEXPANSION

#include <avr/pgmspace.h>

// Watchdog Timers
#if defined WDT || defined WDT_FORCE
#include <avr/wdt.h>
#endif  // defined WDT || defined WDT_FORCE

// WDT overrides WDT_FORCE
#if defined WDT
	#define wdtenabled pgm_read_word(0x7ffe)==0x400
#elif defined WDT_FORCE
	#define wdtenabled 1
#endif  // WDT


static unsigned long ButtonDebounce;
static unsigned long RAStart;


class ReefAngelClass
{

public:
	int PHMin,PHMax;
	ParamsStruct Params;
	ReefAngelClass();
	RA_NokiaLCD LCD;
	RA_JoystickClass Joystick;
	LEDClass LED;
	DS1307RTC RTC;
	RA_ATOHighClass HighATO;
	RA_ATOLowClass LowATO;
	RA_TempSensorClass TempSensor;
	RelayClass Relay;
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	RA_PWMClass PWM;
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
#if defined SALINITYEXPANSION
	int SalMax;
	SalinityClass Salinity;
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
	int ORPMin, ORPMax;
	ORPClass ORP;
#endif  // ORPEXPANSION IOEXPANSION
#if defined RFEXPANSION
	RFClass RF;
#endif  // defined RFEXPANSION
#if defined AI_LED
	AIClass AI;
#endif  // defined AI_LED
#if defined IOEXPANSION
	IOClass IO;
#endif  // defined IOEXPANSION

	/*
	Timers:
	0 - Feeding Mode timer
	1 - Wavemaker 1 or Dosing Pump 1
	2 - Wavemaker 2 or Dosing Pump 2
	3 - Backlight timer / Sleep timer
	4 - Portal
	5 - Store params to eeprom
	*/
	TimerClass Timer[6];
	byte SelectedMenuItem;

	// Ports to toggle during different modes
	byte FeedingModePorts;
	byte WaterChangePorts;
	byte OverheatShutoffPorts;
#ifdef RelayExp
	// Expansion Module ports
	byte FeedingModePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte WaterChangePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte OverheatShutoffPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp
#ifndef RemoveAllLights
	byte LightsOnPorts;
#ifdef RelayExp
	byte LightsOnPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp
#endif  // RemoveAllLights
#ifdef WavemakerSetup
	// TODO find a better way to save the wavemaker ports for restarting once timers are updated from setup screen
	byte WM1Port;
	byte WM2Port;
#endif  // WavemakerSetup

	byte OverheatProbe;
	byte TempProbe;

	void Init();
	void Refresh();
	void SetTemperatureUnit(byte unit);
	void ConvertTempUnit();
	void inline AddStandardMenu() {};
	void inline AddWifi() {};
	void inline AddDateTimeMenu() {};
	void inline AddRFExpansion() {};
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void CO2Control(byte CO2Relay, int LowPH, int HighPH);
	void PHControl(byte PHControlRelay, int LowPH, int HighPH);
	void StandardATO(byte ATORelay, int ATOTimeout);
	void SingleATO(bool bLow, byte ATORelay, int intTimeout, byte byteHrInterval);
	void DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, int RunTime);
	void DosingPump(byte DPRelay, byte OnHour, byte OnMinute, int RunTime);
	void DosingPumpRepeat(byte DPRelay, int OffsetMinute, int RepeatMinute, int RunTime);
	void Wavemaker(byte WMRelay, int WMTimer);
	void WavemakerRandom(byte WMRelay, int MinWMTimer, int MaxWMTimer);
	void WavemakerRandom1(byte WMRelay, int MinWMTimer, int MaxWMTimer);
	void WavemakerRandom2(byte WMRelay, int MinWMTimer, int MaxWMTimer);
	void WavemakerToggle(byte WMRelay1, byte WMRelay2, int WMTimer);

	// Simplified PDE versions of the calls
	void StandardLights(byte Relay);
	void StandardLights(byte Relay, byte MinuteOffset);
	void DayLights(byte Relay);
	void ActinicLights(byte Relay);
	void DelayedStartLights(byte Relay);
	void MoonLights(byte Relay);
	void MHLights(byte Relay);
	void StandardHeater(byte Relay);
	void StandardFan(byte Relay);
	void StandardATO(byte Relay);
	void CO2Control(byte Relay);
	void PHControl(byte Relay);
	void SingleATOLow(byte Relay);
	void SingleATOHigh(byte Relay);
	void StandardATOExtended(byte Relay);
	void SingleATOLowExtended(byte Relay);
	void SingleATOHighExtended(byte Relay);
	void DosingPump1(byte Relay);
	void DosingPump2(byte Relay);
	void DosingPumpRepeat1(byte Relay);
	void DosingPumpRepeat2(byte Relay);
	void Wavemaker1(byte WMRelay);
	void Wavemaker2(byte WMRelay);
#ifdef VersionMenu
	void DisplayVersion();
#endif  // VersionMenu
	void ClearScreen(byte Color);
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	void MoonlightPWM(byte RelayID, bool ShowPWM);
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

	// WebBanner
#ifdef wifi
	void LoadWebBanner(int pointer, byte qty);
	void WebBanner();
	void Portal(char *username);
	void Portal(char *username, char*key);
	void SendPortal(char *username, char*key);
	byte EM;
	byte REM;
#endif  // wifi

	void FeedingModeStart();
	void WaterChangeModeStart();
	void ATOClear();
	void OverheatClear();

    // Nested Menu Functions
#ifdef CUSTOM_MENU
	void InitMenu(int ptr, byte qty);
#else
    void InitMenus();
#endif  // CUSTOM_MENU
    void ShowInterface();
    void DisplayMenu();
    void DisplayMenuHeading();
    void DisplayMenuEntry(char *text);
    void ExitMenu();
    void ProcessButtonPress();
#ifdef CUSTOM_MENU
	void ProcessButtonPressCustom();
#else
    void ProcessButtonPressMain();

#ifndef SIMPLE_MENU
    void ProcessButtonPressSetup();
#ifndef RemoveAllLights
    void ProcessButtonPressLights();
#endif  // RemoveAllLights
    void ProcessButtonPressTemps();
#if defined SetupExtras || defined ATOSetup
    void ProcessButtonPressTimeouts();
#endif  // if defined SetupExtras || defined ATOSetup

    // Setup Screens
    bool SetupOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                     char* unit, char* subunit, char* title,
                     char* prefix1, char* prefix2);
    void SetupLightsOptionDisplay(bool bMetalHalide);
#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU

    void SetupCalibratePH();
#if defined SALINITYEXPANSION
    void SetupCalibrateSalinity();
#endif  // defined SALINITYEXPANSION
#if defined ORPEXPANSION
    void SetupCalibrateORP();
#endif  // defined ORPEXPANSION
#ifdef DateTimeSetup
    void SetupDateTime();
#endif  // DateTimeSetup
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
#ifdef DosingPumpSetup
    void SetupDosingPump();
#endif  // DosingPumpSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU

#ifdef CUSTOM_VARIABLES
    byte CustomVar[8];
#endif //CUSTOM_VARIABLES

private:
	bool showmenu;
	time_t menutimeout;
	byte taddr;

	// Nested Menu variables
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
public:
	byte DisplayedMenu;
private:
	byte PreviousMenu;
	bool redrawmenu;

//#ifdef wifi
//	// WebBanner variables
//	int webbannerpointer;
//	byte webbannerqty;
//#endif  // wifi

#ifdef SaveRelayState
	byte CurrentRelayState;
#endif  // SaveRelayState

};

#ifdef CUSTOM_MAIN
void DrawCustomMain();
void DrawCustomGraph();
#endif  // CUSTOM_MAIN

#ifdef CUSTOM_MENU
void MenuEntry1();
#if CUSTOM_MENU_ENTRIES >= 2
void MenuEntry2();
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
void MenuEntry3();
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
void MenuEntry4();
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
void MenuEntry5();
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
void MenuEntry6();
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
void MenuEntry7();
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
void MenuEntry8();
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
void MenuEntry9();
#endif  // CUSTOM_MENU_ENTRIES >= 9
#endif  // CUSTOM_MENU

extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif  // __REEFANGEL_H__

