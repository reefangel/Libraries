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

#ifndef __RA_JOYSTICK_H__
#define __RA_JOYSTICK_H__

#include <Globals.h>

class RA_JoystickClass
{

public:
	RA_JoystickClass();
	void Init();
	bool IsButtonPressed();
	bool IsUp();
	bool IsDown();
	bool IsRight();
	bool IsLeft();

private:
	void JoystickCenter();
	void CheckTurbo();
	int CalV, CalH;
	unsigned long KeyKeep;
	byte KeyTurbo;
	byte KeyCount;
#if defined REEFANGEL_MINI || defined RANET 	
	unsigned long ButtonDebounce;
#endif // REEFANGEL_MINI
};

#endif  // __RA_JOYSTICK_H__
