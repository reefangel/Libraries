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
#ifdef SUNLOCATION
#include <RA_SunLocation.h>
#endif  // SUNLOCATION

class RA_PWMClass
{
public:
	RA_PWMClass();
	void inline SetActinic(byte value) { ActinicPWMValue = value; };
	void inline SetDaylight(byte value) { DaylightPWMValue = value; };
	void inline SetActinicOverride(byte value) { ActinicPWMOverride = value; };
	void inline SetDaylightOverride(byte value) { DaylightPWMOverride = value; };
	byte GetActinicValue();
	byte GetDaylightValue();
	byte inline GetActinicOverrideValue() { return ActinicPWMOverride; };
	byte inline GetDaylightOverrideValue() { return DaylightPWMOverride; };
	void ActinicPWMSlope(byte MinuteOffset);
	void DaylightPWMSlope(byte MinuteOffset);
	void ActinicPWMSlope();
	void DaylightPWMSlope();
	void ActinicPWMParabola(byte MinuteOffset);
	void DaylightPWMParabola(byte MinuteOffset);
	void ActinicPWMParabola();
	void DaylightPWMParabola();
#ifdef SUNLOCATION
  void SunLocationActinicPWMSlope();
  void SunLocationActinicPWMSlope(byte MinuteOffset);
  void SunLocationActinicPWMSlope(byte Start, byte End, byte Duration, byte MinuteOffset);
  void SunLocationDaylightPWMSlope();
  void SunLocationDaylightPWMSlope(byte MinuteOffset);
  void SunLocationDaylightPWMSlope(byte Start, byte End, byte Duration, byte MinuteOffset);
  void SunLocationActinicPWMParabola();
  void SunLocationActinicPWMParabola(byte MinuteOffset);
  void SunLocationActinicPWMParabola(byte Start, byte End, byte MinuteOffset);
  void SunLocationDaylightPWMParabola();
  void SunLocationDaylightPWMParabola(byte MinuteOffset);
  void SunLocationDaylightPWMParabola(byte Start, byte End, byte MinuteOffset);
#endif  // SUNLOCATION
	void StandardActinic();
	void StandardDaylight();
	void StandardActinic(byte MinuteOffset);
	void StandardDaylight(byte MinuteOffset);
	void Override(byte Channel, byte Value);
	
#ifdef PWMEXPANSION
	byte ExpansionChannel[PWM_EXPANSION_CHANNELS];
	byte ExpansionChannelOverride[PWM_EXPANSION_CHANNELS];
	void inline SetChannel(byte Channel, byte Value) { if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=Value; };
	void inline SetChannelOverride(byte Channel, byte Value) { if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannelOverride[Channel]=Value; };
	void Expansion(byte cmd, byte data);
	void ExpansionSetPercent(byte p);
	void ExpansionWrite();
	byte GetChannelValue(byte Channel);
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
	boolean inline IsPresent() { return Present; }
	boolean Present;
#ifdef SUNLOCATION
  void SunlocationChannel0PWMSlope();
  void SunlocationChannel1PWMSlope();
  void SunlocationChannel2PWMSlope();
  void SunlocationChannel3PWMSlope();
  void SunlocationChannel4PWMSlope();
  void SunlocationChannel5PWMSlope();
  void SunlocationChannel0PWMSlope(byte MinuteOffset);
  void SunlocationChannel1PWMSlope(byte MinuteOffset);
  void SunlocationChannel2PWMSlope(byte MinuteOffset);
  void SunlocationChannel3PWMSlope(byte MinuteOffset);
  void SunlocationChannel4PWMSlope(byte MinuteOffset);
  void SunlocationChannel5PWMSlope(byte MinuteOffset);
  void SunlocationChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration);
  void SunlocationChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, byte MinuteOffset);
  void SunlocationChannel0PWMParabola();
  void SunlocationChannel1PWMParabola();
  void SunlocationChannel2PWMParabola();
  void SunlocationChannel3PWMParabola();
  void SunlocationChannel4PWMParabola();
  void SunlocationChannel5PWMParabola();
  void SunlocationChannel0PWMParabola(byte MinuteOffset);
  void SunlocationChannel1PWMParabola(byte MinuteOffset);
  void SunlocationChannel2PWMParabola(byte MinuteOffset);
  void SunlocationChannel3PWMParabola(byte MinuteOffset);
  void SunlocationChannel4PWMParabola(byte MinuteOffset);
  void SunlocationChannel5PWMParabola(byte MinuteOffset);
  void SunlocationChannelPWMParabola(byte Channel, byte Start, byte End);
  void SunlocationChannelPWMParabola(byte Channel, byte Start, byte End, byte MinuteOffset);
#endif  // SUNLOCATION
#endif  // PWMEXPANSION

private:
	byte ActinicPWMValue;
	byte DaylightPWMValue;
	byte ActinicPWMOverride;
	byte DaylightPWMOverride;
	
};

#endif  // __RA_PWM_H__
