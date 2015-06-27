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
	byte DaylightPWMOverride;
	byte ActinicPWMOverride;
#ifdef RA_STAR
	byte DaylightPercentage;
	byte ActinicPercentage;
	byte Daylight2Percentage;
	byte Actinic2Percentage;
	byte Daylight2PWMOverride;
	byte Actinic2PWMOverride;
#endif // RA_STAR
	void inline SetActinicRaw(int value) { ActinicPWMValue = value; };
	void inline SetDaylightRaw(int value) { DaylightPWMValue = value; };
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
	void ActinicPWMSlope(int MinuteOffset);
	void DaylightPWMSlope(int MinuteOffset);
	void ActinicPWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void DaylightPWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void ActinicPWMSlope();
	void DaylightPWMSlope();
	void ActinicPWMParabola(int MinuteOffset);
	void DaylightPWMParabola(int MinuteOffset);
	void ActinicPWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void DaylightPWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void ActinicPWMParabola();
	void DaylightPWMParabola();
	void ActinicPWMSmoothRamp(int MinuteOffset);
	void DaylightPWMSmoothRamp(int MinuteOffset);
	void ActinicPWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void DaylightPWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void ActinicPWMSmoothRamp();
	void DaylightPWMSmoothRamp();
	void ActinicPWMSigmoid(int MinuteOffset);
	void DaylightPWMSigmoid(int MinuteOffset);
	void ActinicPWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void DaylightPWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void ActinicPWMSigmoid();
	void DaylightPWMSigmoid();
	void StandardActinic();
	void StandardDaylight();
	void StandardActinic(int MinuteOffset);
	void StandardDaylight(int MinuteOffset);
	void StandardActinic(int PreMinuteOffset, int PostMinuteOffset);
	void StandardDaylight(int PreMinuteOffset, int PostMinuteOffset);
	void Override(byte Channel, byte Value);
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	void inline SetActinic2Raw(int value) { Actinic2PWMValue = value; };
	void inline SetDaylight2Raw(int value) { Daylight2PWMValue = value; };
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
	void Actinic2PWMSlope(int MinuteOffset);
	void Daylight2PWMSlope(int MinuteOffset);
	void Actinic2PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Daylight2PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Actinic2PWMSlope();
	void Daylight2PWMSlope();
	void Actinic2PWMParabola(int MinuteOffset);
	void Daylight2PWMParabola(int MinuteOffset);
	void Actinic2PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Daylight2PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Actinic2PWMParabola();
	void Daylight2PWMParabola();
	void Actinic2PWMSmoothRamp(int MinuteOffset);
	void Daylight2PWMSmoothRamp(int MinuteOffset);
	void Actinic2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Daylight2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Actinic2PWMSmoothRamp();
	void Daylight2PWMSmoothRamp();
	void Actinic2PWMSigmoid(int MinuteOffset);
	void Daylight2PWMSigmoid(int MinuteOffset);
	void Actinic2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Daylight2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Actinic2PWMSigmoid();
	void Daylight2PWMSigmoid();
	void StandardActinic2();
	void StandardDaylight2();
	void StandardActinic2(int MinuteOffset);
	void StandardDaylight2(int MinuteOffset);
	void StandardActinic2(int PreMinuteOffset, int PostMinuteOffset);
	void StandardDaylight2(int PreMinuteOffset, int PostMinuteOffset);
#endif
	
#ifdef PWMEXPANSION
    byte ExpansionPercentage[PWM_EXPANSION_CHANNELS];
	int ExpansionChannel[PWM_EXPANSION_CHANNELS];
	byte ExpansionChannelOverride[PWM_EXPANSION_CHANNELS];
	void inline SetChannelRaw(byte Channel, int Value) { if (Channel<PWM_EXPANSION_CHANNELS) ExpansionChannel[Channel]=Value; };
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
	void Channel0PWMSlope(int MinuteOffset);
	void Channel1PWMSlope(int MinuteOffset);
	void Channel2PWMSlope(int MinuteOffset);
	void Channel3PWMSlope(int MinuteOffset);
	void Channel4PWMSlope(int MinuteOffset);
	void Channel5PWMSlope(int MinuteOffset);
	void Channel0PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Channel1PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Channel2PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Channel3PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Channel4PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void Channel5PWMSlope(int PreMinuteOffset, int PostMinuteOffset);
	void ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration);
	void ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, int MinuteOffset);
	void ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, int PreMinuteOffset, int PostMinuteOffset);
	void Channel0PWMParabola();
	void Channel1PWMParabola();
	void Channel2PWMParabola();
	void Channel3PWMParabola();
	void Channel4PWMParabola();
	void Channel5PWMParabola();
	void Channel0PWMParabola(int MinuteOffset);
	void Channel1PWMParabola(int MinuteOffset);
	void Channel2PWMParabola(int MinuteOffset);
	void Channel3PWMParabola(int MinuteOffset);
	void Channel4PWMParabola(int MinuteOffset);
	void Channel5PWMParabola(int MinuteOffset);
	void Channel0PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Channel1PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Channel2PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Channel3PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Channel4PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void Channel5PWMParabola(int PreMinuteOffset, int PostMinuteOffset);
	void ChannelPWMParabola(byte Channel, byte Start, byte End);
	void ChannelPWMParabola(byte Channel, byte Start, byte End, int MinuteOffset);	
	void ChannelPWMParabola(byte Channel, byte Start, byte End, int PreMinuteOffset, int PostMinuteOffset);	
	void Channel0PWMSmoothRamp();
	void Channel1PWMSmoothRamp();
	void Channel2PWMSmoothRamp();
	void Channel3PWMSmoothRamp();
	void Channel4PWMSmoothRamp();
	void Channel5PWMSmoothRamp();
	void Channel0PWMSmoothRamp(int MinuteOffset);
	void Channel1PWMSmoothRamp(int MinuteOffset);
	void Channel2PWMSmoothRamp(int MinuteOffset);
	void Channel3PWMSmoothRamp(int MinuteOffset);
	void Channel4PWMSmoothRamp(int MinuteOffset);
	void Channel5PWMSmoothRamp(int MinuteOffset);
	void Channel0PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Channel1PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Channel2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Channel3PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Channel4PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void Channel5PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset);
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End);
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength);	
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, int MinuteOffset);	
	void ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, int PreMinuteOffset, int PostMinuteOffset);	
	void Channel0PWMSigmoid();
	void Channel1PWMSigmoid();
	void Channel2PWMSigmoid();
	void Channel3PWMSigmoid();
	void Channel4PWMSigmoid();
	void Channel5PWMSigmoid();
	void Channel0PWMSigmoid(int MinuteOffset);
	void Channel1PWMSigmoid(int MinuteOffset);
	void Channel2PWMSigmoid(int MinuteOffset);
	void Channel3PWMSigmoid(int MinuteOffset);
	void Channel4PWMSigmoid(int MinuteOffset);
	void Channel5PWMSigmoid(int MinuteOffset);
	void Channel0PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Channel1PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Channel2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Channel3PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Channel4PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void Channel5PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset);
	void ChannelPWMSigmoid(byte Channel, byte Start, byte End);
	void ChannelPWMSigmoid(byte Channel, byte Start, byte End, int MinuteOffset);	
	void ChannelPWMSigmoid(byte Channel, byte Start, byte End, int PreMinuteOffset, int PostMinuteOffset);	
	boolean inline IsPresent() { return Present; }
	boolean Present;
#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION
	int SIXTEENChExpansionChannel[SIXTEENCH_PWM_EXPANSION_CHANNELS];
	byte SIXTEENChExpansionChannelOverride[SIXTEENCH_PWM_EXPANSION_CHANNELS];
	void inline Set16ChannelRaw(byte Channel, int Value) { if (Channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[Channel]=Value; };
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
	void SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, int MinuteOffset);
	void SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, int PreMinuteOffset, int PostMinuteOffset);
	void SIXTEENChannelPWMParabola(byte Channel, int Start, int End);	
	void SIXTEENChannelPWMParabola(byte Channel, int Start, int End, int MinuteOffset);	
	void SIXTEENChannelPWMParabola(byte Channel, int Start, int End, int PreMinuteOffset, int PostMinuteOffset);	
	void SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength);	
	void SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, int MinuteOffset);	
	void SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, int PreMinuteOffset, int PostMinuteOffset);	
	void SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End);	
	void SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End, int MinuteOffset);	
	void SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End, int PreMinuteOffset, int PostMinuteOffset);	
	boolean inline SIXTEENChIsPresent() { return SIXTEENChPresent; }
	boolean SIXTEENChPresent;
#endif  // SIXTEENCHPWMEXPANSION

private:
	byte lastcrc;
	int ActinicPWMValue;
	int DaylightPWMValue;
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	int Actinic2PWMValue;
	int Daylight2PWMValue;
#endif
};

#endif  // __RA_PWM_H__
