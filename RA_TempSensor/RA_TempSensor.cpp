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

 /*
  * Updated by:  Mike Duigou
  * Updates Released under Apache License, Version 2.0
  */

#include <limits.h>

#include "RA_TempSensor.h"

#include <Globals.h>
#include <OneWire.h>
#include <ReefAngel.h>

#define ONEWIRE_DS18B20_RESCAN_INTERVAL  ((uint32_t) (20 * 1000))

// Initial default temperature (26.5C)
#define INITAL_DEFAULT_TEMP                         ((int) 26.5)

// power on initial temperature (85.0C)
#define ONEWIRE_DS18B20_FAMILY_POWERON_DEFAULT_TEMP ((int) 850)

#define ONEWIRE_DS18B20_FAMILY           ((uint8_t) 0x28)

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

void RA_TempSensorClass::begin()
{
        all_sensors = 0;
        temp_sensors = 0;
        last_probe_search = ULONG_MAX;

        RequestConversion();
}

unsigned int RA_TempSensorClass::SensorSearch()
{
    temp_sensors = 0;
    all_sensors = 0;
	if( ds.reset() ) {
	    // something lives!
    	while ((temp_sensors < MAX_TEMP_SENSORS) && ds.search(sensors[temp_sensors].addr))
        {
            ReefAngel.Yield();
            all_sensors++;
            if(sensors[temp_sensors].addr[0]==ONEWIRE_DS18B20_FAMILY)
            {
                if( OneWire::crc8(sensors[temp_sensors].addr, ONEWIRE_ADDR_LENGTH - 1) != sensors[temp_sensors].addr[ONEWIRE_ADDR_LENGTH - 1])
                {
                continue;
                }
                sensors[temp_sensors].lastTempRead = INITAL_DEFAULT_TEMP;
                temp_sensors++;
            }

        }
        ds.reset_search();
        ReefAngel.Yield();

        // clear empty slots.
        for(int clear = temp_sensors; clear < MAX_TEMP_SENSORS; clear++)
        {
            sensors[clear].addr[0] = 0;
        }
    }

    last_probe_search = millis();

	return temp_sensors;
}

unsigned int RA_TempSensorClass::Sensors() const
{
    return temp_sensors;
}

void RA_TempSensorClass::RequestConversion()
{
    if((temp_sensors < MAX_TEMP_SENSORS) &&
      ((ULONG_MAX == last_probe_search) || (millis() - last_probe_search > ONEWIRE_DS18B20_RESCAN_INTERVAL)))
    {
      SensorSearch();
    }

    for(int each = 0; each < temp_sensors; each++)
    {
	    SendRequest(sensors[each]);
    }
}

bool RA_TempSensorClass::ConversionDone() const
{
    bool done = (temp_sensors == 0) || 1 == ds.read_bit();

    return done;
}

void RA_TempSensorClass::SendRequest(OneWireTempSensor& sensor)
{
	if(sensor.addr[0]==ONEWIRE_DS18B20_FAMILY)
	{
        ds.reset();
		ReefAngel.Yield();
		ds.select(sensor.addr);
		ReefAngel.Yield();
		ds.write(ONEWIRE_DS18B20_CONVERT);
        ReefAngel.Yield();
	}
}

int RA_TempSensorClass::ReadTemperature(int sensor)
{
    if (sensor >= temp_sensors)
    {
        return INT_MIN;
    }

    int temp = ReadTemperature(sensors[sensor]);

    if((INT_MIN == temp) && (sensor == temp_sensors - 1))
    {
        // remove this sensor completely.
        temp_sensors--;
    }

    return temp;
}

int RA_TempSensorClass::ReadTemperature(OneWireTempSensor& sensor)
{
	int temp = INT_MIN;
	uint8_t data[ONEWIRE_DS18B20_SCRATCH_LENGTH];

    if(sensor.addr[0]==ONEWIRE_DS18B20_FAMILY)
    {
		ds.reset();
		ReefAngel.Yield();
		ds.select(sensor.addr);
		ReefAngel.Yield();
		ds.write(ONEWIRE_DS18B20_READ_SCRATCHPAD);
		ReefAngel.Yield();
		// read only first two bytes.
		ds.read_bytes(data, 2);
		ReefAngel.Yield();

        temp = (data[ONEWIRE_DS18B20_SCRATCH_TEMP_MSB] << 8) + data[ONEWIRE_DS18B20_SCRATCH_TEMP_LSB];
        temp = (temp * 10) >> 4; // (multiply by 10 and divide by 16 for 10ths of a degree)
        if(-1 == temp)
        {
            // -1 is what we get if the sensor cannot be contacted.
            // read remainder of scratch
            ds.read_bytes(&data[2], ONEWIRE_DS18B20_SCRATCH_LENGTH - 2);
            ReefAngel.Yield();
            if( OneWire::crc8( data, ONEWIRE_DS18B20_SCRATCH_LENGTH - 1) != data[ONEWIRE_DS18B20_SCRATCH_CRC])
            {
                // bad CRC. Sensor is missing or suspect.
                sensor.addr[0] = 0; // disable this sensor
                temp = INT_MIN;
            }
        }
        else if((ONEWIRE_DS18B20_FAMILY_POWERON_DEFAULT_TEMP == temp) && (millis()<1000))
        {
            // de-bounce power-on reading of 85C degrees
            temp = INITAL_DEFAULT_TEMP;
        }

#ifndef NDEBUG
		sensor.lastTempRead = temp;
#endif
    }

	return temp;
}

