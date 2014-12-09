
// functions adapted and incorporated into a Class by Curt Binder for Reef Angel

//By Matthew Hockin 2012.
//SWFLTEK library functions, #includes, and #define were copied directly from the Epherma library
//Epherma (SWFLTEK.com) was written by Michael Rice- thanks Michael it works great.
//If you copy from this (its all open source) please
//acknowledge Michael for SWFLTEK library code (obviously labeled) or Matthew Hockin (for the rest).

#include <stdlib.h>
#include <math.h>
#include <Time.h>
#include "SunLocation.h"
#include "Globals.h"
#include "InternalEEPROM.h"

SunLocation::SunLocation()
{
	m_Latitude = ddToSeconds(-18.285833); // Great Barrier Reef, Australia
	m_Longitude = ddToSeconds(147.699722); // Great Barrier Reef, Australia

	m_UserRiseOffset = 0;
	m_UserSetOffset = 0;

	m_DayOfMonth = 0;

	UseMemory=true;
}

void SunLocation::Init(long latitude, long longitude)
{
	m_Latitude = ddToSeconds(latitude);
	m_Longitude = ddToSeconds(longitude);
}


void SunLocation::SetOffset(long risehour, long risesec, long sethour, long setsec)
{
	m_UserRiseOffset = (risehour*3600)+risesec;
	m_UserSetOffset = (sethour*3600)+setsec;
}

void SunLocation::CheckAndUpdate()
{
	static byte MinuteOffset=InternalMemory.ActinicOffset_read();
	
	// check if we are in a new day and recalculate sunrise and sunset as necessary
	if ((m_DayOfMonth != day()) || (MinuteOffset != InternalMemory.ActinicOffset_read() ))
	{
		CalSun();
		m_DayOfMonth = day();
		MinuteOffset = InternalMemory.ActinicOffset_read();

		if (UseMemory) {
		  // Write sunrise/sunset to memory for Standard and Actinic Lights to work
	  	  // Actinic will come on at Sunrise and turn off at Sunset
		  // Standards will come on based on the ActinicOffset memory location
		  StdLightsOnOff_write(); 
		}
	}
}

void SunLocation::StdLightsOnOff_write()
{
  byte MinuteOffset=InternalMemory.ActinicOffset_read();

  int onTime=NumMins(InternalMemory.StdLightsOnHour_read(),InternalMemory.StdLightsOnMinute_read());
  int offTime=NumMins(InternalMemory.StdLightsOffHour_read(),InternalMemory.StdLightsOffMinute_read());

  int riseTime=NumMins(m_SunriseHour,m_SunriseMin)+MinuteOffset;
  int setTime=NumMins(m_SunsetHour,m_SunsetMin)-MinuteOffset;

  if (riseTime/60!=onTime/60) InternalMemory.StdLightsOnHour_write(riseTime/60);
  if (riseTime%60!=onTime%60) InternalMemory.StdLightsOnMinute_write(riseTime%60);
  if (setTime/60!=onTime/60) InternalMemory.StdLightsOffHour_write(setTime/60);
  if (setTime%60!=onTime%60) InternalMemory.StdLightsOffMinute_write(setTime%60);
}

boolean SunLocation::IsDaytime(){

  if (now()>=m_rise && now()<=m_set) {
    return true;
  } else {
    return false;
  }
}

// CalcSun function created by Matthew Hockin
void SunLocation::CalSun(){
	//Start of sunrise, sunset and cloud calculations- runs on reset and once a day thereafter.
	long newDay=now();;

	newDay=newDay-(newDay%SECS_PER_DAY); // Go to midnight of today
	newDay-=SECS_YR_2000; //convert GMT unix Epoch to seconds elasped since 2000

	m_rise=newDay;
	m_set=newDay;

	//Calculate rise time and set time using Epherma Library functions (see end of code)
	SunRise(&m_rise);//call to Epherma function
	SunSet(&m_set);//Call to Epherma functionunsigned long newDay;

	m_rise+=SECS_YR_2000+m_UserRiseOffset;
	m_set+=SECS_YR_2000+m_UserSetOffset;

	// Convert rise/set to normal hours/minutes 
	m_SunriseHour=hour(m_rise);
	m_SunriseMin=minute(m_rise);
	m_SunsetHour=hour(m_set);
	m_SunsetMin=minute(m_set);
}

char SunLocation::CalcSunRise(unsigned long* when)
{
	*when-=SECS_YR_2000;
	SunRise(when);
	*when+=SECS_YR_2000;
	
	return 0;
}
char SunLocation::CalcSunSet(unsigned long* when)
{
	*when-=SECS_YR_2000;
	SunSet(when);
	*when+=SECS_YR_2000;
	
	return 0;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
long SunLocation::ddToSeconds(long dd)
{
	// decimal degress
	return dd * 3600.0;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
/*
long SunLocation::dmsToSeconds(int d, unsigned char m, unsigned char s)
{
	// degrees, minutes, seconds
	long ret;
	ret = labs((long)d);
	ret = ret * 3600L + 60L * m + s;
	ret = (d<0L) ? -ret : ret;
	return ret;
}
*/

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
int SunLocation::equation_of_time(unsigned long dt)
{
/* ------------------------------------------------------------------------------------------------
   'Equation of Time'
   We use the 'short form equation, which has a theoretical accuracy of about 40 seconds.
   The returned value is in seconds.
*/
	double t;

	dt -= 192540UL; // refer to Jan 3 2000 05:29 (first periapsis)
	dt %= _tropical_year;
	t = dt;
	t /= _tropical_year;
	t *= 6.283185307179586;
	t = -459.27672 * sin(t) + 575.333472 * sin(2.0 * t + 3.588414);
	return t;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
double SunLocation::SolarDeclination(unsigned long dt)
{
/* -----------------------------------------------------------------------------------------------
   'Solar Declination'
   Returns declination in radians
   Accurate to within 50 arc-seconds
*/
	double y;

	dt %= _tropical_year;
	y = dt;
	y /= _tropical_year; // fractional year
	y *= 6.283185307179586;
	y=0.006918-0.399912*cos(y)+0.070257*sin(y)-0.006758*cos(y*2)+0.000907*sin(y*2)-0.002697*cos(y*3)+0.00148*sin(y*3);
	return y;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
unsigned long SunLocation::daylightseconds(unsigned long dt){
/* ------------------------------------------------------------------------------------------------
   Return the period between sunrise and sunset, in seconds.
   At high latitudes around the time of the solstices, this could be zero, or all day.
*/
	float l, d, e;
	long n;

	d = -SolarDeclination(dt); // will be positive in Northern winter
	l = m_Latitude / _sec_rad; // latitude in radians

	e += 60.0 * l * tan(l + d); // latitudinal error
	d = tan(l) * tan(d); //

	if(d>1.0) return 86400UL;
	if(d < -1.0) return 0UL;

	d = acos(d);
	d /= _zenith;

	n = 86400UL * d;
	n += e;
	return n;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
char SunLocation::SunRiseSet(unsigned long * dt, char set){
/* ------------------------------------------------------------------------------------------------
   Modify the passed time stamp to the time of sunrise (or sunset if 'set' is non-zero).
   Returns 0 to signal 'normal' completion. If the position is in a polar circle, 1 will be
   returned if the sun is above the horizon all day, and -1 if the sun is below the horizon
   all day.

*/
	unsigned long daylen;

	daylen = daylightseconds(*dt);
	if(daylen == 86400UL) return 1;   // there is no 'night' today (midnight sun)
	if(daylen == 0UL) return -1; // there is no 'day' today

	*dt /= 86400UL;
	*dt *= 86400UL;
	*dt += 43200UL; // set the time stamp to 12:00:00 GMT

	*dt -= daylen / 2; //        sunrise at the prime meridian
	if(set) *dt += daylen; //     sunset at the prime meridian

	*dt -= equation_of_time(*dt);

	*dt -= m_Longitude / 15.0; // rotate to our own meridian

	return 0;
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
char SunLocation::SunRise(unsigned long* when)
{
	return SunRiseSet(when, 0);
}

//THE CODE BELOW THIS copied directly from the SWFLTEK Epherma library constructed by Michael Rice.
//this code is being used freely with attribution to Micahel Rice in accord with his request
//  A big thank you for these library functions.  Its great!
char SunLocation::SunSet(unsigned long* when)
{
	return SunRiseSet(when, 1);
}
