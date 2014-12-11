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

#define ReefAngel_Version "1.1.1"

#include <Globals.h>
#include <InternalEEPROM.h>  // NOTE read/write internal memory
#include <Time.h>
#include <RA_ATO.h>
#include <LED.h>
#include <RA_TempSensor.h>
#include <Relay.h>
#ifdef SC16IS750
#include <RA_SC16IS750.h>
#endif // SC16IS750
#ifdef DisplayLEDPWM
#include <RA_PWM.h>
#endif  // DisplayLEDPWM
#include <Timer.h>
#include <Memory.h>
#ifdef DCPUMPCONTROL
#include <DCPump.h>
#endif  // DCPUMPCONTROL
#include <DS1307RTC.h>
#if defined wifi || defined RA_STAR
#include <RA_Wifi.h>
#endif  // wifi
#if defined ORPEXPANSION
#include <ORP.h>
#endif  // defined ORPEXPANSION
#if defined SALINITYEXPANSION
#include <Salinity.h>
#endif  // defined SALINITYEXPANSION
#if defined PHEXPANSION
#include <PH.h>
#endif  // defined PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
#include <WaterLevel.h>
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
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
#if defined PAREXPANSION
#include <PAR.h>
#endif  // defined PAREXPANSION

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
	byte Board;
	int PHMin,PHMax;
	ParamsStruct Params;
	byte AlertFlags,StatusFlags;
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
#ifdef KALKDOSER
	RA_KalkDoserClass KWDoser;
#endif //  KALKDOSER
	RA_TempSensorClass TempSensor;
#ifndef SC16IS750
  RelayClass Relay;
#else
  RA_SC16IS750 Relay;
#endif // SC16IS750
#ifdef ETH_WIZ5100
	RA_Wiznet5100 Network;
#elif defined wifi
  RA_Wifi Network;
#endif // ETH_WIZ5100

#if defined DisplayLEDPWM && ! defined RemoveAllLights || defined DCPUMPCONTROL
#if defined(__SAM3X8E__)
	RA_PWMClass VariableControl;
#else // __SAM3X8E__
	RA_PWMClass PWM;
#endif // __SAM3X8E__
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
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	WaterLevelClass WaterLevel;
	RA_ATOWLClass WLATO;
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
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
#if defined PAREXPANSION
	PARClass PAR;
#endif  // defined PAREXPANSION
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
	boolean Splash;
#ifdef MAIN_2014
	byte MenuItem_2014;
	String CustomLabels[72];
	void InitCustomLabels();
	void Draw2014Main();
#endif // MAIN_2014

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
	Bit 1 - DCPumpbit
	Bit 2 - Leakbit
	Bit 3 - PARbit
	Bit 4 - SCPWMbit
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
	void StartSetupCalibrateChoicePH();
	inline void SetupCalibratePH() { StartSetupCalibrateChoicePH(); }
	void DisplaySetupCalibrateChoicePH();
	void ClearScreen(byte Color);
	void ExitMenu();
	void SetDisplayedMenu(byte value);
	void WDTReset();
	void CheckDrawGraph();
	void CheckFeedingDrawing();
	void CheckWaterChangeDrawing();
#ifdef DCPUMPCONTROL
	void SetDCPumpChannels(byte SyncSpeed,byte AntiSyncSpeed);
#endif //DCPUMPCONTROL

#ifdef CUSTOM_VARIABLES
	byte CustomVar[8];
#endif //CUSTOM_VARIABLES

#ifdef I2CMASTER
#define MASTERARRAYSIZE	100
	byte olddata[MASTERARRAYSIZE];
	byte I2CCommand;
	void UpdateTouchDisplay();
	void MasterWrite(int value, byte index);
	unsigned long lastmasterupdate;
#endif // I2CMASTER

	void inline Use2014Screen() {};
	void inline AddSalinityExpansion() {};
	void inline AddORPExpansion() {};
	void inline AddPHExpansion() {};
	void inline AddWaterLevelExpansion() {};
	void inline AddMultiChannelWaterLevelExpansion() {};
	void inline AddHumidityExpansion() {};
	void inline AddPARExpansion() {};
	void inline AddStandardMenu() {};
	void inline AddWifi() {};
	void inline AddRANet() {};
	void inline AddDateTimeMenu() {};
	void inline AddRFExpansion() {};
	void inline Add16ChPWM() {};
	void inline AddCustomColors() {};
	void inline AddBusCheck() {};
	void inline AddPortOverrides() {};
	void inline AddSPILCD() {};
	void inline AddExtraTempProbes() {};
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
	void inline Evolution() {};
	void inline ChangeWifiPort() {};
	
#ifdef BUSCHECK
	boolean isBusLock();
#endif //BUSCHECK

#ifdef LEAKDETECTOREXPANSION
#ifdef RA_TOUCHDISPLAY
	boolean LeakStatus;
#endif // RA_TOUCHDISPLAY
	boolean IsLeakDetected();
	void LeakCheck();
	void LeakClear();
	boolean isLeak();
#endif  // LEAKDETECTOREXPANSION

	boolean isATOTimeOut();
	boolean isOverheat();
	
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void CO2Control(byte CO2Relay, int LowPH, int HighPH);
	void CO2Control(byte CO2Relay, int LowPH, int HighPH, bool useExp);
	void PHControl(byte PHControlRelay, int LowPH, int HighPH);
	void PHControl(byte PHControlRelay, int LowPH, int HighPH, bool useExp);
	void StandardATO(byte ATORelay, int ATOTimeout);
#ifdef MULTIWATERLEVELEXPANSION
	void WaterLevelATO(byte Channel, byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel);
	inline void WaterLevelATO(byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel) { WaterLevelATO(0, ATORelay, ATOTimeout, LowLevel, HighLevel); };
#else
	void WaterLevelATO(byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel);
#endif  // MULTIWATERLEVELEXPANSION
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
	void MHLights(byte Relay, byte MinuteOffset);
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
#ifdef KALKDOSER
	void KalkDoser(byte KalkRelay, int LowPH, int TimeoutSeconds, byte MinuteInterval = 0);
#endif //  KALKDOSER
#ifdef MULTIWATERLEVELEXPANSION
	void WaterLevelATO(byte Channel, byte Relay);
#else
	void WaterLevelATO(byte Relay);
#endif  // MULTIWATERLEVELEXPANSION
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

#if defined wifi || defined RA_STAR
	void Portal(char *username);
	void Portal(char *username, char *key);
	void DDNS(char *subdomain);
#endif

private:
	time_t menutimeout;
	byte taddr;

	// Nested Menu variables
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
	byte PreviousMenu;
	bool redrawmenu;
	void CheckOffset(byte &x, byte &y);
        
        /* Constants defined in ReefAngel.cpp */
        static const byte PH_MAXIMUM_RANGE[2];
        static const byte PH_DEFAULT_RANGE[2];
        static const char PH_SETUP_MENU_LABEL[2][19];
        static const char PH_SETUP_MENU_STEP[2][13];
        
        enum {SETUP_PH, SETUP_CANCEL, SETUP_OK};
        
	unsigned int ph_target_range[2];
	unsigned int ph_read_range[2];
	unsigned int salinity_read;
        
	byte setup_option;
	byte setup_step;
	
	bool setup_input_select;
	bool setup_input_render;
	bool setup_screen_refresh;
	bool setup_save;
	
    #if defined DateTimeSetup
    enum{
        SETUP_DATETIME_MONTH, 
        SETUP_DATETIME_DAY, 
        SETUP_DATETIME_YEAR, 
        SETUP_DATETIME_HOUR, 
        SETUP_DATETIME_MINUTE, 
        #if !defined DATETIME24
        SETUP_DATETIME_PERIOD, 
        #endif//DATETIME24
        SETUP_DATETIME_CANCEL,
        SETUP_DATETIME_OK
    };

    struct{
        byte month;
        byte day;
        int year;
        byte hour;
        byte minute;
        #if !defined DATETIME24
        char *period;
        #endif//DATETIME24
    } currentDateTime;

    byte lastDayOfEachMonth[12];
    #endif//DateTimeSetup

};

extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif  // __REEFANGEL_H__

