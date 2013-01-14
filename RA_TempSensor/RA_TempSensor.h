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

#ifndef __RA_TEMPSENSOR_H__
#define __RA_TEMPSENSOR_H__

#include <limits.h>
#include <Globals.h>

#define ONEWIRE_ADDR_LENGTH 8

typedef uint8_t OneWireAddr[ONEWIRE_ADDR_LENGTH];

typedef struct OneWireTempSensor {
#ifndef NDEBUG
   int lastTempRead;
#endif
   OneWireAddr addr;
} OneWireTempSensor;

class RA_TempSensorClass
{
public:
    /**
    * Initializes OneWire bus on temp pin, enumerates sensors and starts a temp conversion.
    */
	void begin();

	/**
	* Returns the number of temperature sensors available.
	*
	* @return number of sensors available.
	*/
	unsigned int Sensors() const;

	/**
	*   Request temperature conversion from all of the available sensors.
	*   Temperature conversion takes, by default, a maximum of 750 msec.
	*/
	void RequestConversion();

	/**
	*   Return <tt>true</tt> if the temperature conversion is complete.
	*   When complete the temperature can be read by <tt>ReadTemperature</tt>.
	*
	*   @return <tt>true</tt> if the temperature conversion is complete otherwise false.
	*/
	bool ConversionDone() const;

    /**
    * Return temperature in Celsius 10ths of a degree.
    *
    * @param sensor Which temperature probe to read. Must be <tt>0 <= probe < Sensors()</tt>
    * @return temperature in Celsius 10ths of a degree. ie. 25.6C == 256
    */
	int ReadTemperature(int sensor);

private:
    unsigned int all_sensors;
    unsigned int temp_sensors;
    unsigned long last_probe_search;
    OneWireTempSensor sensors[MAX_TEMP_SENSORS];

    /**
    * Search OneWire bus for temperatures sensors and return the number of sensors found.
    *
    * @return number of temperature sensors found during search.
    */
    unsigned int SensorSearch();

    /**
    * Return temperature conversion for single temperature sensor.
    *
    * @param addr uniq addr of DS18B20 (Family 0x28) probe
    */
	void SendRequest(OneWireTempSensor& sensor);

    /**
    * Return temperature in Celsius 10ths of a degree.
    *
    * @param addr uniq addr of DS18B20 (Family 0x28) probe
    * @return temperature in Celsius 10ths of a degree. ie. 25.6C == 256
    */
    int ReadTemperature(OneWireTempSensor& sensor);
};

#endif // __RA_TEMPSENSOR_H__
