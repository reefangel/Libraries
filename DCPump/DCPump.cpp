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

#include "Globals.h"
#include "DCPump.h"

DCPumpClass::DCPumpClass()
{
	UseMemory=true;
	FeedingSpeed=0;
	WaterChangeSpeed=0;
	DaylightChannel=None;
	ActinicChannel=None;
	LowATOChannel=None;
	Threshold=30;
#ifdef PWMEXPANSION
	for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
		ExpansionChannel[a]=None;
#endif // PWMEXPANSION
}

void DCPumpClass::SetMode(byte mode, byte speed, byte duration)
{
	 Mode=mode;
	 Speed=speed;
	 Duration=duration;
         Threshold=30;
}

void DCPumpClass::SetMode(byte mode, byte speed, byte duration, byte threshold)
{
	 Mode=mode;
	 Speed=speed;
	 Duration=duration;
         Threshold=threshold;
}
