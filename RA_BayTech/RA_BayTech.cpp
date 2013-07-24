/*
 * Copyright 2013 Reef Angel / Benjamin Runnels
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

/**
 * RA_BayTech emulates RA Relay to control a BayTech power distribution switch and gathers stats via serial connection
 */

#include "RA_BayTech.h"

RA_BayTech::RA_BayTech(void) :
  BT1(Serial1)
{
  RelayData = 0;
  RelayMaskOn = 0;
  RelayMaskOff = 0xff;
#ifdef SaveRelaysPresent
  // assume relay is present till we hear otherwise.
  RelayPresent = true;
#endif
#ifdef RelayExp
  for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
  {
    RelayDataE[EID] = 0;
    RelayMaskOnE[EID] = 0;
    RelayMaskOffE[EID] = 0xff;
#ifdef SaveRelaysPresent
    RelayPresentE[EID] = true;
#endif  // SaveRelaysPresent
  }
#endif  // RelayExp
}

void RA_BayTech::Write(void)
{
#ifndef RA_TOUCHDISPLAY
  if (BT1.Available())
  {
    byte TempRelay = RelayData;
    byte present = 0;
    TempRelay &= RelayMaskOff;
    TempRelay |= RelayMaskOn;

    if (TempRelay != BT1.OutletData)
    {
//      Serial.print("Average Power: '");
//      Serial.print(BT1.GetAveragePower());
//      Serial.println("' Watts");
//
//      Serial.print("Apparent Power: '");
//      Serial.print(BT1.GetAveragePower());
//      Serial.println("' VA");
//
//      Serial.print("True RMS Voltage: '");
//      Serial.print(BT1.GetVoltage());
//      Serial.println("' Volts");
//
//      Serial.print("True RMS Current: '");
//      Serial.print(BT1.GetCurrent());
//      Serial.println("' Amps");
//      Serial.print("Maximum Detected: '");
//      Serial.print(BT1.GetMaxCurrent());
//      Serial.println("' Amps");
//
//      Serial.print("Internal Temperature: '");
//      Serial.print(BT1.GetTemperature());
//      Serial.println("' C");
//
//      Serial.print("Outlet Circuit Breaker: '");
//      Serial.print(BT1.GetBreaker());
//      Serial.println("'");

      if (TempRelay == 0)
      {
//        Serial.println("BT1 All Off");
        BT1.AllOff();
      }
      else if (TempRelay == 255)
      {
//        Serial.println("BT1 All On");
        BT1.AllOn();
      }
      else
      {
        for (int i = 0; i < 8; i++)
        {
          boolean status = BT1.Status(i);

          if (bitRead(TempRelay, i) && !status)
          {
//            Serial.print("Turning On BT1 outlet ");
//            Serial.println(i+1);

            BT1.On(i);
            return;
          }
          else if (!bitRead(TempRelay, i) && status)
          {
//            Serial.print("Turning Off BT1 outlet ");
//            Serial.println(i+1);

            BT1.Off(i);
            return;
          }
        }
      }
    }
  }
#ifdef SaveRelaysPresent
  RelayPresent = (present == 0);
#endif  // SaveRelaysPresent
#ifdef RelayExp
  for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
  {
    TempRelay = RelayDataE[EID];
    TempRelay &= RelayMaskOffE[EID];
    TempRelay |= RelayMaskOnE[EID];
    Wire.beginTransmission(I2CExpModule+EID);
    Wire.write(~TempRelay);  // MSB
    present = Wire.endTransmission();
#ifdef SaveRelaysPresent
    RelayPresentE[EID] = (present == 0);
#endif  // SaveRelaysPresent
  }
#endif  // RelayExp
#endif  // RA_TOUCHDISPLAY
}
