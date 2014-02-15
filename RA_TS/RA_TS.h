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

#ifndef __RA_TS_H__
#define __RA_TS_H__

#include <Globals.h>
#include <SPI.h>
#include <InternalEEPROM.h>
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY

#include <avr/eeprom.h>
#define TP0	cbi(PORTD, 7)
#define TP1 sbi(PORTD, 7)

#elif defined(__SAM3X8E__)

#define TP0  PIOD->PIO_CODR |= 1<<3;
#define TP1  PIOD->PIO_SODR |= 1<<3;

#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY

class RA_TS
{
	public:

		int X,Y,uX,uY,uZ1,uZ2;
		CALIBRATION calibration;
		RA_TS();
		void Init();
		void SaveCalibration();
		boolean GetTouch();
		boolean IsTouched();
		boolean IsTouchedInside(int x1, int y1, int x2, int y2);
		boolean IsCalibrationNeeded();
		int FirstX, FirstY;
		signed char SlideIndex;
		void SetOrientation(byte o);
	private:
		boolean CalibrationNeeded;
		void ApplyCalibration();
		byte orientation;
		boolean touchinsideenabled;
};
#endif // __RA_TS_H__
