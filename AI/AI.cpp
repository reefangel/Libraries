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

#include "AI.h"
#include <Globals.h>


AIClass::AIClass()
{
	AIChannels[0]=0;
	AIChannels[1]=0;
	AIChannels[2]=0;
	StreamDelay=300;
	AImillis=millis();
}

void AIClass::SetPort(byte portnum)
{
	port=portnum;
	pinMode(port,OUTPUT);
	digitalWrite(port,LOW);
}

void AIClass::SetChannel(byte channel, byte intensity)
{
	AIChannels[channel]=intensity;
}

byte AIClass::GetChannel(byte channel)
{
	return AIChannels[channel];
}

void AIClass::SetStreamDelay(int delay)
{
	StreamDelay=delay;
}

void AIClass::Send()
{
	for (byte a=0;a<AI_CHANNELS;a++)
	{
		SWprint(164);
		SWprint(161);
		SWprint(114);
		SWprint(RawChannel[a]);
		SWprint(AIChannels[a]*2.55);
	}
}

void AIClass::SWprint(byte data)
{
  byte mask;
  //startbit
  digitalWrite(port,HIGH);
  delayMicroseconds(bit9600Delay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ // choose bit
     digitalWrite(port,LOW); // send 1
    }
    else{
     digitalWrite(port,HIGH); // send 0
    }
    delayMicroseconds(bit9600Delay);
  }
  //stop bit
  digitalWrite(port, LOW);
  delayMicroseconds(bit9600Delay);
}

void AIClass::ChannelWhiteSlope()
{
	ChannelAISlope(White,InternalMemory.AISlopeStartW_read(),InternalMemory.AISlopeEndW_read(),InternalMemory.AISlopeDurationW_read());
}

void AIClass::ChannelBlueSlope()
{
	ChannelAISlope(Blue,InternalMemory.AISlopeStartB_read(),InternalMemory.AISlopeEndB_read(),InternalMemory.AISlopeDurationB_read());
}

void AIClass::ChannelRoyalBlueSlope()
{
	ChannelAISlope(RoyalBlue,InternalMemory.AISlopeStartRB_read(),InternalMemory.AISlopeEndRB_read(),InternalMemory.AISlopeDurationRB_read());
}

void AIClass::ChannelWhiteSlope(byte MinuteOffset)
{
	ChannelAISlope(White,InternalMemory.AISlopeStartW_read(),InternalMemory.AISlopeEndW_read(),InternalMemory.AISlopeDurationW_read(), MinuteOffset);
}

void AIClass::ChannelBlueSlope(byte MinuteOffset)
{
	ChannelAISlope(Blue,InternalMemory.AISlopeStartB_read(),InternalMemory.AISlopeEndB_read(),InternalMemory.AISlopeDurationB_read(), MinuteOffset);
}

void AIClass::ChannelRoyalBlueSlope(byte MinuteOffset)
{
	ChannelAISlope(RoyalBlue,InternalMemory.AISlopeStartRB_read(),InternalMemory.AISlopeEndRB_read(),InternalMemory.AISlopeDurationRB_read(), MinuteOffset);
}

void AIClass::ChannelAISlope(byte Channel, byte Start, byte End, byte Duration)
{
	SetChannel(Channel,PWMSlope(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End,
		Duration,
		AIChannels[Channel]
	));
}

void AIClass::ChannelAISlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset)
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
		AIChannels[Channel]
	));
}

void AIClass::ChannelWhiteParabola()
{
	ChannelAIParabola(White,InternalMemory.AISlopeStartW_read(),InternalMemory.AISlopeEndW_read());
}

void AIClass::ChannelBlueParabola()
{
	ChannelAIParabola(Blue,InternalMemory.AISlopeStartB_read(),InternalMemory.AISlopeEndB_read());
}

void AIClass::ChannelRoyalBlueParabola()
{
	ChannelAIParabola(RoyalBlue,InternalMemory.AISlopeStartRB_read(),InternalMemory.AISlopeEndRB_read());
}

void AIClass::ChannelWhiteParabola(byte MinuteOffset)
{
	ChannelAIParabola(White,InternalMemory.AISlopeStartW_read(),InternalMemory.AISlopeEndW_read(), MinuteOffset);
}

void AIClass::ChannelBlueParabola(byte MinuteOffset)
{
	ChannelAIParabola(Blue,InternalMemory.AISlopeStartB_read(),InternalMemory.AISlopeEndB_read(), MinuteOffset);
}

void AIClass::ChannelRoyalBlueParabola(byte MinuteOffset)
{
	ChannelAIParabola(RoyalBlue,InternalMemory.AISlopeStartRB_read(),InternalMemory.AISlopeEndRB_read(), MinuteOffset);
}

void AIClass::ChannelAIParabola(byte Channel, byte Start, byte End)
{
	SetChannel(Channel,PWMParabola(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End,
		AIChannels[Channel]
	));
}

void AIClass::ChannelAIParabola(byte Channel, byte Start, byte End, byte MinuteOffset)
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
		AIChannels[Channel]
	));
}

