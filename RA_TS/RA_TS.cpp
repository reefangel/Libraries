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
	ApplyCalibration();
}

void RA_TS::ApplyCalibration()
{
	eeprom_read_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
	if (calibration.XMin<TS_CALIBRATION_XMIN - TS_CALIBRATION_DELTA || calibration.XMin>TS_CALIBRATION_XMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.XMax<TS_CALIBRATION_XMAX - TS_CALIBRATION_DELTA || calibration.XMax>TS_CALIBRATION_XMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMin<TS_CALIBRATION_YMIN - TS_CALIBRATION_DELTA || calibration.YMin>TS_CALIBRATION_YMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMax<TS_CALIBRATION_YMAX - TS_CALIBRATION_DELTA || calibration.YMax>TS_CALIBRATION_YMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
}	

void RA_TS::SaveCalibration()
{
	eeprom_write_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
}

void RA_TS::GetTouch()
{
	int a,b;
	unsigned long averageX=0;
	unsigned long averageY=0;
		
	SPCR=0x53;
	TP0;
	for (int i=0;i<TouchSample;i++)
	{
		SPI.transfer(0xd0);
		a= SPI.transfer(0);
		b= SPI.transfer(0);
		uX=(a<<5|b>>3);
		averageX+=uX;
	}
	averageX/=TouchSample;
	uX=averageX;

	
	for (int i=0;i<TouchSample;i++)
	{
		SPI.transfer(0x90);
		a= SPI.transfer(0);
		b= SPI.transfer(0);
		averageY+=(a<<5|b>>3);
	}
	averageY/=TouchSample;
	uY=averageY;

//	SPI.transfer(0xb0);
//	a= SPI.transfer(0);
//	b= SPI.transfer(0);
//	uZ1=(a<<5|b>>3);
//
//	SPI.transfer(0xc0);
//	a= SPI.transfer(0);
//	b= SPI.transfer(0);
//	uZ2=(a<<5|b>>3);
//
//	double pressure;
//	pressure=uZ2;
//	pressure/=uZ1;
//	pressure*=100;

//	Serial.println(pressure);
//	if (pressure>TouchPressure) uX=0;

	TP1;
	SPCR=0x50;
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
#ifdef HX8347D	
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
#ifdef HX8347D	
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
	
}

boolean RA_TS::IsTouched()
{
	boolean t=!(PIND&(1<<5));
	if (t) 
	{
		GetTouch();
//		if (FirstX==0 && FirstY==0)
//		{
//			FirstX=X;
//			FirstY=Y;
//		}
//		if (X-FirstX>TouchSlideDelta) SlideIndex++;
//		if (FirstX-X>TouchSlideDelta) SlideIndex--;
	}
//	else
//	{
//		FirstX=FirstY=0;
//		if (SlideIndex>0) ReefTouch.ChangeDisplayedScreen(1);
//		if (SlideIndex<0) ReefTouch.ChangeDisplayedScreen(-1);
//		SlideIndex=0;
//	}
	return t;
}

boolean RA_TS::IsTouchedInside(int x1, int y1, int x2, int y2)
{
//	if (IsTouched()) // check touch
//	{
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
//	}
//	else
//	{
//		touchinsideenabled=true;
//		return false;
//	}
}

boolean RA_TS::IsCalibrationNeeded()
{
	return CalibrationNeeded;
}

void RA_TS::SetOrientation(byte o)
{
	orientation=o;
}