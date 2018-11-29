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
typedef unsigned char byte;
 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#ifndef __RA_TEMPSENSOR_H__
#define __RA_TEMPSENSOR_H__

#include <Globals.h>

class RA_TempSensorClass
{
public:
	RA_TempSensorClass();
	void Init();
	void RequestConversion();
	int ReadTemperature(byte addr[8]);
	void SendRequest(byte addr[8]);

	byte addrT1[8];
	byte addrT2[8];
	byte addrT3[8];
#ifdef EXTRA_TEMP_PROBES
	static const int ProbeCount = 6;
	byte addrT4[8];
	byte addrT5[8];
	byte addrT6[8];
#else
    static const int ProbeCount = 3;
#endif
    byte* addrArray[ProbeCount];
    void RemapSensors(byte map[ProbeCount]);
	byte unit;
};

#endif // __RA_TEMPSENSOR_H__
