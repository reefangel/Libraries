/*
 * RA_Wiznet5100.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Benjamin
 */

#ifndef RA_WIZNET5100_H_
#define RA_WIZNET5100_H_

#ifdef ETH_WIZ5100
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetDHCP.h>
#include <RA_Wifi.h>
#include <avr/wdt.h>
#include <PubSubClient.h>
#include <RA_CustomSettings.h>

static EthernetServer NetServer(STARPORT);
static byte NetMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static IPAddress NetIP(192,168,1,200);

static EthernetClient NetClient;
static EthernetClient PortalClient;
static EthernetClient ethClient;
static PubSubClient MQTTClient(MQTTServer, MQTTPORT, MQTTSubCallback, ethClient);
static boolean PortalWaiting;
static boolean FirmwareWaiting;
#define PORTAL_TIMEOUT  10000
#define RETRY_COUNT  3

class RA_Wiznet5100 : public RA_Wifi
{
public:
	RA_Wiznet5100();
	void Init();
	void ReceiveData();
	void ProcessEthernet();
	void Update();
	void Cloud();
	void CloudPublish(char* message);
	boolean PortalConnection;
	boolean PortalDataReceived;
	unsigned long PortalTimeOut;
	boolean FoundIP;
	void PortalConnect();
	void FirmwareConnect();
	boolean FirmwareConnection;
	boolean IsPortalConnected();
	boolean IsMQTTConnected();
	unsigned long downloadsize;
	boolean payload_ready;
	unsigned long lheader;
	boolean downloading;

private:
	unsigned long MQTTReconnectmillis;
	unsigned long MQTTSendmillis;
	File firwareFile;
protected:
	size_t write(uint8_t c);
	size_t write(unsigned long n);
	size_t write(long n);
	size_t write(unsigned int n);
	size_t write(int n);
};

#endif  // ETH_WIZ5100
#endif /* RA_WIZNET5100_H_ */

