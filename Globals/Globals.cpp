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
    byte digits = 0;
    if (intin <= 0) digits = 1; // "-" and 0 counts as 1 character
    while (intin) {
    	intin /= 10;
        digits++;
    }
    return digits;
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
	// Contribution of thekameleon
	// http://forum.reefangel.com/viewtopic.php?p=23893#p23893
	LightsOverride=true;
	int current_hour = hour();
	int start = NumMins(startHour, startMinute);
	int end = NumMins(endHour, endMinute);
	if (start > end) //Start is greater than End so its over midnight
	{
		//Example: 2300hrs to 0200hrs
		if (current_hour < endHour) start -= 1440; //past midnight
		if (current_hour >= startHour) end += 1440; //before midnight
	}
	int current = NumMins(current_hour, minute());
	int startD = start + Duration;
	int stopD = end - Duration;

	if ( current >= start && current <= startD )
		return constrain(map(current, start, startD, startPWM, endPWM),startPWM, endPWM);
	else if ( current >= stopD && current <= end )
	{
		byte v = constrain(map(current, stopD, end, startPWM, endPWM),startPWM, endPWM);
		return endPWM - v + startPWM;
	}
	else if ( current > startD && current < stopD )
		return endPWM;

	// lastly return the existing value
	return oldValue;
}

byte PWMParabola(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte oldValue)
{
	// Contribution of thekameleon
	// http://forum.reefangel.com/viewtopic.php?p=23813#p23813
	LightsOverride=true;
	int current_hour = hour();
	int start = NumMins(startHour, startMinute);
	int end = NumMins(endHour, endMinute);
	if (start > end) //Start is greater than End so its over midnight
	{
		//Example: 2300hrs to 0200hrs
		if (current_hour < endHour) start -= 1440; //past midnight
		if (current_hour >= startHour) end += 1440; //before midnight
	}

	int current = NumMins(current_hour, minute());

	if ( current <= start || current >= end)
		return oldValue;
	else
	{
		byte pwmDelta = endPWM - startPWM;
		byte parabolaPhase = constrain(map(current, start, end, 0, 180), 0, 180);
		return startPWM + (pwmDelta * sin(radians(parabolaPhase)));
	}
}

byte MoonPhase()
{
	int m,d,y;
	int yy,mm;
	long K1,K2,K3,J,V;
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
	return (byte)(2*abs(50-V));
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

#ifdef MOONPHASELABEL
char* MoonPhaseLabel()
{
  int m,d,y;
  int yy,mm;
  long K1,K2,K3,J;
  double V;
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
  V = (J-2451550.1)/29.530588853;
  V -= floor(V);
  if (V<0) V++;
  if (V<0.0625) return "New Moon";
  else if (V<0.1875) return "Waxing Crescent";
  else if (V<0.3125) return "First Quarter";
  else if (V<0.4375) return "Waxing Gibbous";
  else if (V<0.5625) return "Full Moon";
  else if (V<0.6875) return "Waning Gibbous";
  else if (V<0.8125) return "Last Quarter";
  else if (V<0.9375) return "Waning Crescent";
  else return "New Moon";
}
#endif // MOONPHASELABEL

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

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
/*********************************************/
// These read data from the SD card file and convert them to big endian 
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f) 
{
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f) 
{
  uint32_t d;
  uint16_t b;

  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}
#endif //  REEFTOUCH

byte ShortPulseMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	byte tspeed=0;
	LightsOverride=false;
	PulseMinSpeed=constrain(PulseMinSpeed,30,100);
	PulseMaxSpeed=constrain(PulseMaxSpeed,30,100);
	tspeed=(millis()%(PulseDuration*2)<PulseDuration?PulseMinSpeed:PulseMaxSpeed);
	if (PulseSync)
		return tspeed;
	else
		return (tspeed==PulseMinSpeed)?PulseMaxSpeed:PulseMinSpeed;
}

byte LongPulseMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	byte tspeed=0;
	LightsOverride=false;
	PulseMinSpeed=constrain(PulseMinSpeed,30,100);
	PulseMaxSpeed=constrain(PulseMaxSpeed,30,100);
	tspeed=(now()%(PulseDuration*2)<PulseDuration?PulseMinSpeed:PulseMaxSpeed);
	if (PulseSync)
		return tspeed;
	else
		return (tspeed==PulseMinSpeed)?PulseMaxSpeed:PulseMinSpeed;
}

byte SineMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	// Contribution of Discocarp
	// http://forum.reefangel.com/viewtopic.php?f=2&t=2386&p=18240
	double x,y;

	LightsOverride=false;
	x=double(now()%(PulseDuration));
	x/=PulseDuration;
	x*=2.0*PI;
	if (!PulseSync) x+=PI; // shift the sine wave for the right pump 

	y=sin(x);// y is now between -1 and 1
	y+=1.0; // y is now between 0 and 2
	y/=2.0; // y is now between 0 and 1  

	// now compute the tunze speed
	y*=double(PulseMaxSpeed-PulseMinSpeed);
	y+=double(PulseMinSpeed); 

	y+=0.5; // for proper rounding

	// y is now between PulseMinSpeed and PulseMaxSpeed, constrain for safety  
	return constrain(byte(y),30,100); 
}

byte ReefCrestMode(byte WaveSpeed, byte WaveOffset, boolean PulseSync)
{
	static unsigned long lastwavemillis=millis();
	static int newspeed=WaveSpeed;
	LightsOverride=false;
	if ((millis()-lastwavemillis) > 5000)
	{
		if (random(100)<50) newspeed--; else newspeed++;
		newspeed=constrain(newspeed,WaveSpeed-WaveOffset,WaveSpeed+WaveOffset);
		newspeed=constrain(newspeed,0,100);
		lastwavemillis=millis();
	}  
	if (PulseSync)
		return newspeed;
	else
		return WaveSpeed-(newspeed-WaveSpeed);
}

byte NutrientTransportMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	static unsigned long lastwavemillis=millis();
	static byte WavePhase=0;
	static time_t WaveStart=0;
	static byte speed=PulseMinSpeed;
	static byte anti_speed=PulseMinSpeed;

	LightsOverride=false;
	if (WavePhase==0)
	{
		WavePhase++;
		WaveStart=now();
	}
	else if (WavePhase==1)
	{
		if (now()-WaveStart>2700)
		{
			WavePhase++;
		}
		if ((millis()-lastwavemillis) > PulseDuration)
		{
			if (speed==PulseMinSpeed)
			{  
				speed=PulseMaxSpeed;
				anti_speed=PulseMinSpeed;
			}
			else
			{
				speed=PulseMinSpeed;
				anti_speed=PulseMaxSpeed;
			}
			lastwavemillis=millis();
		}
	}
	else if (WavePhase==2)
	{
		if (now()-WaveStart>4500) WavePhase++;
		if (now()-WaveStart<=2760)
			speed=PulseMinSpeed; 
		else
			speed=PulseMaxSpeed;
		if (now()-WaveStart<=3300)
			anti_speed=PulseMinSpeed;
		else
			anti_speed=PulseMaxSpeed*sin(radians(map(now()-WaveStart,3300,4500,0,180)));
	}
	else if (WavePhase==3)
	{
		if (now()-WaveStart>7200) WavePhase++;
		if ((millis()-lastwavemillis) > PulseDuration)
		{
			if (speed==PulseMinSpeed)
			{  
				speed=PulseMaxSpeed;
				anti_speed=PulseMinSpeed;
			}
			else
			{
				speed=PulseMinSpeed;
				anti_speed=PulseMaxSpeed;
			}
			lastwavemillis=millis();
		}
	}
	else if (WavePhase==4)
	{
		if (now()-WaveStart>9000) WavePhase=0;
		if (now()-WaveStart<=7260) 
			speed=PulseMinSpeed; 
		else
			speed=PulseMaxSpeed;
		if (now()-WaveStart<=8400)
			anti_speed=PulseMaxSpeed*sin(radians(map(now()-WaveStart,7200,8400,0,180)));
		else
			anti_speed=0;
	}
	if (PulseSync)
		return speed;
	else
		return anti_speed;
}

byte TidalSwellMode(byte WaveMaxSpeed, boolean PulseSync)
{
	static unsigned long lastwavemillis=millis();
	static byte WavePhase=0;
	static time_t WaveStart=0;
	static byte speed=0;
	static byte anti_speed=0;

	LightsOverride=false;
	if (WavePhase==0)
	{
		WavePhase++;
		WaveStart=now();
	}
	else if (WavePhase==1)
	{
		if (now()-WaveStart>900) WavePhase++;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,0,900,0,90))))/10;
		speed+=WaveMaxSpeed/2;

		anti_speed=(WaveMaxSpeed*2*sin(radians(map(now()-WaveStart,0,900,0,90))))/5;
		anti_speed+=WaveMaxSpeed/2;
	}
	else if (WavePhase==2)
	{
		if (now()-WaveStart>1800) WavePhase++;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,900,1800,90,180))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=WaveMaxSpeed/20;

		anti_speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,900,1800,90,180))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=WaveMaxSpeed/4;

	}
	else if (WavePhase==3)
	{
		if (now()-WaveStart>2700) WavePhase++;
		speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,1800,2700,0,90))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=WaveMaxSpeed/20;

		anti_speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,1800,2700,0,90))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=WaveMaxSpeed/4;
	}
	else if (WavePhase==4)
	{
		if (now()-WaveStart>3600) WavePhase++;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,2700,3600,90,180))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*3)/20;

		anti_speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,2700,3600,90,180))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=(WaveMaxSpeed*3)/20;
	}
	else if (WavePhase==5)
	{
		if (now()-WaveStart>4500) WavePhase++;
		speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,3600,4500,0,90))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*3)/20;

		anti_speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,3600,4500,0,90))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=(WaveMaxSpeed*3)/20;
	}
	else if (WavePhase==6)
	{
		if (now()-WaveStart>5400) WavePhase++;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,4500,5400,90,180))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*5)/20;

		anti_speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,4500,5400,90,180))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=WaveMaxSpeed/20;
	}
	else if (WavePhase==7)
	{
		if (now()-WaveStart>6300) WavePhase++;
		speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,5400,6300,0,90))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*5)/20;

		anti_speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,5400,6300,0,90))))/20;
		anti_speed+=WaveMaxSpeed/2;
		anti_speed+=WaveMaxSpeed/20;
	}
	else if (WavePhase==8)
	{
		if (now()-WaveStart>7200) WavePhase++;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,6300,7200,90,180))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*7)/20;

		anti_speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,6300,7200,90,180))))/10;
		anti_speed+=WaveMaxSpeed/2;
	}
	else if (WavePhase==9)
	{
		if (now()-WaveStart>8100) WavePhase++;
		speed=(WaveMaxSpeed*3*sin(radians(map(now()-WaveStart,7200,8100,0,90))))/20;
		speed+=WaveMaxSpeed/2;
		speed+=(WaveMaxSpeed*7)/20;

		anti_speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,7200,8100,0,90))))/2;
		anti_speed+=WaveMaxSpeed/2;
	}
	else if (WavePhase==10)
	{
		if (now()-WaveStart>9000) WavePhase=0;
		speed=(WaveMaxSpeed*sin(radians(map(now()-WaveStart,8100,9000,90,180))))/2;
		speed+=WaveMaxSpeed/2;

		anti_speed=speed;
	}

	if (PulseSync)
		return speed;
	else
		return anti_speed;
}

byte TideMode(byte WaveSpeed, byte minOffset, byte maxOffset)
{
	// Contribution of lnevo
	// http://forum.reefangel.com/viewtopic.php?f=11&t=2708
	double moonOffset; // gap between high and low
	double amplitude;  // tide curve
	double wavelength=12*SECS_PER_HOUR;

	LightsOverride=false;
	// Calculate the gap between high and low tide based on MoonPhase()
	moonOffset=cos(((2*PI)/100)*MoonPhase());
	moonOffset=((moonOffset+1)/2)*100; // Convert to percentage

	// Find out the current tidal height
	amplitude=sin(((2*PI)/wavelength)*now()); 

	moonOffset=map(moonOffset,0,100,minOffset,maxOffset);
	amplitude=amplitude*moonOffset; 

	// Adjust the calculate speed to be in our adjusted range
	return constrain(WaveSpeed+amplitude,0,100);
}

// for pure virtual functions
void __cxa_pure_virtual(void){};
// other fixes
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
