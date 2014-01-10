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

#ifndef __WaterLevel_H__
#define __WaterLevel_H__

#include <Globals.h>

class WaterLevelClass
{
public:
	WaterLevelClass();
	int LastLevel[WATERLEVEL_CHANNELS];
	int Read();
	int Read(byte channel);
	void Convert();
	inline byte GetLevel() { return level[0]; } ;
	inline byte GetLevel(byte channel) { if (channel>=WATERLEVEL_CHANNELS) return 0; else return level[channel]; } ;
	inline byte SetLevel(byte channel, int value) { if (channel<=WATERLEVEL_CHANNELS) level[channel]=value; } ;
private:
	byte level[WATERLEVEL_CHANNELS];
};

#endif  // __WaterLevel_H__
