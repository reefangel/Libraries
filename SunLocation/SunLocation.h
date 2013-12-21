
// functions adapted and incorporated into a Class by Curt Binder for Reef Angel

//By Matthew Hockin 2012.
//SWFLTEK library functions, #includes, and #define were copied directly from the Epherma library
//Epherma (SWFLTEK.com) was written by Michael Rice- thanks Michael it works great.
//If you copy from this (its all open source) please
//acknowledge Michael for SWFLTEK library code (obviously labeled) or Matthew Hockin (for the rest).

#ifndef __SUNLOCATION_H__
#define __SUNLOCATION_H__

#include <Globals.h>

class SunLocation
{
public:
	SunLocation();
	void Init(long latitude, long longitude);
	void SetOffset(long risehour, long risesec, long sethour, long setsec);
	void CheckAndUpdate();

	boolean UseMemory;

	inline int GetRiseHour() { return m_SunriseHour; }
	inline int GetRiseMinute() { return m_SunriseMin; }
	inline int GetSetHour() { return m_SunsetHour; }
	inline int GetSetMinute() { return m_SunsetMin; }

	inline unsigned long GetSunRise() { return m_rise; }
	inline unsigned long GetSunSet() { return m_set; }

	char CalcSunRise(unsigned long *when);
	char CalcSunSet(unsigned long *when);

  	boolean IsDaytime();
  
private:
	// private functions for the class
	void CalSun();
	void StdLightsOnOff_write();
	long ddToSeconds(long dd);
	//long dmsToSeconds(int d, unsigned char m, unsigned char s);
	int equation_of_time(unsigned long dt);
	double SolarDeclination(unsigned long dt);
	unsigned long daylightseconds(unsigned long dt);
	char SunRiseSet(unsigned long *dt, char set);
	char SunRise(unsigned long *when);
	char SunSet(unsigned long *when);

private:
	// sunrise and set calculations
	int m_SunriseHour;
	int m_SunriseMin;
	int m_SunsetHour;
	int m_SunsetMin;

	byte m_DayOfMonth;
	
	// actual rise / set variables
	unsigned long m_rise; //time in seconds from the year 2000 (GMT) for sunrise
	unsigned long m_set; //time in seconds from the year 2000 (GMT) for sunset

	// lat & long for the location
	long m_Latitude;
	long m_Longitude;

	// improved accuracy for user sunrise / sunset
	long m_UserRiseOffset;
	long m_UserSetOffset;
};


#define _sec_rad 206264.806247096370813 // arc-seconds per radian
#define _tilt 0.409092804222329 // axial tilt of earth at epoch, in radians
#define _tropical_year 31556925 // tropical year in seconds... rounding error accumulates to 26 seconds by the year 2136
#define _zenith 3.11250383272322 // 'zenith' of rising (setting) sun in radians (360 - 2 * 90.833 degrees)

#endif  // __SUNLOCATION_H__
