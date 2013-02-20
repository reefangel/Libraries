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

#ifndef	__RA_TOUCHLCD_H__
#define __RA_TOUCHLCD_H__

#include <Globals.h>
#include <Time.h>
#include <Wire.h>
#include <InternalEEPROM.h>
#include <Memory.h>
#include <SD.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "RA_TFT.h"
#include "Font.h"

class RA_TouchLCD
{

public:
    RA_TouchLCD();

	void Init();
	void SetBacklight(byte bl);
	void SetOrientation(byte O);
	byte GetOrientation();
	int GetWidth();
	int GetHeight();
	void Scroll(int offset);
	void FullClear(int color);
	void Clear(int color, int x1, int y1, int x2, int y2);
	void DrawPixel(int color, int x, int y);
	void DrawLine(int color, int x1, int y1, int x2, int y2);
	void DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill);
	void DrawCircle(int color, int x, int y, int radius, boolean fill);
	void DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill);
	void DrawBMP(int ix, int iy, const prog_uchar *iPtr);
	void DrawSDImage(char *bmp, int x, int y);
	void DrawSDRawImage(char *bmp, int x, int y, int w, int h);
	void DrawDateTime(int x, int y, boolean militarytime, FontClass Font);
	void DrawRelayStatus(int x, int y, boolean status, boolean override);
private:
	int w,h;
	byte orientation;		
};

#endif  // __RA_TOUCHLCD_H__