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

#include <Globals.h>
#include <TimerClass.h>

TimerClass::TimerClass()
{
	Trigger = 0;
	Interval = 0;
}

void TimerClass::Start()
{
	if (Interval)
	{
	    Trigger = now() + Interval;
	}
}

bool TimerClass::IsTriggered()
{
	if (now()>Trigger && Trigger!=0)
	{
		Trigger = 0;
		return true;
	}
	else
	{
		return false;
	}
}
