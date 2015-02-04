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

byte ButtonPress = 0;

#if defined DisplayLEDPWM && ! defined RemoveAllLights || defined DCPUMPCONTROL
boolean LightsOverride=true;
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights

#ifdef RA_STANDARD
#include <Standard/instance.h>
#elif defined RA_PLUS
#include <Plus/instance.h>
#elif defined RA_STAR
#include <Star/instance.h>
#elif defined RA_TOUCH || defined RA_TOUCHDISPLAY
#include <Touch/instance.h>
#elif defined RA_EVOLUTION
#include <Evolution/instance.h>
#endif //  RA_STANDARD

/* Constants declare in ReefAngel.h */
const byte ReefAngelClass::PH_MAXIMUM_RANGE[2]={4, 10};
const byte ReefAngelClass::PH_DEFAULT_RANGE[2]={7, 10};
const char ReefAngelClass::PH_SETUP_MENU_LABEL[2][19]={"Calibrate pH", "Calibrate pH(Exp.)"};
const char ReefAngelClass::PH_SETUP_MENU_STEP[2][13]={"First value", "Second value"};


void ReefAngelClass::Init()
{
	Serial.begin(57600);
	Serial.setTimeout(100);
	while (!Serial) {
	; // wait for serial port to connect. Needed for Leonardo only
	}
#ifdef RA_STANDARD
#include <Standard/init.h>
#elif defined RA_PLUS
#include <Plus/init.h>
#elif defined RA_STAR
#include <Star/init.h>
#elif defined RA_TOUCH || defined RA_TOUCHDISPLAY
#include <Touch/init.h>
#elif defined RA_EVOLUTION
#include <Evolution/init.h>
#endif //  RA_STANDARD

	pinMode(lowATOPin,INPUT);
	pinMode(highATOPin,INPUT);
	digitalWrite(lowATOPin,HIGH); //pull up resistor on lowATOPin
	digitalWrite(highATOPin,HIGH); //pull up resistor on highATOPin
#if defined DisplayLEDPWM
	pinMode(actinicPWMPin,OUTPUT);
	pinMode(daylightPWMPin,OUTPUT);
	digitalWrite(actinicPWMPin,LOW); //pull down resistor on actinicPWMPin
	digitalWrite(daylightPWMPin,LOW); //pull down resistor on daylightPWMPin
#endif // DisplayLEDPWM
	digitalWrite(0,HIGH); //pull up resistor on RX
	digitalWrite(1,HIGH); //pull up resistor on TX
#ifdef __AVR_ATmega2560__
	digitalWrite(14,HIGH); //pull up resistor on TX3
	digitalWrite(15,HIGH); //pull up resistor on RX3
	digitalWrite(16,HIGH); //pull up resistor on TX2
	digitalWrite(17,HIGH); //pull up resistor on RX2
	digitalWrite(18,HIGH); //pull up resistor on TX1
	digitalWrite(19,HIGH); //pull up resistor on RX1
#endif // __AVR_ATmega2560__
	TempSensor.Init();
	RAStart=now();
	LastStart = RAStart;  // Set the time normal mode is started
	BusLocked=false;  // Bus is not locked
	ChangeMode=0;
	AlertFlags = 0;
	StatusFlags = 0;
	Splash=true;
	Relay.AllOff();
	OverheatProbe = T2_PROBE;
	TempProbe = T1_PROBE;
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

	// Set the default ports to be turned on & off during the 2 modes
	FeedingModePorts = 0;
	WaterChangePorts = 0;
	// Set the ports that get shutoff when the overheat value is reached
	OverheatShutoffPorts = 0;
	// DelayedOn ports, do not manually modify this variable, let the DelayedOn function modify it
	DelayedOnPorts = 0;
	// Set the ports that get turned on when you select the Lights On
	LightsOnPorts = 0;
#ifdef OVERRIDE_PORTS
	// Override all relay masks for the following ports
	OverridePorts = 0;
#endif // OVERRIDE_PORTS

#ifdef RelayExp
	// Expansion Module ports to toggle, defaults to not toggle any ports
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		FeedingModePortsE[i] = 0;
		WaterChangePortsE[i] = 0;
		OverheatShutoffPortsE[i] = 0;
		DelayedOnPortsE[i] = 0;
		LightsOnPortsE[i] = 0;
#ifdef OVERRIDE_PORTS
		// Override all relay masks for the following ports
		OverridePortsE[i] = 0;
#endif // OVERRIDE_PORTS
	}
#endif  // RelayExp

#if defined wifi || defined I2CMASTER || defined ETH_WIZ5100
	EM = PWMEbit + RFEbit + AIbit + Salbit + ORPbit + IObit + PHbit + WLbit;
	EM1 = HUMbit + DCPumpbit + Leakbit + PARbit + SCPWMbit;

#ifdef RelayExp
	for (byte a=0;a<InstalledRelayExpansionModules;a++)
	{
		REM+=1<<a;
	}
#else  // RelayExp
	REM = 0;
#endif  // RelayExp
#endif  // wifi || I2CMASTER
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		CustomVar[EID]=0;
	}
#endif //CUSTOM_VARIABLES
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_RESEND_ALL,0,0);
#endif // RA_TOUCHDISPLAY
}

void ReefAngelClass::Refresh()
{
	WDTReset();
	switch (ChangeMode)
	{
	case FEEDING_MODE:
		FeedingModeStart();
		break;
	case WATERCHANGE_MODE:
		WaterChangeModeStart();
		break;
	case PH_CALIBRATE_MENU:
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
		SetupTouchCalibratePH();
#else
		StartSetupCalibrateChoicePH();
#endif // RA_TOUCH
		break;
#ifdef SALINITYEXPANSION
	case SAL_CALIBRATE_MENU:
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
		SetupTouchCalibrateSal();
#else
		StartSetupCalibrateSalinity();
#endif // RA_TOUCH
		break;
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
	case ORP_CALIBRATE_MENU:
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
		SetupTouchCalibrateORP();
#else
		SetupCalibrateORP();
#endif // RA_TOUCH
		break;
#endif // ORPEXPANSION
#ifdef PHEXPANSION
	case PHE_CALIBRATE_MENU:
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
		SetupTouchCalibratePHExp();
#else
		StartSetupCalibrateChoicePHExp();
#endif // RA_TOUCH
		break;
#endif // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	case WL_CALIBRATE_MENU:
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
		SetupTouchCalibrateWL();
#else
		SetupCalibrateWaterLevel();
#endif // RA_TOUCH
		break;
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
	}
	ChangeMode=0;
	boolean LightRelayOn=false;
	for (int l=0;l<8;l++)
	{
		if (LightsOnPorts & 1<<l)
			if (ReefAngel.Relay.RelayMaskOn & 1<<l) LightRelayOn=true;
	}

#ifdef DCPUMPCONTROL
	if (DCPump.UseMemory)
	{
		DCPump.Mode=InternalMemory.DCPumpMode_read();
		DCPump.Speed=InternalMemory.DCPumpSpeed_read();
		DCPump.Duration=InternalMemory.DCPumpDuration_read();
		DCPump.Threshold=InternalMemory.DCPumpThreshold_read();
	}
	byte SyncSpeed;
	byte AntiSyncSpeed;
	switch (DCPump.Mode)
	{
	case Constant:
	{
		SyncSpeed=DCPump.Speed;
		AntiSyncSpeed=DCPump.Speed;
		break;
	}
	case Lagoon:
	{
		SyncSpeed=ReefCrestMode(DCPump.Speed,10,true);
		AntiSyncSpeed=ReefCrestMode(DCPump.Speed,10,false);
		break;
	}
	case ReefCrest:
	{
		SyncSpeed=ReefCrestMode(DCPump.Speed,20,true);
		AntiSyncSpeed=ReefCrestMode(DCPump.Speed,20,false);
		break;
	}
	case ShortPulse:
	{
		SyncSpeed=ShortPulseMode(0,DCPump.Speed,DCPump.Duration*10,true);
		AntiSyncSpeed=ShortPulseMode(0,DCPump.Speed,DCPump.Duration*10,false);
		break;
	}
	case LongPulse:
	{
		SyncSpeed=LongPulseMode(0,DCPump.Speed,DCPump.Duration,true);
		AntiSyncSpeed=LongPulseMode(0,DCPump.Speed,DCPump.Duration,false);
		break;
	}
	case Gyre:
	{
		SyncSpeed=GyreMode(DCPump.Threshold,DCPump.Speed,DCPump.Duration,true);
		AntiSyncSpeed=GyreMode(DCPump.Threshold,DCPump.Speed,DCPump.Duration,false);
		break;
	}
	case NutrientTransport:
	{
		SyncSpeed=NutrientTransportMode(0,DCPump.Speed,DCPump.Duration*10,true);
		AntiSyncSpeed=NutrientTransportMode(0,DCPump.Speed,DCPump.Duration*10,false);
		break;
	}
	case TidalSwell:
	{
		SyncSpeed=TidalSwellMode(DCPump.Speed,true);
		AntiSyncSpeed=TidalSwellMode(DCPump.Speed,false);
		break;
	}
	case Sine:
	{
		SyncSpeed=SineMode(DCPump.Threshold,DCPump.Speed,DCPump.Duration,true);
		AntiSyncSpeed=SineMode(DCPump.Threshold,DCPump.Speed,DCPump.Duration,false);
		break;
	}
	case Else:
	{
		int offset = DCPump.Speed;
		if (DCPump.Speed > 50) offset = 100 - DCPump.Speed;

		SyncSpeed=ElseMode(DCPump.Speed,offset,true);
		AntiSyncSpeed=ElseMode(DCPump.Speed,offset,false);
		break;
	}
    }
	if (DisplayedMenu==FEEDING_MODE)
	{
		if (DCPump.FeedingSpeed < 100) 
		{
			SyncSpeed=DCPump.FeedingSpeed;
			AntiSyncSpeed=DCPump.FeedingSpeed;
		}
	}
	if (DisplayedMenu==WATERCHANGE_MODE)
	{
		if (DCPump.WaterChangeSpeed < 100) 
		{
			SyncSpeed=DCPump.WaterChangeSpeed;
			AntiSyncSpeed=DCPump.WaterChangeSpeed;
		}
	}
	SetDCPumpChannels(SyncSpeed,AntiSyncSpeed);
#endif  // DCPUMPCONTROL

#if defined DisplayLEDPWM && !defined REEFANGEL_MINI && !defined DCPUMPCONTROL
	if (LightRelayOn && LightsOverride)
	{
#if defined(__SAM3X8E__)
		VariableControl.SetActinic(InternalMemory.LEDPWMActinic_read());
		VariableControl.SetDaylight(InternalMemory.LEDPWMDaylight_read());
#else  // __SAM3X8E__
		PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
		PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
#endif  // __SAM3X8E__
#if defined RA_STAR
#if defined(__SAM3X8E__)
		VariableControl.SetActinic2(InternalMemory.LEDPWMActinic2_read());
		VariableControl.SetDaylight2(InternalMemory.LEDPWMDaylight2_read());
#else  // __SAM3X8E__
		PWM.SetActinic2(InternalMemory.LEDPWMActinic2_read());
		PWM.SetDaylight2(InternalMemory.LEDPWMDaylight2_read());
#endif  // __SAM3X8E__
#endif  // RA_STAR
	}
	// issue #3: Redundant code
	// issue #12: Revert back
#if defined(__SAM3X8E__)
	analogWrite(actinicPWMPin, map(VariableControl.GetActinicValueRaw(),0,4095,0,255));
	analogWrite(daylightPWMPin, map(VariableControl.GetDaylightValueRaw(),0,4095,0,255));
#else  // __SAM3X8E__
	analogWrite(actinicPWMPin, map(PWM.GetActinicValueRaw(),0,4095,0,255));
	analogWrite(daylightPWMPin, map(PWM.GetDaylightValueRaw(),0,4095,0,255));
#endif  // __SAM3X8E__

#if defined RA_STAR
	analogWrite(actinic2PWMPin, map(PWM.GetActinic2ValueRaw(),0,4095,0,255));
	analogWrite(daylight2PWMPin, map(PWM.GetDaylight2ValueRaw(),0,4095,0,255));
	SDFound=(PINJ & (1<<PJ3))==0;
#endif  // RA_STAR

#if defined(__SAM3X8E__)
	analogWrite(actinic2PWMPin, map(VariableControl.GetActinic2ValueRaw(),0,4095,0,255));
	analogWrite(daylight2PWMPin, map(VariableControl.GetDaylight2ValueRaw(),0,4095,0,255));
#endif  // __SAM3X8E__
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI


#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
	if (!Splash)
	{
#if not defined NOTILT
		if (!ReefAngel.Sleeping)
		{
			Tilt.Refresh();
			SetOrientation(Tilt.GetOrientation());
		}
#endif // NOTILT
	}
	if ((millis()>SplashDuration) && Splash)
	{
		Splash=false;
		if (TS.IsCalibrationNeeded())
		{
			CalibrateTouchScreen();
		}
		if(SDFound)	TouchLCD.FullClear(BKCOLOR);
	}
#if defined (__AVR_ATmega2560__)
	if (PINJ&(1<<7)) // Check for bus lock
		bitClear(AlertFlags,BusLockFlag);
	else
		bitSet(AlertFlags,BusLockFlag);
#endif // (__AVR_ATmega2560__)
#endif //  RA_TOUCH

#if not defined RA_TOUCHDISPLAY
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
#ifdef RADION_COMM
	RF.RadionWrite();
#endif // RADION_COMM
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
#if defined(__SAM3X8E__)
	VariableControl.ExpansionWrite();
#else // __SAM3X8E__
	PWM.ExpansionWrite();
#endif // __SAM3X8E__
#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION && defined DisplayLEDPWM
#if defined(__SAM3X8E__)
	VariableControl.SIXTEENChExpansionWrite();
#else // __SAM3X8E__
	PWM.SIXTEENChExpansionWrite();
#endif // __SAM3X8E__
#endif  // SIXTEENCHPWMEXPANSION

#ifdef IOEXPANSION
	IO.GetChannel();
#endif  // IOEXPANSION
#endif  // RA_TOUCHDISPLAY

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

#ifdef ETH_WIZ5100
	Network.Update();
#endif // ETH_WIZ5100

#ifdef RANET
	// Send RANet data
	if (millis()-RANetlastmillis>RANetDelay)
	{
		RANetlastmillis=millis();
		RANetCRC=0;
		RANetData[0]=RANetSeq;
		RANetData[1]=RANET_SIZE;
		for (int a=0;a<MAX_RELAY_EXPANSION_MODULES;a++)
		{
#ifdef RelayExp
			byte TempRelay = Relay.RelayDataE[a];
			TempRelay &= Relay.RelayMaskOffE[a];
			TempRelay |= Relay.RelayMaskOnE[a];
			RANetData[2+a]=TempRelay;
			RANetData[10+a]=Relay.RANetFallBackE[a];
#else
			RANetData[2+a]=0;
			RANetData[10+a]=0;
#endif // RelayExp
		}
		for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
		{
#ifdef PWMEXPANSION
#if defined(__SAM3X8E__)
			RANetData[18+a]=VariableControl.GetChannelValue(a);
#else
			RANetData[18+a]=PWM.GetChannelValue(a);
#endif
#else
			RANetData[18+a]=0;
#endif // PWMEXPANSION
		}
		for (int a=0;a<SIXTEENCH_PWM_EXPANSION_CHANNELS;a++)
		{
#ifdef SIXTEENCHPWMEXPANSION
#if defined(__SAM3X8E__)
			RANetData[24+a]=VariableControl.Get16ChannelValue(a);
#else
			RANetData[24+a]=PWM.Get16ChannelValue(a);
#endif
#else
			RANetData[24+a]=0;
#endif // SIXTEENCHPWMEXPANSION
		}
//		char buf[3];
		for (int a=0;a<RANET_SIZE-2;a++)
		{
			RANetCRC+=RANetData[a];
			RANET_SERIAL.write(RANetData[a]);
//			sprintf(buf,"%02x",RANetData[a]);
//			RANET_SERIAL.print(buf);
		}
		RANET_SERIAL.write(RANetCRC);
		RANET_SERIAL.println();
//		sprintf(buf,"%02x",RANetCRC);
//		RANET_SERIAL.println(buf);
		RANetSeq++;
	}
#endif // RANET
#if defined wifi || defined RA_STAR
    ReefAngel.Network.ReceiveData();
#endif  // wifi || RA_STAR

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
#ifdef EXTRA_TEMP_PROBES
	Params.Temp[T4_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT4);
	RefreshScreen();
	Params.Temp[T5_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT5);
	RefreshScreen();
	Params.Temp[T6_PROBE]=TempSensor.ReadTemperature(TempSensor.addrT6);
	RefreshScreen();
#endif // EXTRA_TEMP_PROBES	
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
#ifdef EXTRA_TEMP_PROBES
	x = TempSensor.ReadTemperature(TempSensor.addrT4);
	RefreshScreen();
	y = x - Params.Temp[T4_PROBE];
	if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T4_PROBE] == 0 || ~x) Params.Temp[T4_PROBE] = x;
	x = TempSensor.ReadTemperature(TempSensor.addrT5);
	RefreshScreen();
	y = x - Params.Temp[T5_PROBE];
	if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T5_PROBE] == 0 || ~x) Params.Temp[T5_PROBE] = x;
	x = TempSensor.ReadTemperature(TempSensor.addrT6);
	RefreshScreen();
	y = x - Params.Temp[T6_PROBE];
	if ( abs(y) < MAX_TEMP_SWING || Params.Temp[T6_PROBE] == 0 || ~x) Params.Temp[T6_PROBE] = x;
#endif // EXTRA_TEMP_PROBES	
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
	for (int a=0;a<5;a++)
	{
		tempph+=PH.Read();
	}
	Params.PHExp=tempph/5;
	if (Params.PHExp!=0)
	{
		Params.PHExp=map(Params.PHExp, PHExpMin, PHExpMax, 700, 1000); // apply the calibration to the sensor reading
		Params.PHExp=constrain(Params.PHExp,100,1400);
	}
	RefreshScreen();
#endif  // defined PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	WaterLevel.Convert();
	RefreshScreen();
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#if defined HUMIDITYEXPANSION
	Humidity.Read();
	RefreshScreen();
#endif  // defined HUMIDITYEXPANSION
	OverheatCheck();
#ifdef LEAKDETECTOREXPANSION
	LeakCheck();
	RefreshScreen();
#endif  // LEAKDETECTOREXPANSION
#if defined PAREXPANSION
	PAR.Convert();
	RefreshScreen();
#endif  // defined PAREXPANSION
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
		bitSet(AlertFlags,BusLockFlag);
#ifdef RA_STAR
		sbi(PORTH,2); // Turn off exp bus power
#endif // RA_STAR
	}
	else
	{
		BusLocked=false;  // Bus is not locked
		bitClear(AlertFlags,BusLockFlag);
#ifdef RA_STAR
		cbi(PORTH,2); // Turn on exp bus power
#endif // RA_STAR
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

#if defined SALINITYEXPANSION
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

#ifdef BUSCHECK 
boolean ReefAngelClass::isBusLock()
{
	return bitRead(AlertFlags, BusLockFlag);
}
#endif //BUSCHECK

#ifdef LEAKDETECTOREXPANSION
boolean ReefAngelClass::IsLeakDetected()
{
	boolean detect=false;
	int iLeak=0;
	Wire.requestFrom(I2CLeak, 2);
	if (Wire.available())
	{
		iLeak = Wire.read();
		iLeak = iLeak<<8;
		iLeak += Wire.read();
	}
	detect=iLeak>2000;
#ifdef EMBEDDED_LEAK
	detect|=analogRead(LeakPin)<400;
#endif // EMBEDDED_LEAK
#ifdef RA_TOUCHDISPLAY
	detect=LeakStatus;
#endif // RA_TOUCHDISPLAY

	return detect;
}

void ReefAngelClass::LeakCheck()
{
	// if leak is detected
	if ( !IsLeakDetected() )
		Leakmillis=millis();
	if (millis()-Leakmillis>3000) // Only flag leak if we have a leak for 3 seconds
	{
		LED.On();
		bitSet(AlertFlags,LeakFlag);
		// invert the ports that are activated
		Relay.RelayMaskOff &= ~LeakShutoffPorts;
#ifdef RelayExp
		for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
		{
			Relay.RelayMaskOffE[i] &= ~LeakShutoffPortsE[i];
		}
#endif  // RelayExp
	}
}

void ReefAngelClass::LeakClear()
{
	LED.Off();
	bitClear(AlertFlags,LeakFlag);
	Relay.RelayMaskOff |= LeakShutoffPorts;
#ifdef RelayExp
	for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] |= LeakShutoffPortsE[i];
	}
#endif  // RelayExp
	Relay.Write();
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // RA_TOUCH
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_CLEAR_LEAK,0);
#endif // RA_TOUCHDISPLAY
}

boolean ReefAngelClass::isLeak()
{
	return bitRead(AlertFlags, LeakFlag);
}

#endif  // LEAKDETECTOREXPANSION

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

void ReefAngelClass::StandardHeater(byte Probe, byte HeaterRelay, int LowTemp, int HighTemp)
{
	if (LowTemp < 100) LowTemp *= 10; // Correct temp settings that aren't in the correct range
	if (HighTemp < 100) HighTemp *= 10; // Correct temp settings that aren't in the correct range
	if (Params.Temp[Probe] == 0) return;  // Don't turn the heater on if the temp is reading 0
	if (Params.Temp[Probe] <= LowTemp && Params.Temp[Probe] > 0) Relay.On(HeaterRelay);  // If sensor 1 temperature <= LowTemp - turn on heater
	if (Params.Temp[Probe] >= HighTemp) Relay.Off(HeaterRelay);  // If sensor 1 temperature >= HighTemp - turn off heater
}

void ReefAngelClass::StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp)
{
	StandardHeater(T1_PROBE, HeaterRelay, LowTemp, HighTemp);
}

void ReefAngelClass::StandardHeater2(byte HeaterRelay, int LowTemp, int HighTemp)
{
	StandardHeater(T2_PROBE, HeaterRelay, LowTemp, HighTemp);
}

void ReefAngelClass::StandardHeater3(byte HeaterRelay, int LowTemp, int HighTemp)
{
	StandardHeater(T3_PROBE, HeaterRelay, LowTemp, HighTemp);
}

void ReefAngelClass::StandardFan(byte Probe, byte FanRelay, int LowTemp, int HighTemp)
{
	if (LowTemp < 100) LowTemp *= 10; // Correct temp settings that aren't in the correct range
	if (HighTemp < 100) HighTemp *= 10; // Correct temp settings that aren't in the correct range
	if (Params.Temp[Probe] == 0) return;  // Don't turn the fan/chiller on if the temp is reading 0
	if (Params.Temp[Probe] >= HighTemp) Relay.On(FanRelay);  // If sensor 1 temperature >= HighTemp - turn on fan
	if (Params.Temp[Probe] <= LowTemp) Relay.Off(FanRelay);  // If sensor 1 temperature <= LowTemp - turn off fan
}

void ReefAngelClass::StandardFan(byte FanRelay, int LowTemp, int HighTemp)
{
	StandardFan(T1_PROBE, FanRelay, LowTemp, HighTemp);
}

void ReefAngelClass::StandardFan2(byte FanRelay, int LowTemp, int HighTemp)
{
	StandardFan(T2_PROBE, FanRelay, LowTemp, HighTemp);
}

void ReefAngelClass::StandardFan3(byte FanRelay, int LowTemp, int HighTemp)
{
	StandardFan(T3_PROBE, FanRelay, LowTemp, HighTemp);
}

void ReefAngelClass::CO2Control(byte CO2Relay, int LowPH, int HighPH)
{
	CO2Control(CO2Relay,LowPH,HighPH,false);
}

void ReefAngelClass::CO2Control(byte CO2Relay, int LowPH, int HighPH, bool useExp)
{
	int ph=Params.PH;
	if (useExp) ph=Params.PHExp;

	if (ph <= LowPH) Relay.Off(CO2Relay);  // If PH <= LowPH - turn on CO2
	if (ph >= HighPH) Relay.On(CO2Relay);  // If sensor 1 PH >= HighPH - turn off CO2
}

void ReefAngelClass::PHControl(byte PHControlRelay, int LowPH, int HighPH)
{
	PHControl(PHControlRelay,LowPH,HighPH,false);
}

void ReefAngelClass::PHControl(byte PHControlRelay, int LowPH, int HighPH, bool useExp)
{
	int ph=Params.PH;
	if (useExp) ph=Params.PHExp;

	if (ph <= LowPH) Relay.On(PHControlRelay);  // If PH <= LowPH - turn on PHControlRelay
	if (ph >= HighPH) Relay.Off(PHControlRelay);  // If sensor 1 PH >= HighPH - turn off PHControlRelay
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
		bitSet(AlertFlags,ATOTimeOutFlag);
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

#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
#ifdef MULTIWATERLEVELEXPANSION
void ReefAngelClass::WaterLevelATO(byte Channel, byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel)
{
#else
void ReefAngelClass::WaterLevelATO(byte ATORelay, int ATOTimeout, byte LowLevel, byte HighLevel)
{
  byte Channel = 0;
#endif  // MULTIWATERLEVELEXPANSION
	// Input:  Relay port and timeout value (max number of seconds that ATO pump is allowed to run)
	// Input:  Low and High Water Level to start and stop ATO pump
	unsigned long TempTimeout = ATOTimeout;
	TempTimeout *= 1000;

	/*
	Is the low level is reached (meaning we need to top off) and are we not currently topping off
	Then we set the timer to be now and start the topping pump
	 */
	if ( WaterLevel.GetLevel(Channel) < LowLevel && ( !WLATO.IsTopping()) )
	{
		WLATO.Timer = millis();
		WLATO.StartTopping();
		Relay.On(ATORelay);
	}

	// If the high level is reached, this is a safeguard to prevent over running of the top off pump
	if ( WaterLevel.GetLevel(Channel) > HighLevel )
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
		bitSet(AlertFlags,ATOTimeOutFlag);
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
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION

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
		bitSet(AlertFlags,ATOTimeOutFlag);
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

boolean ReefAngelClass::isATOTimeOut()
{
	return bitRead(AlertFlags, ATOTimeOutFlag);
}

#ifdef KALKDOSER
void ReefAngelClass::KalkDoser(byte KalkRelay, int LowPH, int TimeoutSeconds, byte MinuteInterval)
{
  bool canRun = true;
  static int lastRun = -1;

  if (MinuteInterval)
  {
    int nextSafeRun = NumMins(hour(), minute()) - lastRun;

    if ( nextSafeRun < 0 )
    {
      nextSafeRun += 24;
    }

    if( nextSafeRun < MinuteInterval && lastRun >= 0 )
    {
      canRun = false;
    }
  }

  // if pH is low
  if(Params.PH <= LowPH && !KWDoser.IsTopping() && canRun)
  {
    KWDoser.Timer = millis();
    KWDoser.StartTopping();
    Relay.On(KalkRelay);
  }

  if(Params.PH > LowPH && KWDoser.IsTopping() == true)
  {
    lastRun = NumMins(hour(), minute());
    KWDoser.StopTopping();
    Relay.Off(KalkRelay);
  }

  unsigned long t = TimeoutSeconds * 1000;
  if ((millis() - KWDoser.Timer) > t && KWDoser.IsTopping())
  {
    lastRun = NumMins(hour(), minute());
    KWDoser.StopTopping();
    Relay.Off(KalkRelay);
  }
}
#endif //  KALKDOSER

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

void ReefAngelClass::MHLights(byte Relay, byte MinuteOffset)
{
  int onTime=NumMins(InternalMemory.MHOnHour_read(),InternalMemory.MHOnMinute_read())-MinuteOffset;
  int offTime=NumMins(InternalMemory.MHOffHour_read(),InternalMemory.MHOffMinute_read())+MinuteOffset;
  MHLights(Relay,
      onTime/60,
      onTime%60,
      offTime/60,
      offTime%60,
      InternalMemory.MHDelay_read()
  );
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

#ifdef MULTIWATERLEVELEXPANSION
void ReefAngelClass::WaterLevelATO(byte Channel, byte Relay)
{
  WaterLevelATO(Channel, Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.WaterLevelLow_read(), InternalMemory.WaterLevelHigh_read());
}
#else
void ReefAngelClass::WaterLevelATO(byte Relay)
{
	WaterLevelATO(Relay, InternalMemory.ATOExtendedTimeout_read(), InternalMemory.WaterLevelLow_read(), InternalMemory.WaterLevelHigh_read());
}
#endif  // MULTIWATERLEVELEXPANSION

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

void ReefAngelClass::FeedingModeStart()
{
	bitSet(StatusFlags,FeedingFlag);
	// turn off ports
	Relay.RelayMaskOff &= ~FeedingModePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] &= ~FeedingModePortsE[i];
	}
#endif  // RelayExp
	CheckFeedingDrawing();
#ifdef RFEXPANSION
	if (RF.FeedingSpeed < 100) RF.SetMode(Constant,RF.FeedingSpeed,0);
	else RF.SetMode(Feeding_Start,0,0);
#endif  // RFEXPANSION
	Timer[FEEDING_TIMER].Start();  //Start Feeding Mode timer
	// Tell controller what mode we are in
	SetDisplayedMenu(FEEDING_MODE);
}

void ReefAngelClass::WaterChangeModeStart()
{
	bitSet(StatusFlags,WaterChangeFlag);
	// turn off ports
	Relay.RelayMaskOff &= ~WaterChangePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] &= ~WaterChangePortsE[i];
	}
#endif  // RelayExp
#ifdef RFEXPANSION
	if (RF.WaterChangeSpeed < 100) RF.SetMode(Constant,RF.WaterChangeSpeed,0);
#endif  // RFEXPANSION
	CheckWaterChangeDrawing();
	// Tell controller what mode we are in
	SetDisplayedMenu(WATERCHANGE_MODE);
}

void ReefAngelClass::ATOClear()
{
	LED.Off();
	bitClear(AlertFlags,ATOTimeOutFlag);
#ifdef ENABLE_EXCEED_FLAGS
	InternalMemory.write(ATO_Single_Exceed_Flag, 0);
	InternalMemory.write(ATO_Exceed_Flag, 0);
#endif  // ENABLE_EXCEED_FLAGS
	LowATO.StopTopping();
	HighATO.StopTopping();
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	WLATO.StopTopping();
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // RA_TOUCH
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_CLEAR_ATO,0);
#endif // RA_TOUCHDISPLAY

}

void ReefAngelClass::OverheatCheck()
{
	// if overheat probe exceeds the temp
	if ( Params.Temp[OverheatProbe] <= InternalMemory.OverheatTemp_read() )
		Overheatmillis=millis();
	if (millis()-Overheatmillis>3000) // Only flag overheat if we have overheat for 3 seconds
	{
		LED.On();
		bitSet(AlertFlags,OverheatFlag);
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
	bitClear(AlertFlags,OverheatFlag);
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // RA_TOUCH
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_CLEAR_OVERHEAT,0);
#endif // RA_TOUCHDISPLAY
}

boolean ReefAngelClass::isOverheat()
{
	return bitRead(AlertFlags, OverheatFlag);
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
	bitSet(StatusFlags,LightsOnFlag);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
	menu_button_functions1[4] = &ReefAngelClass::LightsOff;
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // RA_TOUCH
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_LIGHTS_ON,0);
#endif // RA_TOUCHDISPLAY
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
#if defined(__SAM3X8E__)
	VariableControl.SetActinic(0);
	VariableControl.SetDaylight(0);
#else
	PWM.SetActinic(0);
	PWM.SetDaylight(0);
#endif
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI
	Relay.Write();
	bitClear(StatusFlags,LightsOnFlag);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
	menu_button_functions1[4] = &ReefAngelClass::LightsOn;
	if (DisplayedMenu==TOUCH_MENU)
		SetDisplayedMenu(DEFAULT_MENU);
#endif  // RA_TOUCH
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_COMMAND,COMMAND_LIGHTS_OFF,0);
#endif // RA_TOUCHDISPLAY
}

void ReefAngelClass::ExitMenu()
{
	// Handles the cleanup to return to the main screen
	bitClear(StatusFlags,FeedingFlag);
	bitClear(StatusFlags,WaterChangeFlag);
	WDTReset();
	ClearScreen(DefaultBGColor);
	Timer[LCD_TIMER].Start();
	SetDisplayedMenu(DEFAULT_MENU);
	CheckDrawGraph();
	redrawmenu=true;
}

void ReefAngelClass::SetDisplayedMenu(byte value)
{
	DisplayedMenu=value;
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_MENU,value,value); 	// Change Menu
#endif // RA_TOUCHDISPLAY
}

#if defined wifi || defined ETH_WIZ5100
void ReefAngelClass::Portal(char *username)
{
	Network.Portal(username);
}

void ReefAngelClass::Portal(char *username, char *key)
{
	Network.Portal(username,key);
}

void ReefAngelClass::DDNS(char *subdomain)
{
	Network.DDNS(subdomain);
}
#endif  // wifi || ETH_WIZ5100

#ifdef RA_TOUCHDISPLAY
void receiveEvent(int howMany) {
	byte d[4];
	byte crc=0;
	wdt_reset();
	if (ReefAngel.Sleeping)
	{
		while(Wire.available())
			Wire.read();
	}
	else
	{
		if (howMany==4)
		{
			crc=0;
			for(int a=0;a<4;a++)
			{
				d[a]=Wire.read();
//				Serial.print(d[a]);
//				Serial.print("\t");
				crc+=d[a];
			}
//			Serial.println();
			crc-=d[3];
			if (crc==d[3] && d[0]=='$')
			{
				switch (d[1])
				{
				case 0:
					if (ReefAngel.DisplayedMenu!=d[2])
					{
						if (ReefAngel.DisplayedMenu!=TOUCH_MENU)
						{
							// deduct 100 from the value to indicate we are coming from an interrupt.
						ReefAngel.DisplayedMenu=d[2]-100;
						}
					}
					break;
				case 1:
					ReefAngel.Board=d[2];
					break;
				case 2:
					ReefAngel.AlertFlags=d[2];
					break;
				case 3:
					ReefAngel.StatusFlags=d[2];
					break;
				case 4:
					ReefAngel.Params.Temp[T1_PROBE]=((ReefAngel.Params.Temp[T1_PROBE]/256)<<8)+d[2];
					break;
				case 5:
					ReefAngel.Params.Temp[T1_PROBE]=(d[2]<<8) + (ReefAngel.Params.Temp[T1_PROBE]%256);
					break;
				case 6:
					ReefAngel.Params.Temp[T2_PROBE]=((ReefAngel.Params.Temp[T2_PROBE]/256)<<8)+d[2];
					break;
				case 7:
					ReefAngel.Params.Temp[T2_PROBE]=(d[2]<<8) + (ReefAngel.Params.Temp[T2_PROBE]%256);
					break;
				case 8:
					ReefAngel.Params.Temp[T3_PROBE]=((ReefAngel.Params.Temp[T3_PROBE]/256)<<8)+d[2];
					break;
				case 9:
					ReefAngel.Params.Temp[T3_PROBE]=(d[2]<<8) + (ReefAngel.Params.Temp[T3_PROBE]%256);
					break;
				case 10:
					ReefAngel.Params.PH=((ReefAngel.Params.PH/256)<<8)+d[2];
					break;
				case 11:
					ReefAngel.Params.PH=(d[2]<<8) + (ReefAngel.Params.PH%256);
					break;
				case 12:
					ReefAngel.LowATO.SetActive(bitRead(d[2],0));
					ReefAngel.HighATO.SetActive(bitRead(d[2],1));
					ReefAngel.AlarmInput.SetActive(bitRead(d[2],2));
					ReefAngel.LeakStatus=bitRead(d[2],3);
					break;
				case 13:
					ReefAngel.PWM.SetDaylight(d[2]);
					break;
				case 14:
					ReefAngel.PWM.SetActinic(d[2]);
					break;
				case 15:
					ReefAngel.PWM.SetDaylight2(d[2]);
					break;
				case 16:
					ReefAngel.PWM.SetActinic2(d[2]);
					break;
				case 17:
					ReefAngel.Relay.RelayData=d[2];
					break;
				case 18:
					ReefAngel.Relay.RelayMaskOn=d[2];
					break;
				case 19:
					ReefAngel.Relay.RelayMaskOff=d[2];
					break;
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
				case 25:
					ReefAngel.PWM.SetChannel(d[1]-20,d[2]);
					break;
				case 26:
					ReefAngel.RF.Mode=d[2];
					break;
				case 27:
					ReefAngel.RF.Speed=d[2];
					break;
				case 28:
					ReefAngel.RF.Duration=d[2];
					break;
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
					ReefAngel.RF.RadionChannels[d[1]-29]=d[2];
					break;
				case 35:
				case 36:
				case 37:
					ReefAngel.AI.SetChannel(d[1]-35,d[2]);
					break;
				case 38:
					ReefAngel.IO.IOPorts=d[2];
					break;
				case 39:
					ReefAngel.DCPump.Mode=d[2];
					break;
				case 40:
					ReefAngel.DCPump.Speed=d[2];
					break;
				case 41:
					ReefAngel.DCPump.Duration=d[2];
					break;
				case 42:
					ReefAngel.Relay.RelayDataE[0]=d[2];
					break;
				case 43:
					ReefAngel.Relay.RelayMaskOnE[0]=d[2];
					break;
				case 44:
					ReefAngel.Relay.RelayMaskOffE[0]=d[2];
					break;
				case 45:
					ReefAngel.Relay.RelayDataE[1]=d[2];
					break;
				case 46:
					ReefAngel.Relay.RelayMaskOnE[1]=d[2];
					break;
				case 47:
					ReefAngel.Relay.RelayMaskOffE[1]=d[2];
					break;
				case 48:
					ReefAngel.Relay.RelayDataE[2]=d[2];
					break;
				case 49:
					ReefAngel.Relay.RelayMaskOnE[2]=d[2];
					break;
				case 50:
					ReefAngel.Relay.RelayMaskOffE[2]=d[2];
					break;
				case 51:
					ReefAngel.Relay.RelayDataE[3]=d[2];
					break;
				case 52:
					ReefAngel.Relay.RelayMaskOnE[3]=d[2];
					break;
				case 53:
					ReefAngel.Relay.RelayMaskOffE[3]=d[2];
					break;
				case 54:
					ReefAngel.Relay.RelayDataE[4]=d[2];
					break;
				case 55:
					ReefAngel.Relay.RelayMaskOnE[4]=d[2];
					break;
				case 56:
					ReefAngel.Relay.RelayMaskOffE[4]=d[2];
					break;
				case 57:
					ReefAngel.Relay.RelayDataE[5]=d[2];
					break;
				case 58:
					ReefAngel.Relay.RelayMaskOnE[5]=d[2];
					break;
				case 59:
					ReefAngel.Relay.RelayMaskOffE[5]=d[2];
					break;
				case 60:
					ReefAngel.Relay.RelayDataE[6]=d[2];
					break;
				case 61:
					ReefAngel.Relay.RelayMaskOnE[6]=d[2];
					break;
				case 62:
					ReefAngel.Relay.RelayMaskOffE[6]=d[2];
					break;
				case 63:
					ReefAngel.Relay.RelayDataE[7]=d[2];
					break;
				case 64:
					ReefAngel.Relay.RelayMaskOnE[7]=d[2];
					break;
				case 65:
					ReefAngel.Relay.RelayMaskOffE[7]=d[2];
					break;
				case 66:
					ReefAngel.Params.Salinity=((ReefAngel.Params.Salinity/256)<<8)+d[2];
					break;
				case 67:
					ReefAngel.Params.Salinity=(d[2]<<8) + (ReefAngel.Params.Salinity%256);
					break;
				case 68:
					ReefAngel.Params.ORP=((ReefAngel.Params.ORP/256)<<8)+d[2];
					break;
				case 69:
					ReefAngel.Params.ORP=(d[2]<<8) + (ReefAngel.Params.ORP%256);
					break;
				case 70:
					ReefAngel.Params.PHExp=((ReefAngel.Params.PHExp/256)<<8)+d[2];
					break;
				case 71:
					ReefAngel.Params.PHExp=(d[2]<<8) + (ReefAngel.Params.PHExp%256);
					break;
				case 72:
					ReefAngel.Humidity.SetLevel(((ReefAngel.Humidity.GetLevel()/256)<<8)+d[2]);
					break;
				case 73:
					ReefAngel.Humidity.SetLevel((d[2]<<8) + (ReefAngel.Humidity.GetLevel()%256));
					break;
				case 74:
					ReefAngel.WaterLevel.SetLevel(0,((ReefAngel.WaterLevel.GetLevel()/256)<<8)+d[2]);
					break;
				case 75:
					ReefAngel.WaterLevel.SetLevel(0,(d[2]<<8) + (ReefAngel.WaterLevel.GetLevel()%256));
					break;
				case 76:
					ReefAngel.WaterLevel.SetLevel(1,((ReefAngel.WaterLevel.GetLevel(1)/256)<<8)+d[2]);
					break;
				case 77:
					ReefAngel.WaterLevel.SetLevel(1,(d[2]<<8) + (ReefAngel.WaterLevel.GetLevel(1)%256));
					break;
				case 78:
					ReefAngel.WaterLevel.SetLevel(2,((ReefAngel.WaterLevel.GetLevel(2)/256)<<8)+d[2]);
					break;
				case 79:
					ReefAngel.WaterLevel.SetLevel(2,(d[2]<<8) + (ReefAngel.WaterLevel.GetLevel(2)%256));
					break;
				case 80:
					ReefAngel.WaterLevel.SetLevel(3,((ReefAngel.WaterLevel.GetLevel(3)/256)<<8)+d[2]);
					break;
				case 81:
					ReefAngel.WaterLevel.SetLevel(3,(d[2]<<8) + (ReefAngel.WaterLevel.GetLevel(3)%256));
					break;
				case 82:
					ReefAngel.WaterLevel.SetLevel(4,((ReefAngel.WaterLevel.GetLevel(4)/256)<<8)+d[2]);
					break;
				case 83:
					ReefAngel.WaterLevel.SetLevel(4,(d[2]<<8) + (ReefAngel.WaterLevel.GetLevel(4)%256));
					break;
				case 84:
					ReefAngel.EM=d[2];
					break;
				case 85:
					ReefAngel.EM1=d[2];
					break;
				case 86:
					ReefAngel.REM=d[2];
					break;
				case 87:
					ReefAngel.CustomVar[0]=d[2];
					break;
				case 88:
					ReefAngel.CustomVar[1]=d[2];
					break;
				case 89:
					ReefAngel.CustomVar[2]=d[2];
					break;
				case 90:
					ReefAngel.CustomVar[3]=d[2];
					break;
				case 91:
					ReefAngel.CustomVar[4]=d[2];
					break;
				case 92:
					ReefAngel.CustomVar[5]=d[2];
					break;
				case 93:
					ReefAngel.CustomVar[6]=d[2];
					break;
				case 94:
					ReefAngel.CustomVar[7]=d[2];
					break;
				// Don't go over 99. The max array is set to 100
				}
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

#endif //  RA_TOUCHDISPLAY

#ifdef I2CMASTER
void ReefAngelClass::UpdateTouchDisplay()
{
	if (millis()-lastmasterupdate>1000)
	{
		lastmasterupdate=millis();
		byte atostatus=0;
		if (ReefAngel.LowATO.IsActive())
			bitSet(atostatus,0);
		else
			bitClear(atostatus,0);
		if (ReefAngel.HighATO.IsActive())
			bitSet(atostatus,1);
		else
			bitClear(atostatus,1);
#ifdef RA_STAR
		if (ReefAngel.AlarmInput.IsActive())
			bitSet(atostatus,2);
		else
			bitClear(atostatus,2);
#endif // RA_STAR
#if defined RA_STAR || defined LEAKDETECTOREXPANSION
		if (ReefAngel.IsLeakDetected())
			bitSet(atostatus,3);
		else
			bitClear(atostatus,3);
#endif // defined RA_STAR || defined LEAKDETECTOREXPANSION

		MasterWrite(DisplayedMenu,0);
		MasterWrite(Board,1);
		MasterWrite(AlertFlags,2);
		MasterWrite(StatusFlags,3);
		MasterWrite(Params.Temp[T1_PROBE],4);
		MasterWrite(Params.Temp[T2_PROBE],6);
		MasterWrite(Params.Temp[T3_PROBE],8);
		MasterWrite(Params.PH,10);
		MasterWrite(atostatus,12);
		MasterWrite(PWM.GetDaylightValue(),13);
		MasterWrite(PWM.GetActinicValue(),14);
		MasterWrite(PWM.GetDaylight2Value(),15);
		MasterWrite(PWM.GetActinic2Value(),16);
		MasterWrite(Relay.RelayData,17);
		MasterWrite(Relay.RelayMaskOn,18);
		MasterWrite(Relay.RelayMaskOff,19);
		for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
			MasterWrite(PWM.GetChannelValue(a),20+a);
		MasterWrite(RF.Mode,26);
		MasterWrite(RF.Speed,27);
		MasterWrite(RF.Duration,28);
		for (int a=0;a<RF_CHANNELS;a++)
			MasterWrite(RF.RadionChannels[a],29+a);
		MasterWrite(AI.GetChannel(0),35);
		MasterWrite(AI.GetChannel(1),36);
		MasterWrite(AI.GetChannel(2),37);
		MasterWrite(IO.IOPorts,38);
		MasterWrite(DCPump.Mode,39);
		MasterWrite(DCPump.Speed,40);
		MasterWrite(DCPump.Duration,41);
		for (int a=0;a<MAX_RELAY_EXPANSION_MODULES;a++)
		{
			MasterWrite(Relay.RelayDataE[a],42+(a*3));
			MasterWrite(Relay.RelayMaskOnE[a],43+(a*3));
			MasterWrite(Relay.RelayMaskOffE[a],44+(a*3));
		}
		MasterWrite(Params.Salinity,66);
		MasterWrite(Params.ORP,68);
		MasterWrite(Params.PHExp,70);
		MasterWrite(Humidity.GetLevel(),72);
		MasterWrite(WaterLevel.GetLevel(),74);
		MasterWrite(WaterLevel.GetLevel(1),76);
		MasterWrite(WaterLevel.GetLevel(2),78);
		MasterWrite(WaterLevel.GetLevel(3),80);
		MasterWrite(WaterLevel.GetLevel(4),82);
		MasterWrite(EM,84);
		MasterWrite(EM1,85);
		MasterWrite(REM,86);
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
		MasterWrite(CustomVar[EID],87+EID);
		// Don't go over 99. The max array is set to 100

#endif //CUSTOM_VARIABLES

	}
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
	if (I2CCommand==COMMAND_CLEAR_LEAK)
		LeakClear();
	I2CCommand=0;
}

void ReefAngelClass::MasterWrite(int value, byte index)
{
	byte data[3];
	byte crc=0;
	if ((value&0xff)!=olddata[index])
	{
		olddata[index]=value&0xff;
		crc=0;
		data[0]='$';
		data[1]=index;
		data[2]=olddata[index];
		for (int a=0;a<3;a++)
			crc+=data[a];
		Wire.beginTransmission(I2CRA_TouchDisplay);
		Wire.write(data,3);
		Wire.write(crc);
		Wire.endTransmission();
		delay(10);
		wdt_reset();
	}
	if (value>255)
	{
		if (value!=(olddata[index]<<8)+olddata[index+1])
		{
			olddata[index+1]=value>>8;
			crc=0;
			data[0]='$';
			data[1]=index+1;
			data[2]=olddata[index+1];
			for (int a=0;a<3;a++)
				crc+=data[a];
			Wire.beginTransmission(I2CRA_TouchDisplay);
			Wire.write(data,3);
			Wire.write(crc);
			Wire.endTransmission();
			delay(10);
			wdt_reset();
		}
	}
}

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
				if (d[1]>=OVERRIDE_DAYLIGHT2 && d[1]<=OVERRIDE_ACTINIC2)
					ReefAngel.PWM.Override(d[1],d[2]);

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
					case TOUCH_MENU:
					case DATE_TIME_MENU:
					case PH_CALIBRATE_MENU:
					case SAL_CALIBRATE_MENU:
					case ORP_CALIBRATE_MENU:
					case PHE_CALIBRATE_MENU:
					case WL_CALIBRATE_MENU:
					case DEFAULT_MENU:
						ReefAngel.DisplayedMenu=d[1];
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
			case MESSAGE_RESEND_ALL: // Resend all data
			{
				for (int a=0;a<MASTERARRAYSIZE;a++)
					ReefAngel.olddata[a]=0;
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

#ifdef DCPUMPCONTROL
void ReefAngelClass::SetDCPumpChannels(byte SyncSpeed, byte AntiSyncSpeed) 
{
		// Apply the Threshold
		SyncSpeed=PumpThreshold(SyncSpeed,DCPump.Threshold);

		// Apply the Threshold and AntiSyncOffset
		AntiSyncSpeed*=((float) DCPump.AntiSyncOffset/100);
		AntiSyncSpeed=PumpThreshold(AntiSyncSpeed,DCPump.Threshold);

        if (DCPump.DaylightChannel==Sync)
#if defined(__SAM3X8E__)
            VariableControl.SetDaylight(SyncSpeed);
#else // __SAM3X8E__
            PWM.SetDaylight(SyncSpeed);
#endif // __SAM3X8E__
		else if (DCPump.DaylightChannel==AntiSync)
#if defined(__SAM3X8E__)
            VariableControl.SetDaylight(AntiSyncSpeed);
#else // __SAM3X8E__
            PWM.SetDaylight(AntiSyncSpeed);
#endif // __SAM3X8E__

        if (DCPump.ActinicChannel==Sync) 
#if defined(__SAM3X8E__)
            VariableControl.SetActinic(SyncSpeed);
#else // __SAM3X8E__
            PWM.SetActinic(SyncSpeed);
#endif // __SAM3X8E__
		else if (DCPump.ActinicChannel==AntiSync) 
#if defined(__SAM3X8E__)
            VariableControl.SetActinic(AntiSyncSpeed);
#else // __SAM3X8E__
            PWM.SetActinic(AntiSyncSpeed);
#endif // __SAM3X8E__

        if (DCPump.LowATOChannel==Sync)
            analogWrite(lowATOPin, 2.55*SyncSpeed);
		else if (DCPump.LowATOChannel==AntiSync)
            analogWrite(lowATOPin, 2.55*AntiSyncSpeed);

        if (DCPump.HighATOChannel==Sync)
            analogWrite(highATOPin, 2.55*SyncSpeed);
		else if (DCPump.HighATOChannel==AntiSync)
            analogWrite(highATOPin, 2.55*AntiSyncSpeed);

#ifdef PWMEXPANSION
        for (int a=0; a<PWM_EXPANSION_CHANNELS;a++) 
		{
            if (DCPump.ExpansionChannel[a]==Sync) {
#if defined(__SAM3X8E__)
                VariableControl.SetChannel(a,SyncSpeed);
#else // __SAM3X8E__
                PWM.SetChannel(a,SyncSpeed);
#endif // __SAM3X8E__
			} else if (DCPump.ExpansionChannel[a]==AntiSync) {
#if defined(__SAM3X8E__)
                VariableControl.SetChannel(a,AntiSyncSpeed);
#else // __SAM3X8E__
                PWM.SetChannel(a,AntiSyncSpeed);
#endif // __SAM3X8E__
			}
		}
#endif // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION
        for (int a=0; a<SIXTEENCH_PWM_EXPANSION_CHANNELS;a++) 
		{
            if (DCPump.SIXTEENChExpansionChannel[a]==Sync) {
#if defined(__SAM3X8E__)
                VariableControl.SetChannel(a,SyncSpeed);
#else // __SAM3X8E__
                PWM.Set16Channel(a,SyncSpeed);
#endif // __SAM3X8E__
            } else if (DCPump.SIXTEENChExpansionChannel[a]==AntiSync) {
#if defined(__SAM3X8E__)
                VariableControl.SetChannel(a,AntiSyncSpeed);
#else // __SAM3X8E__
                PWM.Set16Channel(a,AntiSyncSpeed);
#endif // __SAM3X8E__
			}
		}
#endif // SIXTEENCHPWMEXPANSION
}
#endif // DCPUMPCONTROL

ReefAngelClass ReefAngel = ReefAngelClass() ;
