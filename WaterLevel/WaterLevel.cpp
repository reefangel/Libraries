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

#include "WaterLevel.h"
#include <Globals.h>
#include <Wire.h>
#include <InternalEEPROM.h>


WaterLevelClass::WaterLevelClass()
{
	for (int a=0;a<WATERLEVEL_CHANNELS;a++)
		level[a]=0;
}

int WaterLevelClass::Read()
{
	int iWaterLevel=0;
	Wire.requestFrom(I2CWaterLevel, 2);
	if (Wire.available())
	{
		iWaterLevel = Wire.read();
		iWaterLevel = iWaterLevel<<8;
		iWaterLevel += Wire.read();
	}
	return iWaterLevel;
}

int WaterLevelClass::Read(byte channel)
{
	if (channel==0) return Read();
	int iWaterLevel=0;
	Wire.beginTransmission(I2CMultiWaterLevel);
	Wire.write(1); // Config Pointer
	byte addr=(0xb+channel)<<4; // Select which channel to read
	addr+=0x03; // Programmable Gain
	Wire.write(addr);
	Wire.write(0x83);
	Wire.endTransmission();
	delay(10); // It takes 10ms for conversion to be completed
	Wire.beginTransmission(I2CMultiWaterLevel);
	Wire.write(0); // Convert Pointer
	Wire.endTransmission();
	Wire.requestFrom(I2CMultiWaterLevel,2); // Request converted value
	if (Wire.available())
	{
		iWaterLevel = Wire.read();
		iWaterLevel = iWaterLevel<<8;
		iWaterLevel += Wire.read();
	}
	return iWaterLevel>>4;
}

void WaterLevelClass::Convert()
{
	unsigned long t=0;
	for (int a=0;a<20;a++) t+=Read();
	t/=20;
	if (t!=0)
	{
		t=constrain(map(t, InternalMemory.WaterLevelMin_read(), InternalMemory.WaterLevelMax_read(), 0, 100),0,255); // apply the calibration to the sensor reading
	}
	level[0] = t;

	t=Read(1);
	if (t!=0)
	{
		t=constrain(map(t, InternalMemory.WaterLevel1Min_read(), InternalMemory.WaterLevel1Max_read(), 0, 100),0,255); // apply the calibration to the sensor reading
	}
	level[1] = t;

	t=Read(2);
	if (t!=0)
	{
		t=constrain(map(t, InternalMemory.WaterLevel2Min_read(), InternalMemory.WaterLevel2Max_read(), 0, 100),0,255); // apply the calibration to the sensor reading
	}
	level[2] = t;

	t=Read(3);
	if (t!=0)
	{
		t=constrain(map(t, InternalMemory.WaterLevel3Min_read(), InternalMemory.WaterLevel3Max_read(), 0, 100),0,255); // apply the calibration to the sensor reading
	}
	level[3] = t;

	t=Read(4);
	if (t!=0)
	{
		t=constrain(map(t, InternalMemory.WaterLevel4Min_read(), InternalMemory.WaterLevel4Max_read(), 0, 100),0,255); // apply the calibration to the sensor reading
	}
	level[4] = t;
}
