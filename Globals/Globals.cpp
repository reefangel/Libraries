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

#include "Globals.h"

byte intlength(int intin)
{
  if (intin>9999) return 5;
  if (intin>999) return 4;
  if (intin>99) return 3;
  if (intin>9) return 2;
  if (intin>=0 && intin<=9) return 1;
  if (intin<0) return 2;
}

int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute)
{
	return (ScheduleHour*60) + ScheduleMinute;
}

bool IsLeapYear(int year)
{
    if (year % 4 != 0)
    {
        return false;
    }
    else if (year % 400 == 0)
    {
        return true;
    }
    else if (year % 100 == 0)
    {
        return false;
    }
    return true;
}

byte PWMSlope(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte Duration, byte oldValue)
{
	int Now = NumMins(hour(), minute());
	int Start = NumMins(startHour, startMinute);
	int StartD = Start + Duration;
	int End = NumMins(endHour, endMinute);
	int StopD = End - Duration;

	if ( Now >= Start && Now <= StartD )
		return constrain(map(Now, Start, StartD, startPWM, endPWM),startPWM, endPWM);
	else if ( Now >= StopD && Now <= End )
	{
		byte v = constrain(map(Now, StopD, End, startPWM, endPWM),startPWM, endPWM);
		return endPWM-v+startPWM;
	}
	else if ( Now > StartD && Now < StopD )
		return endPWM;

    // lastly return the existing value
    return oldValue;
}

byte PWMParabola(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte oldValue)
{
	int Now = NumMins(hour(), minute());
	int Start = NumMins(startHour, startMinute);
	int End = NumMins(endHour, endMinute);
	byte PWMDelta = endPWM-startPWM;
	byte ParabolaPhase=constrain(map(Now,Start,End,0,180),0,180);

	if ( Now <= Start || Now >= End)
		return oldValue;
	else
	{
		return startPWM+(PWMDelta*sin(radians(ParabolaPhase)));
	}

}

byte MoonPhase()
{
	int m,d,y;
	int yy,mm;
	long K1,K2,K3,J,V;
	byte PWMvalue;
	m = month();
	d = day();
	y = year();
	yy = y-((12-m)/10);
	mm = m+9;
	if (mm>=12) mm -= 12;
	K1 = 365.25*(yy+4712);
	K2 = 30.6*mm+.5;
	K3 = int(int((yy/100)+49)*.75)-38;
	J = K1+K2+d+59-K3;
	V = (J-2451550.1)/0.29530588853;
	V -= int(V/100)*100;
	V = abs(V-50);
	PWMvalue = 4*abs(50-V);  // 5.12=100%    4=~80%
	//pinMode(lowATOPin,OUTPUT);
	return (PWMvalue*100)/255;
}

void ConvertNumToString(char* string, int num, byte decimal)
{
    char temptxt[3];
    int Temp = num;
    if (Temp==0xFFFF) Temp=0;
	itoa(Temp/decimal,string,10);
	if (decimal>1)
	{
		itoa(Temp%decimal,temptxt,10);
		strcat(string , ".");
		if (Temp%decimal<10 && decimal==100) strcat(string , "0");
		strcat(string , temptxt);
	}
}

int alphaBlend(int fgcolor, byte a)
{
	int r=((fgcolor>>11)&0x1f)<<3;
	int g=((fgcolor>>5)&0x3f)<<2;
	int b=(fgcolor&0x1F)<<3;
	r*=a;
	r/=100;
	g*=a;
	g/=100;
	b*=a;
	b/=100;
	return RGB565(r,g,b);
}

int alphaBlend(int fgcolor, int bgcolor, byte a)
{
	int r=((fgcolor>>11)&0x1f)<<3;
	int g=((fgcolor>>5)&0x3f)<<2;
	int b=(fgcolor&0x1F)<<3;
	r*=a;
	g*=a;
	b*=a;
	int r1=((bgcolor>>11)&0x1f)<<3;
	int g1=((bgcolor>>5)&0x3f)<<2;
	int b1=(bgcolor&0x1F)<<3;
	r1*=(100-a);
	g1*=(100-a);
	b1*=(100-a);
	r+=r1;
	g+=g1;
	b+=b1;
	r/=100;
	g/=100;
	b/=100;

	return RGB565(r,g,b);
}


// for pure virtual functions
void __cxa_pure_virtual(void){};
// other fixes
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
