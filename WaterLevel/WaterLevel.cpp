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
	level=0;
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

void WaterLevelClass::Convert()
{
	unsigned long t=0;
	for (int a=0;a<20;a++) t+=Read();
	t/=20;
	if (t!=0)
	{
		t=map(t, InternalMemory.WaterLevelMin_read(), InternalMemory.WaterLevelMax_read(), 0, 100); // apply the calibration to the sensor reading
		t=constrain(t,0,200);
	}
	level = t;
}