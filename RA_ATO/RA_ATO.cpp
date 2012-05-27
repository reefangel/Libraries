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

#include "RA_ATO.h"
#include "InternalEEPROM.h"

RA_ATOClass::RA_ATOClass()
{
    topping = false;
    Timer = 0;
}

#ifdef ENABLE_ATO_LOGGING
byte AtoEventCount = 0;

void RA_ATOClass::StartTopping(bool fHighAto /*= false*/)
{
	topping = true;
	int loc = (AtoEventCount * ATOEventSize) + ATOEventStart;
	if ( fHighAto )
	{
		loc += (ATOEventSize * MAX_ATO_LOG_EVENTS);
	}
	InternalMemory.write_dword(loc, now());
	// also clear out the stop time whenever we set a new start time
	InternalMemory.write_dword(loc+ATOEventOffStart, 0);
}

void RA_ATOClass::StopTopping(bool fHighAto /*= false*/)
{
	topping = false;
	int loc = (AtoEventCount * ATOEventSize) + ATOEventStart + ATOEventOffStart;
	if ( fHighAto )
	{
		loc += (ATOEventSize * MAX_ATO_LOG_EVENTS);
	}
	InternalMemory.write_dword(loc, now());
	AtoEventCount++;
	// check if we exceed the max logged events, if so, reset back to 0
	if ( AtoEventCount >= MAX_ATO_LOG_EVENTS ) { AtoEventCount = 0; }
}
#endif  // ENABLE_ATO_LOGGING
