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

#define ReefAngel_Version "1.0.9"

#include <Globals.h>
#include <InternalEEPROM.h>  // NOTE read/write internal memory
#include <Time.h>
#include <RA_ATO.h>
#include <LED.h>
#include <RA_TempSensor.h>
#include <Relay.h>
#include <RA_PWM.h>
#include <Timer.h>
#include <Memory.h>
#include <DCPump.h>
#if defined ORPEXPANSION
#include <ORP.h>
#endif  // defined ORPEXPANSION
#if defined SALINITYEXPANSION
#include <Salinity.h>
#endif  // defined SALINITYEXPANSION
#if defined PHEXPANSION
#include <PH.h>
#endif  // defined PHEXPANSION
#if defined WATERLEVELEXPANSION
#include <WaterLevel.h>
#endif  // defined WATERLEVELEXPANSION
#if defined RFEXPANSION
#include <RF.h>
#endif  // defined RFEXPANSION
#if defined AI_LED
#include <AI.h>
#endif  // defined AI_LED
#if defined IOEXPANSION
#include <IO.h>
#endif  // defined IOEXPANSION
#if defined HUMIDITYEXPANSION
#include <Humidity.h>
#endif  // defined HUMIDITYEXPANSION

#ifdef RA_STANDARD
#include <Standard/includes.h>
#elif defined RA_PLUS
#include <Plus/includes.h>
#elif defined RA_STAR
#include <Star/includes.h>
#elif defined RA_TOUCH || defined RA_TOUCHDISPLAY
#include <Touch/includes.h>
#elif defined RA_EVOLUTION
#include <Evolution/includes.h>
#endif //  RA_STANDARD

class ReefAngelClass
{

public:
	int PHMin,PHMax;
	ParamsStruct Params;
	byte Flags,AlertFlags,StatusFlags;
	bool BusLocked;

	ReefAngelClass();

#ifdef RA_STANDARD
#include <Standard/public.h>
#elif defined RA_PLUS
#include <Plus/public.h>
#elif defined RA_STAR
#include <Star/public.h>
#elif defined RA_TOUCH || defined RA_TOUCHDISPLAY
#include <Touch/public.h>
#elif defined RA_EVOLUTION
#include <Evolution/public.h>
#endif //  RA_STANDARD

	LEDClass LED;
	RA_ATOHighClass HighATO;
	RA_ATOLowClass LowATO;
	RA_TempSensorClass TempSensor;
	RelayClass Relay;
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	RA_PWMClass PWM;
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
#ifdef DCPUMPCONTROL
	DCPumpClass DCPump;
#endif  // DCPUMPCONTROL
#if defined ORPEXPANSION
	int ORPMin, ORPMax;
	ORPClass ORP;
#endif  // ORPEXPANSION
#if defined SALINITYEXPANSION
	int SalMax;
	SalinityClass Salinity;
#endif  // defined SALINITYEXPANSION
#if defined PHEXPANSION
	int PHExpMin, PHExpMax;
	PHClass PH;
#endif  // PHEXPANSION	
#if defined WATERLEVELEXPANSION
	WaterLevelClass WaterLevel;
	RA_ATOWLClass WLATO;
#endif  // WATERLEVELEXPANSION	
#if defined RFEXPANSION
	RFClass RF;
#endif  // defined RFEXPANSION
#if defined AI_LED
	AIClass AI;
#endif  // defined AI_LED
#if defined IOEXPANSION
	IOClass IO;
#endif  // defined IOEXPANSION
#if defined HUMIDITYEXPANSION
	HumidityClass Humidity;
#endif  // defined HUMIDITYEXPANSION
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
	byte DisplayedMenu;
	bool showmenu;

	// Ports to toggle during different modes
	byte FeedingModePorts;
	byte WaterChangePorts;
	byte OverheatShutoffPorts;
	byte LightsOnPorts;
#ifdef LEAKDETECTOREXPANSION
	byte LeakShutoffPorts;
#endif  // LEAKDETECTOREXPANSION
#ifdef OVERRIDE_PORTS
	byte OverridePorts;
#endif
	byte EM,EM1;
	byte REM;

	/*
	EM Bits
	Bit 0 - PWMEbit
	Bit 1 - RFEbit
	Bit 2 - AIbit
	Bit 3 - Salbit
	Bit 4 - ORPbit
	Bit 5 - IObit
	Bit 6 - PHbit
	Bit 7 - WLbit

	EM1 Bits
	Bit 0 - HUMbit
	 */

#ifdef RelayExp
	// Expansion Module ports
	byte FeedingModePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte WaterChangePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte OverheatShutoffPortsE[MAX_RELAY_EXPANSION_MODULES];
	byte LightsOnPortsE[MAX_RELAY_EXPANSION_MODULES];
#ifdef LEAKDETECTOREXPANSION
	byte LeakShutoffPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // LEAKDETECTOREXPANSION
#ifdef OVERRIDE_PORTS
	byte OverridePortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // OVERRIDE_PORTS
#endif  // RelayExp

	byte ChangeMode;
	byte OverheatProbe;
	byte TempProbe;

	time_t Overheatmillis;
#ifdef LEAKDETECTOREXPANSION
	time_t Leakmillis;
#endif  // LEAKDETECTOREXPANSION

	void Init();
	void Refresh();
	void SetTemperatureUnit(byte unit);
	void ConvertTempUnit();
	void FeedingModeStart();
	void WaterChangeModeStart();
	void ATOClear();
	void OverheatCheck();
	void OverheatClear();
	void LightsOn();
	void LightsOff();
	void RefreshScreen();
	void SetupCalibratePH();
	void SetupCalibrateChoicePH();
	void ClearScreen(byte Color);
	void ExitMenu();
	void SetDisplayedMenu(byte value);
	void WDTReset();
	void CheckDrawGraph();
	void CheckFeedingDrawing();
	void CheckWaterChangeDrawing();

#ifdef CUSTOM_VARIABLES
	byte CustomVar[8];
#endif //CUSTOM_VARIABLES

#ifdef TEMP_MEMORY
	byte TempMem[8];
#endif //TEMP_MEMORY

#ifdef I2CMASTER
	byte I2CCommand;
	void UpdateTouchDisplay();
#endif // I2CMASTER

	void inline AddStandardMenu() {};
	void inline AddWifi() {};
	void inline AddDateTimeMenu() {};
	void inline AddRFExpansion() {};
	void inline AddCustomColors() {};
	void inline AddBusCheck() {};
	void inline AddPortOverrides() {};
	void inline Display24h() {};
	void inline UseFlexiblePhCalibration() {};
	void inline ReverseATOLow() {};
	void inline ReverseATOHigh() {};
	void inline Mini() {}; // deprecated
	void inline Touch() {};
	void inline TouchDisplay() {};
	void inline NoWifi() {};
	void inline NoSD() {};
	void inline NoTilt() {};
	void inline Star() {};

#ifdef LEAKDETECTOREXPANSION
	boolean IsLeakDetected();
	void LeakCheck();
	void LeakClear();
#endif  // LEAKDETECTOREXPANSION

	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void CO2Control(byte CO2Relay, int LowPH, int HighPH);
	void PHControl(byte PHControlRelay, int LowPH, int HighPH);
	void StandardATO(byte ATORelay, int ATOTimeout);
#ifdef WATERLEVELEXPANSION	
	void WaterLevelATO(byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel);
#endif  // WATERLEVELEXPANSION	
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
#ifdef WATERLEVELEXPANSION
	void WaterLevelATO(byte Relay);
#endif  // WATERLEVELEXPANSION
	void DosingPump1(byte Relay);
	void DosingPump2(byte Relay);
	void DosingPumpRepeat1(byte Relay);
	void DosingPumpRepeat2(byte Relay);
	void DosingPumpRepeat3(byte Relay);
	void Wavemaker1(byte WMRelay);
	void Wavemaker2(byte WMRelay);
#ifdef VersionMenu
	void DisplayVersion();
#endif  // VersionMenu

	// WebBanner
#ifdef wifi
	void LoadWebBanner(int pointer, byte qty);
	void WebBanner();
	void Portal(char *username);
	void Portal(char *username, char*key);
	void SendPortal(char *username, char*key);
	char *portalusername;
#endif  // wifi

private:
	time_t menutimeout;
	byte taddr;

	// Nested Menu variables
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
	byte PreviousMenu;
	bool redrawmenu;

};

extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif  // __REEFANGEL_H__

