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
#include <Wire.h>
#include <InternalEEPROM.h>
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR

#include <avr/eeprom.h>
#define TP0	cbi(PORTD, 7)
#define TP1 sbi(PORTD, 7)

// Capacitive Touch FT6206

#define FT6206_ADDR           0x38
#define FT6206_G_FT5201ID     0xA8
#define FT6206_REG_NUMTOUCHES 0x02

#define FT6206_NUM_X             0x33
#define FT6206_NUM_Y             0x34

#define FT6206_INT_POLLING	0x00
#define FT6206_INT_TRIGGER	0x01

#define FT6206_REG_MODE 0x00
#define FT6206_REG_GEST_ID 0x01
#define FT6206_REG_CALIBRATE 0x02
#define FT6206_REG_WORKMODE 0x00
#define FT6206_REG_FACTORYMODE 0x40
#define FT6206_REG_THRESHOLD 0x80
#define FT6206_REG_POINTRATE 0x88
#define FT6206_REG_FIRMVERS 0xA6
#define FT6206_REG_CHIPID 0xA3
#define FT6206_REG_G_MODE 0xA4
#define FT6206_REG_VENDID 0xA8

#define FT6206_DEFAULT_THRESHOLD 128
#define FT6206_CUSTOM_THRESHOLD 40

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
#ifdef TOUCHCAP
		void writeRegister8(uint8_t reg, uint8_t val);
		uint8_t readRegister8(uint8_t reg);
		void enableI2CChannel1();
		void disableI2CChannel1();
#endif // TOUCHCAP
		
	private:
		boolean CalibrationNeeded;
		void ApplyCalibration();
		byte orientation;
		boolean touchinsideenabled;
#ifdef TOUCHCAP
		  uint8_t touches;
		  uint16_t touchX[2], touchY[2], touchID[2];
#endif // TOUCHCAP	
};
#endif // __RA_TS_H__
