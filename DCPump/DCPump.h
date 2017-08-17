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

#ifndef __DCPump_H__
#define __DCPump_H__

#include <InternalEEPROM.h>

class DCPumpClass
{
public:
	 DCPumpClass();
	 boolean UseMemory;
	 byte Mode;
	 byte Speed;
	 byte Duration;
	 byte Threshold;
	 byte AntiSyncOffset;
	 byte DaylightChannel;
	 byte ActinicChannel;
#ifdef RA_STAR
	 byte Daylight2Channel;
	 byte Actinic2Channel;
#endif
	 byte LowATOChannel;
	 byte HighATOChannel;
	 byte FeedingSpeed;
	 byte WaterChangeSpeed;
#ifdef PWMEXPANSION
	 byte ExpansionChannel[PWM_EXPANSION_CHANNELS];
#endif // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
	 byte SIXTEENChExpansionChannel[SIXTEENCH_PWM_EXPANSION_CHANNELS];
#endif // SIXTEENCHPWMEXPANSION
	 void SetMode(byte mode, byte speed, byte duration);
	 void SetMode(byte mode, byte speed, byte duration, byte threshold);
private:
};

#endif  // __DCPump_H__
