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

#ifndef __RA_ATO_H__
#define __RA_ATO_H__

#include <Globals.h>

/*
    Auto Top Off Class
*/

class RA_ATOClass
{
public:
	RA_ATOClass();
  virtual bool IsActive() = 0;
	inline bool IsTopping() { return topping; }
#ifdef ENABLE_ATO_LOGGING
	virtual void StartTopping(bool fHighAto = false);
	virtual void StopTopping(bool fHighAto = false);
#else
	inline void StartTopping() { topping = true; }
	inline void StopTopping() { topping = false; }
#endif  // ENABLE_ATO_LOGGING
	unsigned long Timer;
#ifdef RA_TOUCHDISPLAY
    bool activestatus;
    void inline SetActive(bool status) { activestatus=status; }
#endif // RA_TOUCHDISPLAY

private:
    bool topping;
};

class RA_ATOHighClass : public RA_ATOClass
{
public:
    byte Status;
#ifdef ATO_HIGH_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return !activestatus; }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=digitalRead(highATOPin); return digitalRead(highATOPin); }
#endif // RA_TOUCHDISPLAY
#else // ATO_HIGH_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return activestatus; }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=!digitalRead(highATOPin); return !digitalRead(highATOPin); }
#endif // RA_TOUCHDISPLAY
#endif // ATO_HIGH_REVERSE
};

class RA_ATOLowClass : public RA_ATOClass
{
public:
    byte Status;
#ifdef ATO_LOW_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return !activestatus; }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=digitalRead(lowATOPin); return digitalRead(lowATOPin); }
#endif // RA_TOUCHDISPLAY
#else // ATO_LOW_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return activestatus; }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=!digitalRead(lowATOPin); return !digitalRead(lowATOPin); }
#endif // RA_TOUCHDISPLAY
#endif // ATO_LOW_REVERSE
};

#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
class RA_ATOWLClass : public RA_ATOClass
{
public:
	unsigned long Timer;
	inline bool IsActive() { return false; }
};
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION

#if defined RA_STAR || defined RA_TOUCHDISPLAY || defined(__SAM3X8E__)
class RA_ATOALARMClass : public RA_ATOClass
{
public:
    byte Status;
#ifdef ATO_LOW_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return !activestatus; }
#elif defined(__SAM3X8E__)
    inline bool IsActive() { return digitalRead(AlarmPin); }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=(PINJ & (1<<PJ4)); return (PINJ & (1<<PJ4)); }
#endif // RA_TOUCHDISPLAY
#else // ATO_LOW_REVERSE
#ifdef RA_TOUCHDISPLAY
    inline bool IsActive() { return activestatus; }
#elif defined(__SAM3X8E__)
    inline bool IsActive() { return !digitalRead(AlarmPin); }
#else // RA_TOUCHDISPLAY
    inline bool IsActive() { Status=!(PINJ & (1<<PJ4)); return !(PINJ & (1<<PJ4)); }
#endif // RA_TOUCHDISPLAY
#endif // ATO_LOW_REVERSE
};
#endif // RA_STAR

#ifdef KALKDOSER
class RA_KalkDoserClass : public RA_ATOClass
{
public:
    inline bool IsActive() { return true; }
};
#endif //  KALKDOSER

#endif  // __RA_ATO_H__

