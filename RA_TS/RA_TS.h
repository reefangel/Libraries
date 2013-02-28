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
#include <avr/eeprom.h>
#include <SPI.h>

#define TP0	cbi(PORTD, 7)
#define TP1 sbi(PORTD, 7)

class RA_TS
{
	public:

		int X,Y,uX,uY;
		CALIBRATION calibration;
		RA_TS();
		void Init();
		void SaveCalibration();
		void GetTouch();
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
