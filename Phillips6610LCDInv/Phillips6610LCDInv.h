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

#ifndef	Phillips6610LCDInv_h

#define Phillips6610LCDInv_h

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

class Phillips6610LCDInv
{

public:



Phillips6610LCDInv();

void lcd_init();
void lcd_Sleep();
void lcd_Wake();
void lcd_BacklightOn();
void lcd_BacklightOff();
void lcd_clear(byte color, byte x1, byte y1, byte x2, byte y2);
void lcd_draw_text(byte fcolor, byte bcolor, byte x, byte y,char *text);
void lcd_put_pixel(byte color, byte x, byte y);
void lcd_set_box(byte x1, byte y1, byte x2, byte y2);
void sendCMD(byte data);
void sendData(byte data);

private:
void shiftBits(byte b);
void draw_text_line(byte fcolor, byte bcolor,byte x, byte y,char c);

};
#endif
