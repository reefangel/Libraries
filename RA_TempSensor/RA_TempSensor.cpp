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

#include "RA_TempSensor.h"
#include <Globals.h>
#include <OneWire.h>
#include <RA_Wifi.h>

OneWire ds(tempPin);

RA_TempSensorClass::RA_TempSensorClass()
{
    unit = DEGREE_F;
}

void RA_TempSensorClass::Init()
{
	byte addr[8];
	byte count=0;
	while (ds.search(addr))
	{
		count++;
		if(addr[0]==0x28)
		{
			if (count==1) memcpy(addrT1,addr,8);
			if (count==2) memcpy(addrT2,addr,8);
			if (count==3) memcpy(addrT3,addr,8);
		}
	}
	ds.reset_search();
}

void RA_TempSensorClass::RequestConversion()
{
	if(addrT1[0]==0x28)
	{
		ds.reset();
		pingSerial();
		ds.select(addrT1);
		pingSerial();
		ds.write(0x44,0);
		pingSerial();
	}
	if(addrT2[0]==0x28)
	{
		ds.reset();
		pingSerial();
		ds.select(addrT2);
		pingSerial();
		ds.write(0x44,0);
		pingSerial();
	}
	if(addrT3[0]==0x28)
	{
		ds.reset();
		pingSerial();
		ds.select(addrT3);
		pingSerial();
		ds.write(0x44,0);
		pingSerial();
	}
}

int RA_TempSensorClass::ReadTemperature(byte addr[8])
{
	//byte count=0;
	unsigned int Temp=0;
	byte data[12];


	//while (ds.search(addr))
	//{
		if(addr[0]==0x28)
		{
	//		count+=1;
			ds.reset();
			pingSerial();
			ds.select(addr);
			pingSerial();
			ds.write(0xBE);
			pingSerial();
			for (byte i = 0; i < 2; i++)
			{	     // we need 9 bytes
				data[i] = ds.read();
			}
	//		if (SensorID==count)
	//		{
                if ((millis()<1000) && (data[0]==80) && (data[1]==5)) return 0;
				Temp=(data[1]<<8)+data[0];//take the two bytes from the response relating to temperature
				Temp=Temp/1.6;
				if (Temp == 0) return 0;
				if (Temp > 850) return 0;
				if ( unit == DEGREE_F )
                    Temp = Temp * 1.8 + 320;
	//		}
		}
	//	if (Temp != 0xffff) return Temp;
	//}
	//ds.reset_search();
	return Temp;
}

