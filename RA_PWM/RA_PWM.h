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
	void inline SetActinic(int value) { ActinicPWMValue = value; };
	void inline SetDaylight(int value) { DaylightPWMValue = value; };
	void inline SetActinic(byte value) { ActinicPWMValue = (int)value*40.95; };
	void inline SetDaylight(byte value) { DaylightPWMValue = (int)value*40.95; };
	void inline SetActinicOverride(byte value) { if (value>100) value=255; ActinicPWMOverride = value; };
	void inline SetDaylightOverride(byte value) { if (value>100) value=255; DaylightPWMOverride = value; };
	byte GetActinicValue();
	byte GetDaylightValue();
	int GetActinicValueRaw();
	int GetDaylightValueRaw();
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
	void ActinicPWMSmoothRamp(byte MinuteOffset);
	void DaylightPWMSmoothRamp(byte MinuteOffset);
	void ActinicPWMSmoothRamp();
	void DaylightPWMSmoothRamp();
	void StandardActinic();
	void StandardDaylight();
	void StandardActinic(byte MinuteOffset);
	void StandardDaylight(byte MinuteOffset);
	void Override(byte Channel, byte Value);
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	void inline SetActinic2(int value) { Actinic2PWMValue = value; };
	void inline SetDaylight2(int value) { Daylight2PWMValue = value; };
	void inline SetActinic2(byte value) { Actinic2PWMValue = (int)value*40.95; };
	void inline SetDaylight2(byte value) { Daylight2PWMValue = (int)value*40.95; };
	void inline SetActinic2Override(byte value) { if (value>100) value=255; Actinic2PWMOverride = value; };
	void inline SetDaylight2Override(byte value) { if (value>100) value=255; Daylight2PWMOverride = value; };
	byte GetActinic2Value();
	byte GetDaylight2Value();
	int GetActinic2ValueRaw();
	int GetDaylight2ValueRaw();
	byte inline GetActinic2OverrideValue() { return Actinic2PWMOverride; };
	byte inline GetDaylight2OverrideValue() { return Daylight2PWMOverride; };
	void Actinic2PWMSlope(byte MinuteOffset);
	void Daylight2PWMSlope(byte MinuteOffset);
	void Actinic2PWMSlope();
	void Daylight2PWMSlope();
	void Actinic2PWMParabola(byte MinuteOffset);
	void Daylight2PWMParabola(byte MinuteOffset);
	void Actinic2PWMParabola();
	void Daylight2PWMParabola();
	void Actinic2PWMSmoothRamp(byte MinuteOffset);
	void Daylight2PWMSmoothRamp(byte MinuteOffset);
	void Actinic2PWMSmoothRamp();
	void Daylight2PWMSmoothRamp();
	void StandardActinic2();
	void StandardDaylight2();
	void StandardActinic2(byte MinuteOffset);
	void StandardDaylight2(byte MinuteOffset);
#endif
	
#ifdef PWMEXPANSION
        boolean NewExpansion;
	int ExpansionChannel[PWM_EXPANSION_CHANNELS];
	byte ExpansionChannelOverride[PWM_EXPANSION_CHANNELS];
	void inline SetChannel(byte Channel, int Value) { if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=Value; };
	void inline SetChannel(byte Channel, byte Value) { if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=(int)Value*40.95; };
	void inline SetChannelOverride(byte Channel, byte Value) { if (Value>100) Value=255; if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannelOverride[Channel]=Value; };
	void Expansion(byte cmd, byte data);
	void Expansion(byte cmd, int data);
	void ExpansionSetPercent(byte p);
	void ExpansionWrite();
	byte GetChannelValue(byte Channel);
	int GetChannelValueRaw(byte Channel);
	byte inline GetChannelOverrideValue(byte Channel) { return ExpansionChannelOverride[Channel]; };
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
	void Channel0PWMSmoothRamp();
	void Channel1PWMSmoothRamp();
	void Channel2PWMSmoothRamp();
	void Channel3PWMSmoothRamp();
	void Channel4PWMSmoothRamp();
	void Channel5PWMSmoothRamp();
	void Channel0PWMSmoothRamp(byte MinuteOffset);
	void Channel1PWMSmoothRamp(byte MinuteOffset);
	void Channel2PWMSmoothRamp(byte MinuteOffset);
	void Channel3PWMSmoothRamp(byte MinuteOffset);
	void Channel4PWMSmoothRamp(byte MinuteOffset);
	void Channel5PWMSmoothRamp(byte MinuteOffset);
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End);
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength);	
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, byte MinuteOffset);	
	boolean inline IsPresent() { return Present; }
	boolean Present;
#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION
	int SIXTEENChExpansionChannel[SIXTEENCH_PWM_EXPANSION_CHANNELS];
	byte SIXTEENChExpansionChannelOverride[SIXTEENCH_PWM_EXPANSION_CHANNELS];
	void inline Set16Channel(byte Channel, int Value) { if (Channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[Channel]=Value; };
	void inline Set16Channel(byte Channel, byte Value) { if (Channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[Channel]=(int)Value*40.95; };
	void inline Set16ChannelOverride(byte Channel, byte Value) { if (Value>100) Value=255; if (Channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannelOverride[Channel]=Value; };
	void SIXTEENChExpansion(byte cmd, int data);
	void SIXTEENChExpansion(byte cmd, byte data);
	void SIXTEENChExpansionSetPercent(byte p);
	void SIXTEENChExpansionWrite();
	byte Get16ChannelValue(byte Channel);
	int Get16ChannelValueRaw(byte Channel);
	byte inline Get16ChannelOverrideValue(byte Channel) { return SIXTEENChExpansionChannelOverride[Channel]; };
	void SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration);
	void SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, byte MinuteOffset);
	void SIXTEENChannelPWMParabola(byte Channel, int Start, int End);	
	void SIXTEENChannelPWMParabola(byte Channel, int Start, int End, byte MinuteOffset);	
	void SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength);	
	void SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, byte MinuteOffset);	
	boolean inline SIXTEENChIsPresent() { return SIXTEENChPresent; }
	boolean SIXTEENChPresent;
#endif  // SIXTEENCHPWMEXPANSION

private:
	byte lastcrc;
	int ActinicPWMValue;
	int DaylightPWMValue;
	byte ActinicPWMOverride;
	byte DaylightPWMOverride;
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	int Actinic2PWMValue;
	int Daylight2PWMValue;
	byte Actinic2PWMOverride;
	byte Daylight2PWMOverride;
#endif
};

#endif  // __RA_PWM_H__
