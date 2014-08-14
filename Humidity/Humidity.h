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

#ifndef __Humidity_H__
#define __Humidity_H__

#include <Globals.h>

class HumidityClass
{
public:
	HumidityClass();
	int LastLevel;
	void Read();
	inline int GetLevel() {  return level; } ;
	inline int SetLevel(int value) {  level=value; } ;
private:
	int level;
	unsigned long lastlevel;
};

#endif  // __Humidity_H__
