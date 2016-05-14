/*
 * RA_Wiznet5100.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: Benjamin
 */
#include <Globals.h>
#ifdef ETH_WIZ5100
#include <ReefAngel.h>
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
	FoundIP=false;
//	RelayConnected=false;
//	RelayIndex=0;
//	ConnectionRetry=0;
//	RelayClient.setTimeout(100);
	PortalClient.setTimeout(100);
	PortalConnection=false;
	PortalWaiting=false;
	PortalDataReceived=false;
	MQTTReconnectmillis=millis();
	MQTTSendmillis=millis();
}

void RA_Wiznet5100::Update()
{
	// Check for any changes on the DHCP status
	EthernetDHCP.poll();

	const byte* ipAddr=EthernetDHCP.ipAddress();

	if (ipAddr[0]!=0)
	{
		if (!FoundIP)
		{
	        Serial.println(ip_to_str(ipAddr));
		}
		FoundIP=true;
		// Let's check for any incoming data
	    ReceiveData();

	    //Portal server
	    // Read and dump what the server is returning from the Portal GET request.
		if (PortalClient.available() && PortalConnection)
		{
			Serial.println(F("Receiving Data..."));
			while(PortalClient.available())
			{
				wdt_reset();
				char c = PortalClient.read();
				Serial.write(c);
			}
			PortalDataReceived=true;
			Serial.println();
			Serial.println(F("Portal Received"));
		}

		// if the server has disconnected, stop the client
		if (!PortalClient.connected() && PortalConnection)
		{
			Serial.println(F("Portal Disconnected"));
			PortalConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
		}

		// if request timed out, stop the client
		if (PortalClient.connected() && PortalConnection && millis()-PortalTimeOut>PORTAL_TIMEOUT)
		{
			Serial.println(F("Portal Timeout"));
			PortalConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
		}
		
//		EthernetClient RelayClient = RelayServer.available();
//		if (RelayClient && RelayClient.connected()) {
//			RelayClient.find("\r\n\r\n"); // Consume incoming request
//
//			RelayClient.print("HTTP/1.1 200 OK\r\n");
//			RelayClient.print("Connection: close\r\n");
//			RelayClient.print("Content-Length: 5\r\n");
//			RelayClient.print("\r\n");
//			RelayClient.print("Hello");
//
//			delay(1); // Give the Web browser time to receive the data
//			RelayClient.stop();
//		}
		
//		// Relay server
//		if (!RelayClient.connected()) // Check for relay server closed connection
//		{
//			Serial.println("Relay Disconnected");
//			ConnectionRetry++;
//			RelayConnected=false;
//			RelayIndex=0;
//			RelayClient.stop(); // Make sure we free up the client
//			if (ConnectionRetry>=RETRY_COUNT) // Connection failed too many times
//			{
//				Serial.println("Reinitialzing");
//				Init();
//			}
//			else
//			{
//				delay(100);
//				Serial.println("Relay Connecting...");
//				RelayClient.noblockconnect(RelayServer, 80);
//			}
//		}
//		else
//		{
//			if (!RelayConnected)
//			{
//				Serial.print(".");
//				delay(100);
//				wdt_reset();
//				if (RelayClient.checkconnect()==0x17) // Check for connection established
//				{
//					Serial.println();
//					Serial.println("Relay Connected");
//					ConnectionRetry=0;
//					RelayConnected=true;
//					RelayClient.print("POST /");
//					RelayClient.print(uid);
//					RelayClient.println(" HTTP/1.1");
//					RelayClient.println("Upgrade: PTTH/1.0");
//					RelayClient.println("Connection: Upgrade");
//					RelayClient.println("Host: try.yaler.net");
//					RelayClient.println();
//				}
//			}
//		}
	}
}

void RA_Wiznet5100::ReceiveData()
{
	if (FoundIP)
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
	}
	
//	if (RelayClient.available())
//	{
//		wdt_reset();
////			if (RelayClient.available()>54) // length of the return header of the HTTP upgrade
////				RelayClient.find("\r\n\r\n"); // Discard header
////			else
////				RelayClient.read(); // Most likely HTTP/1.1 204, so we read one byte to cause timeout
////			RelayClient.find("\r\n\r\n"); // Discard header
//		Serial.print("Relay Data available: ");
//		Serial.println(RelayClient.available());
//		if (RelayClient.available()==17)
//		{
//			while(RelayClient.available()) Serial.write(RelayClient.read());
//			Serial.println("Relay Repost");
//			RelayClient.print("POST /");
//			RelayClient.print(uid);
//			RelayClient.println(" HTTP/1.1");
//			RelayClient.println("Upgrade: PTTH/1.0");
//			RelayClient.println("Connection: Upgrade");
//			RelayClient.println("Host: try.yaler.net");
//			RelayClient.println();
//		}
//		if (RelayClient.available()>0)
//		{
//			RelayClient.find("\r\n\r\n"); // Discard header
//			ProcessRelayEthernet();
//		}
//	}
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

	NetClient.stop();
	m_pushbackindex=0;
}

//void RA_Wiznet5100::ProcessRelayEthernet()
//{
//	bIncomingR=true;
//	timeoutR=millis();
//	Serial.println("Incoming...");
//	while (bIncomingR)
//	{
//		if (millis()-timeoutR>100)
//		{
//			Serial.println("Timeout");
//			bIncomingR=false;
//			RelayIndex=0;
//			RelayClient.stop();
//		}
//		if (RelayClient.available()>0)
//		{
//			if (RelayIndex++==5)
//			{
//				// Commented to allow for direct access subdomain instead of folder
////				for (int a=0;a<uid.length();a++)
////				{
////					RelayClient.read(); // Consume the unique id
////				}
////				if (RelayClient.peek()=='/') RelayClient.read(); // Consume the slash, we already have one
//			}
//			char c=RelayClient.read();
//			Serial.write(c);
//			PushBuffer(c);
//			timeoutR=millis();
//			wdt_reset();
//			if (reqtype>0 && reqtype<128)
//			{
//				bIncomingR=false;
//				while(RelayClient.available())
//				{
//					wdt_reset();
//					RelayClient.read();
//				}
//			}
//		}
//	}
////	Serial1.println();
////	Serial1.println(reqtype);
//	wdt_reset();
//	ProcessHTTP();
//	Serial.println("Done processing");
//	RelayIndex=0;
//	RelayClient.stop();
//	m_pushbackindex=0;
//}

//void RA_Wiznet5100::DirectAccess(String uniqueid)
//{
//	uid=uniqueid;
//}

void RA_Wiznet5100::PortalConnect()
{
	  PortalClient.noblockconnect(PortalServer, 80);
	  PortalTimeOut=millis();
}

boolean RA_Wiznet5100::IsPortalConnected()
{
	return PortalClient.checkconnect()==0x17;
}

boolean RA_Wiznet5100::IsMQTTConnected()
{
	return MQTTClient.connected();
}

void RA_Wiznet5100::Cloud()
{
	if (FoundIP)
	{
		char username[16];
		char password[16];
		strcpy_P(username, CLOUD_USERNAME); 
		strcpy_P(password, CLOUD_PASSWORD);
		MQTTClient.loop();
		if (millis()-MQTTReconnectmillis>5000)
		{
			if (!MQTTClient.connected())
			{
				char sub_buffer[sizeof(username)+6];
				MQTTReconnectmillis=millis();
				Serial.println(F("MQTT Connecting..."));
				wdt_reset();
				sprintf(sub_buffer, "RA-%s", username);
				if (MQTTClient.connect(sub_buffer,username,password))
				{
					sprintf(sub_buffer, "%s/in/#", username);
					Serial.println(F("MQTT succeeded"));
					MQTTClient.subscribe(sub_buffer);
					wdt_reset();
				}
				else
				{
					Serial.println(F("MQTT failed"));
					MQTTClient.disconnect();
				}
			}
		}
		
		if (millis()-MQTTSendmillis>1000 && MQTTClient.connected())
		{
			MQTTSendmillis=millis();
			for (byte a=0; a<NumParamByte;a++)
			{
				if (*ReefAngel.ParamArrayByte[a]!=ReefAngel.OldParamArrayByte[a])
				{
					char buffer[15];
					strcpy_P(buffer, (char*)pgm_read_word(&(param_items_byte[a]))); 
					sprintf(buffer, "%s:%d", buffer, *ReefAngel.ParamArrayByte[a]);
					CloudPublish(buffer);
					ReefAngel.OldParamArrayByte[a]=*ReefAngel.ParamArrayByte[a];
				}
			}
			for (byte a=0; a<NumParamInt;a++)
			{
				if (*ReefAngel.ParamArrayInt[a]!=ReefAngel.OldParamArrayInt[a])
				{
					char buffer[15];
					strcpy_P(buffer, (char*)pgm_read_word(&(param_items_int[a]))); 
					sprintf(buffer, "%s:%d", buffer, *ReefAngel.ParamArrayInt[a]);
					CloudPublish(buffer);
					ReefAngel.OldParamArrayInt[a]=*ReefAngel.ParamArrayInt[a];
				}
			}
		}		
	}
}

void RA_Wiznet5100::CloudPublish(char* message)
{
	if (MQTTClient.connected())
	{
		char username[16];
		strcpy_P(username, CLOUD_USERNAME); 
		char pub_buffer[sizeof(username)+5];
		sprintf(pub_buffer, "%s/out", username);
		MQTTClient.publish(pub_buffer,message);

	}
}

//size_t RA_Wiznet5100::write(uint8_t c) { if (RelayIndex) return RelayClient.write((uint8_t)c); else if (PortalConnection) return PortalClient.write((uint8_t)c); else return NetClient.write((uint8_t)c); }
size_t RA_Wiznet5100::write(uint8_t c) { if (PortalConnection) return PortalClient.write((uint8_t)c); else return NetClient.write((uint8_t)c); }
//size_t RA_Wiznet5100::write(unsigned long n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(long n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(unsigned int n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }
//size_t RA_Wiznet5100::write(int n) { if (RelayIndex) return RelayClient.write((uint8_t)n); else if (PortalConnection) return PortalClient.write((uint8_t)n); else return NetClient.write((uint8_t)n); }

#endif // ETH_WIZ5100



