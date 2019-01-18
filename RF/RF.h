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

#ifndef __RF_H__
#define __RF_H__

#include <limits.h>
#include <Globals.h>
#include <InternalEEPROM.h>

class RFClass
{
public:
	RFClass();
	boolean UseMemory;
	byte Mode;
	byte Speed;
	byte Duration;
    byte FeedingSpeed;
    byte WaterChangeSpeed;
	boolean VortechEnable;
	byte RadionChannels[RF_CHANNELS];
	byte RadionChannelsOverride[RF_CHANNELS];
	void SetMode(byte mode, byte speed, byte duration);
	void SendData(byte mode, byte speed, byte duration);
	byte RFCheck();
	void inline SetChannel(byte Channel, byte Value) { if (Channel<RF_CHANNELS) RadionChannels[Channel]=Value; };
	void inline SetChannelOverride(byte Channel, byte Value) { if (Value>100) Value=UCHAR_MAX; if (Channel<RF_CHANNELS) RadionChannelsOverride[Channel]=Value; };
	byte GetChannel(byte Channel);
	byte inline GetOverrideChannel(byte Channel) { return RadionChannelsOverride[Channel]; };
	void RadionWrite();
	void RadionSlope();
	void ChannelWhiteSlope();
	void ChannelRoyalBlueSlope();
	void ChannelRedSlope();
	void ChannelGreenSlope();
	void ChannelBlueSlope();
	void ChannelIntensitySlope();
	void RadionSlope(byte MinuteOffset);
	void ChannelWhiteSlope(byte MinuteOffset);
	void ChannelRoyalBlueSlope(byte MinuteOffset);
	void ChannelRedSlope(byte MinuteOffset);
	void ChannelGreenSlope(byte MinuteOffset);
	void ChannelBlueSlope(byte MinuteOffset);
	void ChannelIntensitySlope(byte MinuteOffset);
	void ChannelRadionSlope(byte Channel, byte Start, byte End, byte Duration);
	void ChannelRadionSlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset);
	void RadionParabola();
	void ChannelWhiteParabola();
	void ChannelRoyalBlueParabola();
	void ChannelRedParabola();
	void ChannelGreenParabola();
	void ChannelBlueParabola();
	void ChannelIntensityParabola();
	void RadionParabola(byte MinuteOffset);
	void ChannelWhiteParabola(byte MinuteOffset);
	void ChannelRoyalBlueParabola(byte MinuteOffset);
	void ChannelRedParabola(byte MinuteOffset);
	void ChannelGreenParabola(byte MinuteOffset);
	void ChannelBlueParabola(byte MinuteOffset);
	void ChannelIntensityParabola(byte MinuteOffset);
	void ChannelRadionParabola(byte Channel, byte Start, byte End, byte Duration);
	void ChannelRadionParabola(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset);
	inline void VortechOff() {SetMode(TurnOff,0,0);}
	inline void VortechOn() {SetMode(TurnOn,0,0);}
	void inline Override(byte Channel, byte Value) { SetChannelOverride(Channel,Value); };

private:
	unsigned long lastWrite;
	byte lastcrc;
};

#endif  // __RF_H__
