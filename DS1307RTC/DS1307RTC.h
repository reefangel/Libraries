/*
 * DS1307RTC.h - library for DS1307 RTC
 * This library is intended to be uses with Arduino Time.h library functions
 */

#ifndef DS1307RTC_h
#define DS1307RTC_h

#include <Time.h>
#include <Wire.h>

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

// library interface description
class DS1307RTC
{
  // user-accessible "public" interface
  public:
    DS1307RTC();
    static time_t get();
	static void set(time_t t);
	static void read(tmElements_t &tm);
	static void write(tmElements_t &tm);

  private:
	static uint8_t dec2bcd(uint8_t num);
    static uint8_t bcd2dec(uint8_t num);
};

#if defined(__SAM3X8E__)
extern DS1307RTC I2CRTC;
#else // defined(__SAM3X8E__)
extern DS1307RTC RTC;
#endif // defined(__SAM3X8E__)

#endif
 
