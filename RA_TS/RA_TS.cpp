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

#include "RA_TS.h"

RA_TS::RA_TS()
{
}

void RA_TS::Init()
{
	X=0;Y=0;uX=0;uY=0;
	orientation=1;
	CalibrationNeeded=false;
#ifdef TOUCHCAP
	digitalWrite(i2cEnable1,LOW);
	digitalWrite(i2cEnable2,HIGH);
	if ((readRegister8(FT6206_REG_VENDID) != 17) || (readRegister8(FT6206_REG_CHIPID) != 6))
	{
		Serial.println(F("Unknown Touch controller"));
		return;
	}
	else
	{
		writeRegister8(FT6206_REG_THRESHOLD, FT6206_CUSTOM_THRESHOLD);
		writeRegister8(FT6206_REG_G_MODE, FT6206_INT_POLLING);
	}
	digitalWrite(i2cEnable1,HIGH);
	digitalWrite(i2cEnable2,LOW);
#else // TOUCHCAP
	ApplyCalibration();
#endif // TOUCHCAP

#if defined(__SAM3X8E__)
	pinMode(TPINTPin,INPUT); 
	pinMode(TPCSPin,OUTPUT);
#else
	pinMode(okPin,INPUT);
	DDRD|=(1<<7); //PD7 (Output) - TP Chip Select
	PORTD|=(1<<7); //PD7 pull up
#endif // defined(__SAM3X8E__)
	
}

void RA_TS::ApplyCalibration()
{
#if not defined(__SAM3X8E__)
	eeprom_read_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
#else
	calibration.XMin=(InternalMemory.read(TS_CALIBRATION_ADDRESS)<<8)+InternalMemory.read(TS_CALIBRATION_ADDRESS+1);
	calibration.XMax=(InternalMemory.read(TS_CALIBRATION_ADDRESS+2)<<8)+InternalMemory.read(TS_CALIBRATION_ADDRESS+3);
	calibration.YMin=(InternalMemory.read(TS_CALIBRATION_ADDRESS+4)<<8)+InternalMemory.read(TS_CALIBRATION_ADDRESS+5);
	calibration.YMax=(InternalMemory.read(TS_CALIBRATION_ADDRESS+6)<<8)+InternalMemory.read(TS_CALIBRATION_ADDRESS+7);
#endif // defined(__SAM3X8E__)
	if (calibration.XMin<TS_CALIBRATION_XMIN - TS_CALIBRATION_DELTA || calibration.XMin>TS_CALIBRATION_XMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.XMax<TS_CALIBRATION_XMAX - TS_CALIBRATION_DELTA || calibration.XMax>TS_CALIBRATION_XMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMin<TS_CALIBRATION_YMIN - TS_CALIBRATION_DELTA || calibration.YMin>TS_CALIBRATION_YMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMax<TS_CALIBRATION_YMAX - TS_CALIBRATION_DELTA || calibration.YMax>TS_CALIBRATION_YMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
}	

void RA_TS::SaveCalibration()
{
#if not defined(__SAM3X8E__)
	eeprom_write_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
#else
	InternalMemory.write(TS_CALIBRATION_ADDRESS,calibration.XMin>>8);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+1,calibration.XMin&0xff);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+2,calibration.XMax>>8);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+3,calibration.XMax&0xff);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+4,calibration.YMin>>8);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+5,calibration.YMin&0xff);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+6,calibration.YMax>>8);
	InternalMemory.write(TS_CALIBRATION_ADDRESS+7,calibration.YMax&0xff);
#endif // defined(__SAM3X8E__)
}

boolean RA_TS::GetTouch()
{
#ifdef TOUCHCAP
	digitalWrite(i2cEnable1,LOW);
	digitalWrite(i2cEnable2,HIGH);
	X = Y = 0;
	uint8_t n = readRegister8(FT6206_REG_NUMTOUCHES);
	if ((n == 0) || (n > 2)) return false;
	
	uint8_t i2cdat[16];
	enableI2CChannel1();
	Wire.beginTransmission(FT6206_ADDR);
	Wire.write((byte)0);  
	Wire.endTransmission();
	Wire.beginTransmission(FT6206_ADDR);
	Wire.requestFrom((byte)FT6206_ADDR, (byte)32);
	for (uint8_t i=0; i<16; i++)
	i2cdat[i] = Wire.read();
	Wire.endTransmission();  
	disableI2CChannel1();
	touches = i2cdat[0x02];
	digitalWrite(i2cEnable1,HIGH);
	digitalWrite(i2cEnable2,LOW);
	
	if ((touches == 0) || (touches > 2))
	{
		touches=0;
		return false;
	}
	
	for (uint8_t i=0; i<2; i++) {
		touchX[i] = i2cdat[0x03 + i*6] & 0x0F;
		touchX[i] <<= 8;
		touchX[i] |= i2cdat[0x04 + i*6]; 
		touchY[i] = i2cdat[0x05 + i*6] & 0x0F;
		touchY[i] <<= 8;
		touchY[i] |= i2cdat[0x06 + i*6];
		touchID[i] = i2cdat[0x05 + i*6] >> 4;
	}
	uX = touchX[0];
	uY = touchY[0];
	switch (orientation)
	{
	case 1:
		X=240-uX;
		Y=320-uY;
		break;
	case 2:
		X=320-uY;
		Y=uX;
		break;
	case 3:
		X=uX;
		Y=uY;
		break;
	case 4:
		X=uY;
		Y=240-uX;
		break;
	}	
	return true;
	

#else //TOUCHCAP
	int a,b;
	unsigned long averageX=0;
	unsigned long averageY=0;
	double pressure;

//	SPCR=0x53;
#if defined(__SAM3X8E__)
	SPI.setClockDivider(255); 
#endif // defined(__SAM3X8E__)
	TP0;
	for (int i=0;i<TouchSample;i++)
	{
//		SPI.transfer(0xb0);
//		a= SPI.transfer(0);
//		b= SPI.transfer(0);
//		uZ1=(a<<5|b>>3);
//		SPI.transfer(0xc0);
//		a= SPI.transfer(0);
//		b= SPI.transfer(0);
//		uZ2=(a<<5|b>>3);
//
//		pressure=uZ2;
//		pressure/=uZ1;
//		pressure*=100;
//		Serial.println(pressure);

		pressure=0;
		if (pressure<TouchPressure)
		{
			SPI.transfer(0xd0);
			a= SPI.transfer(0);
			b= SPI.transfer(0);
			averageX+=(a<<5|b>>3);
		}
		else
		{
			averageX=0;
			break;
		}
	}
	averageX/=TouchSample;
	uX=averageX;
//	Serial.println(uX);

	for (int i=0;i<TouchSample;i++)
	{
//		SPI.transfer(0xb0);
//		a= SPI.transfer(0);
//		b= SPI.transfer(0);
//		uZ1=(a<<5|b>>3);
//
//		SPI.transfer(0xc0);
//		a= SPI.transfer(0);
//		b= SPI.transfer(0);
//		uZ2=(a<<5|b>>3);
//
//		pressure=uZ2;
//		pressure/=uZ1;
//		pressure*=100;
//		Serial.println(pressure);

		pressure=0;
		if (pressure<TouchPressure)
		{
			SPI.transfer(0x90);
			a= SPI.transfer(0);
			b= SPI.transfer(0);
			averageY+=(a<<5|b>>3);
		}
		else
		{
			averageY=0;
			break;
		}
	}
	averageY/=TouchSample;
	uY=averageY;

	TP1;
#if defined(__SAM3X8E__)
	SPI.setClockDivider(21); 
#endif // defined(__SAM3X8E__)
//	SPCR=0x50;
	if (uX==0) uY=0;

	switch (orientation)
	{
	case 1:
		X=map(uX,calibration.XMin,calibration.XMax,0,239);
		Y=map(uY,calibration.YMin,calibration.YMax,0,319);
		X=constrain(X,0,239);
		Y=constrain(Y,0,319);
		break;
	case 2:
#if defined HX8347D || defined HX8347G
		X=map(uY,calibration.YMax,calibration.YMin,0,319);
		Y=map(uX,calibration.XMin,calibration.XMax,0,239);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
#endif
#ifdef ILI9341
		X=map(uY,calibration.YMax,calibration.YMin,319,0);
		Y=map(uX,calibration.XMin,calibration.XMax,239,0);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
#endif		
		break;
	case 3:
		X=map(uX,calibration.XMax,calibration.XMin,0,239);
		Y=map(uY,calibration.YMax,calibration.YMin,0,319);
		X=constrain(X,0,239);
		Y=constrain(Y,0,319);
		break;
	case 4:
#if defined HX8347D || defined HX8347G
		X=map(uY,calibration.YMin,calibration.YMax,0,319);
		Y=map(uX,calibration.XMax,calibration.XMin,0,239);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
#endif
#ifdef ILI9341
		X=map(uY,calibration.YMin,calibration.YMax,319,0);
		Y=map(uX,calibration.XMax,calibration.XMin,239,0);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
#endif		
		break;
	}
	if (X <= 0) X = 0;
	if (Y <= 0) Y = 0;
	if (uX==0 && uY==0) return false; else return true;
#endif //TOUCHCAP
}

boolean RA_TS::IsTouched()
{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
	boolean t=!digitalRead(okPin);
#elif defined(__SAM3X8E__)
	boolean t=!digitalRead(TPINTPin);
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY	
	if (t) 
		t=GetTouch();
	else
		X = Y = 0;
	return t;
}

boolean RA_TS::IsTouchedInside(int x1, int y1, int x2, int y2)
{
	if (X>x1 && X<x2 && Y>y1 && Y<y2 && touchinsideenabled) // Let's see if it touch was inside the box
	{
		touchinsideenabled=false;
		return true;
	}
	else
	{
		touchinsideenabled=true;
		return false;
	}
}

boolean RA_TS::IsCalibrationNeeded()
{
	return CalibrationNeeded;
}

void RA_TS::SetOrientation(byte o)
{
	orientation=o;
}

uint8_t RA_TS::readRegister8(uint8_t reg) {
	uint8_t x ;
	enableI2CChannel1();
	Wire.beginTransmission(FT6206_ADDR);
	Wire.write((byte)reg);
	Wire.endTransmission();
	Wire.beginTransmission(FT6206_ADDR);
	Wire.requestFrom((byte)FT6206_ADDR, (byte)1);
	x = Wire.read();
	Wire.endTransmission();
    disableI2CChannel1();
	return x;
}

void RA_TS::writeRegister8(uint8_t reg, uint8_t val) {
	enableI2CChannel1();
    Wire.beginTransmission(FT6206_ADDR);
    Wire.write((byte)reg);
    Wire.write((byte)val);
    Wire.endTransmission();
    disableI2CChannel1();
}

void RA_TS::enableI2CChannel1()
{
	Wire.beginTransmission(i2cMux);
	Wire.write(0x1);
	Wire.endTransmission();
}

void RA_TS::disableI2CChannel1()
{
	Wire.beginTransmission(i2cMux);
	Wire.write(0x6);
	Wire.endTransmission();
}


