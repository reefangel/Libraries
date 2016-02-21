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

byte* ParamArrayByte[NumParamByte] = {&ReefAngel.Relay.RelayDataE[0],&ReefAngel.Relay.RelayMaskOffE[0],&ReefAngel.Relay.RelayMaskOnE[0],&ReefAngel.Relay.RelayDataE[1],&ReefAngel.Relay.RelayMaskOffE[1],&ReefAngel.Relay.RelayMaskOnE[1],&ReefAngel.Relay.RelayDataE[2],&ReefAngel.Relay.RelayMaskOffE[2],&ReefAngel.Relay.RelayMaskOnE[2],&ReefAngel.Relay.RelayDataE[3],&ReefAngel.Relay.RelayMaskOffE[3],&ReefAngel.Relay.RelayMaskOnE[3],&ReefAngel.Relay.RelayDataE[4],&ReefAngel.Relay.RelayMaskOffE[4],&ReefAngel.Relay.RelayMaskOnE[4],&ReefAngel.Relay.RelayDataE[5],&ReefAngel.Relay.RelayMaskOffE[5],&ReefAngel.Relay.RelayMaskOnE[5],&ReefAngel.Relay.RelayDataE[6],&ReefAngel.Relay.RelayMaskOffE[6],&ReefAngel.Relay.RelayMaskOnE[6],&ReefAngel.Relay.RelayDataE[7],&ReefAngel.Relay.RelayMaskOffE[7],&ReefAngel.Relay.RelayMaskOnE[7],&ReefAngel.LowATO.Status,&ReefAngel.HighATO.Status,&ReefAngel.AlarmInput.Status,&ReefAngel.EM,&ReefAngel.EM1,&ReefAngel.REM,&ReefAngel.Board,&ReefAngel.AlertFlags,&ReefAngel.StatusFlags,&ReefAngel.PWM.DaylightPercentage,&ReefAngel.PWM.ActinicPercentage,&ReefAngel.PWM.Daylight2Percentage,&ReefAngel.PWM.Actinic2Percentage,&ReefAngel.PWM.DaylightPWMOverride,&ReefAngel.PWM.ActinicPWMOverride,&ReefAngel.PWM.Daylight2PWMOverride,&ReefAngel.PWM.Actinic2PWMOverride,&ReefAngel.WaterLevel.level[0],&ReefAngel.WaterLevel.level[1],&ReefAngel.WaterLevel.level[2],&ReefAngel.WaterLevel.level[3],&ReefAngel.WaterLevel.level[4],&ReefAngel.Humidity.level,&ReefAngel.DCPump.Mode,&ReefAngel.DCPump.Speed,&ReefAngel.DCPump.Duration,&ReefAngel.DCPump.Threshold,&ReefAngel.PWM.ExpansionPercentage[0],&ReefAngel.PWM.ExpansionPercentage[1],&ReefAngel.PWM.ExpansionPercentage[2],&ReefAngel.PWM.ExpansionPercentage[3],&ReefAngel.PWM.ExpansionPercentage[4],&ReefAngel.PWM.ExpansionPercentage[5],&ReefAngel.PWM.ExpansionChannelOverride[0],&ReefAngel.PWM.ExpansionChannelOverride[1],&ReefAngel.PWM.ExpansionChannelOverride[2],&ReefAngel.PWM.ExpansionChannelOverride[3],&ReefAngel.PWM.ExpansionChannelOverride[4],&ReefAngel.PWM.ExpansionChannelOverride[5],&ReefAngel.AI.AIChannels[0],&ReefAngel.AI.AIChannels[1],&ReefAngel.AI.AIChannels[2],&ReefAngel.RF.Mode,&ReefAngel.RF.Speed,&ReefAngel.RF.Duration,&ReefAngel.RF.RadionChannels[0],&ReefAngel.RF.RadionChannels[1],&ReefAngel.RF.RadionChannels[2],&ReefAngel.RF.RadionChannels[3],&ReefAngel.RF.RadionChannels[4],&ReefAngel.RF.RadionChannels[5],&ReefAngel.RF.RadionChannelsOverride[0],&ReefAngel.RF.RadionChannelsOverride[1],&ReefAngel.RF.RadionChannelsOverride[2],&ReefAngel.RF.RadionChannelsOverride[3],&ReefAngel.RF.RadionChannelsOverride[4],&ReefAngel.RF.RadionChannelsOverride[5],&ReefAngel.IO.IOPorts,&ReefAngel.LeakValue,&ReefAngel.CustomVar[0],&ReefAngel.CustomVar[1],&ReefAngel.CustomVar[2],&ReefAngel.CustomVar[3],&ReefAngel.CustomVar[4],&ReefAngel.CustomVar[5],&ReefAngel.CustomVar[6],&ReefAngel.CustomVar[7]};
static PROGMEM const char *param_items_byte[] = {JSON_R1, JSON_ROFF1, JSON_RON1, JSON_R2, JSON_ROFF2, JSON_RON2, JSON_R3, JSON_ROFF3, JSON_RON3, JSON_R4, JSON_ROFF4, JSON_RON4, JSON_R5, JSON_ROFF5, JSON_RON5, JSON_R6, JSON_ROFF6, JSON_RON6, JSON_R7, JSON_ROFF7, JSON_RON7, JSON_R8, JSON_ROFF8, JSON_RON8, JSON_ATOLOW, JSON_ATOHIGH, JSON_ALARM, JSON_EM, JSON_EM1, JSON_REM, JSON_BOARDID, JSON_ALERTFLAG, JSON_STATUSFLAG, JSON_PWMD, JSON_PWMA, JSON_PWMD2, JSON_PWMA2, JSON_PWMDO, JSON_PWMAO, JSON_PWMD2O, JSON_PWMA2O, JSON_WL, JSON_WL1, JSON_WL2, JSON_WL3, JSON_WL4, JSON_HUM, JSON_DCM, JSON_DCS, JSON_DCD, JSON_DCT, JSON_PWME0, JSON_PWME1, JSON_PWME2, JSON_PWME3, JSON_PWME4, JSON_PWME5, JSON_PWME0O, JSON_PWME1O, JSON_PWME2O, JSON_PWME3O, JSON_PWME4O, JSON_PWME5O, JSON_AIW, JSON_AIB, JSON_AIRB, JSON_RFM, JSON_RFS, JSON_RFD, JSON_RFW, JSON_RFRB, JSON_RFR, JSON_RFG, JSON_RFB, JSON_RFI, JSON_RFWO, JSON_RFRBO, JSON_RFRO, JSON_RFGO, JSON_RFBO, JSON_RFIO, JSON_IO, JSON_LEAK, JSON_C0, JSON_C1, JSON_C2, JSON_C3, JSON_C4, JSON_C5, JSON_C6, JSON_C7};
int* ParamArrayInt[NumParamInt] = {&ReefAngel.Params.Temp[T1_PROBE],&ReefAngel.Params.Temp[T2_PROBE],&ReefAngel.Params.Temp[T3_PROBE],&ReefAngel.Params.PH,&ReefAngel.Params.ORP,&ReefAngel.Params.Salinity,&ReefAngel.Params.PHExp,&ReefAngel.PAR.level,&ReefAngel.CustomExpansionValue[0],&ReefAngel.CustomExpansionValue[1],&ReefAngel.CustomExpansionValue[2],&ReefAngel.CustomExpansionValue[3],&ReefAngel.CustomExpansionValue[4],&ReefAngel.CustomExpansionValue[5],&ReefAngel.CustomExpansionValue[6],&ReefAngel.CustomExpansionValue[7]};
static PROGMEM const char *param_items_int[] = {JSON_T1, JSON_T2, JSON_T3, JSON_PH, JSON_ORP, JSON_SAL, JSON_PHEXP, JSON_PAR, JSON_CEXP0, JSON_CEXP1, JSON_CEXP2, JSON_CEXP3, JSON_CEXP4, JSON_CEXP5, JSON_CEXP6, JSON_CEXP7};

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
				if (*ParamArrayByte[a]!=OldParamArrayByte[a])
				{
					char buffer[15];
					strcpy_P(buffer, (char*)pgm_read_word(&(param_items_byte[a]))); 
					sprintf(buffer, "%s:%d", buffer, *ParamArrayByte[a]);
					CloudPublish(buffer);
					OldParamArrayByte[a]=*ParamArrayByte[a];
				}
			}
			for (byte a=0; a<NumParamInt;a++)
			{
				if (*ParamArrayInt[a]!=OldParamArrayInt[a])
				{
					char buffer[15];
					strcpy_P(buffer, (char*)pgm_read_word(&(param_items_int[a]))); 
					sprintf(buffer, "%s:%d", buffer, *ParamArrayInt[a]);
					CloudPublish(buffer);
					OldParamArrayInt[a]=*ParamArrayInt[a];
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



