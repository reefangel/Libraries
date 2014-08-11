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

#include "RA_PWM.h"
#include <Globals.h>
#include <Wire.h>

RA_PWMClass::RA_PWMClass()
{
    // initialize variables
    ActinicPWMValue = 0;
    DaylightPWMValue = 0;
    ActinicPWMOverride = 255; // Anything over 100 disables override
    DaylightPWMOverride = 255; // Anything over 100 disables override
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
    Actinic2PWMValue = 0;
    Daylight2PWMValue = 0;
    Actinic2PWMOverride = 255; // Anything over 100 disables override
    Daylight2PWMOverride = 255; // Anything over 100 disables override
#endif
    LightsOverride = true; // Enable PWM override when lights on
#ifdef PWMEXPANSION
    lastcrc=-1;
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		ExpansionChannel[a]=0;
		ExpansionChannelOverride[a]=255;
	}
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
    lastcrc=-1;
    for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
    {
        SIXTEENChExpansionChannel[a]=0;
        SIXTEENChExpansionChannelOverride[a]=255;
    }
#endif  // SIXTEENCHPWMEXPANSION
}

byte RA_PWMClass::GetActinicValue()
{
	if (ActinicPWMOverride<=100)
		return ActinicPWMOverride;
	else
		return (byte)(ActinicPWMValue/40.95);
}

int RA_PWMClass::GetActinicValueRaw()
{
	if (ActinicPWMOverride<=100)
		return (int)ActinicPWMOverride*40.95;
	else
		return ActinicPWMValue;
}

byte RA_PWMClass::GetDaylightValue()
{
	if (DaylightPWMOverride<=100)
		return DaylightPWMOverride;
	else
		return (byte)(DaylightPWMValue/40.95);
}

int RA_PWMClass::GetDaylightValueRaw()
{
	if (DaylightPWMOverride<=100)
		return (int)DaylightPWMOverride*40.95;
	else
		return DaylightPWMValue;
}

void RA_PWMClass::ActinicPWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			InternalMemory.PWMSlopeDurationA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSlope()
{
	ActinicPWMSlope(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMSlope()
{
	SetDaylight(PWMSlopeHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMParabola()
{
	ActinicPWMParabola(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMParabola()
{
	SetDaylight(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSmoothRamp(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			InternalMemory.PWMSlopeDurationA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMSmoothRamp(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSmoothRamp()
{
	ActinicPWMSmoothRamp(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMSmoothRamp()
{
	SetDaylight(PWMSmoothRampHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::StandardActinic()
{
	SetDaylight(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMActinic_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight()
{
	SetDaylight(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMDaylight_read(),
			0
			));
}

void RA_PWMClass::StandardActinic(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMActinic_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMDaylight_read(),
			0
			));
}

void RA_PWMClass::Override(byte Channel, byte Value)
{
	if (Channel==OVERRIDE_DAYLIGHT) // Daylight channel
		SetDaylightOverride(Value);
	else if (Channel==OVERRIDE_ACTINIC) // Actinic channel
		SetActinicOverride(Value);
#ifdef PWMEXPANSION
	else if (Channel>=OVERRIDE_CHANNEL0 && Channel<=OVERRIDE_CHANNEL5) // Dimming Expansion channel 0-5
		SetChannelOverride(Channel-OVERRIDE_CHANNEL0,Value);
#endif // PWMEXPANSION
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	else if (Channel==OVERRIDE_DAYLIGHT2) // Daylight2 channel
		SetDaylight2Override(Value);
	else if (Channel==OVERRIDE_ACTINIC2) // Actinic2 channel
		SetActinic2Override(Value);
#endif
}

#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
byte RA_PWMClass::GetActinic2Value()
{
	if (Actinic2PWMOverride<=100)
		return Actinic2PWMOverride;
	else
		return (byte)(Actinic2PWMValue/40.95);
}

int RA_PWMClass::GetActinic2ValueRaw()
{
	if (Actinic2PWMOverride<=100)
		return (int)Actinic2PWMOverride*40.95;
	else
		return Actinic2PWMValue;
}

byte RA_PWMClass::GetDaylight2Value()
{
	if (Daylight2PWMOverride<=100)
		return Daylight2PWMOverride;
	else
		return (byte)(Daylight2PWMValue/40.95);
}

int RA_PWMClass::GetDaylight2ValueRaw()
{
	if (Daylight2PWMOverride<=100)
		return (int)Daylight2PWMOverride*40.95;
	else
		return Daylight2PWMValue;
}

void RA_PWMClass::Actinic2PWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic2(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			InternalMemory.PWMSlopeDurationA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight2(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSlope()
{
	Actinic2PWMSlope(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMSlope()
{
	SetDaylight2(PWMSlopeHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic2(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight2(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMParabola()
{
	Actinic2PWMParabola(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMParabola()
{
	SetDaylight2(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSmoothRamp(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic2(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			InternalMemory.PWMSlopeDurationA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMSmoothRamp(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight2(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSmoothRamp()
{
	Actinic2PWMSmoothRamp(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMSmoothRamp()
{
	SetDaylight2(PWMSmoothRampHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::StandardActinic2()
{
	SetDaylight2(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMActinic2_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight2()
{
	SetDaylight2(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMDaylight2_read(),
			0
			));
}

void RA_PWMClass::StandardActinic2(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic2(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMActinic2_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight2(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight2(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMDaylight2_read(),
			0
			));
}
#endif //defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)

#ifdef PWMEXPANSION

void RA_PWMClass::Expansion(byte cmd, byte data)
{
        // if you're sending in bytes, we assume it's a percentage
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write((int)(data*2.55));			// send the data
	Present=Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
	// Also send data to new module PCA9685
	int newdata=(int)(data*40.95);
    Wire.beginTransmission(I2CPWM_PCA9685);
    Wire.write(0x8+(4*cmd));
    Wire.write(newdata&0xff);
    Wire.write(newdata>>8);
    Wire.endTransmission();
}

void RA_PWMClass::Expansion(byte cmd, int data)
{
        // assume if you're sending integers from 0 to 4095 in you're not sending percentages
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write(map(data,0,4095,0,255));			// send the data to the older chip after mapping to 255 bits
	Present=Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
	// Also send data to new module PCA9685
    Wire.beginTransmission(I2CPWM_PCA9685);
    Wire.write(0x8+(4*cmd));
    Wire.write(data&0xff);
    Wire.write(data>>8);
    Wire.endTransmission();
}

void RA_PWMClass::ExpansionSetPercent(byte p)
{
	// loop through all 6 channels and send the value
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		Expansion(a, p);
	}
}

void RA_PWMClass::ExpansionWrite()
{
	byte thiscrc=0;
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
		thiscrc+=GetChannelValueRaw(a)*(a+1);
	if (millis()%60000<200) lastcrc=-1;
	if (lastcrc!=thiscrc || millis()<5000)
	{
		lastcrc=thiscrc;
		// setup PCA9685 for data receive
		// we need this to make sure it will work if connected ofter controller is booted, so we need to send it all the time.
		Wire.beginTransmission(I2CPWM_PCA9685);
		Wire.write(0);
		Wire.write(0xa1);
		Wire.endTransmission();
		for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
		{
			Expansion(a,GetChannelValueRaw(a));
		}
	}
}

byte RA_PWMClass::GetChannelValue(byte Channel)
{
	if (ExpansionChannelOverride[Channel]<=100)
		return ExpansionChannelOverride[Channel];
	else
		return (byte)(ExpansionChannel[Channel]/40.95);
}

int RA_PWMClass::GetChannelValueRaw(byte Channel)
{
	if (ExpansionChannelOverride[Channel]<=100)
		return (int)ExpansionChannelOverride[Channel]*40.95;
	else
		return ExpansionChannel[Channel];
}

void RA_PWMClass::Channel0PWMSlope()
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read());
}

void RA_PWMClass::Channel1PWMSlope()
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read());
}

void RA_PWMClass::Channel2PWMSlope()
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read());
}

void RA_PWMClass::Channel3PWMSlope()
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read());
}

void RA_PWMClass::Channel4PWMSlope()
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read());
}

void RA_PWMClass::Channel5PWMSlope()
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read());
}

void RA_PWMClass::Channel0PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read(), MinuteOffset);
}

void RA_PWMClass::Channel1PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read(), MinuteOffset);
}

void RA_PWMClass::Channel2PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read(), MinuteOffset);
}

void RA_PWMClass::Channel3PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read(), MinuteOffset);
}

void RA_PWMClass::Channel4PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read(), MinuteOffset);
}

void RA_PWMClass::Channel5PWMSlope(byte MinuteOffset)
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read(), MinuteOffset);
}

void RA_PWMClass::ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration)
{
	SetChannel(Channel,PWMSlopeHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		Duration,  
		ExpansionChannel[Channel] 
	));	
}


void RA_PWMClass::ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetChannel(Channel,PWMSlopeHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		Duration,  
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::Channel0PWMParabola()
{
	ChannelPWMParabola(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read());
}

void RA_PWMClass::Channel1PWMParabola()
{
	ChannelPWMParabola(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read());
}

void RA_PWMClass::Channel2PWMParabola()
{
	ChannelPWMParabola(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read());
}

void RA_PWMClass::Channel3PWMParabola()
{
	ChannelPWMParabola(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read());
}

void RA_PWMClass::Channel4PWMParabola()
{
	ChannelPWMParabola(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read());
}

void RA_PWMClass::Channel5PWMParabola()
{
	ChannelPWMParabola(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read());
}

void RA_PWMClass::Channel0PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(), MinuteOffset);
}

void RA_PWMClass::Channel1PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(), MinuteOffset);
}

void RA_PWMClass::Channel2PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(), MinuteOffset);
}

void RA_PWMClass::Channel3PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(), MinuteOffset);
}

void RA_PWMClass::Channel4PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(), MinuteOffset);
}

void RA_PWMClass::Channel5PWMParabola(byte MinuteOffset)
{
	ChannelPWMParabola(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(), MinuteOffset);
}

void RA_PWMClass::ChannelPWMParabola(byte Channel, byte Start, byte End)
{
	SetChannel(Channel,PWMParabolaHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::ChannelPWMParabola(byte Channel, byte Start, byte End, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetChannel(Channel,PWMParabolaHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::Channel0PWMSmoothRamp()
{
	ChannelPWMSmoothRamp(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read());
}

void RA_PWMClass::Channel1PWMSmoothRamp()
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read());
}

void RA_PWMClass::Channel2PWMSmoothRamp()
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read());
}

void RA_PWMClass::Channel3PWMSmoothRamp()
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read());
}

void RA_PWMClass::Channel4PWMSmoothRamp()
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read());
}

void RA_PWMClass::Channel5PWMSmoothRamp()
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read());
}

void RA_PWMClass::Channel0PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read(), MinuteOffset);
}

void RA_PWMClass::Channel1PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read(), MinuteOffset);
}

void RA_PWMClass::Channel2PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read(), MinuteOffset);
}

void RA_PWMClass::Channel3PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read(), MinuteOffset);
}

void RA_PWMClass::Channel4PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read(), MinuteOffset);
}

void RA_PWMClass::Channel5PWMSmoothRamp(byte MinuteOffset)
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read(), MinuteOffset);
}


void RA_PWMClass::ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength)
{
	SetChannel(Channel,PWMSmoothRampHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SlopeLength, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetChannel(Channel,PWMSmoothRampHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
                SlopeLength,
		ExpansionChannel[Channel] 
	));	
}

#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION

void RA_PWMClass::SIXTEENChExpansion(byte channel, byte data)
{
    // the data is in byte, so it's assumed to be a percentage, like from a pump
    if (channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[channel]=data;
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0);
    Wire.write(0xa1);
    Wire.endTransmission();
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    int newdata = (int)data*40.95;
    Wire.write(0x8+(4*channel));
    Wire.write(newdata&0xff);
    Wire.write(newdata>>8);
    Wire.endTransmission();
}

void RA_PWMClass::SIXTEENChExpansion(byte channel, int data)
{
    // the data is in int, so just send that int to the module
    if (channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[channel]=data;
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0);
    Wire.write(0xa1);
    Wire.endTransmission();
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0x8+(4*channel));
    Wire.write(data&0xff);
    Wire.write(data>>8);
    Wire.endTransmission();
}

void RA_PWMClass::SIXTEENChExpansionSetPercent(byte p)
{
	// loop through all 16 channels and send the value
	for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
	{
		SIXTEENChExpansion(a, p);
	}
}

void RA_PWMClass::SIXTEENChExpansionWrite()
{
	byte thiscrc=0;
	for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
		thiscrc+=Get16ChannelValueRaw(a)*(a+1);
	if (millis()%60000<200) lastcrc=-1;
	if (lastcrc!=thiscrc || millis()<5000)
	{
		lastcrc=thiscrc;
		// setup PCA9685 for data receive
		// we need this to make sure it will work if connected ofter controller is booted, so we need to send it all the time.
		Wire.beginTransmission(I2CPWM_16CH_PCA9685);
		Wire.write(0);
		Wire.write(0xa1);
		Wire.endTransmission();
		for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
		{
			SIXTEENChExpansion(a,Get16ChannelValueRaw(a));
		}
	}
}

byte RA_PWMClass::Get16ChannelValue(byte Channel)
{
	if (SIXTEENChExpansionChannelOverride[Channel]<=100)
		return SIXTEENChExpansionChannelOverride[Channel];
	else
		return (byte)(SIXTEENChExpansionChannel[Channel]/40.95);
}

int RA_PWMClass::Get16ChannelValueRaw(byte Channel)
{
	if (SIXTEENChExpansionChannelOverride[Channel]<=100)
		return (int)SIXTEENChExpansionChannelOverride[Channel]*40.95;
	else
		return SIXTEENChExpansionChannel[Channel];
}

void RA_PWMClass::SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration)
{
	Set16Channel(Channel,PWMSlopeHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		Duration,  
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	Set16Channel(Channel,PWMSlopeHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		Duration,  
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMParabola(byte Channel, int Start, int End)
{
	Set16Channel(Channel,PWMParabolaHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMParabola(byte Channel, int Start, int End, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	Set16Channel(Channel,PWMParabolaHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength)
{
	Set16Channel(Channel,PWMSmoothRampHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SlopeLength, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	Set16Channel(Channel,PWMSmoothRampHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
                SlopeLength,
		SIXTEENChExpansionChannel[Channel] 
	));	
}

#endif //SIXTEENCHPWMEXPANSION

