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

#ifndef __LED_H__
#define __LED_H__

#include <Globals.h>

class LEDClass
{
public:
	LEDClass();
	inline void On() { digitalWrite(ledPin, HIGH); }
	inline void Off() { digitalWrite(ledPin, LOW); }
#ifdef REEFANGEL_MINI
	void RGB(byte R, byte G, byte B);
#endif //REEFANGEL_MINI
};


#endif // __LED_H__
