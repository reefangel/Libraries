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

#include "RF.h"
#include <Globals.h>
#include <Wire.h>

RFClass::RFClass()
{
	UseMemory=true;
}

void RFClass::SetMode(byte mode, byte speed, byte duration)
{
    if (mode<Slave_Start)
    {
    	Mode=mode;
    	Speed=speed;
    	Duration=duration;
    	speed*=2.55;
    }
    Wire.beginTransmission(I2CRF); // transmit to device #16
    Wire.write('$');              // sends $
    Wire.write('$');              // sends $
    Wire.write('$');              // sends $
    Wire.write(mode);              // sends mode
    Wire.write(speed);              // sends speed
    Wire.write(duration);              // sends duration
    Wire.endTransmission();    // stop transmitting
}

byte RFClass::RFCheck()
{
    Wire.requestFrom(I2CRF,1);
    if (Wire.available())
    {
        return Wire.read();
    }
}

void RFClass::SetChannel(byte Channel, byte Value)
{
	if (Channel<RF_CHANNELS) RadionChannels[Channel]=Value;

}

void RFClass::RadionWrite()
{
	for (byte a=0;a<RF_CHANNELS;a++)
	{
		SetMode(Radion, RadionChannels[a]*2, a);
	}
	SetMode(Radion,0,Radion_Ready);
}

void RFClass::ChannelWhiteSlope()
{
	ChannelRadionSlope(Radion_White,InternalMemory.RadionSlopeStartW_read(),InternalMemory.RadionSlopeEndW_read(),InternalMemory.RadionSlopeDurationW_read());
}

void RFClass::ChannelRoyalBlueSlope()
{
	ChannelRadionSlope(Radion_RoyalBlue,InternalMemory.RadionSlopeStartRB_read(),InternalMemory.RadionSlopeEndRB_read(),InternalMemory.RadionSlopeDurationRB_read());
}

void RFClass::ChannelRedSlope()
{
	ChannelRadionSlope(Radion_Red,InternalMemory.RadionSlopeStartR_read(),InternalMemory.RadionSlopeEndR_read(),InternalMemory.RadionSlopeDurationR_read());
}

void RFClass::ChannelGreenSlope()
{
	ChannelRadionSlope(Radion_Green,InternalMemory.RadionSlopeStartG_read(),InternalMemory.RadionSlopeEndG_read(),InternalMemory.RadionSlopeDurationG_read());
}

void RFClass::ChannelBlueSlope()
{
	ChannelRadionSlope(Radion_Blue,InternalMemory.RadionSlopeStartB_read(),InternalMemory.RadionSlopeEndB_read(),InternalMemory.RadionSlopeDurationB_read());
}

void RFClass::ChannelIntensitySlope()
{
	ChannelRadionSlope(Radion_Intensity,InternalMemory.RadionSlopeStartI_read(),InternalMemory.RadionSlopeEndI_read(),InternalMemory.RadionSlopeDurationI_read());
}

void RFClass::ChannelWhiteSlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_White,InternalMemory.RadionSlopeStartW_read(),InternalMemory.RadionSlopeEndW_read(),InternalMemory.RadionSlopeDurationW_read(), MinuteOffset);
}

void RFClass::ChannelRoyalBlueSlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_RoyalBlue,InternalMemory.RadionSlopeStartRB_read(),InternalMemory.RadionSlopeEndRB_read(),InternalMemory.RadionSlopeDurationRB_read(), MinuteOffset);
}

void RFClass::ChannelRedSlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_Red,InternalMemory.RadionSlopeStartR_read(),InternalMemory.RadionSlopeEndR_read(),InternalMemory.RadionSlopeDurationR_read(), MinuteOffset);
}

void RFClass::ChannelGreenSlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_Green,InternalMemory.RadionSlopeStartG_read(),InternalMemory.RadionSlopeEndG_read(),InternalMemory.RadionSlopeDurationG_read(), MinuteOffset);
}

void RFClass::ChannelBlueSlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_Blue,InternalMemory.RadionSlopeStartB_read(),InternalMemory.RadionSlopeEndB_read(),InternalMemory.RadionSlopeDurationB_read(), MinuteOffset);
}

void RFClass::ChannelIntensitySlope(byte MinuteOffset)
{
	ChannelRadionSlope(Radion_Intensity,InternalMemory.RadionSlopeStartI_read(),InternalMemory.RadionSlopeEndI_read(),InternalMemory.RadionSlopeDurationI_read(), MinuteOffset);
}

void RFClass::ChannelRadionSlope(byte Channel, byte Start, byte End, byte Duration)
{
	SetChannel(Channel,PWMSlope(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End,
		Duration,
		RadionChannels[Channel]
	));
}

void RFClass::ChannelRadionSlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset)
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
		RadionChannels[Channel]
	));
}

void RFClass::ChannelWhiteParabola()
{
	ChannelRadionParabola(Radion_White,InternalMemory.RadionSlopeStartW_read(),InternalMemory.RadionSlopeEndW_read(),InternalMemory.RadionSlopeDurationW_read());
}

void RFClass::ChannelRoyalBlueParabola()
{
	ChannelRadionParabola(Radion_RoyalBlue,InternalMemory.RadionSlopeStartRB_read(),InternalMemory.RadionSlopeEndRB_read(),InternalMemory.RadionSlopeDurationRB_read());
}

void RFClass::ChannelRedParabola()
{
	ChannelRadionParabola(Radion_Red,InternalMemory.RadionSlopeStartR_read(),InternalMemory.RadionSlopeEndR_read(),InternalMemory.RadionSlopeDurationR_read());
}

void RFClass::ChannelGreenParabola()
{
	ChannelRadionParabola(Radion_Green,InternalMemory.RadionSlopeStartG_read(),InternalMemory.RadionSlopeEndG_read(),InternalMemory.RadionSlopeDurationG_read());
}

void RFClass::ChannelBlueParabola()
{
	ChannelRadionParabola(Radion_Blue,InternalMemory.RadionSlopeStartB_read(),InternalMemory.RadionSlopeEndB_read(),InternalMemory.RadionSlopeDurationB_read());
}

void RFClass::ChannelIntensityParabola()
{
	ChannelRadionParabola(Radion_Intensity,InternalMemory.RadionSlopeStartI_read(),InternalMemory.RadionSlopeEndI_read(),InternalMemory.RadionSlopeDurationI_read());
}

void RFClass::ChannelWhiteParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_White,InternalMemory.RadionSlopeStartW_read(),InternalMemory.RadionSlopeEndW_read(),InternalMemory.RadionSlopeDurationW_read(), MinuteOffset);
}

void RFClass::ChannelRoyalBlueParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_RoyalBlue,InternalMemory.RadionSlopeStartRB_read(),InternalMemory.RadionSlopeEndRB_read(),InternalMemory.RadionSlopeDurationRB_read(), MinuteOffset);
}

void RFClass::ChannelRedParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_Red,InternalMemory.RadionSlopeStartR_read(),InternalMemory.RadionSlopeEndR_read(),InternalMemory.RadionSlopeDurationR_read(), MinuteOffset);
}

void RFClass::ChannelGreenParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_Green,InternalMemory.RadionSlopeStartG_read(),InternalMemory.RadionSlopeEndG_read(),InternalMemory.RadionSlopeDurationG_read(), MinuteOffset);
}

void RFClass::ChannelBlueParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_Blue,InternalMemory.RadionSlopeStartB_read(),InternalMemory.RadionSlopeEndB_read(),InternalMemory.RadionSlopeDurationB_read(), MinuteOffset);
}

void RFClass::ChannelIntensityParabola(byte MinuteOffset)
{
	ChannelRadionParabola(Radion_Intensity,InternalMemory.RadionSlopeStartI_read(),InternalMemory.RadionSlopeEndI_read(),InternalMemory.RadionSlopeDurationI_read(), MinuteOffset);
}

void RFClass::ChannelRadionParabola(byte Channel, byte Start, byte End, byte Duration)
{
	SetChannel(Channel,PWMSlope(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End,
		Duration,
		RadionChannels[Channel]
	));
}

void RFClass::ChannelRadionParabola(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset)
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
		RadionChannels[Channel]
	));
}

