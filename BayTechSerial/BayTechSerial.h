/*
 * Copyright 2013 Reef Angel / Benjamin Runnels
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

/**
 * BayTechSerial controls BayTech power distribution switches and gathers stats via serial connection
 */
#ifndef __BAY_TECH_SERIAL__
#define __BAY_TECH_SERIAL__

#include <Arduino.h>

class BayTechSerial
{
  public:
    BayTechSerial(HardwareSerial &_bayTechSerial);
    void On(byte Outlet);
    void Off(byte Outlet);
    void AllOn(void);
    void AllOff(void);
    void Toggle(byte Outlet);
    void UpdateStatus(void);
    boolean Status(byte Outlet);
    boolean Available(void);

    int GetAveragePower(void);
    int GetApparentPower(void);
    char* GetVoltage(void);
    char* GetCurrent(void);
    char* GetMaxCurrent(void);
    char* GetTemperature(void);
    char* GetBreaker(void);

    byte OutletData;

  private:
    HardwareSerial* bayTechSerial;
    void _init();
    boolean _serialReceive();

    void _parseStatus();
    void _parsePower(char * line);
    void _parseVoltage(char * line);
    void _parseCurrent(char * line);
    void _parseTemperature(char * line);
    void _parseCircuitBreaker(char * line);
    void _parseOutletStatus(char * line);

    unsigned long _lastStatusUpdate;
    boolean _receiveComplete;
    boolean _statusPending;
    int _returnLength;
    int _bufferCount;
    char _receiveBuffer[503];
    int _avg;
    int _app;
    char _voltage[6];
    char _current[5];
    char _maxCurrent[5];
    char _temperature[6];
    char _breaker[5];

};

#endif // __BAY_TECH_SERIAL__
