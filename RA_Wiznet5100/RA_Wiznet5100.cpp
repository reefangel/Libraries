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
	downloading=false;
}

void RA_Wiznet5100::Init()
{
	NetMac[5] = InternalMemory.read(StarMac);
	if (NetMac[5]==0xff)
	{
		byte tempmac = random(0xff);
		InternalMemory.write(StarMac,tempmac);
		NetMac[5] = tempmac;
	}
	EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
	NetServer.begin();
	FoundIP=false;
	PortalConnection=false;
	PortalWaiting=false;
	PortalDataReceived=false;
	FirmwareConnection=false;
	FirmwareWaiting=false;
	MQTTReconnectmillis=millis();
	MQTTSendmillis=millis();
	downloadsize=0;
	sd_index=0;
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
		if (PortalClient.available() && (PortalConnection || FirmwareConnection))
		{
		    boolean newline = false;
		    String headerline="";

		    PortalTimeOut=millis();
		    Serial.println(F("Receiving..."));
			while(PortalClient.available())
			{
				wdt_reset();
				if (payload_ready)
				{
					if (PortalClient.available()>32)
					{
						for (int a=0;a<32;a++)
							sd_buffer[a]=PortalClient.read();
						firwareFile.write(sd_buffer,32);
						downloadsize+=32;
						sd_index++;
						if (sd_index==32)
						{
							sd_index=0;
							ReefAngel.Timer[PORTAL_TIMER].Start();  // start timer
							PortalTimeOut=millis();
							ReefAngel.Font.DrawTextP(38,9,DOWNLOADING);
							ReefAngel.Font.DrawText((downloadsize*100)/lheader);
							ReefAngel.Font.DrawText("%");
						}
					}
					else
					{
						char c = PortalClient.read();
						downloadsize++;
						firwareFile.write(c);
					}
				}
				else
				{
					char c = PortalClient.read();
					downloadsize++;
					headerline+=c;
					Serial.write(c);
					if (c == '\n')
					{
						byte sheader = headerline.indexOf("Length");
						if (sheader==8)
							lheader=headerline.substring(sheader+8).toInt();
						headerline="";
						newline = true;
						c = PortalClient.read();
						headerline+=c;
						Serial.write(c);
						if (c == '\r' && newline)
						{
							c = PortalClient.read();
							Serial.write(c);
							if (FirmwareConnection)
							{
								payload_ready = true;
								if (lheader>0) downloading = true;
							}
							downloadsize=0;
						}
						else
						{
							newline = false;
						}
					}
				}
			}
			Serial.println(downloadsize);
			if (PortalConnection) PortalDataReceived=true;
			Serial.println(F("Received"));
		}

		// if the server has disconnected, stop the client
		if (!PortalClient.connected() && PortalConnection)
		{
			Serial.println(F("Disconnected"));
			PortalConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
			FirmwareConnection=true;
			PortalWaiting=false;
			FirmwareWaiting=false;
			downloadsize=0;
			lheader=0;
		    payload_ready = false;
			delay(100);
			FirmwareConnect();
			Serial.println(F("Connecting..."));
		}
		
		if (!PortalClient.connected() && FirmwareConnection)
		{
			Serial.print(F("Data: "));
			Serial.println(downloadsize);
			Serial.print(F("Header: "));
			Serial.println(lheader);
			Serial.println(F("Disconnected"));
			FirmwareConnection=false;
			PortalWaiting=false;
			FirmwareWaiting=false;
			PortalClient.stop();
		    payload_ready = false;
			if (firwareFile) firwareFile.close();
			if (lheader==downloadsize && downloadsize>600)
			{
				if (firwareFile) firwareFile.close();
				Serial.println(F("Updating..."));
				InternalMemory.write(RemoteFirmware, 0xf0);
				while(1);
			}
			else
			{
				if (SD.exists("FIRMWARE.BIN")) SD.remove("FIRMWARE.BIN");
			}
			downloadsize=0;
			lheader=0;
		}

		// if request timed out, stop the client
		if (PortalClient.connected() && (PortalConnection || FirmwareConnection) && millis()-PortalTimeOut>PORTAL_TIMEOUT)
		{
			Serial.println(F("Portal Timeout"));
			Serial.println(downloadsize);
			PortalConnection=false;
			FirmwareConnection=false;
			PortalClient.stop();
			if (!PortalDataReceived) Init();
			PortalDataReceived=false;
			PortalWaiting=false;
			FirmwareWaiting=false;
			downloadsize=0;
			lheader=0;
		    payload_ready = false;
			if (firwareFile) firwareFile.close();
			if (SD.exists("FIRMWARE.BIN")) SD.remove("FIRMWARE.BIN");
		}
		if (IsPortalConnected() && FirmwareConnection && !FirmwareWaiting) // Check for connection established
		{
		    payload_ready = false;
			lheader=0;
			FirmwareWaiting=true;
			firwareFile = SD.open("FIRMWARE.BIN", O_WRITE | O_CREAT | O_TRUNC);  // change file name to write to here
		    if (!firwareFile) {
		      Serial.println(F("Could not create file"));
		    }
			
			Serial.println(F("Connected"));
			PortalClient.print("GET /getcodebin.php?u=");
			PortalClient.print(CLOUD_USERNAME);
			PortalClient.println(" HTTP/1.1");
			PortalClient.println("Host: webwizard.reefangel.com");
			PortalClient.println("Connection: close");
			PortalClient.println();
		}
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

void RA_Wiznet5100::PortalConnect()
{
	  PortalClient.noblockconnect(PortalServer, 80);
	  PortalTimeOut=millis();
}

void RA_Wiznet5100::FirmwareConnect()
{
	  PortalClient.noblockconnect(WebWizardServer, 80);
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
	if (!payload_ready)
	{
		if (FoundIP)
		{
			Portal(CLOUD_USERNAME);
			MQTTClient.loop();
			if (millis()-MQTTReconnectmillis>5000)
			{
				if (!MQTTClient.connected())
				{
					char sub_buffer[sizeof(CLOUD_USERNAME)+6];
					MQTTReconnectmillis=millis();
					Serial.println(F("MQTT Connecting..."));
					wdt_reset();
					sprintf(sub_buffer, "RA-%s", CLOUD_USERNAME);
					if (MQTTClient.connect(sub_buffer,CLOUD_USERNAME,CLOUD_PASSWORD))
					{
						sprintf(sub_buffer, "%s/in/#", CLOUD_USERNAME);
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
}

void RA_Wiznet5100::CloudPublish(char* message)
{
	if (MQTTClient.connected())
	{
		char pub_buffer[sizeof(CLOUD_USERNAME)+5];
		sprintf(pub_buffer, "%s/out", CLOUD_USERNAME);
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



