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

#include "Relay.h"
#include <Globals.h>
#include <Wire.h>

uint32_t LastStart;
byte DelayedOnPorts;
#ifdef RelayExp
byte DelayedOnPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp

RelayClass::RelayClass()
{
	RelayData = 0;
	RelayMaskOn = 0;
	RelayMaskOff = 0xff;
#ifdef SaveRelaysPresent
	// assume relay is present till we hear otherwise.
	RelayPresent = true;
#endif
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0;
		RelayMaskOnE[EID] = 0;
		RelayMaskOffE[EID] = 0xff;
#ifdef SaveRelaysPresent
		RelayPresentE[EID] = true;
#endif  // SaveRelaysPresent
#ifdef RANET
		RANetFallBackE[EID] = 0;
#endif  // RANET
	}
#endif  // RelayExp
}

void RelayClass::Auto(byte ID)
{
	Override(ID,2);
}

void RelayClass::On(byte ID)
{
    if ( ID < 9 ) bitSet(RelayData, ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		bitSet(RelayDataE[EID-1],(ID%10)-1);
	}
#endif  // RelayExp
}

void RelayClass::DelayedOn(byte ID, byte MinuteDelay)
{
	/*
	We need to see if the MinuteDelay since LastStart has elapsed before we can turn on our port

	Set the DelayedOnPorts flag indicating that it's a delayed on port
	*/
    if ( ID < 9 ) bitSet(DelayedOnPorts, ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		bitSet(DelayedOnPortsE[EID-1],(ID%10)-1);
	}
#endif  // RelayExp

	uint16_t x = MinuteDelay;
	x *= SECS_PER_MIN;
	if ( now()-LastStart > x )
	{
		On(ID);
	}
}

void RelayClass::DelayedOn(byte ID)
{
	DelayedOn(ID, InternalMemory.DelayedStart_read());
}

void RelayClass::Off(byte ID)
{
    if ( ID < 9 ) bitClear(RelayData, ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		bitClear(RelayDataE[EID-1],(ID%10)-1);
	}
#endif  // RelayExp
}

void RelayClass::AllOn()
{
	RelayData = 0xff;
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0xff;
	}
#endif  // RelayExp
}

void RelayClass::AllOff()
{
	RelayData = 0;
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0;
	}
#endif  // RelayExp
}

void RelayClass::Toggle(byte ID)
{
	if ( ID < 9 ) RelayData ^= 1 << (ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		RelayDataE[EID-1] ^= 1 << ((ID%10)-1);
	}
#endif  // RelayExp
}

void RelayClass::Set(byte ID, boolean Status)
{
	if (Status) On(ID); else Off(ID);
}


void RelayClass::Write()
{
#ifndef RA_TOUCHDISPLAY
    byte TempRelay = RelayData;
	byte present = 0;
    TempRelay &= RelayMaskOff;
    TempRelay |= RelayMaskOn;

    Wire.beginTransmission(I2CExpander1);
    Wire.write(~TempRelay);   // MSB
    present = Wire.endTransmission();
#ifdef SaveRelaysPresent
	RelayPresent = (present == 0);
#endif  // SaveRelaysPresent

#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		TempRelay = RelayDataE[EID];
		TempRelay &= RelayMaskOffE[EID];
		TempRelay |= RelayMaskOnE[EID];
		Wire.beginTransmission(I2CExpModule+EID);
		Wire.write(~TempRelay);  // MSB
		present = Wire.endTransmission();
#ifdef SaveRelaysPresent
		RelayPresentE[EID] = (present == 0);
#endif  // SaveRelaysPresent
	}
#endif  // RelayExp
#endif  // RA_TOUCHDISPLAY
}

boolean RelayClass::Status(byte ID)
{
    if ( ID < 9 ) return bitRead((RelayData & RelayMaskOff) | RelayMaskOn, ID-1);
#ifdef RelayExp
   if ( (ID > 10) && (ID < 89) )
   {
      byte EID = byte(ID/10);
      return bitRead((RelayDataE[EID-1] & RelayMaskOffE[EID-1]) | RelayMaskOnE[EID-1], (ID%10)-1);
   }
#endif  // RelayExp
   return false;
}

boolean RelayClass::isMaskOn(byte ID) {
  if ( ID < 9 ) return bitRead(RelayMaskOn, ID-1);
#ifdef RelayExp
  if ( (ID > 10) && (ID < 89) )
  {
    byte EID = byte(ID/10);
    return bitRead(RelayMaskOnE[EID-1], (ID%10)-1);
  }
#endif  // RelayExp
  return false;
}

boolean RelayClass::isMaskOff(byte ID) {
  if ( ID < 9 ) return !bitRead(RelayMaskOff, ID-1);
#ifdef RelayExp
  if ( (ID > 10) && (ID < 89) )
  {
    byte EID = byte(ID/10);
    return !bitRead(RelayMaskOffE[EID-1], (ID%10)-1);
  }
#endif  // RelayExp
  return false;
}

void RelayClass::Override(byte ID, byte type)
{

    if (type==0)  // Turn port off
    {
        if ( ID < 9 )
        {
            bitClear(RelayMaskOn,ID-1);
            bitClear(RelayMaskOff,ID-1);
        }
#ifdef RelayExp
        if ( (ID > 10) && (ID < 89) )
        {
            byte EID = byte(ID/10);
            bitClear(RelayMaskOnE[EID-1],(ID%10)-1);
            bitClear(RelayMaskOffE[EID-1],(ID%10)-1);
        }
#endif  // RelayExp
    }
    else if (type==1)  // Turn port on
    {
        if ( ID < 9 )
        {
            bitSet(RelayMaskOn,ID-1);
            bitSet(RelayMaskOff,ID-1);
        }
#ifdef RelayExp
        if ( (ID > 10) && (ID < 89) )
        {
            byte EID = byte(ID/10);
            bitSet(RelayMaskOnE[EID-1],(ID%10)-1);
            bitSet(RelayMaskOffE[EID-1],(ID%10)-1);
        }
#endif  // RelayExp
    }
    else if (type==2)  // Set port back to Auto
    {
        if ( ID < 9 )
        {
            bitClear(RelayMaskOn,ID-1);
            bitSet(RelayMaskOff,ID-1);
        }
#ifdef RelayExp
        if ( (ID > 10) && (ID < 89) )
        {
            byte EID = byte(ID/10);
            bitClear(RelayMaskOnE[EID-1],(ID%10)-1);
            bitSet(RelayMaskOffE[EID-1],(ID%10)-1);
        }
#endif  // RelayExp
    }
}

#ifdef SaveRelaysPresent
boolean RelayClass::IsRelayPresent (byte module)
{
	if (module == 0xff)
		return RelayPresent;
	if (module >= MAX_RELAY_EXPANSION_MODULES)
		return false;
#ifdef RelayExp
	return RelayPresentE[module];
#else
	return false;
#endif  // RelayExp
}
#endif  // SaveRelaysPresent
