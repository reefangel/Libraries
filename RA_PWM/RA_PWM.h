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

#ifndef __RA_PWM_H__
#define __RA_PWM_H__

#include <Globals.h>
#include <InternalEEPROM.h>

class RA_PWMClass
{
public:
	RA_PWMClass();
	boolean LightsOverride;
	void SetActinic(byte value);
	void SetDaylight(byte value);
	void ActinicPWMSlope(byte MinuteOffset);
	void DaylightPWMSlope(byte MinuteOffset);
	void ActinicPWMSlope();
	void DaylightPWMSlope();
	void ActinicPWMParabola(byte MinuteOffset);
	void DaylightPWMParabola(byte MinuteOffset);
	void ActinicPWMParabola();
	void DaylightPWMParabola();
	void StandardActinic();
	void StandardDaylight();
	void StandardActinic(byte MinuteOffset);
	void StandardDaylight(byte MinuteOffset);
	
#ifdef PWMEXPANSION
	byte ExpansionChannel[PWM_EXPANSION_CHANNELS];
	void SetChannel(byte Channel, byte Value);
	void Expansion(byte cmd, byte data);
	void ExpansionSetPercent(byte p);
	void ExpansionWrite();
	inline byte GetChannelValue(byte Channel) { return ExpansionChannel[Channel]; }
	void Channel0PWMSlope();
	void Channel1PWMSlope();
	void Channel2PWMSlope();
	void Channel3PWMSlope();
	void Channel4PWMSlope();
	void Channel5PWMSlope();
	void Channel0PWMSlope(byte MinuteOffset);
	void Channel1PWMSlope(byte MinuteOffset);
	void Channel2PWMSlope(byte MinuteOffset);
	void Channel3PWMSlope(byte MinuteOffset);
	void Channel4PWMSlope(byte MinuteOffset);
	void Channel5PWMSlope(byte MinuteOffset);
	void ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration);
	void ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset);
	void Channel0PWMParabola();
	void Channel1PWMParabola();
	void Channel2PWMParabola();
	void Channel3PWMParabola();
	void Channel4PWMParabola();
	void Channel5PWMParabola();
	void Channel0PWMParabola(byte MinuteOffset);
	void Channel1PWMParabola(byte MinuteOffset);
	void Channel2PWMParabola(byte MinuteOffset);
	void Channel3PWMParabola(byte MinuteOffset);
	void Channel4PWMParabola(byte MinuteOffset);
	void Channel5PWMParabola(byte MinuteOffset);
	void ChannelPWMParabola(byte Channel, byte Start, byte End);
	void ChannelPWMParabola(byte Channel, byte Start, byte End, byte MinuteOffset);	

#endif  // PWMEXPANSION
	inline byte GetActinicValue() { return ActinicPWMValue; }
	inline byte GetDaylightValue() { return DaylightPWMValue; }

private:
	byte ActinicPWMValue;
	byte DaylightPWMValue;
};

#endif  // __RA_PWM_H__
