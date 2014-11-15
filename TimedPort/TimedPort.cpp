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

#include "TimedPort.h"
#include "InternalEEPROM.h"
#include "ReefAngel.h"

TimedPort::TimedPort()
{
	Init();
}

void TimedPort::Init() 
{
	ID = 0;
	Delay = 0;
	Repeat = 0;
	Offset = 0;
	Time = 0;
  StartTime = 0;
  StopTime = 0;
	Scale=1;
	
	activestatus = true;
	started = false;
	stopped = false;
	running = false;
  timer = 0;
}

void TimedPort::On()
{
  if (Delay) {
    DelayedOn();
  } else {
    ReefAngel.Relay.On(ID);
    running=true;
  }
}

void TimedPort::Off()
{
  ReefAngel.Relay.Off(ID);
  running=false;
}

void TimedPort::DelayedOn() 
{
  DelayedOn(Delay); 
}

void TimedPort::DelayedOn(byte delay) 
{
  ReefAngel.Relay.DelayedOn(ID, delay);
  running=ReefAngel.Relay.Status(ID);
}

void TimedPort::Auto()
{
	Override(2);
}

void TimedPort::Override(byte type) 
{ 
  ReefAngel.Relay.Override(ID, type);
  running=ReefAngel.Relay.Status(ID);
}

void TimedPort::Set(bool status) 
{
  ReefAngel.Relay.Set(ID, running=status); 
}
  
void TimedPort::Toggle() 
{
  ReefAngel.Relay.Set(ID, running=!running); 
}
	
void TimedPort::Run() {
  Run(Offset, Time, Repeat-Time, true);
}

void TimedPort::Run(int offset, int ontime, int offtime, bool status)
{
  Osc(offset, ontime, offtime, status);
  if (activestatus) {
    if (now()<timer) {
      if (started) {
    	  On();
      } 
      if (stopped) {
        Off();
      } 
    } else {
      if (started) {
        started = false;
      } 
      if (stopped) {
        stopped = false;
      }
    }
  }
}

void TimedPort::Osc() 
{
	Osc(Offset, Time, Repeat-Time, true);
}

// Compatibility with the APEX syntax for OSC
// OSC offset/ontime/offtime THEN on/off
// Currently in seconds instead of minute:seconds
void TimedPort::Osc(int offset, int ontime, int offtime, bool status)
{
  time_t o = offset*Scale;
  time_t t = ontime*Scale;
  time_t f = offtime*Scale;
	
	running=((now()+o)%(t+f))<t;

	if (!status) running=!running;
  ReefAngel.Relay.Set(ID, running);  
}

void TimedPort::Start() 
{
	Start(StartTime);
}

void TimedPort::Start(int time)
{
	time_t t = time*Scale;

	if (!started) {
		timer = now()+t;
		started = true;
		stopped = false;
		On();
	}
}

void TimedPort::Stop() 
{
	Stop(StopTime);
}

void TimedPort::Stop(int time) 
{
	time_t t = time*Scale;
  
	if (!stopped) {
		timer = now()+t;
		started = false;
		stopped = true;
		Off();
	}
}

void TimedPort::Load(int memStart) {
  ID=InternalMemory.read(memStart);
  Delay=InternalMemory.read(memStart+1);
  Time=InternalMemory.read_int(memStart+2);
  Offset=InternalMemory.read_int(memStart+4);
  Repeat=InternalMemory.read_int(memStart+6);
  StartTime=InternalMemory.read_int(memStart+8);
  StopTime=InternalMemory.read_int(memStart+10);
  Scale=InternalMemory.read_dword(memStart+12);
}

void TimedPort::Save(int memStart) {
  if (ID!=InternalMemory.read(memStart)) InternalMemory.write(memStart, ID);
  if (Delay!=InternalMemory.read(memStart+1)) InternalMemory.write(memStart+1, Delay);
  if (Time!=InternalMemory.read_int(memStart+2)) InternalMemory.write_int(memStart+2, Time);
  if (Offset!=InternalMemory.read_int(memStart+4)) InternalMemory.write_int(memStart+4, Offset);
  if (Repeat!=InternalMemory.read_int(memStart+6)) InternalMemory.write_int(memStart+6, Repeat);
  if (StartTime!=InternalMemory.read_int(memStart+8)) InternalMemory.write_int(memStart+8, StartTime);
  if (StopTime!=InternalMemory.read_int(memStart+10)) InternalMemory.write_int(memStart+10, StopTime);
  if (Scale!=InternalMemory.read_dword(memStart+12)) InternalMemory.write_dword(memStart+12, Scale); 
}

