/*
 * Copyright 2010 Curt Binder
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

#include "InternalEEPROM.h"
#include <Globals.h>

// Byte Functions
uint8_t InternalEEPROMClass::MHOnHour_read()
{
    return read(Mem_B_MHOnHour);
}

void InternalEEPROMClass::MHOnHour_write(const uint8_t value)
{
    write(Mem_B_MHOnHour, value);
}

uint8_t InternalEEPROMClass::MHOnMinute_read()
{
    return read(Mem_B_MHOnMinute);
}

void InternalEEPROMClass::MHOnMinute_write(const uint8_t value)
{
    write(Mem_B_MHOnMinute, value);
}

uint8_t InternalEEPROMClass::MHOffHour_read()
{
    return read(Mem_B_MHOffHour);
}

void InternalEEPROMClass::MHOffHour_write(const uint8_t value)
{
    write(Mem_B_MHOffHour, value);
}

uint8_t InternalEEPROMClass::MHOffMinute_read()
{
    return read(Mem_B_MHOffMinute);
}

void InternalEEPROMClass::MHOffMinute_write(const uint8_t value)
{
    write(Mem_B_MHOffMinute, value);
}

uint8_t InternalEEPROMClass::StdLightsOnHour_read()
{
    return read(Mem_B_StdLightsOnHour);
}

void InternalEEPROMClass::StdLightsOnHour_write(const uint8_t value)
{
    write(Mem_B_StdLightsOnHour, value);
}

uint8_t InternalEEPROMClass::StdLightsOnMinute_read()
{
    return read(Mem_B_StdLightsOnMinute);
}

void InternalEEPROMClass::StdLightsOnMinute_write(const uint8_t value)
{
    write(Mem_B_StdLightsOnMinute, value);
}

uint8_t InternalEEPROMClass::StdLightsOffHour_read()
{
    return read(Mem_B_StdLightsOffHour);
}

void InternalEEPROMClass::StdLightsOffHour_write(const uint8_t value)
{
    write(Mem_B_StdLightsOffHour, value);
}

uint8_t InternalEEPROMClass::StdLightsOffMinute_read()
{
    return read(Mem_B_StdLightsOffMinute);
}

void InternalEEPROMClass::StdLightsOffMinute_write(const uint8_t value)
{
    write(Mem_B_StdLightsOffMinute, value);
}

uint8_t InternalEEPROMClass::DP1Timer_read()
{
    return read(Mem_B_DP1Timer);
}

void InternalEEPROMClass::DP1Timer_write(const uint8_t value)
{
    write(Mem_B_DP1Timer, value);
}

uint8_t InternalEEPROMClass::DP2Timer_read()
{
    return read(Mem_B_DP2Timer);
}

void InternalEEPROMClass::DP2Timer_write(const uint8_t value)
{
    write(Mem_B_DP2Timer, value);
}

uint8_t InternalEEPROMClass::LEDPWMDaylight_read()
{
    return read(Mem_B_LEDPWMDaylight);
}

void InternalEEPROMClass::LEDPWMDaylight_write(const uint8_t value)
{
    write(Mem_B_LEDPWMDaylight, value);
}

uint8_t InternalEEPROMClass::LEDPWMActinic_read()
{
    return read(Mem_B_LEDPWMActinic);
}

void InternalEEPROMClass::LEDPWMActinic_write(const uint8_t value)
{
    write(Mem_B_LEDPWMActinic, value);
}

uint8_t InternalEEPROMClass::ATOTimeout_read()
{
    return read(Mem_B_ATOTimeout);
}

void InternalEEPROMClass::ATOTimeout_write(const uint8_t value)
{
    write(Mem_B_ATOTimeout, value);
}

uint8_t InternalEEPROMClass::ATOHighTimeout_read()
{
    return read(Mem_B_ATOHighTimeout);
}

void InternalEEPROMClass::ATOHighTimeout_write(const uint8_t value)
{
    write(Mem_B_ATOHighTimeout, value);
}

uint8_t InternalEEPROMClass::ATOHourInterval_read()
{
    return read(Mem_B_ATOHourInterval);
}

void InternalEEPROMClass::ATOHourInterval_write(const uint8_t value)
{
    write(Mem_B_ATOHourInterval, value);
}

uint8_t InternalEEPROMClass::ATOHighHourInterval_read()
{
    return read(Mem_B_ATOHighHourInterval);
}

void InternalEEPROMClass::ATOHighHourInterval_write(const uint8_t value)
{
    write(Mem_B_ATOHighHourInterval, value);
}

uint8_t InternalEEPROMClass::MHDelay_read()
{
    return read(Mem_B_MHDelay);
}

void InternalEEPROMClass::MHDelay_write(const uint8_t value)
{
    write(Mem_B_MHDelay, value);
}

uint8_t InternalEEPROMClass::DP1OnHour_read()
{
    return read(Mem_B_DP1OnHour);
}

void InternalEEPROMClass::DP1OnHour_write(const uint8_t value)
{
    write(Mem_B_DP1OnHour, value);
}

uint8_t InternalEEPROMClass::DP1OnMinute_read()
{
    return read(Mem_B_DP1OnMinute);
}

void InternalEEPROMClass::DP1OnMinute_write(uint8_t value)
{
    write(Mem_B_DP1OnMinute, value);
}

uint8_t InternalEEPROMClass::DP2OnHour_read()
{
    return read(Mem_B_DP2OnHour);
}

void InternalEEPROMClass::DP2OnHour_write(const uint8_t value)
{
    write(Mem_B_DP2OnHour, value);
}

uint8_t InternalEEPROMClass::DP2OnMinute_read()
{
    return read(Mem_B_DP2OnMinute);
}

void InternalEEPROMClass::DP2OnMinute_write(const uint8_t value)
{
    write(Mem_B_DP2OnMinute, value);
}

uint8_t InternalEEPROMClass::T1Pointer_read()
{
    return read(T1Pointer);
}

void InternalEEPROMClass::T1Pointer_write(const uint8_t value)
{
    write(T1Pointer, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStartD_read()
{
    return read(Mem_B_PWMSlopeStartD);
}

void InternalEEPROMClass::PWMSlopeStartD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartD, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEndD_read()
{
    return read(Mem_B_PWMSlopeEndD);
}

void InternalEEPROMClass::PWMSlopeEndD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndD, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDurationD_read()
{
    return read(Mem_B_PWMSlopeDurationD);
}

void InternalEEPROMClass::PWMSlopeDurationD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationD, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStartA_read()
{
    return read(Mem_B_PWMSlopeStartA);
}

void InternalEEPROMClass::PWMSlopeStartA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartA, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEndA_read()
{
    return read(Mem_B_PWMSlopeEndA);
}

void InternalEEPROMClass::PWMSlopeEndA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndA, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDurationA_read()
{
    return read(Mem_B_PWMSlopeDurationA);
}

void InternalEEPROMClass::PWMSlopeDurationA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationA, value);
}

uint8_t InternalEEPROMClass::RFMode_read()
{
    return read(Mem_B_RFMode);
}

void InternalEEPROMClass::RFMode_write(const uint8_t value)
{
    write(Mem_B_RFMode, value);
}

uint8_t InternalEEPROMClass::RFSpeed_read()
{
    return read(Mem_B_RFSpeed);
}

void InternalEEPROMClass::RFSpeed_write(const uint8_t value)
{
    write(Mem_B_RFSpeed, value);
}

uint8_t InternalEEPROMClass::RFDuration_read()
{
    return read(Mem_B_RFDuration);
}

void InternalEEPROMClass::RFDuration_write(const uint8_t value)
{
    write(Mem_B_RFDuration, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart0_read()
{
    return read(Mem_B_PWMSlopeStart0);
}

void InternalEEPROMClass::PWMSlopeStart0_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart0, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd0_read()
{
    return read(Mem_B_PWMSlopeEnd0);
}

void InternalEEPROMClass::PWMSlopeEnd0_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd0, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration0_read()
{
    return read(Mem_B_PWMSlopeDuration0);
}

void InternalEEPROMClass::PWMSlopeDuration0_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration0, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart1_read()
{
    return read(Mem_B_PWMSlopeStart1);
}

void InternalEEPROMClass::PWMSlopeStart1_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart1, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd1_read()
{
    return read(Mem_B_PWMSlopeEnd1);
}

void InternalEEPROMClass::PWMSlopeEnd1_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd1, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration1_read()
{
    return read(Mem_B_PWMSlopeDuration1);
}

void InternalEEPROMClass::PWMSlopeDuration1_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration1, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart2_read()
{
    return read(Mem_B_PWMSlopeStart2);
}

void InternalEEPROMClass::PWMSlopeStart2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd2_read()
{
    return read(Mem_B_PWMSlopeEnd2);
}

void InternalEEPROMClass::PWMSlopeEnd2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration2_read()
{
    return read(Mem_B_PWMSlopeDuration2);
}

void InternalEEPROMClass::PWMSlopeDuration2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart3_read()
{
    return read(Mem_B_PWMSlopeStart3);
}

void InternalEEPROMClass::PWMSlopeStart3_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart3, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd3_read()
{
    return read(Mem_B_PWMSlopeEnd3);
}

void InternalEEPROMClass::PWMSlopeEnd3_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd3, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration3_read()
{
    return read(Mem_B_PWMSlopeDuration3);
}

void InternalEEPROMClass::PWMSlopeDuration3_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration3, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart4_read()
{
    return read(Mem_B_PWMSlopeStart4);
}

void InternalEEPROMClass::PWMSlopeStart4_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart4, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd4_read()
{
    return read(Mem_B_PWMSlopeEnd4);
}

void InternalEEPROMClass::PWMSlopeEnd4_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd4, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration4_read()
{
    return read(Mem_B_PWMSlopeDuration4);
}

void InternalEEPROMClass::PWMSlopeDuration4_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration4, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStart5_read()
{
    return read(Mem_B_PWMSlopeStart5);
}

void InternalEEPROMClass::PWMSlopeStart5_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStart5, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEnd5_read()
{
    return read(Mem_B_PWMSlopeEnd5);
}

void InternalEEPROMClass::PWMSlopeEnd5_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEnd5, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDuration5_read()
{
    return read(Mem_B_PWMSlopeDuration5);
}

void InternalEEPROMClass::PWMSlopeDuration5_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDuration5, value);
}

uint8_t InternalEEPROMClass::ActinicOffset_read()
{
    return read(Mem_B_ActinicOffset);
}

void InternalEEPROMClass::ActinicOffset_write(const uint8_t value)
{
    write(Mem_B_ActinicOffset, value);
}

uint8_t InternalEEPROMClass::AISlopeStartW_read()
{
    return read(Mem_B_AISlopeStartW);
}

void InternalEEPROMClass::AISlopeStartW_write(const uint8_t value)
{
    write(Mem_B_AISlopeStartW, value);
}

void InternalEEPROMClass::AISlopeEndW_write(const uint8_t value)
{
    write(Mem_B_AISlopeEndW, value);
}

uint8_t InternalEEPROMClass::AISlopeEndW_read()
{
    return read(Mem_B_AISlopeEndW);
}

uint8_t InternalEEPROMClass::AISlopeDurationW_read()
{
    return read(Mem_B_AISlopeDurationW);
}

void InternalEEPROMClass::AISlopeDurationW_write(const uint8_t value)
{
    write(Mem_B_AISlopeDurationW, value);
}

uint8_t InternalEEPROMClass::AISlopeStartB_read()
{
    return read(Mem_B_AISlopeStartB);
}

void InternalEEPROMClass::AISlopeStartB_write(const uint8_t value)
{
    write(Mem_B_AISlopeStartB, value);
}

void InternalEEPROMClass::AISlopeEndB_write(const uint8_t value)
{
    write(Mem_B_AISlopeEndB, value);
}

uint8_t InternalEEPROMClass::AISlopeEndB_read()
{
    return read(Mem_B_AISlopeEndB);
}

uint8_t InternalEEPROMClass::AISlopeDurationB_read()
{
    return read(Mem_B_AISlopeDurationB);
}

void InternalEEPROMClass::AISlopeDurationB_write(const uint8_t value)
{
    write(Mem_B_AISlopeDurationB, value);
}

uint8_t InternalEEPROMClass::AISlopeStartRB_read()
{
    return read(Mem_B_AISlopeStartRB);
}

void InternalEEPROMClass::AISlopeStartRB_write(const uint8_t value)
{
    write(Mem_B_AISlopeStartRB, value);
}

void InternalEEPROMClass::AISlopeEndRB_write(const uint8_t value)
{
    write(Mem_B_AISlopeEndRB, value);
}

uint8_t InternalEEPROMClass::AISlopeEndRB_read()
{
    return read(Mem_B_AISlopeEndRB);
}

uint8_t InternalEEPROMClass::AISlopeDurationRB_read()
{
    return read(Mem_B_AISlopeDurationRB);
}

void InternalEEPROMClass::AISlopeDurationRB_write(const uint8_t value)
{
    write(Mem_B_AISlopeDurationRB, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartW_read()
{
    return read(Mem_B_RadionSlopeStartW);
}

void InternalEEPROMClass::RadionSlopeStartW_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartW, value);
}

void InternalEEPROMClass::RadionSlopeEndW_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndW, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndW_read()
{
    return read(Mem_B_RadionSlopeEndW);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationW_read()
{
    return read(Mem_B_RadionSlopeDurationW);
}

void InternalEEPROMClass::RadionSlopeDurationW_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationW, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartRB_read()
{
    return read(Mem_B_RadionSlopeStartRB);
}

void InternalEEPROMClass::RadionSlopeStartRB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartRB, value);
}

void InternalEEPROMClass::RadionSlopeEndRB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndRB, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndRB_read()
{
    return read(Mem_B_RadionSlopeEndRB);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationRB_read()
{
    return read(Mem_B_RadionSlopeDurationRB);
}

void InternalEEPROMClass::RadionSlopeDurationRB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationRB, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartR_read()
{
    return read(Mem_B_RadionSlopeStartR);
}

void InternalEEPROMClass::RadionSlopeStartR_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartR, value);
}

void InternalEEPROMClass::RadionSlopeEndR_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndR, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndR_read()
{
    return read(Mem_B_RadionSlopeEndR);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationR_read()
{
    return read(Mem_B_RadionSlopeDurationR);
}

void InternalEEPROMClass::RadionSlopeDurationR_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationR, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartG_read()
{
    return read(Mem_B_RadionSlopeStartG);
}

void InternalEEPROMClass::RadionSlopeStartG_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartG, value);
}

void InternalEEPROMClass::RadionSlopeEndG_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndG, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndG_read()
{
    return read(Mem_B_RadionSlopeEndG);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationG_read()
{
    return read(Mem_B_RadionSlopeDurationG);
}

void InternalEEPROMClass::RadionSlopeDurationG_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationG, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartB_read()
{
    return read(Mem_B_RadionSlopeStartB);
}

void InternalEEPROMClass::RadionSlopeStartB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartB, value);
}

void InternalEEPROMClass::RadionSlopeEndB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndB, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndB_read()
{
    return read(Mem_B_RadionSlopeEndB);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationB_read()
{
    return read(Mem_B_RadionSlopeDurationB);
}

void InternalEEPROMClass::RadionSlopeDurationB_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationB, value);
}

uint8_t InternalEEPROMClass::RadionSlopeStartI_read()
{
    return read(Mem_B_RadionSlopeStartI);
}

void InternalEEPROMClass::RadionSlopeStartI_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeStartI, value);
}

void InternalEEPROMClass::RadionSlopeEndI_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeEndI, value);
}

uint8_t InternalEEPROMClass::RadionSlopeEndI_read()
{
    return read(Mem_B_RadionSlopeEndI);
}

uint8_t InternalEEPROMClass::RadionSlopeDurationI_read()
{
    return read(Mem_B_RadionSlopeDurationI);
}

void InternalEEPROMClass::RadionSlopeDurationI_write(const uint8_t value)
{
    write(Mem_B_RadionSlopeDurationI, value);
}

uint8_t InternalEEPROMClass::DelayedStart_read()
{
    return read(Mem_B_DelayedStart);
}

void InternalEEPROMClass::DelayedStart_write(const uint8_t value)
{
    write(Mem_B_DelayedStart, value);
}

// Int Functions
int InternalEEPROMClass::WM1Timer_read()
{
    return read_int(Mem_I_WM1Timer);
}

void InternalEEPROMClass::WM1Timer_write(const int value)
{
    write_int(Mem_I_WM1Timer, value);
}

int InternalEEPROMClass::WM2Timer_read()
{
    return read_int(Mem_I_WM2Timer);
}

void InternalEEPROMClass::WM2Timer_write(const int value)
{
    write_int(Mem_I_WM2Timer, value);
}

int InternalEEPROMClass::FeedingTimer_read()
{
    return read_int(Mem_I_FeedingTimer);
}

void InternalEEPROMClass::FeedingTimer_write(int value)
{
    write_int(Mem_I_FeedingTimer, value);
}

int InternalEEPROMClass::LCDTimer_read()
{
    return read_int(Mem_I_LCDTimer);
}

void InternalEEPROMClass::LCDTimer_write(const int value)
{
    write_int(Mem_I_LCDTimer, value);
}

int InternalEEPROMClass::OverheatTemp_read()
{
    return read_int(Mem_I_OverheatTemp);
}

void InternalEEPROMClass::OverheatTemp_write(const int value)
{
    write_int(Mem_I_OverheatTemp, value);
}

int InternalEEPROMClass::HeaterTempOn_read()
{
    return read_int(Mem_I_HeaterTempOn);
}

void InternalEEPROMClass::HeaterTempOn_write(const int value)
{
    write_int(Mem_I_HeaterTempOn, value);
}

int InternalEEPROMClass::HeaterTempOff_read()
{
    return read_int(Mem_I_HeaterTempOff);
}

void InternalEEPROMClass::HeaterTempOff_write(int value)
{
    write_int(Mem_I_HeaterTempOff, value);
}

int InternalEEPROMClass::ChillerTempOn_read()
{
    return read_int(Mem_I_ChillerTempOn);
}

void InternalEEPROMClass::ChillerTempOn_write(const int value)
{
    write_int(Mem_I_ChillerTempOn, value);
}

int InternalEEPROMClass::ChillerTempOff_read()
{
    return read_int(Mem_I_ChillerTempOff);
}

void InternalEEPROMClass::ChillerTempOff_write(int value)
{
    write_int(Mem_I_ChillerTempOff, value);
}

int InternalEEPROMClass::PHMax_read()
{
    return read_int(Mem_I_PHMax);
}

void InternalEEPROMClass::PHMax_write(const int value)
{
    write_int(Mem_I_PHMax, value);
}

int InternalEEPROMClass::PHMin_read()
{
    return read_int(Mem_I_PHMin);
}

void InternalEEPROMClass::PHMin_write(const int value)
{
    write_int(Mem_I_PHMin, value);
}

int InternalEEPROMClass::DP1RepeatInterval_read()
{
	return read_int(Mem_I_DP1RepeatInterval);
}

void InternalEEPROMClass::DP1RepeatInterval_write(const int value)
{
	write_int(Mem_I_DP1RepeatInterval, value);
}

int InternalEEPROMClass::DP2RepeatInterval_read()
{
	return read_int(Mem_I_DP2RepeatInterval);
}

void InternalEEPROMClass::DP2RepeatInterval_write(const int value)
{
	write_int(Mem_I_DP2RepeatInterval, value);
}

int InternalEEPROMClass::SalMax_read()
{
    return read_int(Mem_I_SalMax);
}

void InternalEEPROMClass::SalMax_write(const int value)
{
    write_int(Mem_I_SalMax, value);
}

int InternalEEPROMClass::ATOExtendedTimeout_read()
{
    return read_int(Mem_I_ATOExtendedTimeout);
}

void InternalEEPROMClass::ATOExtendedTimeout_write(const int value)
{
    write_int(Mem_I_ATOExtendedTimeout, value);
}

int InternalEEPROMClass::ATOHighExtendedTimeout_read()
{
    return read_int(Mem_I_ATOHighExtendedTimeout);
}

void InternalEEPROMClass::ATOHighExtendedTimeout_write(const int value)
{
    write_int(Mem_I_ATOHighExtendedTimeout, value);
}

int InternalEEPROMClass::ORPMin_read()
{
    return read_int(Mem_I_ORPMin);
}

void InternalEEPROMClass::ORPMin_write(const int value)
{
    write_int(Mem_I_ORPMin, value);
}

int InternalEEPROMClass::ORPMax_read()
{
    return read_int(Mem_I_ORPMax);
}

void InternalEEPROMClass::ORPMax_write(const int value)
{
    write_int(Mem_I_ORPMax, value);
}

int InternalEEPROMClass::CO2ControlOn_read()
{
    return read_int(Mem_I_CO2ControlOn);
}

void InternalEEPROMClass::CO2ControlOn_write(const int value)
{
    write_int(Mem_I_CO2ControlOn, value);
}

int InternalEEPROMClass::CO2ControlOff_read()
{
    return read_int(Mem_I_CO2ControlOff);
}

void InternalEEPROMClass::CO2ControlOff_write(int value)
{
    write_int(Mem_I_CO2ControlOff, value);
}

int InternalEEPROMClass::PHControlOn_read()
{
    return read_int(Mem_I_PHControlOn);
}

void InternalEEPROMClass::PHControlOn_write(const int value)
{
    write_int(Mem_I_PHControlOn, value);
}

int InternalEEPROMClass::PHControlOff_read()
{
    return read_int(Mem_I_PHControlOff);
}

void InternalEEPROMClass::PHControlOff_write(int value)
{
    write_int(Mem_I_PHControlOff, value);
}

unsigned long InternalEEPROMClass::IMCheck_read()
{
    return read_dword(IMPointer);
}

void InternalEEPROMClass::IMCheck_write(const unsigned long value)
{
	write_dword(IMPointer, value);
}

// Private functions
uint8_t InternalEEPROMClass::read(uint8_t address)
{
    return eeprom_read_byte((unsigned char *) address);
}

void InternalEEPROMClass::write(uint8_t address, const uint8_t value)
{
    eeprom_write_byte((unsigned char *) address, value);
}

int InternalEEPROMClass::read_int(int address)
{
    return eeprom_read_word((const uint16_t *) address);
}

void InternalEEPROMClass::write_int(int address, const int value)
{
    eeprom_write_word((uint16_t *) address, (uint16_t) value);
}

uint32_t InternalEEPROMClass::read_dword(int address)
{
	return eeprom_read_dword((const uint32_t *)address);
}

void InternalEEPROMClass::write_dword(int address, const uint32_t value)
{
	eeprom_write_dword((uint32_t *) address, (uint32_t) value);
}

InternalEEPROMClass InternalMemory;
