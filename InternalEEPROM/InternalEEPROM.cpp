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

uint8_t InternalEEPROMClass::WaterLevelLow_read()
{
    return read(Mem_B_WaterLevelLow);
}

void InternalEEPROMClass::WaterLevelLow_write(const uint8_t value)
{
    write(Mem_B_WaterLevelLow, value);
}

uint8_t InternalEEPROMClass::WaterLevelHigh_read()
{
    return read(Mem_B_WaterLevelHigh);
}

void InternalEEPROMClass::WaterLevelHigh_write(const uint8_t value)
{
    write(Mem_B_WaterLevelHigh, value);
}

uint8_t InternalEEPROMClass::DP3Timer_read()
{
    return read(Mem_B_DP3Timer);
}

void InternalEEPROMClass::DP3Timer_write(const uint8_t value)
{
    write(Mem_B_DP3Timer, value);
}

uint8_t InternalEEPROMClass::LCDID_read()
{
    return read(Mem_B_LCDID);
}

void InternalEEPROMClass::LCDID_write(const uint8_t value)
{
    write(Mem_B_LCDID, value);
}

uint8_t InternalEEPROMClass::DCPumpMode_read()
{
    return read(Mem_B_DCPumpMode);
}

void InternalEEPROMClass::DCPumpMode_write(const uint8_t value)
{
    write(Mem_B_DCPumpMode, value);
}

uint8_t InternalEEPROMClass::DCPumpSpeed_read()
{
    return read(Mem_B_DCPumpSpeed);
}

void InternalEEPROMClass::DCPumpSpeed_write(const uint8_t value)
{
    write(Mem_B_DCPumpSpeed, value);
}

uint8_t InternalEEPROMClass::DCPumpDuration_read()
{
    return read(Mem_B_DCPumpDuration);
}

void InternalEEPROMClass::DCPumpDuration_write(const uint8_t value)
{
    write(Mem_B_DCPumpDuration, value);
}

uint8_t InternalEEPROMClass::DCPumpThreshold_read()
{
    return read(Mem_B_DCPumpThreshold);
}

void InternalEEPROMClass::DCPumpThreshold_write(const uint8_t value)
{
    write(Mem_B_DCPumpThreshold, value);
}

uint8_t InternalEEPROMClass::LEDPWMDaylight2_read()
{
    return read(Mem_B_LEDPWMDaylight2);
}

void InternalEEPROMClass::LEDPWMDaylight2_write(const uint8_t value)
{
    write(Mem_B_LEDPWMDaylight2, value);
}

uint8_t InternalEEPROMClass::LEDPWMActinic2_read()
{
    return read(Mem_B_LEDPWMActinic2);
}

void InternalEEPROMClass::LEDPWMActinic2_write(const uint8_t value)
{
    write(Mem_B_LEDPWMActinic2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStartD2_read()
{
    return read(Mem_B_PWMSlopeStartD2);
}

void InternalEEPROMClass::PWMSlopeStartD2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartD2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEndD2_read()
{
    return read(Mem_B_PWMSlopeEndD2);
}

void InternalEEPROMClass::PWMSlopeEndD2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndD2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDurationD2_read()
{
    return read(Mem_B_PWMSlopeDurationD2);
}

void InternalEEPROMClass::PWMSlopeDurationD2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationD2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeStartA2_read()
{
    return read(Mem_B_PWMSlopeStartA2);
}

void InternalEEPROMClass::PWMSlopeStartA2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartA2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeEndA2_read()
{
    return read(Mem_B_PWMSlopeEndA2);
}

void InternalEEPROMClass::PWMSlopeEndA2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndA2, value);
}

uint8_t InternalEEPROMClass::PWMSlopeDurationA2_read()
{
    return read(Mem_B_PWMSlopeDurationA2);
}

void InternalEEPROMClass::PWMSlopeDurationA2_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationA2, value);
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

int InternalEEPROMClass::PHEControlOn_read()
{
    return read_int(Mem_I_PHEControlOn);
}

void InternalEEPROMClass::PHEControlOn_write(const int value)
{
    write_int(Mem_I_PHEControlOn, value);
}

int InternalEEPROMClass::PHEControlOff_read()
{
    return read_int(Mem_I_PHEControlOff);
}

void InternalEEPROMClass::PHEControlOff_write(int value)
{
    write_int(Mem_I_PHEControlOff, value);
}

void InternalEEPROMClass::PHExpMax_write(const int value)
{
    write_int(Mem_I_PHExpMax, value);
}

int InternalEEPROMClass::PHExpMax_read()
{
    return read_int(Mem_I_PHExpMax);
}

void InternalEEPROMClass::PHExpMin_write(const int value)
{
    write_int(Mem_I_PHExpMin, value);
}

int InternalEEPROMClass::PHExpMin_read()
{
    return read_int(Mem_I_PHExpMin);
}

void InternalEEPROMClass::WaterLevelMax_write(const int value)
{
    write_int(Mem_I_WaterLevelMax, value);
}

int InternalEEPROMClass::WaterLevelMax_read()
{
    return read_int(Mem_I_WaterLevelMax);
}

void InternalEEPROMClass::WaterLevelMin_write(const int value)
{
    write_int(Mem_I_WaterLevelMin, value);
}

int InternalEEPROMClass::WaterLevelMin_read()
{
    return read_int(Mem_I_WaterLevelMin);
}

void InternalEEPROMClass::SalTempComp_write(const int value)
{
    write_int(Mem_I_SalTempComp, value);
}

int InternalEEPROMClass::SalTempComp_read()
{
    return read_int(Mem_I_SalTempComp);
}

int InternalEEPROMClass::DP3RepeatInterval_read()
{
	return read_int(Mem_I_DP3RepeatInterval);
}

void InternalEEPROMClass::DP3RepeatInterval_write(const int value)
{
	write_int(Mem_I_DP3RepeatInterval, value);
}

unsigned long InternalEEPROMClass::IMCheck_read()
{
    return read_dword(IMPointer);
}

void InternalEEPROMClass::IMCheck_write(const unsigned long value)
{
	write_dword(IMPointer, value);
}

void InternalEEPROMClass::WaterLevel1Max_write(const int value)
{
    write_int(Mem_I_WaterLevel1Max, value);
}

int InternalEEPROMClass::WaterLevel1Max_read()
{
    return read_int(Mem_I_WaterLevel1Max);
}

void InternalEEPROMClass::WaterLevel1Min_write(const int value)
{
    write_int(Mem_I_WaterLevel1Min, value);
}

int InternalEEPROMClass::WaterLevel1Min_read()
{
    return read_int(Mem_I_WaterLevel1Min);
}

void InternalEEPROMClass::WaterLevel2Max_write(const int value)
{
    write_int(Mem_I_WaterLevel2Max, value);
}

int InternalEEPROMClass::WaterLevel2Max_read()
{
    return read_int(Mem_I_WaterLevel2Max);
}

void InternalEEPROMClass::WaterLevel2Min_write(const int value)
{
    write_int(Mem_I_WaterLevel2Min, value);
}

int InternalEEPROMClass::WaterLevel2Min_read()
{
    return read_int(Mem_I_WaterLevel2Min);
}

void InternalEEPROMClass::WaterLevel3Max_write(const int value)
{
    write_int(Mem_I_WaterLevel3Max, value);
}

int InternalEEPROMClass::WaterLevel3Max_read()
{
    return read_int(Mem_I_WaterLevel3Max);
}

void InternalEEPROMClass::WaterLevel3Min_write(const int value)
{
    write_int(Mem_I_WaterLevel3Min, value);
}

int InternalEEPROMClass::WaterLevel3Min_read()
{
    return read_int(Mem_I_WaterLevel3Min);
}

void InternalEEPROMClass::WaterLevel4Max_write(const int value)
{
    write_int(Mem_I_WaterLevel4Max, value);
}

int InternalEEPROMClass::WaterLevel4Max_read()
{
    return read_int(Mem_I_WaterLevel4Max);
}

void InternalEEPROMClass::WaterLevel4Min_write(const int value)
{
    write_int(Mem_I_WaterLevel4Min, value);
}

int InternalEEPROMClass::WaterLevel4Min_read()
{
    return read_int(Mem_I_WaterLevel4Min);
}

// Private functions
uint8_t InternalEEPROMClass::read(int address)
{
#if not defined __SAM3X8E__
	return eeprom_read_byte((unsigned char *) address);
#else
	return SPIEEPROM.Read(address);
#endif
}

void InternalEEPROMClass::write(int address, const uint8_t value)
{
  if(read(address) != value)
  {
#if not defined __SAM3X8E__
    eeprom_write_byte((unsigned char *) address, value);
#else
    SPIEEPROM.Write(address,value);
#endif
  }
}

int InternalEEPROMClass::read_int(int address)
{
#if not defined __SAM3X8E__
  return eeprom_read_word((const uint16_t *) address);
#else
	return SPIEEPROM.Read(address)+(SPIEEPROM.Read(address+1)<<8);
#endif
}

void InternalEEPROMClass::write_int(int address, const int value)
{
  if(read_int(address) != value)
  {
#if not defined __SAM3X8E__
    eeprom_write_word((uint16_t *) address, (uint16_t) value);
#else
    SPIEEPROM.Write(address,value&0xff);
    SPIEEPROM.Write(address+1,(value>>8));
#endif
  }
}

uint32_t InternalEEPROMClass::read_dword(int address)
{
#if not defined __SAM3X8E__
	return eeprom_read_dword((const uint32_t *)address);
#else
	return SPIEEPROM.Read(address)+(SPIEEPROM.Read(address+1)<<8)+(SPIEEPROM.Read(address+2)<<16)+(SPIEEPROM.Read(address+3)<<24);
#endif
}

void InternalEEPROMClass::write_dword(int address, const uint32_t value)
{
  if(read_dword(address) != value)
  {
#if not defined __SAM3X8E__
    eeprom_write_dword((uint32_t *) address, (uint32_t) value);
#else
    SPIEEPROM.Write(address,value&0xff);
    SPIEEPROM.Write(address+1,(value>>8)&0xff);
    SPIEEPROM.Write(address+2,(value>>16)&0xff);
    SPIEEPROM.Write(address+3,(value>>24));
#endif
  }
}

InternalEEPROMClass InternalMemory;
