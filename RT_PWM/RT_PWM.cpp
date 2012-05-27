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

#include "RT_PWM.h"
#include <Globals.h>
#include <Wire.h>

RT_PWMClass::RT_PWMClass()
{
    // initialize variables
    ActinicPWMValue = 0;
    DaylightPWMValue = 0;
#ifdef PWMEXPANSION
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		ExpansionChannel[a]=0;
	}
#endif  // PWMEXPANSION
}

void RT_PWMClass::SetActinic(byte value)
{
    analogWrite(actinicPWMPin, value*2.55);
    ActinicPWMValue = value;

}

void RT_PWMClass::SetDaylight(byte value)
{
    analogWrite(daylightPWMPin, value*2.55);
    DaylightPWMValue = value;
}

void RT_PWMClass::ActinicPWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMSlope(
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

void RT_PWMClass::DaylightPWMSlope(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMSlope(
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

void RT_PWMClass::ActinicPWMSlope()
{
	SetActinic(PWMSlope(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			InternalMemory.PWMSlopeDurationA_read(),
			ActinicPWMValue
			));
}

void RT_PWMClass::DaylightPWMSlope()
{
	SetDaylight(PWMSlope(
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

void RT_PWMClass::ActinicPWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetActinic(PWMParabola(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			ActinicPWMValue
			));
}

void RT_PWMClass::DaylightPWMParabola(byte MinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-MinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+MinuteOffset;
	SetDaylight(PWMParabola(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RT_PWMClass::ActinicPWMParabola()
{
	SetActinic(PWMParabola(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			ActinicPWMValue
			));
}

void RT_PWMClass::DaylightPWMParabola()
{
	SetDaylight(PWMParabola(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

#ifdef PWMEXPANSION

void RT_PWMClass::SetChannel(byte Channel, byte Value)
{
	if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=Value;
}

void RT_PWMClass::Expansion(byte cmd, byte data)
{
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write(data);			// send the data
	Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
}

void RT_PWMClass::ExpansionSetPercent(byte p)
{
	// loop through all 6 channels and send the value
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		Expansion(a, int(2.55*p));
	}
}

void RT_PWMClass::ExpansionWrite()
{
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		Expansion(a,ExpansionChannel[a]*2.55);
	}	
}

#endif  // PWMEXPANSION
