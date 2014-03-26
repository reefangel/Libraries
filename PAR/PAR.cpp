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

#include "PAR.h"
#include <Globals.h>
#include <Wire.h>
#include <InternalEEPROM.h>


PARClass::PARClass()
{
	level=0;
}

int PARClass::Read()
{
	int iPAR=0;
	Wire.requestFrom(I2CPAR, 2);
	if (Wire.available())
	{
		iPAR = Wire.read();
		iPAR = iPAR<<8;
		iPAR += Wire.read();
	}
	return iPAR;
}

void PARClass::Convert()
{
	unsigned long t=0;
	for (int a=0;a<10;a++) t+=Read();
	t/=10;
	if (t!=0)
	{
		t*=5000; // apply the calibration to the sensor reading
		t/=8192;
	}
	level = t;
}
