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

#ifndef __INTERNAL_EEPROM_H__
#define __INTERNAL_EEPROM_H__

#include <inttypes.h>
#if not defined __SAM3X8E__
#include <avr/eeprom.h>
#else
#include "SPIEEPROM.h"
#endif // __SAM3X8E__
/*
This class reads/writes to the internal EEPROM memory
*/

class InternalEEPROMClass {
    public:
        // Functions that read / write a byte (uint8_t)
        uint8_t MHOnHour_read();
        void MHOnHour_write(const uint8_t value);
        uint8_t MHOnMinute_read();
        void MHOnMinute_write(const uint8_t value);
        uint8_t MHOffHour_read();
        void MHOffHour_write(const uint8_t value);
        uint8_t MHOffMinute_read();
        void MHOffMinute_write(const uint8_t value);
        uint8_t StdLightsOnHour_read();
        void StdLightsOnHour_write(const uint8_t value);
        uint8_t StdLightsOnMinute_read();
        void StdLightsOnMinute_write(const uint8_t value);
        uint8_t StdLightsOffHour_read();
        void StdLightsOffHour_write(const uint8_t value);
        uint8_t StdLightsOffMinute_read();
        void StdLightsOffMinute_write(const uint8_t value);
        uint8_t DP1Timer_read();
        void DP1Timer_write(const uint8_t value);
        uint8_t DP2Timer_read();
        void DP2Timer_write(const uint8_t value);
        uint8_t LEDPWMDaylight_read();
        void LEDPWMDaylight_write(const uint8_t value);
        uint8_t LEDPWMActinic_read();
        void LEDPWMActinic_write(const uint8_t value);
        uint8_t ATOTimeout_read();
        void ATOTimeout_write(const uint8_t value);
        uint8_t ATOHighTimeout_read();
        void ATOHighTimeout_write(const uint8_t value);
        uint8_t ATOHourInterval_read();
        void ATOHourInterval_write(const uint8_t value);
        uint8_t ATOHighHourInterval_read();
        void ATOHighHourInterval_write(const uint8_t value);
        uint8_t MHDelay_read();
        void MHDelay_write(const uint8_t value);
        uint8_t DP1OnHour_read();
        void DP1OnHour_write(const uint8_t value);
        uint8_t DP1OnMinute_read();
        void DP1OnMinute_write(const uint8_t value);
        uint8_t DP2OnHour_read();
        void DP2OnHour_write(const uint8_t value);
        uint8_t DP2OnMinute_read();
        void DP2OnMinute_write(const uint8_t value);
        uint8_t T1Pointer_read();
        void T1Pointer_write(const uint8_t value);
        uint8_t PWMSlopeStartD_read();
        void PWMSlopeStartD_write(const uint8_t value);
        uint8_t PWMSlopeEndD_read();
        void PWMSlopeEndD_write(const uint8_t value);
        uint8_t PWMSlopeDurationD_read();
        void PWMSlopeDurationD_write(const uint8_t value);
        uint8_t PWMSlopeStartA_read();
        void PWMSlopeStartA_write(const uint8_t value);
        uint8_t PWMSlopeEndA_read();
        void PWMSlopeEndA_write(const uint8_t value);
        uint8_t PWMSlopeDurationA_read();
        void PWMSlopeDurationA_write(const uint8_t value);
        uint8_t RFMode_read();
        void RFMode_write(const uint8_t value);
        uint8_t RFSpeed_read();
        void RFSpeed_write(const uint8_t value);
        uint8_t RFDuration_read();
        void RFDuration_write(const uint8_t value);
        uint8_t PWMSlopeStart0_read();
        void PWMSlopeStart0_write(const uint8_t value);
        uint8_t PWMSlopeEnd0_read();
        void PWMSlopeEnd0_write(const uint8_t value);
        uint8_t PWMSlopeDuration0_read();
        void PWMSlopeDuration0_write(const uint8_t value);
        uint8_t PWMSlopeStart1_read();
        void PWMSlopeStart1_write(const uint8_t value);
        uint8_t PWMSlopeEnd1_read();
        void PWMSlopeEnd1_write(const uint8_t value);
        uint8_t PWMSlopeDuration1_read();
        void PWMSlopeDuration1_write(const uint8_t value);
        uint8_t PWMSlopeStart2_read();
        void PWMSlopeStart2_write(const uint8_t value);
        uint8_t PWMSlopeEnd2_read();
        void PWMSlopeEnd2_write(const uint8_t value);
        uint8_t PWMSlopeDuration2_read();
        void PWMSlopeDuration2_write(const uint8_t value);
        uint8_t PWMSlopeStart3_read();
        void PWMSlopeStart3_write(const uint8_t value);
        uint8_t PWMSlopeEnd3_read();
        void PWMSlopeEnd3_write(const uint8_t value);
        uint8_t PWMSlopeDuration3_read();
        void PWMSlopeDuration3_write(const uint8_t value);
        uint8_t PWMSlopeStart4_read();
        void PWMSlopeStart4_write(const uint8_t value);
        uint8_t PWMSlopeEnd4_read();
        void PWMSlopeEnd4_write(const uint8_t value);
        uint8_t PWMSlopeDuration4_read();
        void PWMSlopeDuration4_write(const uint8_t value);
        uint8_t PWMSlopeStart5_read();
        void PWMSlopeStart5_write(const uint8_t value);
        uint8_t PWMSlopeEnd5_read();
        void PWMSlopeEnd5_write(const uint8_t value);
        uint8_t PWMSlopeDuration5_read();
        void PWMSlopeDuration5_write(const uint8_t value);
        uint8_t ActinicOffset_read();
        void ActinicOffset_write(const uint8_t value);
        uint8_t AISlopeStartW_read();
        void AISlopeStartW_write(const uint8_t value);
        uint8_t AISlopeEndW_read();
        void AISlopeEndW_write(const uint8_t value);
        uint8_t AISlopeDurationW_read();
        void AISlopeDurationW_write(const uint8_t value);
        uint8_t AISlopeStartB_read();
        void AISlopeStartB_write(const uint8_t value);
        uint8_t AISlopeEndB_read();
        void AISlopeEndB_write(const uint8_t value);
        uint8_t AISlopeDurationB_read();
        void AISlopeDurationB_write(const uint8_t value);
        uint8_t AISlopeStartRB_read();
        void AISlopeStartRB_write(const uint8_t value);
        uint8_t AISlopeEndRB_read();
        void AISlopeEndRB_write(const uint8_t value);
        uint8_t AISlopeDurationRB_read();
        void AISlopeDurationRB_write(const uint8_t value);
        uint8_t RadionSlopeStartW_read();
        void RadionSlopeStartW_write(const uint8_t value);
        uint8_t RadionSlopeEndW_read();
        void RadionSlopeEndW_write(const uint8_t value);
        uint8_t RadionSlopeDurationW_read();
        void RadionSlopeDurationW_write(const uint8_t value);
        uint8_t RadionSlopeStartRB_read();
        void RadionSlopeStartRB_write(const uint8_t value);
        uint8_t RadionSlopeEndRB_read();
        void RadionSlopeEndRB_write(const uint8_t value);
        uint8_t RadionSlopeDurationRB_read();
        void RadionSlopeDurationRB_write(const uint8_t value);
        uint8_t RadionSlopeStartR_read();
        void RadionSlopeStartR_write(const uint8_t value);
        uint8_t RadionSlopeEndR_read();
        void RadionSlopeEndR_write(const uint8_t value);
        uint8_t RadionSlopeDurationR_read();
        void RadionSlopeDurationR_write(const uint8_t value);
        uint8_t RadionSlopeStartG_read();
        void RadionSlopeStartG_write(const uint8_t value);
        uint8_t RadionSlopeEndG_read();
        void RadionSlopeEndG_write(const uint8_t value);
        uint8_t RadionSlopeDurationG_read();
        void RadionSlopeDurationG_write(const uint8_t value);
        uint8_t RadionSlopeStartB_read();
        void RadionSlopeStartB_write(const uint8_t value);
        uint8_t RadionSlopeEndB_read();
        void RadionSlopeEndB_write(const uint8_t value);
        uint8_t RadionSlopeDurationB_read();
        void RadionSlopeDurationB_write(const uint8_t value);
        uint8_t RadionSlopeStartI_read();
        void RadionSlopeStartI_write(const uint8_t value);
        uint8_t RadionSlopeEndI_read();
        void RadionSlopeEndI_write(const uint8_t value);
        uint8_t RadionSlopeDurationI_read();
        void RadionSlopeDurationI_write(const uint8_t value);        
        uint8_t DelayedStart_read();
        void DelayedStart_write(const uint8_t value);        
        uint8_t WaterLevelLow_read();
        void WaterLevelLow_write(const uint8_t value);
        uint8_t WaterLevelHigh_read();
        void WaterLevelHigh_write(const uint8_t value);
        uint8_t DP3Timer_read();
        void DP3Timer_write(const uint8_t value);
        uint8_t LCDID_read();
        void LCDID_write(const uint8_t value);
        uint8_t DCPumpMode_read();
        void DCPumpMode_write(const uint8_t value);
        uint8_t DCPumpSpeed_read();
        void DCPumpSpeed_write(const uint8_t value);
        uint8_t DCPumpDuration_read();
        void DCPumpDuration_write(const uint8_t value);
        uint8_t DCPumpThreshold_read();
        void DCPumpThreshold_write(const uint8_t value);
        uint8_t LEDPWMDaylight2_read();
        void LEDPWMDaylight2_write(const uint8_t value);
        uint8_t LEDPWMActinic2_read();
        void LEDPWMActinic2_write(const uint8_t value);
        uint8_t PWMSlopeStartD2_read();
        void PWMSlopeStartD2_write(const uint8_t value);
        uint8_t PWMSlopeEndD2_read();
        void PWMSlopeEndD2_write(const uint8_t value);
        uint8_t PWMSlopeDurationD2_read();
        void PWMSlopeDurationD2_write(const uint8_t value);
        uint8_t PWMSlopeStartA2_read();
        void PWMSlopeStartA2_write(const uint8_t value);
        uint8_t PWMSlopeEndA2_read();
        void PWMSlopeEndA2_write(const uint8_t value);
        uint8_t PWMSlopeDurationA2_read();
        void PWMSlopeDurationA2_write(const uint8_t value);
        uint8_t CustomExpansion0Decimal_read();
        void CustomExpansion0Decimal_write(const uint8_t value);
        uint8_t CustomExpansion1Decimal_read();
        void CustomExpansion1Decimal_write(const uint8_t value);
        uint8_t CustomExpansion2Decimal_read();
        void CustomExpansion2Decimal_write(const uint8_t value);
        uint8_t CustomExpansion3Decimal_read();
        void CustomExpansion3Decimal_write(const uint8_t value);
        uint8_t CustomExpansion4Decimal_read();
        void CustomExpansion4Decimal_write(const uint8_t value);
        uint8_t CustomExpansion5Decimal_read();
        void CustomExpansion5Decimal_write(const uint8_t value);
        uint8_t CustomExpansion6Decimal_read();
        void CustomExpansion6Decimal_write(const uint8_t value);
        uint8_t CustomExpansion7Decimal_read();
        void CustomExpansion7Decimal_write(const uint8_t value);
        uint8_t Touch_Orientation_read();
        void Touch_Orientation_write(const uint8_t value);
        
        // Functions that read/write an int
        int WM1Timer_read();
        void WM1Timer_write(const int value);
        int WM2Timer_read();
        void WM2Timer_write(const int value);
        int FeedingTimer_read();
        void FeedingTimer_write(const int value);
        int LCDTimer_read();
        void LCDTimer_write(const int value);
        int OverheatTemp_read();
        void OverheatTemp_write(const int value);
        int HeaterTempOn_read();
        void HeaterTempOn_write(const int value);
        int HeaterTempOff_read();
        void HeaterTempOff_write(const int value);
        int ChillerTempOn_read();
        void ChillerTempOn_write(const int value);
        int ChillerTempOff_read();
        void ChillerTempOff_write(const int value);
        int PHMax_read();
        void PHMax_write(const int value);
        int PHMin_read();
        void PHMin_write(const int value);
        int DP1RepeatInterval_read();
        void DP1RepeatInterval_write(const int value);
        int DP2RepeatInterval_read();
        void DP2RepeatInterval_write(const int value);
        int SalMax_read();
        void SalMax_write(const int value);
        int ATOExtendedTimeout_read();
        void ATOExtendedTimeout_write(const int value);
        int ATOHighExtendedTimeout_read();
        void ATOHighExtendedTimeout_write(const int value);
        int ORPMin_read();
        void ORPMin_write(const int value);
        int ORPMax_read();
        void ORPMax_write(const int value);
        int CO2ControlOn_read();
        void CO2ControlOn_write(const int value);
        int CO2ControlOff_read();
        void CO2ControlOff_write(const int value);
        int PHControlOn_read();
        void PHControlOn_write(const int value);
        int PHControlOff_read();
        void PHControlOff_write(const int value);
        int PHEControlOn_read();
        void PHEControlOn_write(const int value);
        int PHEControlOff_read();
        void PHEControlOff_write(const int value);
        int PHExpMax_read();
        void PHExpMax_write(const int value);
        int PHExpMin_read();
        void PHExpMin_write(const int value);
        int WaterLevelMax_read();
        void WaterLevelMax_write(const int value);
        int WaterLevelMin_read();
        void WaterLevelMin_write(const int value);
        int SalTempComp_read();
        void SalTempComp_write(const int value);
        int DP3RepeatInterval_read();
        void DP3RepeatInterval_write(const int value);
        int WaterLevel1Max_read();
        void WaterLevel1Max_write(const int value);
        int WaterLevel1Min_read();
        void WaterLevel1Min_write(const int value);
        int WaterLevel2Max_read();
        void WaterLevel2Max_write(const int value);
        int WaterLevel2Min_read();
        void WaterLevel2Min_write(const int value);
        int WaterLevel3Max_read();
        void WaterLevel3Max_write(const int value);
        int WaterLevel3Min_read();
        void WaterLevel3Min_write(const int value);
        int WaterLevel4Max_read();
        void WaterLevel4Max_write(const int value);
        int WaterLevel4Min_read();
        void WaterLevel4Min_write(const int value);
        
        unsigned long IMCheck_read();
        void IMCheck_write(const unsigned long value);
        // Functions that do the reading/writing to memory
        uint8_t read(int);
        void write(int, const uint8_t);
        int read_int(int);
        void write_int(int, const int);
        uint32_t read_dword(int);
        void write_dword(int, const uint32_t);
};

extern InternalEEPROMClass InternalMemory;

#endif  // __INTERNAL_EEPROM_H__
