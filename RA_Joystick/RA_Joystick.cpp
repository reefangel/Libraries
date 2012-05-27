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
	digitalWrite(okPin, HIGH); //pull up resistor on okPin
}

void RA_JoystickClass::Init()
{
	CalV = analogRead(VPin);
	CalH = analogRead(HPin);
	ButtonPress = 0;
	KeyTurbo = 1;
	KeyCount = 0;
}

bool RA_JoystickClass::IsButtonPressed()
{
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
	if ( (analogRead(VPin)>CalV+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
	{
		KeyCount++;
		if (KeyCount > 5)
            KeyTurbo = 5;
	    KeyKeep = millis();
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
	if ( (analogRead(VPin)<CalV-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
	{
		KeyCount++;
		if (KeyCount > 5)
            KeyTurbo = 5;
	    KeyKeep = millis();
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
	if ( (analogRead(HPin)<CalH-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
	{
		KeyCount++;
		if (KeyCount > 5)
            KeyTurbo = 5;
	    KeyKeep = millis();
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
	if ( (analogRead(HPin)>CalH+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo) )
	{
		KeyCount++;
		if (KeyCount > 5)
            KeyTurbo = 5;
	    KeyKeep = millis();
		return true;
	}
	else
	{
		return false;
	}
}

void RA_JoystickClass::JoystickCenter()
{
	if ( (analogRead(VPin)<CalV+70) &&
         (analogRead(VPin)>CalV-70) &&
         (analogRead(HPin)<CalH+70) &&
         (analogRead(HPin)>CalH-70) )
	{
		KeyCount = 0;
		KeyTurbo = 1;
		KeyKeep = 0;
	}
}

