/*
 * RA_Wiznet5100.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: Benjamin
 */
#include <Globals.h>
#ifdef RA_STAR
#include "RA_Wiznet5100.h"
#include <Ethernet.h>
#include <EthernetDHCP.h>
#include <RA_Wifi.h>
#include <avr/wdt.h>

RA_Wiznet5100::RA_Wiznet5100()
{
  PortalConnection=false;
  PortalTimeOut=millis();
}

void RA_Wiznet5100::Init()
{
  EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
  NetServer.begin();
//  Serial.print("My IP address: ");
//  for (byte thisByte = 0; thisByte < 4; thisByte++) {
//    // print the value of each byte of the IP address:
//    Serial.print(EthernetDHCP.ipAddress()[thisByte], DEC);
//    Serial.print(".");
//  }
//  Serial.println();

}

void RA_Wiznet5100::Update()
{
  EthernetDHCP.poll();
  if (NetClient.available() && PortalConnection)
  {
    while(NetClient.available())
    {
      wdt_reset();
      char c = NetClient.read();
    }
  }

  // if the server's disconnected, stop the client
  if (!NetClient.connected() && PortalConnection)
  {
    PortalConnection=false;
    NetClient.stop();
  }

  // if request timed out, stop the client
  if (NetClient.connected() && PortalConnection && millis()-PortalTimeOut>PORTAL_TIMEOUT)
  {
    PortalConnection=false;
    NetClient.stop();
  }
}

void RA_Wiznet5100::ReceiveData()
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

void RA_Wiznet5100::ProcessEthernet()
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

size_t RA_Wiznet5100::write(uint8_t c) { return NetClient.write((uint8_t)c); }
size_t RA_Wiznet5100::write(unsigned long n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(long n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(unsigned int n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(int n) { return NetClient.write((uint8_t)n); }

#endif


