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

#include "BayTechSerial.h"

BayTechSerial::BayTechSerial(HardwareSerial &_bayTechSerial)
{
  bayTechSerial = &_bayTechSerial;
  _init();
}

boolean BayTechSerial::Available(void)
{
  if (millis() - _lastStatusUpdate > 60000)
  {
    _lastStatusUpdate = millis();
    UpdateStatus();
  }

  if (bayTechSerial->available())
  {
    _receiveComplete = _serialReceive();
  }

  if (_receiveComplete && _statusPending)
  {
    _parseStatus();
    _statusPending = false;
  }

  return _receiveComplete;
}

void BayTechSerial::_init()
{
  bayTechSerial->begin(9600);
  _statusPending = false;
  _bufferCount = 0;
  _lastStatusUpdate = 0;
  OutletData = 0;
  _receiveComplete = false;
  UpdateStatus();
}

void BayTechSerial::On(byte Outlet)
{
  OutletData |= 1 << Outlet;
  _returnLength = 12;
  _receiveComplete = false;
  bayTechSerial->print("on ");
  bayTechSerial->println(Outlet + 1);
}

void BayTechSerial::Off(byte Outlet)
{
  bitClear(OutletData, Outlet);
  _returnLength = 13;
  _receiveComplete = false;
  bayTechSerial->print("off ");
  bayTechSerial->println(Outlet + 1);
}

void BayTechSerial::AllOn(void)
{
  OutletData = 0xff;
  _returnLength = 10;
  _receiveComplete = false;
  bayTechSerial->println("on");
}

void BayTechSerial::AllOff(void)
{
  OutletData = 0;
  _returnLength = 11;
  _receiveComplete = false;
  bayTechSerial->println("off");
}

void BayTechSerial::Toggle(byte Outlet)
{
  bitRead(OutletData, Outlet) ? Off(Outlet) : On(Outlet);
}

boolean BayTechSerial::Status(byte Outlet)
{
  return bitRead(OutletData, Outlet);
}

void BayTechSerial::UpdateStatus()
{
  _returnLength = 502;
  _receiveComplete = false;
  _statusPending = true;
  bayTechSerial->println("status");
}

boolean BayTechSerial::_serialReceive()
{
  while (bayTechSerial->available() > 0)
  {
    byte c = bayTechSerial->read();
    if (c != '\r')
    {
      _receiveBuffer[_bufferCount++] = c;
    }

    if (_bufferCount == _returnLength)
    {
      _receiveBuffer[_bufferCount++] = '\0';
      _bufferCount = 0;
      _receiveComplete = true;
      return true;
    }
  }
  return false;
}

void BayTechSerial::_parseStatus()
{
//  Serial.println(_receiveBuffer);
  int cnt = 0;
  char * result = _receiveBuffer;
  char *line;
  while ((line = strtok_r(result, "\n", &result)) != NULL)
  {
    if (line != " " && line != "")
    {
      switch (cnt)
      {
        case 1:
          _parsePower(line);
//          Serial.print("Average Power: '");
//          Serial.print(_avg);
//          Serial.println("' Watts");
//
//          Serial.print("Apparent Power: '");
//          Serial.print(_app);
//          Serial.println("' VA");
          break;
        case 2:
          _parseVoltage(line);
//          Serial.print("True RMS Voltage: '");
//          Serial.print(_voltage);
//          Serial.println("' Volts");
          break;
        case 3:
          _parseCurrent(line);
//          Serial.print("True RMS Current: '");
//          Serial.print(_current);
//          Serial.println("' Amps");
//          Serial.print("Maximum Detected: '");
//          Serial.print(_maxCurrent);
//          Serial.println("' Amps");
          break;
        case 4:
          _parseTemperature(line);
//          Serial.print("Internal Temperature: '");
//          Serial.print(_temperature);
//          Serial.println("' C");
          break;
        case 5:
          _parseCircuitBreaker(line);
//          Serial.print("Outlet Circuit Breaker: '");
//          Serial.print(_breaker);
//          Serial.println("'");
          break;
        case 6:
          case 7:
          case 8:
          case 9:
          case 10:
          case 11:
          case 12:
          case 13:
          _parseOutletStatus(line);
//          Serial.print("Outlet ");
//          Serial.print(cnt - 5);
//          Serial.print(" status: ");
//          Serial.println(bitRead(OutletData, cnt - 6) ? "On" : "Off");
          break;
      }

      cnt++;
    }
  }
}

void BayTechSerial::_parsePower(char * line)
{
  sscanf(line, "Average Power:    %i Watts        Apparent Power:    %i VA", &_avg, &_app);
}

void BayTechSerial::_parseVoltage(char * line)
{
  sscanf(line, "True RMS Voltage: %s Volts", &_voltage);
}

void BayTechSerial::_parseCurrent(char * line)
{
  sscanf(line, "True RMS Current:  %s Amps     Maximum Detected:   %s Amps", &_current, &_maxCurrent);
}

void BayTechSerial::_parseTemperature(char * line)
{
  sscanf(line, "Internal Temperature:  %s C", &_temperature);
}

void BayTechSerial::_parseCircuitBreaker(char * line)
{
  sscanf(line, "Outlet Circuit Breaker: %s", &_breaker);
}

int BayTechSerial::GetAveragePower(void)
{
  return _avg;
}

int BayTechSerial::GetApparentPower(void)
{
  return _app;
}

char* BayTechSerial::GetVoltage(void)
{
  return _voltage;
}

char* BayTechSerial::GetCurrent(void)
{
  return _current;
}

char* BayTechSerial::GetMaxCurrent(void)
{
  return _maxCurrent;
}

char* BayTechSerial::GetTemperature(void)
{
  return _temperature;
}

char* BayTechSerial::GetBreaker(void)
{
  return _breaker;
}

void BayTechSerial::_parseOutletStatus(char * line)
{
  int Outlet;
  char status[4];
  sscanf(line, " %i)...%*s  %*i       : %s", &Outlet, &status);
  if (strstr(status, "Off"))
  {
    bitClear(OutletData, (Outlet - 1));
  }
  else
  {
    OutletData |= 1 << (Outlet - 1);
  }
}
