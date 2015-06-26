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

#ifndef __AI_H__
#define __AI_H__

#include <Globals.h>
#include <InternalEEPROM.h>

class AIClass
{
public:
	AIClass();
	byte AIChannels[AI_CHANNELS];
	byte AIChannelsOverride[AI_CHANNELS];
	int StreamDelay;
	unsigned long AImillis;
	void SetPort(byte portnum);
	void inline SetChannel(byte Channel, byte Value) { if (Channel<AI_CHANNELS) AIChannels[Channel]=Value; };
	void inline SetChannelOverride(byte Channel, byte Value) { if (Value>100) Value=255; if (Channel<AI_CHANNELS) AIChannelsOverride[Channel]=Value; };
	byte GetChannel(byte Channel);
	byte inline GetOverrideChannel(byte Channel) { return AIChannelsOverride[Channel]; };
	void inline SetStreamDelay(int delay) { StreamDelay=delay; };
	void Send();
	void ChannelWhiteSlope();	
	void ChannelBlueSlope();	
	void ChannelRoyalBlueSlope();	
	void ChannelWhiteSlope(byte MinuteOffset);	
	void ChannelBlueSlope(byte MinuteOffset);	
	void ChannelRoyalBlueSlope(byte MinuteOffset);	
	void ChannelAISlope(byte Channel, byte Start, byte End, byte Duration);
	void ChannelAISlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset);
	void ChannelWhiteParabola();	
	void ChannelBlueParabola();	
	void ChannelRoyalBlueParabola();	
	void ChannelWhiteParabola(byte MinuteOffset);	
	void ChannelBlueParabola(byte MinuteOffset);	
	void ChannelRoyalBlueParabola(byte MinuteOffset);	
	void ChannelAIParabola(byte Channel, byte Start, byte End);
	void ChannelAIParabola(byte Channel, byte Start, byte End, byte MinuteOffset);	
	void inline Override(byte Channel, byte Value) { SetChannelOverride(Channel,Value); };
	
private:
	byte port;
	void SWprint(byte data);
};

#endif  // __REEFANGEL_AI_H__
