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

#include "RA_Joystick.h"
#include <Globals.h>

RA_JoystickClass::RA_JoystickClass()
{
	pinMode(okPin, INPUT);
#ifdef DIGITAL_JOYSTICK
	digitalWrite(okPin, LOW); //pull down resistor on okPin
#else	
	digitalWrite(okPin, HIGH); //pull up resistor on okPin
#endif //DIGITAL_JOYSTICK
}

void RA_JoystickClass::Init()
{
	CalV = analogRead(VPin);
	CalH = analogRead(HPin);
	ButtonPress = 0;
	KeyTurbo = 1;
	KeyCount = 0;
#ifdef REEFANGEL_MINI	
	ButtonDebounce=millis();
#endif // REEFANGEL_MINI	
}

bool RA_JoystickClass::IsButtonPressed()
{
#ifdef REEFANGEL_MINI
	if (analogRead(VPin)>750 && analogRead(HPin)>750)
	{
		if (millis()-ButtonDebounce>600)
		{
			ButtonDebounce=millis();
			ButtonPress++;
		}
	}
#endif
#ifdef RANET
	if (!digitalRead(okPin))
	{
		if (millis()-ButtonDebounce>600)
		{
			ButtonDebounce=millis();
			ButtonPress++;
		}
	}
#endif
	if (ButtonPress > 0)
	{
		ButtonPress = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool RA_JoystickClass::IsUp()
{
	JoystickCenter();
#if defined DIGITAL_JOYSTICK
	if ( (analogRead(VPin)>750 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#elif defined REEFANGEL_MINI
	if ( (analogRead(VPin)>750 && analogRead(HPin)<250 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#else
	if ( (analogRead(VPin)>CalV+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#endif //defined DIGITAL_JOYSTICK || defined REEFANGEL_MINI
	{
		CheckTurbo();
		return true;
	}
	else
	{
		return false;
	}
}

bool RA_JoystickClass::IsDown()
{
	JoystickCenter();
#ifdef DIGITAL_JOYSTICK
	if ( (analogRead(HPin)<750 && analogRead(HPin)>250 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#elif defined REEFANGEL_MINI
	if ( (analogRead(HPin)>750 && analogRead(VPin)<250 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#else	
	if ( (analogRead(VPin)<CalV-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#endif //DIGITAL_JOYSTICK
	{
		CheckTurbo();
		return true;
	}
	else
	{
		return false;
	}
}

bool RA_JoystickClass::IsRight()
{
	JoystickCenter();
#ifdef DIGITAL_JOYSTICK
	if ( (analogRead(HPin)>750 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#elif defined REEFANGEL_MINI
	if (false)
#else	
	if ( (analogRead(HPin)<CalH-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#endif //DIGITAL_JOYSTICK
	{
		CheckTurbo();
		return true;
	}
	else
	{
		return false;
	}
}

bool RA_JoystickClass::IsLeft()
{
	JoystickCenter();
#ifdef DIGITAL_JOYSTICK
	if ( (analogRead(VPin)<750 && analogRead(VPin)>250 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#elif defined REEFANGEL_MINI
	if (false)		
#else	
	if ( (analogRead(HPin)>CalH+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
#endif //DIGITAL_JOYSTICK
	{
		CheckTurbo();
		return true;
	}
	else
	{
		return false;
	}
}

void RA_JoystickClass::JoystickCenter()
{
#if defined DIGITAL_JOYSTICK || defined REEFANGEL_MINI
	if ( (analogRead(VPin)<30) &&
         (analogRead(HPin)<30) )
#else
	if ( (analogRead(VPin)<CalV+70) &&
         (analogRead(VPin)>CalV-70) &&
         (analogRead(HPin)<CalH+70) &&
         (analogRead(HPin)>CalH-70) )
#endif //defined DIGITAL_JOYSTICK || defined REEFANGEL_MINI
	{
		KeyCount = 0;
		KeyTurbo = 1;
		KeyKeep = 0;
	}	
}

void RA_JoystickClass::CheckTurbo()
{
	KeyCount++;
	if (KeyCount > 7)
        KeyTurbo = 5;
    KeyKeep = millis();
}
