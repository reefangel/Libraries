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

#include "RA_Tilt.h"

RA_Tilt::RA_Tilt()
{
	orientation=1;
}

void RA_Tilt::Init()
{
	byte c = readRegister(0x2A); // Read Control register
	writeRegister(0x2A, c & ~(0x01)); //Clear the active bit to go into standby

	// Set up portrait/landscap registers - 4 steps:
	// 1. Enable P/L
	// 2. Set the back/front angle trigger points (z-lock)
	// 3. Set the threshold/hysteresis angle
	// 4. Set the debouce rate
	// For more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4068.pdf
	writeRegister(0x11, 0x40);  // 1. Enable P/L
	writeRegister(0x13, 0x44);  // 2. 29deg z-lock (don't think this register is actually writable)
	writeRegister(0x14, 0x84);  // 3. 45deg thresh, 14deg hyst (don't think this register is writable either)
	writeRegister(0x12, 0x50);  // 4. debounce counter at 100ms (at 800 hz)
	
	
	// Set up interrupt 1 and 2
	writeRegister(0x2C, 0x02);  // Active high, push-pull interrupts
	writeRegister(0x2D, 0x19);  // DRDY, P/L and tap ints enabled
	writeRegister(0x2E, 0x01);  // DRDY on INT1, P/L and taps on INT2	
	
	c = readRegister(0x2A); // Read Control register
	writeRegister(0x2A, c | 0x01); //Set the active bit to begin detection
}

void RA_Tilt::Refresh()
{
	if (digitalRead(13))
	{
		pl=readRegister(0x10); // Reads the PL_STATUS register
#ifdef ILI9341
		switch((pl&0x06)>>1)  // Check on the LAPO[1:0] bits
		{
		case 0:
			orientation=2;
			break;
		case 1:
			orientation=4;
			break;
		case 2:
			orientation=3;
			break;
		case 3:
			orientation=1;
			break;		
		}
#endif //  ILI9341
#ifdef HX8347D
//		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=1;
//		if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=2;
//		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=3;
//		if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=4;
#endif //  HX8347D
#ifdef HX8347G
//		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=2;
//		if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=3;
//		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=4;
//		if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=1;
#endif //  HX8347G
	}
}

byte RA_Tilt::GetOrientation()
{
	return orientation;
}

byte RA_Tilt::readRegister(byte addressToRead)
{
	Wire.beginTransmission(I2CTilt);
	Wire.write(addressToRead);
	Wire.endTransmission(false); //endTransmission but keep the connection active
	Wire.requestFrom(I2CTilt, 1); //Ask for 1 byte, once done, bus is released by default
	while(!Wire.available()) ; //Wait for the data to come back
	return Wire.read(); //Return this one byte
}

void RA_Tilt::writeRegister(byte addressToWrite, byte dataToWrite)
{
	Wire.beginTransmission(I2CTilt);
	Wire.write(addressToWrite);
	Wire.write(dataToWrite);
	Wire.endTransmission(); //Stop transmitting
}
