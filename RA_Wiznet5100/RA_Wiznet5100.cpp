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
	FoundIP=false;
	RelayConnected=false;
	RelayIndex=0;
	ConnectionRetry=0;
	RelayClient.setTimeout(100);
	PortalClient.setTimeout(100);
	PortalConnection=false;
	PortalWaiting=false;
	PortalDataReceived=false;
}

void RA_Wiznet5100::Update()
{
	// Check for any changes on the DHCP status
	EthernetDHCP.poll();

	const byte* ipAddr=EthernetDHCP.ipAddress();

	if (ipAddr[0]!=0)
	{
		FoundIP=true;
		sbi(PORTD,5);
		// Let's check for any incoming data
	    ReceiveData();

	    //Portal server
	    // Read and dump what the server is returning from the Portal GET request.
		if (PortalClient.available() && PortalConnection)
		{
			Serial.println("Receiving Data...");
			while(PortalClient.available())
			{
				wdt_reset();
				char c = PortalClient.read();
				Serial.write(c);
			}
			PortalDataReceived=true;
			Serial.println();
			Serial.println("Portal Received");
		}

		// if the server has disconnected, stop the client
		if (!PortalClient.connected() && PortalConnection)
		{
			Serial.println("Portal Disconnected");
			PortalConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
		}

		// if request timed out, stop the client
		if (PortalClient.connected() && PortalConnection && millis()-PortalTimeOut>PORTAL_TIMEOUT)
		{
			Serial.println("Portal Timeout");
			PortalConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
		}
		// Relay server
		if (!RelayClient.connected()) // Check for relay server closed connection
		{
			Serial.println("Relay Disconnected");
			ConnectionRetry++;
			cbi(PORTD,4);
			RelayConnected=false;
			RelayIndex=0;
			RelayClient.stop(); // Make sure we free up the client
			if (ConnectionRetry>=RETRY_COUNT) // Connection failed too many times
			{
				Serial.println("Reinitialzing");
				Init();
			}
			else
			{
				delay(100);
				RelayClient.noblockconnect(RelayServer, 80);
			}
		}
		else
		{
			if (RelayClient.checkconnect()==0x17) // Check for connection established
			{
				if (!RelayConnected)
				{
					Serial.println("Relay Connected");
					ConnectionRetry=0;
					sbi(PORTD,4);
					RelayConnected=true;
					RelayClient.print("POST /");
					RelayClient.print(uid);
					RelayClient.println(" HTTP/1.1");
					RelayClient.println("Upgrade: PTTH/1.0");
					RelayClient.println("Connection: Upgrade");
					RelayClient.println("Host: try.yaler.net");
					RelayClient.println();
				}
			}
		}
	}
	else
	{
		FoundIP=false;
		cbi(PORTD,5);
	}
}

void RA_Wiznet5100::ReceiveData()
{
	NetClient=NetServer.available();
	if (NetClient)
	{
		while (NetClient.connected())
		{
			wdt_reset();
			if ( NetClient.available() > 0 ) ProcessEthernet();
		}
	}
	if (RelayClient.available())
	{
		while (RelayClient.connected())
		{
			wdt_reset();
			if (RelayClient.available()>54) // length of the return header of the HTTP upgrade
				RelayClient.find("\r\n\r\n"); // Discard header
			else
				RelayClient.read(); // Most likely HTTP/1.1 204, so we read one byte to cause timeout
			ProcessRelayEthernet();
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

//	cbi(PORTD,4);
	NetClient.stop();
	m_pushbackindex=0;
}

void RA_Wiznet5100::ProcessRelayEthernet()
{
	bIncomingR=true;
	timeoutR=millis();
//	Serial1.println("Incoming...");
	while (bIncomingR)
	{
		if (millis()-timeoutR>100)
		{
//			Serial1.println("Timeout");
			cbi(PORTD,4);
			bIncomingR=false;
			RelayIndex=0;
			RelayClient.stop();
		}
		if (RelayClient.available()>0)
		{
			if (RelayIndex++==5)
			{
				// Commented to allow for direct access subdomain instead of folder
//				for (int a=0;a<uid.length();a++)
//				{
//					RelayClient.read(); // Consume the unique id
//				}
//				if (RelayClient.peek()=='/') RelayClient.read(); // Consume the slash, we already have one
			}
			char c=RelayClient.read();
//			Serial1.write(c);
			PushBuffer(c);
			timeoutR=millis();
			wdt_reset();
			if (reqtype>0 && reqtype<128)
			{
				bIncomingR=false;
				while(RelayClient.available())
				{
					wdt_reset();
					RelayClient.read();
				}
			}
		}
	}
//	Serial1.println();
//	Serial1.println(reqtype);
	wdt_reset();
	ProcessHTTP();
//	Serial1.println("Done processing");
//	cbi(PORTD,4);
	RelayIndex=0;
	RelayClient.stop();
	m_pushbackindex=0;
}

void RA_Wiznet5100::DirectAccess(String uniqueid)
{
	uid=uniqueid;
}

void RA_Wiznet5100::PortalConnect()
{
	  PortalClient.noblockconnect(PortalServer, 80);
	  PortalTimeOut=millis();
}

boolean RA_Wiznet5100::IsPortalConnected()
{
	return PortalClient.checkconnect()==0x17;
}

size_t RA_Wiznet5100::write(uint8_t c) { if (RelayIndex) return RelayClient.write((uint8_t)c); else if (PortalConnection) return PortalClient.write((uint8_t)c); else return NetClient.write((uint8_t)c); }
//size_t RA_Wiznet5100::write(unsigned long n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(long n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(unsigned int n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(int n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }

#endif // ETH_WIZ5100


