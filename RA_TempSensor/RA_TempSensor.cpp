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

#include <limits.h>

#include "RA_TempSensor.h"

#include <Globals.h>
#include <OneWire.h>
#include <ReefAngel.h>

// Initial default temperature (25.0C)
#define INITAL_DEFAULT_TEMP 250

#define ONEWIRE_DS18B20_FAMILY ((uint8_t) 0x28)

#define ONEWIRE_DS18B20_CONVERT		     ((uint8_t) 0x44)

#define ONEWIRE_SEARCH_ROM               ((uint8_t) 0xF0)
#define ONEWIRE_READ_ROM		         ((uint8_t) 0x33)
#define ONEWIRE_MATCH_ROM		         ((uint8_t) 0x55)
#define ONEWIRE_SKIP_ROM		         ((uint8_t) 0xCC)
#define ONEWIRE_DS18B20_ALARM_SEARCH     ((uint8_t) 0xEC)

#define ONEWIRE_DS18B20_WRITE_SCRATCHPAD ((uint8_t) 0x4E)
#define ONEWIRE_DS18B20_READ_SCRATCHPAD  ((uint8_t) 0xBE)
#define ONEWIRE_DS18B20_COPY_SCRATCHPAD  ((uint8_t) 0x48)
#define ONEWIRE_DS18B20_RECALL_EEPROM    ((uint8_t) 0xB8)
#define ONEWIRE_DS18B20_READ_POWER       ((uint8_t) 0xB4)

#define ONEWIRE_DS18B20_SCRATCH_LENGTH    9
#define ONEWIRE_DS18B20_SCRATCH_TEMP_LSB  0
#define ONEWIRE_DS18B20_SCRATCH_TEMP_MSB  1
#define ONEWIRE_DS18B20_SCRATCH_TH_UB1    2
#define ONEWIRE_DS18B20_SCRATCH_TL_UB2    3
#define ONEWIRE_DS18B20_SCRATCH_CONFIG    4
#define ONEWIRE_DS18B20_SCRATCH_RESERVED1 5
#define ONEWIRE_DS18B20_SCRATCH_RESERVED2 6
#define ONEWIRE_DS18B20_SCRATCH_RESERVED3 7
#define ONEWIRE_DS18B20_SCRATCH_CRC       8

OneWire ds(tempPin);

static void debug(const char* msg, OneWireTempSensor sensor) {
    unsigned long now = millis();
    Serial.print( msg );
    Serial.print( "  " );
    Serial.print( "addr: " );
    Serial.print( sensor.addr[0], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[1], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[2], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[3], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[4], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[5], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[6], HEX );
    Serial.print( " " );
    Serial.print( sensor.addr[7], HEX );
    Serial.print( " reqd: " );
    unsigned long since = now - sensor.lastRequested;
    Serial.print( since, HEX );
    Serial.print( "ms read: " );
    since = now - sensor.lastRead;
    Serial.print( since, HEX );
    Serial.print( "ms interval: " );
    since = sensor.lastRequested - sensor.lastRead;
    Serial.print( since, HEX );
    Serial.print( "ms temp: " );
   Serial.print( sensor.lastTempRead, HEX );
    Serial.println();
}

void RA_TempSensorClass::Init()
{
        searches = 1;
        all_sensors = 0;
        temp_sensors = 0;
        last_probe_search = ULONG_MAX;

        RequestConversion();
}

unsigned int RA_TempSensorClass::SensorSearch()
{
    temp_sensors = 0;
    all_sensors = 0;
	if( !ds.reset() ) {
	    Serial.println("No presence");
	}
	while ((temp_sensors < MAX_TEMP_SENSORS) && ds.search(sensors[temp_sensors].addr))
	{
	    all_sensors++;
		if(sensors[temp_sensors].addr[0]==ONEWIRE_DS18B20_FAMILY)
		{
		    if( OneWire::crc8( sensors[temp_sensors].addr, 7) != sensors[temp_sensors].addr[7])
		    {
		    Serial.println("CRC is not valid!");
		    }
		    sensors[temp_sensors].lastRequested = sensors[temp_sensors].lastRead = 0;
			temp_sensors++;
		}
		ReefAngel.Yield();
	}
	ds.reset_search();

	searches++;

	return temp_sensors;
}

unsigned int RA_TempSensorClass::Sensors()
{
    return temp_sensors;
}

void RA_TempSensorClass::RequestConversion()
{
    unsigned long now = millis();
//    if((temp_sensors < MAX_TEMP_SENSORS) &&
//      (now - last_probe_search > (60 * 1000)))
    {
      SensorSearch();
      last_probe_search = now;
    }

    for(int each = 0; each < temp_sensors; each++)
    {
	    SendRequest(sensors[each]);
    }
}

bool RA_TempSensorClass::ConversionDone()
{
    return 1 == ds.read_bit() ||  temp_sensors == 0;
}

void RA_TempSensorClass::SendRequest(OneWireTempSensor sensor)
{
	if(sensor.addr[0]==ONEWIRE_DS18B20_FAMILY)
	{
        debug("req ", sensor);

        ds.reset();
		ReefAngel.Yield();
		ds.select(sensor.addr);
		ReefAngel.Yield();
		ds.write(ONEWIRE_DS18B20_CONVERT);
        ReefAngel.Yield();

        sensor.lastRequested = millis();
	}
}

int RA_TempSensorClass::ReadTemperature(int probe)
{
    if(probe == 2) {
        return searches * 1000 + all_sensors * 10 + temp_sensors;
    }

    return ReadTemperature(sensors[probe]);
}

int RA_TempSensorClass::ReadTemperature(OneWireTempSensor sensor)
{
	unsigned int temp = INT_MIN;
	uint8_t data[ONEWIRE_DS18B20_SCRATCH_LENGTH];


    if((sensor.addr[0]==ONEWIRE_DS18B20_FAMILY) && (millis() - sensor.lastRequested < 750))
    {
        debug("read", sensor);

		ds.reset();
		ReefAngel.Yield();
		ds.select(sensor.addr);
		ReefAngel.Yield();
		ds.write(ONEWIRE_DS18B20_READ_SCRATCHPAD);
		ReefAngel.Yield();
		// read only first two bytes.
		ds.read_bytes(data, 2);
		ReefAngel.Yield();

		// de-bounce power-on reading of 85C degrees
		if ((millis()<1000) && (data[0]==0x50) && (data[1]==0x05))
		{
		    temp = INITAL_DEFAULT_TEMP;
		}
		else
		{
            temp=(data[1] << 8)+data[0]; //take the two bytes from the response relating to temperature
		    temp= (temp * 10) >> 4; // (multiply by 10 and divide by 16)
		}

		sensor.lastRead = millis();
		sensor.lastTempRead = temp;
    }

	return temp;
}

