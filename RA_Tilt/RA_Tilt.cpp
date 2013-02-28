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
	Write(0x16,0x5);
	ApplyCompensation();
}

void RA_Tilt::Write(byte reg, byte data)
{
	ACC0;
	SPI.transfer(0x80 | (reg << 1));
	SPI.transfer(data);
	ACC1;
}

byte RA_Tilt::Read(byte data)
{
	SPI.transfer(data << 1);
	return SPI.transfer(0);
}

void RA_Tilt::Refresh()
{
	ACC0;
	x=Read(0x6);
	y=Read(0x7);
	ACC1;
//	if (x<0) x=0;
//	if (y<0) y=0;
	//if (Tilt.GetX()>-TT_SENSITIVITY && Tilt.GetX()<TT_SENSITIVITY && Tilt.GetY()>-TT_SENSITIVITY && Tilt.GetY()<TT_SENSITIVITY) LCD.SetOrientation(1);
#ifdef ILI9341	
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=1;
	if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=4;
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=3;
	if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=2;
#endif //  ILI9341
#ifdef HX8347D
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) orientation=1;
	if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=2;
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) orientation=3;
	if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) orientation=4;
#endif //  HX8347D
}

void RA_Tilt::CompensateAccelerometer()
{
	int xoffd, yoffd;
	compensation.XOff = 0;
	compensation.YOff = 0;
	do
	{
		Refresh();
	    xoffd = -2.1*(x);
	    yoffd = -2.1*(y);
	    compensation.XOff += xoffd;
	    compensation.YOff += yoffd;
	    Write(0x10, 0xFF&compensation.XOff);
	    Write(0x11, 0xFF&(compensation.XOff>>8));
	    Write(0x12, 0xFF&compensation.YOff);
	    Write(0x13, 0xFF&(compensation.YOff>>8));
	    delay(10);
	} 
	while (abs(xoffd) > 2 || abs(yoffd) > 2);
	eeprom_write_block((void*)&compensation, (void*)TT_COMPENSATION_ADDRESS, sizeof(COMPENSATION));
}

void RA_Tilt::ApplyCompensation()
{
	eeprom_read_block((void*)&compensation, (void*)TT_COMPENSATION_ADDRESS, sizeof(COMPENSATION));
    Write(0x10, 0xFF&compensation.XOff);
    Write(0x11, 0xFF&(compensation.XOff>>8));
    Write(0x12, 0xFF&compensation.YOff);
    Write(0x13, 0xFF&(compensation.YOff>>8));
    delay(10);
}	

byte RA_Tilt::GetOrientation()
{
	return orientation;
}