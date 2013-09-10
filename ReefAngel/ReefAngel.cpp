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

#if defined DisplayLEDPWM && ! defined RemoveAllLights
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

void ReefAngelClass::Init()
{
	Serial.begin(57600);
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
	TempSensor.Init();
	RAStart=now();
	LastStart = RAStart;  // Set the time normal mode is started
	BusLocked=false;  // Bus is not locked
	ChangeMode=0;
	Flags = 0;
	AlertFlags = 0;
	StatusFlags = 0;
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

#if defined wifi || defined I2CMASTER
	EM = PWMEbit + RFEbit + AIbit + Salbit + ORPbit + IObit + PHbit + WLbit;
	EM1 = HUMbit + DCPumpbit;

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
}

void ReefAngelClass::Refresh()
{
	WDTReset();
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

#ifdef DCPUMPCONTROL
	if (DCPump.UseMemory)
	{
		DCPump.Mode=InternalMemory.DCPumpMode_read();
		DCPump.Speed=InternalMemory.DCPumpSpeed_read();
		DCPump.Duration=InternalMemory.DCPumpDuration_read();
	}
	switch (DCPump.Mode)
	{
	case Constant:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(DCPump.Speed);
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(DCPump.Speed);
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,DCPump.Speed);
#endif // PWMEXPANSION
		break;
	}
	case Lagoon:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(ReefCrestMode(DCPump.Speed,10,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(ReefCrestMode(DCPump.Speed,10,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,ReefCrestMode(DCPump.Speed,10,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	case ReefCrest:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(ReefCrestMode(DCPump.Speed,20,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(ReefCrestMode(DCPump.Speed,20,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,ReefCrestMode(DCPump.Speed,20,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	case ShortPulse:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(ShortPulseMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(ShortPulseMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,ShortPulseMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	case LongPulse:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(LongPulseMode(0,DCPump.Speed,DCPump.Duration,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(LongPulseMode(0,DCPump.Speed,DCPump.Duration,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,LongPulseMode(0,DCPump.Speed,DCPump.Duration,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	case NutrientTransport:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(NutrientTransportMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(NutrientTransportMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,NutrientTransportMode(0,DCPump.Speed,DCPump.Duration*10,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	case TidalSwell:
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(TidalSwellMode(DCPump.Speed,DCPump.DaylightChannel-1));
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(TidalSwellMode(DCPump.Speed,DCPump.ActinicChannel-1));
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,TidalSwellMode(DCPump.Speed,DCPump.ExpansionChannel[a]-1));
#endif // PWMEXPANSION
		break;
	}
	}
	if (DisplayedMenu==FEEDING_MODE)
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(DCPump.FeedingSpeed);
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(DCPump.FeedingSpeed);
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,DCPump.FeedingSpeed);
#endif // PWMEXPANSION
	}
	if (DisplayedMenu==WATERCHANGE_MODE)
	{
		if (DCPump.DaylightChannel!=None)
			PWM.SetDaylight(DCPump.WaterChangeSpeed);
		if (DCPump.ActinicChannel!=None)
			PWM.SetActinic(DCPump.WaterChangeSpeed);
#ifdef PWMEXPANSION
		for (int a=0; a<PWM_EXPANSION_CHANNELS;a++)
			if (DCPump.ExpansionChannel[a]!=None)
				PWM.SetChannel(a,DCPump.WaterChangeSpeed);
#endif // PWMEXPANSION
	}
#endif  // DCPUMPCONTROL

#if defined DisplayLEDPWM && !defined REEFANGEL_MINI
	if (LightRelayOn && LightsOverride)
	{
		PWM.SetActinic(InternalMemory.LEDPWMActinic_read());
		PWM.SetDaylight(InternalMemory.LEDPWMDaylight_read());
#if defined RA_STAR
		PWM.SetActinic2(InternalMemory.LEDPWMActinic2_read());
		PWM.SetDaylight2(InternalMemory.LEDPWMDaylight2_read());
#endif // RA_STAR
	}
	// issue #3: Redundant code
	// issue #12: Revert back
	analogWrite(actinicPWMPin, PWM.GetActinicValue()*2.55);
	analogWrite(daylightPWMPin, PWM.GetDaylightValue()*2.55);
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI

#if defined RA_STAR
	analogWrite(actinic2PWMPin, PWM.GetActinic2Value()*2.55);
	analogWrite(daylight2PWMPin, PWM.GetDaylight2Value()*2.55);
#endif // RA_STAR

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
		bitClear(AlertFlags,BusLockFlag);
	else
		bitSet(AlertFlags,BusLockFlag);

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
	PWM.ExpansionWrite();
#endif  // PWMEXPANSION
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

#ifdef RA_STAR
	// Check for DHCP state changes
	EthernetDHCP.poll();
	if (WIFI_SERIAL.available() && PortalConnection)
	{
		while(WIFI_SERIAL.available())
		{
			wdt_reset();
			char c = WIFI_SERIAL.read();
		}
	}

	// if the server's disconnected, stop the client
	if (!WIFI_SERIAL.connected() && PortalConnection)
	{
		PortalConnection=false;
		WIFI_SERIAL.stop();
	}

	// if request timed out, stop the client
	if (WIFI_SERIAL.connected() && PortalConnection && millis()-PortalTimeOut>PORTAL_TIMEOUT)
	{
		PortalConnection=false;
		WIFI_SERIAL.stop();
	}
#endif // RA_STAR

#ifdef RANET
	// Send RANet data
	if (millis()-RANetlastmillis>RANetDelay)
	{
		RANetlastmillis=millis();
		RANetCRC=0;
		RANetData[0]=RANetSeq;
		RANetData[1]=RANET_SIZE*2;
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
			RANetData[18+a]=PWM.GetChannel(a);
#else
			RANetData[18+a]=0;
#endif // PWMEXPANSION
		}
		char buf[3];
		for (int a=0;a<RANET_SIZE;a++)
		{
			RANetCRC+=RANetData[a];
			sprintf(buf,"%02x",RANetData[a]);
			RANET_SERIAL.print(buf);
		}
		sprintf(buf,"%02x",RANetCRC);
		RANET_SERIAL.println(buf);
		RANetSeq++;
	}
#endif // RANET

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
#if defined HUMIDITYEXPANSION
	Humidity.Read();
#endif  // defined HUMIDITYEXPANSION
	OverheatCheck();
#ifdef LEAKDETECTOREXPANSION
	LeakCheck();
#endif  // LEAKDETECTOREXPANSION
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
	}
	else
	{
		BusLocked=false;  // Bus is not locked
		bitClear(AlertFlags,BusLockFlag);
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
#ifdef LEAKDETECTOREXPANSION
boolean ReefAngelClass::IsLeakDetected()
{
	int iLeak=0;
	Wire.requestFrom(I2CLeak, 2);
	if (Wire.available())
	{
		iLeak = Wire.read();
		iLeak = iLeak<<8;
		iLeak += Wire.read();
	}
	return iLeak>2000?true:false;
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

void ReefAngelClass::FeedingModeStart()
{
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
	RF.SetMode(Feeding_Start,0,0);
#endif  // RFEXPANSION
	Timer[FEEDING_TIMER].Start();  //Start Feeding Mode timer
	// Tell controller what mode we are in
	SetDisplayedMenu(FEEDING_MODE);
}

void ReefAngelClass::WaterChangeModeStart()
{
	// turn off ports
	Relay.RelayMaskOff &= ~WaterChangePorts;
#ifdef RelayExp
	byte i;
	for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
	{
		Relay.RelayMaskOffE[i] &= ~WaterChangePortsE[i];
	}
#endif  // RelayExp
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
#ifdef WATERLEVELEXPANSION
	WLATO.StopTopping();
#endif // WATERLEVELEXPANSION
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
	if ( Params.Temp[OverheatProbe] < InternalMemory.OverheatTemp_read() )
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
	PWM.SetActinic(0);
	PWM.SetDaylight(0);
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
	WDTReset();
	ClearScreen(DefaultBGColor);
	Timer[LCD_TIMER].Start();
	SetDisplayedMenu(DEFAULT_MENU);
	CheckDrawGraph();
}

void ReefAngelClass::SetDisplayedMenu(byte value)
{
	DisplayedMenu=value;
#ifdef RA_TOUCHDISPLAY
	SendMaster(MESSAGE_MENU,value,value); 	// Change Menu
#endif // RA_TOUCHDISPLAY
}

#ifdef RA_TOUCHDISPLAY
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
				ReefAngel.AlertFlags=d[8];
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

#endif //  RA_TOUCHDISPLAY

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

	// ID 1 - R, RON, ROFF, ATO, EM, REM, DisplayedMenu and AlertFlags
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
	Wire.write(AlertFlags);
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

ReefAngelClass ReefAngel = ReefAngelClass() ;
