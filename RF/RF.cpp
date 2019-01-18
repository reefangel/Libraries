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
	VortechEnable=true;
	lastcrc=-1;
    FeedingSpeed=UCHAR_MAX;
    WaterChangeSpeed=UCHAR_MAX;
	for ( byte a = 0; a < RF_CHANNELS; a++ )
	{
		RadionChannels[a]=0;
		RadionChannelsOverride[a]=UCHAR_MAX;
	}
}

void RFClass::SendData(byte mode, byte speed, byte duration)
{
	Wire.beginTransmission(I2CRF); // transmit to device #16
	Wire.write('$');              // sends $
	Wire.write('$');              // sends $
	Wire.write('$');              // sends $
	Wire.write(mode);              // sends mode
	Wire.write(speed);              // sends speed
	Wire.write(duration);              // sends duration
	Wire.endTransmission();    // stop transmitting
}

void RFClass::SetMode(byte mode, byte speed, byte duration)
{
	if (VortechEnable)
	{
		if (mode<Slave_Start && mode!=TurnOn)
		{
			Mode=mode;
			Speed=speed;
			Duration=duration;
			speed*=2.55;
			if (millis()%60000<200) lastcrc=-1;
			if ( (lastcrc!=(byte)(mode+speed+duration)) || millis()<5000 )
			{
				SendData(mode, speed, duration);
				lastcrc=mode+speed+duration;
			}
		}
		if (mode==TurnOff) VortechEnable=false;
	}
	else
	{
		if (mode==TurnOn)
		{
			VortechEnable=true;
			SendData(mode, speed, duration);
		}
	}
}

byte RFClass::RFCheck()
{
    Wire.requestFrom(I2CRF,1);
    if (Wire.available())
    {
        return Wire.read();
    }
}

byte RFClass::GetChannel(byte Channel)
{
	if (RadionChannelsOverride[Channel]<=100)
		return RadionChannelsOverride[Channel];
	else
		return RadionChannels[Channel];
}

void RFClass::RadionWrite()
{
	if (millis()-lastWrite>3000)
	{
		lastWrite=millis();
		for (byte a=0;a<RF_CHANNELS;a++)
			SendData(Radion, GetChannel(a)*2, a);
		SendData(Radion,0,Radion_Ready);
	}
}

void RFClass::RadionSlope()
{
	ChannelWhiteSlope();
	ChannelRoyalBlueSlope();
	ChannelRedSlope();
	ChannelGreenSlope();
	ChannelBlueSlope();
	ChannelIntensitySlope();
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

void RFClass::RadionSlope(byte MinuteOffset)
{
	ChannelWhiteSlope(MinuteOffset);
	ChannelRoyalBlueSlope(MinuteOffset);
	ChannelRedSlope(MinuteOffset);
	ChannelGreenSlope(MinuteOffset);
	ChannelBlueSlope(MinuteOffset);
	ChannelIntensitySlope(MinuteOffset);
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

void RFClass::RadionParabola()
{
	ChannelWhiteParabola();
	ChannelRoyalBlueParabola();
	ChannelRedParabola();
	ChannelGreenParabola();
	ChannelBlueParabola();
	ChannelIntensityParabola();
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

void RFClass::RadionParabola(byte MinuteOffset)
{
	ChannelWhiteParabola(MinuteOffset);
	ChannelRoyalBlueParabola(MinuteOffset);
	ChannelRedParabola(MinuteOffset);
	ChannelGreenParabola(MinuteOffset);
	ChannelBlueParabola(MinuteOffset);
	ChannelIntensityParabola(MinuteOffset);
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
	SetChannel(Channel,PWMParabola(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End,
		RadionChannels[Channel]
	));
}

void RFClass::ChannelRadionParabola(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset)
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
		RadionChannels[Channel]
	));
}
