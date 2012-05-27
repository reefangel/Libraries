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

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <Globals.h>

/*
This class allows for reading/writing to the external I2CEEPROM1.
You must specify the address/location in the I2CEEPROM you want to access.
*/
class MemoryClass
{
public:
	MemoryClass();
	byte Read(unsigned int address);
	void Write(unsigned int address, byte data);

private:

};

extern MemoryClass Memory;

#endif  // __MEMORY_H__
