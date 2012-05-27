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
    LightsOverride = false;
#ifdef PWMEXPANSION
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		ExpansionChannel[a]=0;
	}
#endif  // PWMEXPANSION
}

void RA_PWMClass::SetActinic(byte value)
{
    ActinicPWMValue = value;

}

void RA_PWMClass::SetDaylight(byte value)
{
    DaylightPWMValue = value;
}

void RA_PWMClass::ActinicPWMSlope(byte MinuteOffset)
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

void RA_PWMClass::DaylightPWMSlope(byte MinuteOffset)
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

void RA_PWMClass::ActinicPWMSlope()
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

void RA_PWMClass::DaylightPWMSlope()
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

void RA_PWMClass::ActinicPWMParabola(byte MinuteOffset)
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

void RA_PWMClass::DaylightPWMParabola(byte MinuteOffset)
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

void RA_PWMClass::ActinicPWMParabola()
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

void RA_PWMClass::DaylightPWMParabola()
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

void RA_PWMClass::StandardActinic()
{
	SetDaylight(PWMParabola(
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
	SetDaylight(PWMParabola(
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
	SetActinic(PWMParabola(
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
	SetDaylight(PWMParabola(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMDaylight_read(),
			0
			));
}

#ifdef PWMEXPANSION

void RA_PWMClass::SetChannel(byte Channel, byte Value)
{
	if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=Value;
}

void RA_PWMClass::Expansion(byte cmd, byte data)
{
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write(data*2.55);			// send the data
	Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
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
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		Expansion(a,ExpansionChannel[a]);
	}	
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
	SetChannel(Channel,PWMSlope(
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
	SetChannel(Channel,PWMSlope(
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
	SetChannel(Channel,PWMParabola(
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
	SetChannel(Channel,PWMParabola(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

#endif  // PWMEXPANSION
