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
	void SetPort(byte portnum);
	void SetChannel(byte channel, byte intensity);
	byte GetChannel(byte channel);
	void SetStreamDelay(int delay);
	int StreamDelay;
	unsigned long AImillis;
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
	
private:
	byte port;
	byte AIChannels[AI_CHANNELS];
	void SWprint(byte data);
};

#endif  // __REEFANGEL_AI_H__
