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
	writeRegister(0x2A, c | 0x01); //Set the active bit to begin detection
}

void RA_Tilt::Refresh()
{
	byte c = readRegister(0x0D);  // Read WHO_AM_I register
	if (c == 0x2A) // WHO_AM_I should always be 0x2A
	{
		Wire.beginTransmission(I2CTilt);
		Wire.write(0x01); // Set register to X axis
		Wire.endTransmission(false); //endTransmission but keep the connection active
		Wire.requestFrom(I2CTilt, 3); //Ask for bytes, once done, bus is released by default
		delay(10);
		if (Wire.available() == 3)
		{
			x=Wire.read(); // read X axis MSB
			Wire.read(); // read X axis LSB
			y=Wire.read(); // read Y axis MSB
			Serial.println("OK");
		}
		else
		{
			Serial.println("Wrong data");
			do 
			{
				Wire.read();
			}
			while(Wire.available());
		}
	#ifdef ILI9341	
		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=4;
		if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=3;
		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=2;
		if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=1;
	#endif //  ILI9341
	#ifdef HX8347D
		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=1;
		if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=2;
		if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=3;
		if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=4;
	#endif //  HX8347D
	}
	else
	{
		Serial.println("Error");
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