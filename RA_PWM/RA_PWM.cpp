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

#include "RA_PWM.h"
#include <Globals.h>
#include <Wire.h>

RA_PWMClass::RA_PWMClass()
{
    // initialize variables
    ActinicPWMValue = 0;
    DaylightPWMValue = 0;
    ActinicPWMOverride = 255; // Anything over 100 disables override
    DaylightPWMOverride = 255; // Anything over 100 disables override
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
    Actinic2PWMValue = 0;
    Daylight2PWMValue = 0;
    Actinic2PWMOverride = 255; // Anything over 100 disables override
    Daylight2PWMOverride = 255; // Anything over 100 disables override
#endif
    LightsOverride = true; // Enable PWM override when lights on
#ifdef PWMEXPANSION
    lastcrc=-1;
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		ExpansionChannel[a]=0;
		ExpansionChannelOverride[a]=255;
	}
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
    lastcrc=-1;
    for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
    {
        SIXTEENChExpansionChannel[a]=0;
        SIXTEENChExpansionChannelOverride[a]=255;
    }
#endif  // SIXTEENCHPWMEXPANSION
}

byte RA_PWMClass::GetActinicValue()
{
	if (ActinicPWMOverride<=100)
		return ActinicPWMOverride;
	else
		return (byte)(((float)ActinicPWMValue/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::GetActinicValueRaw()
{
	if (ActinicPWMOverride<=100)
		return (int)ActinicPWMOverride*40.95;
	else
		return ActinicPWMValue;
}

byte RA_PWMClass::GetDaylightValue()
{
	if (DaylightPWMOverride<=100)
		return DaylightPWMOverride;
	else
		return (byte)(((float)DaylightPWMValue/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::GetDaylightValueRaw()
{
	if (DaylightPWMOverride<=100)
		return (int)DaylightPWMOverride*40.95;
	else
		return DaylightPWMValue;
}

void RA_PWMClass::ActinicPWMSlope(int MinuteOffset)
{
	ActinicPWMSlope(MinuteOffset, MinuteOffset);	
}

void RA_PWMClass::ActinicPWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinicRaw(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			InternalMemory.PWMSlopeDurationA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMSlope(int MinuteOffset)
{
	DaylightPWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::DaylightPWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylightRaw(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSlope()
{
	ActinicPWMSlope(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMSlope()
{
	SetDaylightRaw(PWMSlopeHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMParabola(int MinuteOffset)
{
	ActinicPWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ActinicPWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinicRaw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMParabola(int MinuteOffset)
{
	DaylightPWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::DaylightPWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylightRaw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMParabola()
{
	ActinicPWMParabola(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMParabola()
{
	SetDaylightRaw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSmoothRamp(int MinuteOffset)
{
	ActinicPWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ActinicPWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinicRaw(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			InternalMemory.PWMSlopeDurationA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMSmoothRamp(int MinuteOffset)
{
	DaylightPWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::DaylightPWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylightRaw(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSmoothRamp()
{
	ActinicPWMSmoothRamp(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMSmoothRamp()
{
	SetDaylightRaw(PWMSmoothRampHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			InternalMemory.PWMSlopeDurationD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSigmoid(int MinuteOffset)
{
	ActinicPWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ActinicPWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinicRaw(PWMSigmoidHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA_read(),
			InternalMemory.PWMSlopeEndA_read(),
			ActinicPWMValue
			));
}

void RA_PWMClass::DaylightPWMSigmoid(int MinuteOffset)
{
	DaylightPWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::DaylightPWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylightRaw(PWMSigmoidHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::ActinicPWMSigmoid()
{
	ActinicPWMSigmoid(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::DaylightPWMSigmoid()
{
	SetDaylightRaw(PWMSigmoidHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD_read(),
			InternalMemory.PWMSlopeEndD_read(),
			DaylightPWMValue
			));
}

void RA_PWMClass::StandardActinic()
{
	SetDaylightRaw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMActinic_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight()
{
	SetDaylightRaw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMDaylight_read(),
			0
			));
}

void RA_PWMClass::StandardActinic(int MinuteOffset)
{
	StandardActinic(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::StandardActinic(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinicRaw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMActinic_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight(int MinuteOffset)
{
	StandardDaylight(MinuteOffset, MinuteOffset);}

void RA_PWMClass::StandardDaylight(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylightRaw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMDaylight_read(),
			0
			));
}

void RA_PWMClass::Override(byte Channel, byte Value)
{
	if (Channel==OVERRIDE_DAYLIGHT) // Daylight channel
		SetDaylightOverride(Value);
	else if (Channel==OVERRIDE_ACTINIC) // Actinic channel
		SetActinicOverride(Value);
#ifdef PWMEXPANSION
	else if (Channel>=OVERRIDE_CHANNEL0 && Channel<=OVERRIDE_CHANNEL5) // Dimming Expansion channel 0-5
		SetChannelOverride(Channel-OVERRIDE_CHANNEL0,Value);
#endif // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
	else if (Channel>=OVERRIDE_16CH_CHANNEL0 && Channel<=OVERRIDE_16CH_CHANNEL15) // 16Ch Dimming Expansion channel 0-15
		Set16ChannelOverride(Channel-OVERRIDE_16CH_CHANNEL0,Value);
#endif // SIXTEENCHPWMEXPANSION
#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
	else if (Channel==OVERRIDE_DAYLIGHT2) // Daylight2 channel
		SetDaylight2Override(Value);
	else if (Channel==OVERRIDE_ACTINIC2) // Actinic2 channel
		SetActinic2Override(Value);
#endif
}

#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
byte RA_PWMClass::GetActinic2Value()
{
	if (Actinic2PWMOverride<=100)
		return Actinic2PWMOverride;
	else
		return (byte)(((float)Actinic2PWMValue/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::GetActinic2ValueRaw()
{
	if (Actinic2PWMOverride<=100)
		return (int)Actinic2PWMOverride*40.95;
	else
		return Actinic2PWMValue;
}

byte RA_PWMClass::GetDaylight2Value()
{
	if (Daylight2PWMOverride<=100)
		return Daylight2PWMOverride;
	else
		return (byte)(((float)Daylight2PWMValue/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::GetDaylight2ValueRaw()
{
	if (Daylight2PWMOverride<=100)
		return (int)Daylight2PWMOverride*40.95;
	else
		return Daylight2PWMValue;
}

void RA_PWMClass::Actinic2PWMSlope(int MinuteOffset)
{
	Actinic2PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Actinic2PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinic2Raw(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			InternalMemory.PWMSlopeDurationA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMSlope(int MinuteOffset)
{
	Daylight2PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Daylight2PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylight2Raw(PWMSlopeHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSlope()
{
	Actinic2PWMSlope(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMSlope()
{
	SetDaylight2Raw(PWMSlopeHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMParabola(int MinuteOffset)
{
	Actinic2PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Actinic2PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinic2Raw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMParabola(int MinuteOffset)
{
	Daylight2PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Daylight2PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylight2Raw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMParabola()
{
	Actinic2PWMParabola(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMParabola()
{
	SetDaylight2Raw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSmoothRamp(int MinuteOffset)
{
	Actinic2PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Actinic2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinic2Raw(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			InternalMemory.PWMSlopeDurationA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMSmoothRamp(int MinuteOffset)
{
	Daylight2PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Daylight2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylight2Raw(PWMSmoothRampHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSmoothRamp()
{
	Actinic2PWMSmoothRamp(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMSmoothRamp()
{
	SetDaylight2Raw(PWMSmoothRampHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			InternalMemory.PWMSlopeDurationD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSigmoid(int MinuteOffset)
{
	Actinic2PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Actinic2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinic2Raw(PWMSigmoidHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartA2_read(),
			InternalMemory.PWMSlopeEndA2_read(),
			Actinic2PWMValue
			));
}

void RA_PWMClass::Daylight2PWMSigmoid(int MinuteOffset)
{
	Daylight2PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Daylight2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylight2Raw(PWMSigmoidHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::Actinic2PWMSigmoid()
{
	Actinic2PWMSigmoid(InternalMemory.ActinicOffset_read());
}

void RA_PWMClass::Daylight2PWMSigmoid()
{
	SetDaylight2Raw(PWMSigmoidHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			InternalMemory.PWMSlopeStartD2_read(),
			InternalMemory.PWMSlopeEndD2_read(),
			Daylight2PWMValue
			));
}

void RA_PWMClass::StandardActinic2()
{
	SetDaylight2Raw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMActinic2_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight2()
{
	SetDaylight2Raw(PWMParabolaHighRes(
			InternalMemory.StdLightsOnHour_read(),
			InternalMemory.StdLightsOnMinute_read(),
			InternalMemory.StdLightsOffHour_read(),
			InternalMemory.StdLightsOffMinute_read(),
			0,
			InternalMemory.LEDPWMDaylight2_read(),
			0
			));
}

void RA_PWMClass::StandardActinic2(int MinuteOffset)
{
	StandardActinic2(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::StandardActinic2(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetActinic2Raw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMActinic2_read(),
			0
			));
}

void RA_PWMClass::StandardDaylight2(int MinuteOffset)
{
	StandardDaylight2(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::StandardDaylight2(int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetDaylight2Raw(PWMParabolaHighRes(
			onTime/60,
			onTime%60,
			offTime/60,
			offTime%60,
			0,
			InternalMemory.LEDPWMDaylight2_read(),
			0
			));
}
#endif //defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)

#ifdef PWMEXPANSION

void RA_PWMClass::Expansion(byte cmd, byte data)
{
        // if you're sending in bytes, we assume it's a percentage
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write((int)(data*2.55));			// send the data
	Present=Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
	// Also send data to new module PCA9685
	int newdata=(int)(data*40.95);
    Wire.beginTransmission(I2CPWM_PCA9685);
    Wire.write(0x8+(4*cmd));
    Wire.write(newdata&0xff);
    Wire.write(newdata>>8);
    Wire.endTransmission();
}

void RA_PWMClass::Expansion(byte cmd, int data)
{
        // assume if you're sending integers from 0 to 4095 in you're not sending percentages
	Wire.beginTransmission(I2CPWM);  // transmit to device #8, consider having this user defined possibly
	Wire.write('$');				// send the $$$
	Wire.write('$');
	Wire.write('$');
	Wire.write(cmd);				// send the command
	Wire.write(map(data,0,4095,0,255));			// send the data to the older chip after mapping to 255 bits
	Present=Wire.endTransmission();		// stop transmitting
	if (cmd<PWM_EXPANSION_CHANNELS) ExpansionChannel[cmd]=data;
	// Also send data to new module PCA9685
    Wire.beginTransmission(I2CPWM_PCA9685);
    Wire.write(0x8+(4*cmd));
    Wire.write(data&0xff);
    Wire.write(data>>8);
    Wire.endTransmission();
}

void RA_PWMClass::ExpansionSetPercent(byte p)
{
	// loop through all 6 channels and send the value
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
	{
		Expansion(a, p);
	}
}

void RA_PWMClass::ExpansionWrite()
{
	byte thiscrc=0;
	for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
		thiscrc+=GetChannelValueRaw(a)*(a+1);
	if (millis()%60000<200) lastcrc=-1;
	if (lastcrc!=thiscrc || millis()<5000)
	{
		lastcrc=thiscrc;
		// setup PCA9685 for data receive
		// we need this to make sure it will work if connected ofter controller is booted, so we need to send it all the time.
		Wire.beginTransmission(I2CPWM_PCA9685);
		Wire.write(0);
		Wire.write(0xa1);
		Wire.endTransmission();
		for ( byte a = 0; a < PWM_EXPANSION_CHANNELS; a++ )
		{
			Expansion(a,GetChannelValueRaw(a));
		}
	}
}

byte RA_PWMClass::GetChannelValue(byte Channel)
{
	if (ExpansionChannelOverride[Channel]<=100)
		return ExpansionChannelOverride[Channel];
	else
		return (byte)(((float)ExpansionChannel[Channel]/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::GetChannelValueRaw(byte Channel)
{
	if (ExpansionChannelOverride[Channel]<=100)
		return (int)ExpansionChannelOverride[Channel]*40.95;
	else
		return ExpansionChannel[Channel];
}

void RA_PWMClass::Channel0PWMSlope()
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read());
}

void RA_PWMClass::Channel1PWMSlope()
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read());
}

void RA_PWMClass::Channel2PWMSlope()
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read());
}

void RA_PWMClass::Channel3PWMSlope()
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read());
}

void RA_PWMClass::Channel4PWMSlope()
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read());
}

void RA_PWMClass::Channel5PWMSlope()
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read());
}

void RA_PWMClass::Channel0PWMSlope(int MinuteOffset)
{
	Channel0PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel1PWMSlope(int MinuteOffset)
{
	Channel1PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel2PWMSlope(int MinuteOffset)
{
	Channel2PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel3PWMSlope(int MinuteOffset)
{
	Channel3PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel4PWMSlope(int MinuteOffset)
{
	Channel4PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel5PWMSlope(int MinuteOffset)
{
	Channel5PWMSlope(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel0PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel1PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel2PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel3PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel4PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel5PWMSlope(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration)
{
	SetChannelRaw(Channel,PWMSlopeHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		Duration,  
		ExpansionChannel[Channel] 
	));	
}


void RA_PWMClass::ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, int MinuteOffset)
{
	ChannelPWMSlope(Channel, Start, End, Duration, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ChannelPWMSlope(byte Channel, byte Start, byte End, byte Duration, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetChannelRaw(Channel,PWMSlopeHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		Duration,  
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::Channel0PWMParabola()
{
	ChannelPWMParabola(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read());
}

void RA_PWMClass::Channel1PWMParabola()
{
	ChannelPWMParabola(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read());
}

void RA_PWMClass::Channel2PWMParabola()
{
	ChannelPWMParabola(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read());
}

void RA_PWMClass::Channel3PWMParabola()
{
	ChannelPWMParabola(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read());
}

void RA_PWMClass::Channel4PWMParabola()
{
	ChannelPWMParabola(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read());
}

void RA_PWMClass::Channel5PWMParabola()
{
	ChannelPWMParabola(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read());
}

void RA_PWMClass::Channel0PWMParabola(int MinuteOffset)
{
	Channel0PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel1PWMParabola(int MinuteOffset)
{
	Channel1PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel2PWMParabola(int MinuteOffset)
{
	Channel2PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel3PWMParabola(int MinuteOffset)
{
	Channel3PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel4PWMParabola(int MinuteOffset)
{
	Channel4PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel5PWMParabola(int MinuteOffset)
{
	Channel5PWMParabola(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel0PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel1PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel2PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel3PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel4PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel5PWMParabola(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMParabola(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::ChannelPWMParabola(byte Channel, byte Start, byte End)
{
	SetChannelRaw(Channel,PWMParabolaHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::ChannelPWMParabola(byte Channel, byte Start, byte End, int MinuteOffset)
{
	ChannelPWMParabola(Channel, Start, End, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ChannelPWMParabola(byte Channel, byte Start, byte End, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	SetChannelRaw(Channel,PWMParabolaHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::Channel0PWMSmoothRamp()
{
	ChannelPWMSmoothRamp(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read());
}

void RA_PWMClass::Channel1PWMSmoothRamp()
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read());
}

void RA_PWMClass::Channel2PWMSmoothRamp()
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read());
}

void RA_PWMClass::Channel3PWMSmoothRamp()
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read());
}

void RA_PWMClass::Channel4PWMSmoothRamp()
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read());
}

void RA_PWMClass::Channel5PWMSmoothRamp()
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read());
}

void RA_PWMClass::Channel0PWMSmoothRamp(int MinuteOffset)
{
	Channel0PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel1PWMSmoothRamp(int MinuteOffset)
{
	Channel1PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel2PWMSmoothRamp(int MinuteOffset)
{
	Channel2PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel3PWMSmoothRamp(int MinuteOffset)
{
	Channel3PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel4PWMSmoothRamp(int MinuteOffset)
{
	Channel4PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel5PWMSmoothRamp(int MinuteOffset)
{
	Channel5PWMSmoothRamp(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel0PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(),InternalMemory.PWMSlopeDuration0_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel1PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(),InternalMemory.PWMSlopeDuration1_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel2PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(),InternalMemory.PWMSlopeDuration2_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel3PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(),InternalMemory.PWMSlopeDuration3_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel4PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(),InternalMemory.PWMSlopeDuration4_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel5PWMSmoothRamp(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSlope(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(),InternalMemory.PWMSlopeDuration5_read(), PreMinuteOffset, PostMinuteOffset);
}


void RA_PWMClass::ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength)
{
	SetChannelRaw(Channel,PWMSmoothRampHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SlopeLength, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, int MinuteOffset)
{
	ChannelPWMSmoothRamp(Channel, Start, End, SlopeLength, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ChannelPWMSmoothRamp(byte Channel, byte Start, byte End, byte SlopeLength, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PreMinuteOffset;
	SetChannelRaw(Channel,PWMSmoothRampHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
                SlopeLength,
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::Channel0PWMSigmoid()
{
	ChannelPWMSigmoid(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read());
}

void RA_PWMClass::Channel1PWMSigmoid()
{
	ChannelPWMSigmoid(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read());
}

void RA_PWMClass::Channel2PWMSigmoid()
{
	ChannelPWMSigmoid(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read());
}

void RA_PWMClass::Channel3PWMSigmoid()
{
	ChannelPWMSigmoid(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read());
}

void RA_PWMClass::Channel4PWMSigmoid()
{
	ChannelPWMSigmoid(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read());
}

void RA_PWMClass::Channel5PWMSigmoid()
{
	ChannelPWMSigmoid(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read());
}

void RA_PWMClass::Channel0PWMSigmoid(int MinuteOffset)
{
	Channel0PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel1PWMSigmoid(int MinuteOffset)
{
	Channel1PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel2PWMSigmoid(int MinuteOffset)
{
	Channel2PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel3PWMSigmoid(int MinuteOffset)
{
	Channel3PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel4PWMSigmoid(int MinuteOffset)
{
	Channel4PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel5PWMSigmoid(int MinuteOffset)
{
	Channel5PWMSigmoid(MinuteOffset, MinuteOffset);
}

void RA_PWMClass::Channel0PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(0,InternalMemory.PWMSlopeStart0_read(),InternalMemory.PWMSlopeEnd0_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel1PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(1,InternalMemory.PWMSlopeStart1_read(),InternalMemory.PWMSlopeEnd1_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel2PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(2,InternalMemory.PWMSlopeStart2_read(),InternalMemory.PWMSlopeEnd2_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel3PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(3,InternalMemory.PWMSlopeStart3_read(),InternalMemory.PWMSlopeEnd3_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel4PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(4,InternalMemory.PWMSlopeStart4_read(),InternalMemory.PWMSlopeEnd4_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::Channel5PWMSigmoid(int PreMinuteOffset, int PostMinuteOffset)
{
	ChannelPWMSigmoid(5,InternalMemory.PWMSlopeStart5_read(),InternalMemory.PWMSlopeEnd5_read(), PreMinuteOffset, PostMinuteOffset);
}

void RA_PWMClass::ChannelPWMSigmoid(byte Channel, byte Start, byte End)
{
	SetChannelRaw(Channel,PWMSigmoidHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::ChannelPWMSigmoid(byte Channel, byte Start, byte End, int MinuteOffset)
{
	ChannelPWMSigmoid(Channel, Start, End, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::ChannelPWMSigmoid(byte Channel, byte Start, byte End, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PreMinuteOffset;
	SetChannelRaw(Channel,PWMSigmoidHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		ExpansionChannel[Channel] 
	));	
}

#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION

void RA_PWMClass::SIXTEENChExpansion(byte channel, byte data)
{
    // the data is in byte, so it's assumed to be a percentage, like from a pump
    if (channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[channel]=data;
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0);
    Wire.write(0xa1);
    Wire.endTransmission();
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    int newdata = (int)data*40.95;
    Wire.write(0x8+(4*channel));
    Wire.write(newdata&0xff);
    Wire.write(newdata>>8);
    Wire.endTransmission();
}

void RA_PWMClass::SIXTEENChExpansion(byte channel, int data)
{
    // the data is in int, so just send that int to the module
    if (channel<SIXTEENCH_PWM_EXPANSION_CHANNELS) SIXTEENChExpansionChannel[channel]=data;
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0);
    Wire.write(0xa1);
    Wire.endTransmission();
    Wire.beginTransmission(I2CPWM_16CH_PCA9685);
    Wire.write(0x8+(4*channel));
    Wire.write(data&0xff);
    Wire.write(data>>8);
    Wire.endTransmission();
}

void RA_PWMClass::SIXTEENChExpansionSetPercent(byte p)
{
	// loop through all 16 channels and send the value
	for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
	{
		SIXTEENChExpansion(a, p);
	}
}

void RA_PWMClass::SIXTEENChExpansionWrite()
{
	byte thiscrc=0;
	for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
		thiscrc+=Get16ChannelValueRaw(a)*(a+1);
	if (millis()%60000<200) lastcrc=-1;
	if (lastcrc!=thiscrc || millis()<5000)
	{
		lastcrc=thiscrc;
		// setup PCA9685 for data receive
		// we need this to make sure it will work if connected ofter controller is booted, so we need to send it all the time.
		Wire.beginTransmission(I2CPWM_16CH_PCA9685);
		Wire.write(0);
		Wire.write(0xa1);
		Wire.endTransmission();
		for ( byte a = 0; a < SIXTEENCH_PWM_EXPANSION_CHANNELS; a++ )
		{
			SIXTEENChExpansion(a,Get16ChannelValueRaw(a));
		}
	}
}

byte RA_PWMClass::Get16ChannelValue(byte Channel)
{
	if (SIXTEENChExpansionChannelOverride[Channel]<=100)
		return SIXTEENChExpansionChannelOverride[Channel];
	else
		return (byte)(((float)SIXTEENChExpansionChannel[Channel]/40.95)+0.5);// cast to float, then divide, then add 0.5 in order to round correctly
}

int RA_PWMClass::Get16ChannelValueRaw(byte Channel)
{
	if (SIXTEENChExpansionChannelOverride[Channel]<=100)
		return (int)SIXTEENChExpansionChannelOverride[Channel]*40.95;
	else
		return SIXTEENChExpansionChannel[Channel];
}

void RA_PWMClass::SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration)
{
	Set16ChannelRaw(Channel,PWMSlopeHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		Duration,  
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, int MinuteOffset)
{
	SIXTEENChannelPWMSlope(Channel, Start, End, Duration, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::SIXTEENChannelPWMSlope(byte Channel, int Start, int End, byte Duration, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PreMinuteOffset;
	Set16ChannelRaw(Channel,PWMSlopeHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		Duration,  
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMParabola(byte Channel, int Start, int End)
{
	Set16ChannelRaw(Channel,PWMParabolaHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMParabola(byte Channel, int Start, int End, int MinuteOffset)
{
	SIXTEENChannelPWMParabola(Channel, Start, End, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::SIXTEENChannelPWMParabola(byte Channel, int Start, int End, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	Set16ChannelRaw(Channel,PWMParabolaHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength)
{
	Set16ChannelRaw(Channel,PWMSmoothRampHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SlopeLength, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, int MinuteOffset)
{
	SIXTEENChannelPWMSmoothRamp(Channel, Start, End, SlopeLength, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::SIXTEENChannelPWMSmoothRamp(byte Channel, int Start, int End, byte SlopeLength, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	Set16ChannelRaw(Channel,PWMSmoothRampHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
                SlopeLength,
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End)
{
	Set16ChannelRaw(Channel,PWMSigmoidHighRes(
		InternalMemory.StdLightsOnHour_read(),
		InternalMemory.StdLightsOnMinute_read(),
		InternalMemory.StdLightsOffHour_read(),
		InternalMemory.StdLightsOffMinute_read(),
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

void RA_PWMClass::SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End, int MinuteOffset)
{
	SIXTEENChannelPWMSigmoid(Channel, Start, End, MinuteOffset, MinuteOffset);
}

void RA_PWMClass::SIXTEENChannelPWMSigmoid(byte Channel, int Start, int End, int PreMinuteOffset, int PostMinuteOffset)
{
	int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read())-PreMinuteOffset;
	int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read())+PostMinuteOffset;
	Set16ChannelRaw(Channel,PWMSigmoidHighRes(
		onTime/60,
		onTime%60,
		offTime/60,
		offTime%60,
		Start,
		End, 
		SIXTEENChExpansionChannel[Channel] 
	));	
}

#endif //SIXTEENCHPWMEXPANSION

