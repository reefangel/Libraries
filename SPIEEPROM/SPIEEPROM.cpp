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

#include "SPIEEPROM.h"
#include <Globals.h>
#include <SPI.h>

SPIEEPROMClass::SPIEEPROMClass()
{
}

void SPIEEPROMClass::Init()
{
	pinMode(EEPROMPin, OUTPUT);
	digitalWrite(EEPROMPin, HIGH);  // CS pin is high when idle.
//	SPI.begin();
//	SPI.setClockDivider(255);
//	SPI.setDataMode(SPI_MODE0);
}

byte SPIEEPROMClass::Read(unsigned int address)
{
	SPI.setClockDivider(50);
	byte result=255;
	digitalWrite(EEPROMPin, LOW);
	SPI.transfer(SPIEEPROM_READ);
	SPI.transfer(address>>8);
	SPI.transfer(address & 0xff);
	result=SPI.transfer(0);
	digitalWrite(EEPROMPin, HIGH);
	SPI.setClockDivider(21);
	return result;
}

void SPIEEPROMClass::Write(unsigned int address, byte data)
{
	SPI.setClockDivider(50);
	digitalWrite(EEPROMPin, LOW);
	SPI.transfer(SPIEEPROM_WREN);
	digitalWrite(EEPROMPin, HIGH);
	delay(1);
	digitalWrite(EEPROMPin, LOW);
	SPI.transfer(SPIEEPROM_WRITE);
	SPI.transfer(address>>8);
	SPI.transfer(address & 0xff);
	SPI.transfer(data);
	digitalWrite(EEPROMPin, HIGH);
	delay(10);
	SPI.setClockDivider(21);
}

SPIEEPROMClass SPIEEPROM;

