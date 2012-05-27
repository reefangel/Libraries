/*
  TimeAlarms.cpp - Arduino Time alarms for use with Time library   
  Copyright (c) 2010 Michael Margolis. 
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

 */

extern "C" {
#include <string.h> // for memset
}

#include <WProgram.h>
#include "TimeAlarms.h"
#include "Time.h"

#define IS_ONESHOT  true
#define IS_REPEAT   false 
#define IS_ALARM    true
#define IS_TIMER    false 

//**************************************************************
//* Alarm Class Constructor

AlarmClass::AlarmClass()
{
  Mode.isAlarm =  Mode.isEnabled = Mode.isOneShot = 0;
  value = nextTrigger = repeatvalue = repeatcount = 0;
  onTickHandler = NULL;  // prevent a callback until this pointer is explicitly set 
}

//**************************************************************
//* Private Methods

void AlarmClass::updateNextTrigger()
{
  if( (value != 0) && Mode.isEnabled )
  {
    time_t time = now();
    if(Mode.isAlarm && nextTrigger <= time )   // update alarm if next trigger is not yet in the future
    {
	  if ((repeatvalue&0x00ffffff)==0  || ((nextTrigger+(repeatvalue&0x00ffffff))>=(value + previousMidnight(time)) && nextTrigger<(value + previousMidnight(time))) )
      {
		if( value + previousMidnight(time) <= time) 
		{
		  nextTrigger = value + nextMidnight(time); // if time has passed then set for tomorrow
		}
		else
		{
		  nextTrigger = value + previousMidnight(time);  // set the date to today and add the time given in value	
		}
	  }
	  else
	  {
		if (repeatcount==0)
		{
			repeatcount=(repeatvalue&0xff000000)>>24;
			nextTrigger=0;
		}
		if (repeatcount>0) repeatcount-=1;

		if (nextTrigger==0)
		{
		  if( value + previousMidnight(time) <= time) 
		  {
		    nextTrigger = value + nextMidnight(time); // if time has passed then set for tomorrow 
		  }
		  else
		  {
		    nextTrigger = value + previousMidnight(time);  // set the date to today and add the time given in value	
		  }
		}
		else
		{
		  nextTrigger = nextTrigger + (repeatvalue&0x00ffffff);
		}
	  }
	  //Serial.print("nextTrigger[");
	  //Serial.print(relayid, DEC);
	  //Serial.print("] - ");
	  //Serial.print(month(nextTrigger));
	  //Serial.print("/");
	  //Serial.print(day(nextTrigger));
	  //Serial.print("/");
	  //Serial.print(year(nextTrigger));
	  //Serial.print(" ");
	  //Serial.print(hour(nextTrigger));
	  //Serial.print(":");
	  //Serial.print(minute(nextTrigger));
	  //Serial.print(":");
	  //Serial.println(second(nextTrigger));
    }
    if(Mode.isAlarm == false){
      // its a timer
      nextTrigger = time + value;  // add the value to previous time (this ensures delay always at least Value seconds)
    }
  }
  else {
    Mode.isEnabled = 0;  // Disable if the value is 0
  }
}

//**************************************************************
//* Time Alarms Public Methods

TimeAlarmsClass::TimeAlarmsClass()
{
  isServicing = false;
  clear();
}
  
AlarmID_t TimeAlarmsClass::alarmRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid){ // as above with HMS arguments
     return create( value, 0, relayid, onTickHandler, IS_ALARM, IS_REPEAT);
}

AlarmID_t TimeAlarmsClass::alarmRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid, time_t repeatvalue){ // as above with HMS arguments
     return create( value, repeatvalue, relayid, onTickHandler, IS_ALARM, IS_REPEAT);
}

AlarmID_t TimeAlarmsClass::timerOnce(time_t value, OnTick_t onTickHandler, uint8_t relayid){   // trigger once after the given number of seconds 
     return create( value, 0, relayid, onTickHandler, IS_TIMER, IS_ONESHOT);
}
AlarmID_t TimeAlarmsClass::timerRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid){ // trigger after the given number of seconds continuously
     return create( value, 0, relayid, onTickHandler, IS_TIMER, IS_REPEAT);
}

void TimeAlarmsClass::clear()
{
	for(uint8_t id = 0; id < dtNBR_ALARMS; id++)
     Alarm[id].Mode.isAllocated = false;  // ensure  all Alarms are avialable for allocation 
     
}

void TimeAlarmsClass::enable(AlarmID_t ID)
{
  if(ID < dtNBR_ALARMS && Alarm[ID].Mode.isAllocated){
    Alarm[ID].Mode.isEnabled = (Alarm[ID].value != 0) && (Alarm[ID].onTickHandler != 0) ;  // only enable if value is non zero and a tick handler has been set
    Alarm[ID].updateNextTrigger(); // trigger is updated whenever  this is called, even if already enabled	 
  }
}

void TimeAlarmsClass::disable(AlarmID_t ID)
{
  if(ID < dtNBR_ALARMS && Alarm[ID].Mode.isAllocated)
    Alarm[ID].Mode.isEnabled = false;
}

// write the given value to the given alarm
//void TimeAlarmsClass::write(AlarmID_t ID, time_t value)
//{
//  if(ID < dtNBR_ALARMS && Alarm[ID].Mode.isAllocated){
//    Alarm[ID].value = value;
//    Alarm[ID].nextTrigger=0;
//    enable(ID);
//  }
//}

// return the value for the given alarm
//time_t TimeAlarmsClass::read(AlarmID_t ID)
//{
//  if(ID < dtNBR_ALARMS && Alarm[ID].Mode.isAllocated)
    //return Alarm[ID].value ;
//    return Alarm[ID].repeatvalue;
//  else 	
//    return 0l;  
//}

void TimeAlarmsClass::StartUp()
{
  for (time_t a=0;a<SECS_PER_DAY;a+=10)
  {
    adjustTime(10);
    serviceAlarms();
  }
}

// following functions are not Alarm ID specific.
void TimeAlarmsClass::delay(unsigned long ms)
{
  unsigned long start = millis();
  while( millis() - start  <= ms)
    serviceAlarms();
}
		
//***********************************************************
//* Private Methods

void TimeAlarmsClass::serviceAlarms()
{
  if(! isServicing)
  {
    isServicing = true;
    for(uint8_t i = 0; i < dtNBR_ALARMS; i++)
    {
      if( Alarm[i].Mode.isEnabled && (now() >= Alarm[i].nextTrigger)  )
      {
        OnTick_t TickHandler = Alarm[i].onTickHandler;
        if(Alarm[i].Mode.isOneShot)
          Alarm[i].Mode.isEnabled = Alarm[i].Mode.isAllocated = false;  // free the ID if mode is OnShot		
        else   
           Alarm[i].updateNextTrigger();
        if( TickHandler != NULL) {        
          (*TickHandler)(Alarm[i].relayid);     // call the handler  
        }
      }
    }
    isServicing = false;
  }
}

AlarmID_t TimeAlarmsClass::create( time_t value, time_t repeatvalue, uint8_t relayid, OnTick_t onTickHandler,boolean isAlarm, boolean isOneShot, boolean isEnabled){  // returns true if has been registerd ok
  for(uint8_t id = 0; id < dtNBR_ALARMS; id++)
  {
    if(Alarm[id].Mode.isAllocated == false)
    {
      // here if there is an Alarm id is available
	  Alarm[id].Mode.isAllocated = true;
      Alarm[id].onTickHandler = onTickHandler;
      Alarm[id].Mode.isAlarm = isAlarm;
      Alarm[id].Mode.isOneShot = isOneShot;
      Alarm[id].value = value;
      Alarm[id].repeatvalue = repeatvalue;
      Alarm[id].repeatcount = (repeatvalue&0xff000000)>>24;
      Alarm[id].relayid = relayid;
      Alarm[id].nextTrigger=0;
      isEnabled ?  enable(id) : disable(id);
	  return id;
	}  
  }
  return dtINVALID_ALARM_ID; // no IDs available
}

// make one instance for the user to use
TimeAlarmsClass Alarm = TimeAlarmsClass() ;

