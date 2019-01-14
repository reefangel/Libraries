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
#include "math.h"
#if defined(__AVR_ATmega2560__)  
#include <avr/wdt.h>
#endif

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

int PWMSlopeHighRes(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte Duration, int oldValue)
{
	// Contribution of thekameleon
	// http://forum.reefangel.com/viewtopic.php?p=23893#p23893
	LightsOverride=true;
	int current_hour = hour();
	long start = NumMins(startHour, startMinute)*60L;
	long end = NumMins(endHour, endMinute)*60L;

        // convert PWM inputs from percents to 12-bit int values.
        if (startPWM > 100)
           startPWM = 100;
        if (endPWM > 100)
           endPWM = 100;
        int startPWMint = map(startPWM, 0, 100, 0, 4095);
        int endPWMint = map(endPWM, 0, 100, 0, 4095);

	if (start > end) //Start is greater than End so its over midnight
	{
		//Example: 2300hrs to 0200hrs
		if (current_hour < endHour) start -= 1440L*60L; //past midnight
		if (current_hour >= startHour) end += 1440L*60L; //before midnight
	}
	long current = NumMins(current_hour, minute())*60L + second();
	long startD = start + Duration*60L;
	long stopD = end - Duration*60L;

	if ( current >= start && current <= startD )
		return constrain(map(current, start, startD, startPWMint, endPWMint),startPWMint, endPWMint);
	else if ( current >= stopD && current <= end )
	{
		int v = constrain(map(current, stopD, end, startPWMint, endPWMint),startPWMint, endPWMint);
		return endPWMint - v + startPWMint;
	}
	else if ( current > startD && current < stopD )
		return endPWMint;

	// lastly return the existing value
	return oldValue;
}

int PWMParabolaHighRes(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, int oldValue)
{
	// Contribution of thekameleon
	// http://forum.reefangel.com/viewtopic.php?p=23813#p23813
	LightsOverride=true;
	int current_hour = hour();
	long start = NumMins(startHour, startMinute)*60L;
	long end = NumMins(endHour, endMinute)*60L;

        // convert PWM inputs from percents to 12-bit int values.
        if (startPWM > 100)
           startPWM = 100;
        if (endPWM > 100)
           endPWM = 100;
        int startPWMint = map(startPWM, 0, 100, 0, 4095);
        int endPWMint = map(endPWM, 0, 100, 0, 4095);

	if (start > end) //Start is greater than End so its over midnight
	{
		//Example: 2300hrs to 0200hrs
		if (current_hour < endHour) start -= 1440L*60L; //past midnight
		if (current_hour >= startHour) end += 1440L*60L; //before midnight
	}

	long current = NumMins(current_hour, minute())*60L + second();

	if ( current <= start || current >= end)
		return oldValue;
	else
	{
		int pwmDelta = endPWMint - startPWMint;
		float parabolaPhase = ((float)(current-start)/(float)(end-start))*180.0;
		//byte parabolaPhase = constrain(map(current, start, end, 0, 180), 0, 180);
		return startPWMint + (int)(pwmDelta * sin(radians(parabolaPhase)));
	}
}

int PWMSmoothRampHighRes(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte slopeLength, int oldValue)
{
  LightsOverride=true;
  int current_hour = hour();
  long start = NumMins(startHour, startMinute)*60L;
  long end = NumMins(endHour, endMinute)*60L;
  long slopeLengthSecs = slopeLength*60L;

  // convert PWM inputs from percents to 12-bit int values.
  if (startPWM > 100)
     startPWM = 100;
  if (endPWM > 100)
     endPWM = 100;
  int startPWMint = map(startPWM, 0, 100, 0, 4095);
  int endPWMint = map(endPWM, 0, 100, 0, 4095);

  if (start > end) // Start is greater than End so it is over midnight
  {
    if (current_hour < endHour) start -= 1440L*60L; // past midnight
    if (current_hour >= startHour) end += 1440L*60L; // before midnight
  }
  long current = NumMins(current_hour, minute())*60L + second();
  if (slopeLengthSecs > ((end-start)/2) ) slopeLengthSecs = (end-start)/2; // don't allow a slope length greater than half the total period
  if (current <= start || current >= end) 
    return oldValue; // it's before the start or after the end, return the default
  else
  { // do the slope calculation
    int pwmDelta = endPWMint - startPWMint;
    float smoothPhase;
    if ((current > (start + slopeLengthSecs)) && (current < (end - slopeLengthSecs))) 
      return endPWMint; // if it's in the middle of the slope, return the high level
    else if ((current - start) < slopeLengthSecs) 
    {  // it's in the beginning slope up
      smoothPhase = (((float)(current-start)/(float)slopeLengthSecs)*180.0) + 180.0;
      //smoothPhase = constrain(map(current, start, start+slopeLengthSecs, 180, 360), 180, 360);
    }
    else if ((end - current) < slopeLengthSecs)
    { // it's in the end slope down
      smoothPhase = (((float)(current-start)/(float)slopeLengthSecs)*180.0);
      //smoothPhase = constrain(map(current, end-slopeLengthSecs, end, 0, 180), 0, 180);
    }
    return startPWMint + (int)(pwmDelta*((1.0+(cos(radians(smoothPhase))))/2.0));

  }
}

int PWMSmoothRampHighestRes(byte startHour, byte startMinute, byte endHour, byte endMinute, int startPWMint, int endPWMint, byte slopeLength, int oldValue)
{
  LightsOverride=true;
  int current_hour = hour();
  long current_millis = millis();
  long start = NumMins(startHour, startMinute)*600L;
  long end = NumMins(endHour, endMinute)*600L;
  long slopeLengthTenthSecs = slopeLength*600L;

  if (start > end) // Start is greater than End so it is over midnight
  {
    if (current_hour < endHour) start -= 1440L*600L; // past midnight
    if (current_hour >= startHour) end += 1440L*600L; // before midnight
  }
  long current = NumMins(current_hour, minute())*600L + second()*10L + (current_millis%1000 - current_millis%100)/100L;
  if (slopeLengthTenthSecs > ((end-start)/2) ) slopeLengthTenthSecs = (end-start)/2; // don't allow a slope length greater than half the total period
  if (current <= start || current >= end) 
    return oldValue; // it's before the start or after the end, return the default
  else
  { // do the slope calculation
    int pwmDelta = endPWMint - startPWMint;
    float smoothPhase;
    if ((current > (start + slopeLengthTenthSecs)) && (current < (end - slopeLengthTenthSecs))) 
      return endPWMint; // if it's in the middle of the slope, return the high level
    else if ((current - start) < slopeLengthTenthSecs) 
    {  // it's in the beginning slope up
      smoothPhase = (((float)(current-start)/(float)slopeLengthTenthSecs)*180.0) + 180.0;
    }
    else if ((end - current) < slopeLengthTenthSecs)
    { // it's in the end slope down
      smoothPhase = (((float)(end-current)/(float)slopeLengthTenthSecs)*180.0) + 180.0;
    }
    return startPWMint + (int)(pwmDelta*((1.0+(cos(radians(smoothPhase))))/2.0));
  }
}

int PWMSigmoidHighRes(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, int oldValue)
{
  LightsOverride=true;
  int current_hour = hour();
  long start = NumMins(startHour, startMinute)*60L;
  long end = NumMins(endHour, endMinute)*60L;

  if (startPWM > 100)
     startPWM = 100;
  if (endPWM > 100)
     endPWM = 100;
  int startPWMint = map(startPWM, 0, 100, 0, 4095);
  int endPWMint = map(endPWM, 0, 100, 0, 4095);

  if (start > end) // Start is greater than End so it is over midnight
  {
    if (current_hour < endHour) start -= 1440*60L; // past midnight
    if (current_hour >= startHour) end += 1440*60L; // before midnight
  }
  long FWHMSecs = (end-start)*60L/2L;  // fwhm is half of full duration
  long current = (NumMins(current_hour, minute())*60L) + second();
  if (FWHMSecs > ((end-start)/2) ) FWHMSecs = (end-start)/2; // don't allow a slope length greater than half the total period
  if (current <= start || current >= end) 
    return oldValue; // it's before the start or after the end, return the default
  else
  { // do the slope calculation
    int pwmDelta = endPWMint - startPWMint;
    float smoothPhase; // X axis, needs to go from -5.0 to 5.0 over FWHM
    if ((current > (start + FWHMSecs)) && (current < (end - FWHMSecs))) 
      return endPWMint; // if it's in the middle of the slope, return the high level
    else if ((current - start) < FWHMSecs) 
    {  // it's in the beginning slope up go from -5 to 5
      smoothPhase = (10.0*((float)current-(float)start)/(float)FWHMSecs) - 5.0;
    }
    else if ((end - current) < FWHMSecs)
    { // it's in the end slope down, go from 5 to -5
      smoothPhase = (10.0*((float)end-(float)current)/(float)FWHMSecs) - 5.0;
    }
    smoothPhase = -1.0*smoothPhase;
    return startPWMint + (int)((1.0/(1.0+exp(smoothPhase)))*pwmDelta);
  }
}

byte PWMSlope(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte Duration, byte oldValue)
{
	// Contribution of thekameleon
	// http://forum.reefangel.com/viewtopic.php?p=23893#p23893
	LightsOverride=true;
	int current_hour = hour();
	int start = NumMins(startHour, startMinute);
	int end = NumMins(endHour, endMinute);

        if (startPWM > 100)
           startPWM = 100;
        if (endPWM > 100)
           endPWM = 100;

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

        if (startPWM > 100)
           startPWM = 100;
        if (endPWM > 100)
           endPWM = 100;

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

byte PWMSmoothRamp(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte slopeLength, byte oldValue)
{
  LightsOverride=true;
  int current_hour = hour();
  int start = NumMins(startHour, startMinute);
  int end = NumMins(endHour, endMinute);

  if (startPWM > 100)
     startPWM = 100;
  if (endPWM > 100)
     endPWM = 100;

  if (start > end) // Start is greater than End so it is over midnight
  {
    if (current_hour < endHour) start -= 1440; // past midnight
    if (current_hour >= startHour) end += 1440; // before midnight
  }
  int current = NumMins(current_hour, minute());
  if (slopeLength > ((end-start)/2) ) slopeLength = (end-start)/2; // don't allow a slope length greater than half the total period
  if (current <= start || current >= end) 
    return oldValue; // it's before the start or after the end, return the default
  else
  { // do the slope calculation
    byte pwmDelta = endPWM - startPWM;
    int smoothPhase;
    if ((current > (start + slopeLength)) && (current < (end - slopeLength))) 
      return endPWM; // if it's in the middle of the slope, return the high level
    else if ((current - start) < slopeLength) 
    {  // it's in the beginning slope up
      smoothPhase = constrain(map(current, start, start+slopeLength, 180, 360), 180, 360);
    }
    else if ((end - current) < slopeLength)
    { // it's in the end slope down
      smoothPhase = constrain(map(current, end-slopeLength, end, 0, 180), 0, 180);
    }
    return startPWM + (byte)(pwmDelta*((1.0+(cos(radians(smoothPhase))))/2.0));

  }
}

byte PWMSigmoid(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte oldValue)
{
  LightsOverride=true;
  int current_hour = hour();
  int start = NumMins(startHour, startMinute);
  int end = NumMins(endHour, endMinute);

  if (startPWM > 100)
     startPWM = 100;
  if (endPWM > 100)
     endPWM = 100;

  if (start > end) // Start is greater than End so it is over midnight
  {
    if (current_hour < endHour) start -= 1440; // past midnight
    if (current_hour >= startHour) end += 1440; // before midnight
  }
  int FWHM = (end-start)/2; // slopes up to peak and back down determined by the number of minutes in the full duration
  int current = NumMins(current_hour, minute());
  if (FWHM > ((end-start)/2) ) FWHM = (end-start)/2; // don't allow a slope length greater than half the total period
  if (current <= start || current >= end) 
    return oldValue; // it's before the start or after the end, return the default
  else
  { // do the slope calculation
    byte pwmDelta = endPWM - startPWM;
    float smoothPhase; // X axis, needs to go from -5.0 to 5.0 over FWHM
    if ((current > (start + FWHM)) && (current < (end - FWHM))) 
      return endPWM; // if it's in the middle of the slope, return the high level
    else if ((current - start) < FWHM) 
    {  // it's in the beginning slope up go from -5 to 5
      smoothPhase = (10.0*(float)(current-start)/(float)FWHM) - 5.0;
    }
    else if ((end - current) < FWHM)
    { // it's in the end slope down, go from 5 to -5
      smoothPhase = (10.0*(float)(end-current)/(float)FWHM) - 5.0;
    }
    smoothPhase = -1.0*smoothPhase;
    return startPWM + (byte)((1.0/(1.0+exp(smoothPhase)))*pwmDelta);
  }
}

byte PumpThreshold(byte value, byte threshold)
{
	if (value > 0) 
		return constrain(value,threshold,100);
	else 
		return 0;
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

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION || defined RA_STAR
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
#endif //  RA_TOUCH

unsigned int crc16(int *ptr, byte len)
{
  unsigned int crc=0xFFFF;
  byte i;
  byte temp=0;
  int test;
  while(len--)
  {
    crc^=*ptr++;
    for(i=0;i<8;i++)
    {
      if(crc & 0x01)
      {
        crc>>=1;
        crc^=0xA001;
      }
      else
      {
        crc>>=1;
      }
    }
  }
  return crc;
}

byte ShortPulseMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	byte tspeed=0;
	LightsOverride=false;
//	PulseMinSpeed=constrain(PulseMinSpeed,30,100);
//	PulseMaxSpeed=constrain(PulseMaxSpeed,30,100);
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
//	PulseMinSpeed=constrain(PulseMinSpeed,30,100);
//	PulseMaxSpeed=constrain(PulseMaxSpeed,30,100);
	tspeed=(now()%(PulseDuration*2)<PulseDuration?PulseMinSpeed:PulseMaxSpeed);
	if (PulseSync)
		return tspeed;
	else
		return (tspeed==PulseMinSpeed)?PulseMaxSpeed:PulseMinSpeed;
}

byte GyreMode(byte PulseMinSpeed, byte PulseMaxSpeed, int PulseDuration, boolean PulseSync)
{
	double x,y;
        PulseDuration = PulseDuration*60;// Pulse Duration is in minutes, not seconds here, so


	LightsOverride=false;
	x=double(now()%(PulseDuration));
	x/=PulseDuration;
	x*=2.0*PI;

	y=sin(x);// y is now between -1 and 1

	if (y > 0) { // call positive the sync channel
	  // now compute the tunze speed
	  y*=double(PulseMaxSpeed-PulseMinSpeed);
	  y+=double(PulseMinSpeed); 
	  y+=0.5; // for proper rounding
          if (PulseSync) return constrain(byte(y),0,100);
	  if (!PulseSync) return 0;
	} else { // call negative the antisync channel
	  y*=-1; // switch sign
	  // now compute the tunze speed
	  y*=double(PulseMaxSpeed-PulseMinSpeed);
	  y+=double(PulseMinSpeed); 
	  y+=0.5; // for proper rounding
          if (!PulseSync) return constrain(byte(y),0,100);
	  if (PulseSync) return 0;
	}
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

	// don't need to constrain to 30 at the bottom anymore because users have a PumpThreshold function for 
	// safety if they would like to use it.
	return constrain(byte(y),0,100); 
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
		return constrain(WaveSpeed-(newspeed-WaveSpeed),0,100);
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

byte ElseMode( byte midPoint, byte offset, boolean waveSync )
{
  // Contribution of cosmith71
  // http://forum.reefangel.com/viewtopic.php?f=3&t=3481
  // Static's only initialize the first time they are called
  static unsigned long lastChange=millis(); // Set the inital time that the last change occurred
  static int delay = random( 500, 3000); // Set the initial delay
  static int newSpeed = midPoint; // Set the initial speed
  static int antiSpeed = midPoint; // Set the initial anti sync speed
  if ((millis()-lastChange) > delay) // Check if the delay has elapsed
  {
    delay=random(500,5000); // If so, come up with a new delay
    int changeUp = random(offset); // Amount to go up or down
    if (random(100)<50) // 50/50 chance of speed going up or going down
    {
      newSpeed = midPoint - changeUp;
      antiSpeed = midPoint + changeUp;
    }
    else
    {
      newSpeed = midPoint + changeUp;
      antiSpeed = midPoint - changeUp;
    }
    lastChange=millis(); // Reset the time of the last change
  }
  if (waveSync)
  {
    return constrain(newSpeed,0,100);
  }
  else
  {
    return constrain(antiSpeed,0,100);
  }
}

byte StormMode(byte VSpeed, byte VTimer, boolean waveSync)
{
  static unsigned long lastmillis=millis();
  static int WavePhase;
  static byte sync_speed;
  static byte anti_speed;

  if ((millis()-lastmillis) > (VTimer*100))
	{
    WavePhase++;
    if (WavePhase>12)
    {
      WavePhase=0;
      sync_speed=0;
      anti_speed=0;
    }
    if (WavePhase<=7 && WavePhase>0)
    {
      if (sync_speed==0)
      {
        sync_speed=VSpeed; 
      }
      else sync_speed=0;
    }
    if (WavePhase==1) anti_speed=VSpeed;
    if (WavePhase>6) 
    {
      if (anti_speed==0) 
      {
        anti_speed=VSpeed; 
      }
      else anti_speed=0;
    }
    lastmillis=millis();
  }
    if (waveSync) return(constrain(sync_speed,0,100));
    else
      return(constrain(anti_speed,0,100));
  
}   
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}

int I2C_ClearBus() {
#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  for (int a=0;a<6;a++)
  {
	  delay(500);
	  wdt_reset();
  }
  // delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master. 
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
  // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok
}


// for pure virtual functions
//void __cxa_pure_virtual(void){};
// other fixes
//int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
//void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
//void __cxa_guard_abort (__guard *) {};
