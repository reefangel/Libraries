/*
 * RA_Wiznet5100.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: Benjamin
 */
#include <Globals.h>
#ifdef ETH_WIZ5100
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
	DDRD|=(1<<4); // Port PD4 output
	DDRD|=(1<<5); // Port PD5 output
}

void RA_Wiznet5100::Update()
{
	// Check for any changes on the DHCP status
	EthernetDHCP.poll();

	const byte* ipAddr=EthernetDHCP.ipAddress();

	if (ipAddr[0]!=0)
		sbi(PORTD,5);
	else
		cbi(PORTD,5);
	// Let's check for any incoming data
    ReceiveData();

    // Read and dump what the server is returning from the Portal GET request.
	if (NetClient.available() && PortalConnection)
	{
		sbi(PORTD,4);
		while(NetClient.available())
		{
			wdt_reset();
			char c = NetClient.read();
		}
	}

	// if the server has disconnected, stop the client
	if (!NetClient.connected() && PortalConnection)
	{
		cbi(PORTD,4);
		PortalConnection=false;
		NetClient.stop();
	}

	// if request timed out, stop the client
	if (NetClient.connected() && PortalConnection && millis()-PortalTimeOut>PORTAL_TIMEOUT)
	{
		cbi(PORTD,4);
		PortalConnection=false;
		NetClient.stop();
	}
}

void RA_Wiznet5100::ReceiveData()
{
	NetClient=NetServer.available();
	if (NetClient)
	{
		sbi(PORTD,4);
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
			cbi(PORTD,4);
			bIncoming=false;
			NetClient.stop();
		}
		if (NetClient.available()>0)
		{
			PushBuffer(NetClient.read());
			timeout=millis();
			wdt_reset();
			if (reqtype>0 && reqtype<128)
			{
				bIncoming=false;
				while(NetClient.available())
				{
					wdt_reset();
					NetClient.read();
				}
			}
		}
	}
	wdt_reset();
	ProcessHTTP();

	cbi(PORTD,4);
	NetClient.stop();
	m_pushbackindex=0;
}
size_t RA_Wiznet5100::write(uint8_t c) { return NetClient.write((uint8_t)c); }
size_t RA_Wiznet5100::write(unsigned long n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(long n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(unsigned int n) { return NetClient.write((uint8_t)n); }
size_t RA_Wiznet5100::write(int n) { return NetClient.write((uint8_t)n); }

#endif // ETH_WIZ5100


