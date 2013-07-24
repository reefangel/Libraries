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
 * RA_BayTech emulates RA Relay to control a BayTech power distribution switch and gathers stats via serial connection
 */

#ifndef __RA_BAY_TECH__
#define __RA_BAY_TECH__

#include <BayTechSerial.h>
#include <Relay.h>

class RA_BayTech : public RelayClass
{
  public:
    RA_BayTech(void);
    void Write(void);

  private:
    byte _lastBT1OutletData;
    BayTechSerial BT1;
};

#endif // __RA_BAY_TECH__
