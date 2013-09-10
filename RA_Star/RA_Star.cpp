/*
 * RA_Star.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: Benjamin
 */

#include <RA_Wifi.h>
#include "RA_Star.h"
#include <Globals.h>
#include <avr/wdt.h>

#ifdef RA_STAR
RA_Star::RA_Star()
{
  EthernetDHCP.begin(NetMac, 1);
  NetServer.begin();
}

void RA_Star::ReceiveData()
{
  if (NetServer.available())
  {
    while (NetClient.connected())
    {
      wdt_reset();
      if ( NetClient.available() > 0 ) ProcessEthernet();
    }
  }
}

void RA_Star::ProcessEthernet()
{
  bIncoming=true;
  timeout=millis();
  while (bIncoming)
  {
    if (millis()-timeout>100)
    {
      bIncoming=false;
      NetClient.stop();
    }
    if (NetClient.available()>0)
    {
      PushBuffer(NetClient.read());
      timeout=millis();
#if defined WDT || defined WDT_FORCE
      wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
      if (reqtype>0 && reqtype<128)
      {
        bIncoming=false;
        while(NetClient.available()) NetClient.read();
      }
    }
  }

  ProcessHTTP();

  NetClient.flush();
  NetClient.stop();
  m_pushbackindex=0;
}

#endif
