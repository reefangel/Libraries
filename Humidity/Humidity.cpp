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

#include "Humidity.h"
#include <Globals.h>
#include <Wire.h>


HumidityClass::HumidityClass()
{
	level=0;
}

void HumidityClass::Read()
{
	if (millis()-lastlevel>800) // Only get levels every 800ms
	{
		int iHumidity=0;
		int reply[10];
		
		lastlevel=millis();
		Wire.beginTransmission(I2CHumidity);
		Wire.endTransmission();  // For some reason, it needs this to work??? Bug with sensor??
		Wire.beginTransmission(I2CHumidity);
		Wire.write(0x3); // 0x3 for read 0x10 for write to registers
		Wire.write(0x0);  // start at address 0x0 for humidity
		Wire.write(2);  // request 2 bytes data for humidity or 4 bytes for temperature+humidity
		Wire.endTransmission();
		
		Wire.requestFrom(I2CHumidity, 6); // Request 6 bytes
		for (int i=0;i<6;i++)
			if(Wire.available()) reply[i] = Wire.read();
		if (reply[0]==0x3 && reply[1]==0x2) // The response need to contain function (0x3) and length of data (0x2)
		{
			int crc=reply[5];
			crc<<=8;
			crc+=reply[4];
			if (crc == crc16(reply,4))
			{
				iHumidity=reply[2];
				iHumidity<<=8;
				iHumidity+=reply[3];
			}
		}
		level=iHumidity/10;
	}
}
