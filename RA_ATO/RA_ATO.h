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
#ifdef REEFTOUCHDISPLAY
    bool activestatus;
    void inline SetActive(bool status) { activestatus=status; }
#endif // REEFTOUCHDISPLAY

private:
    bool topping;
};

class RA_ATOHighClass : public RA_ATOClass
{
public:
#ifdef ATO_HIGH_REVERSE
#ifdef REEFTOUCHDISPLAY
    inline bool IsActive() { return !activestatus; }
#else // REEFTOUCHDISPLAY
    inline bool IsActive() { return digitalRead(highATOPin); }
#endif // REEFTOUCHDISPLAY
#else // ATO_HIGH_REVERSE
#ifdef REEFTOUCHDISPLAY
    inline bool IsActive() { return activestatus; }
#else // REEFTOUCHDISPLAY
    inline bool IsActive() { return !digitalRead(highATOPin); }
#endif // REEFTOUCHDISPLAY
#endif // ATO_HIGH_REVERSE
};

class RA_ATOLowClass : public RA_ATOClass
{
public:
#ifdef ATO_LOW_REVERSE
#ifdef REEFTOUCHDISPLAY
    inline bool IsActive() { return !activestatus; }
#else // REEFTOUCHDISPLAY
    inline bool IsActive() { return digitalRead(lowATOPin); }
#endif // REEFTOUCHDISPLAY
#else // ATO_LOW_REVERSE
#ifdef REEFTOUCHDISPLAY
    inline bool IsActive() { return activestatus; }
#else // REEFTOUCHDISPLAY
    inline bool IsActive() { return !digitalRead(lowATOPin); }
#endif // REEFTOUCHDISPLAY
#endif // ATO_LOW_REVERSE
};

#if defined WATERLEVELEXPANSION
class RA_ATOWLClass : public RA_ATOClass
{
public:
	inline bool IsActive() { return false; }
};
#endif // WATERLEVELEXPANSION

#endif  // __RA_ATO_H__

