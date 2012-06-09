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

#include "PH.h"
#include <Globals.h>
#include <Wire.h>


PHClass::PHClass()
{
}

int PHClass::Read()
{
	int iPH=0;
	Wire.requestFrom(I2CPH, 2);
	if (Wire.available())
	{
		iPH = Wire.read();
		iPH = iPH<<8;
		iPH += Wire.read();
	}
	return iPH;
}