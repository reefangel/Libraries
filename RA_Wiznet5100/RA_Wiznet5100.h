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
#include <YalerEthernetServer.h>
#include <RA_Wifi.h>
#include <avr/wdt.h>
#include <PubSubClient.h>

static EthernetServer NetServer(2000);
static byte NetMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static byte MQTTServer[] = { 85, 119, 83, 194 }; // test.mosquitto.org
//static byte MQTTServer[] = { 212, 72, 74, 21 }; // broker.mqttdashboard.com
//static byte MQTTServer[] = { 10, 1, 10, 130 }; // local test
static IPAddress NetIP(192,168,1,200);
//const IPAddress PortalServer(198,171,134,6); // www
//const IPAddress PortalServer(192,168,9,140); // local
const IPAddress PortalServer(69,198,171,165); // forum
//const IPAddress RelayServer(46,137,106,125); // try.yaler.net
//const IPAddress RelayServer(10,1,10,130); // local test
//static YalerEthernetServer RelayServer("try.yaler.net", 80, "reefangel");

static EthernetClient NetClient;
//static EthernetClient RelayClient;
static EthernetClient PortalClient;
static EthernetClient ethClient;
static PubSubClient MQTTClient(MQTTServer, 1883, callback, ethClient);
//static boolean RelayConnected;
//static int RelayIndex;
static boolean PortalWaiting;
#define PORTAL_TIMEOUT  10000
#define RETRY_COUNT  3
#define NumParamByte	70
#define NumParamInt		7

class RA_Wiznet5100 : public RA_Wifi
{
public:
	RA_Wiznet5100();
	void Init();
	void ReceiveData();
	void ProcessEthernet();
//	void ProcessRelayEthernet();
//	void DirectAccess(String uniqueid);
	void Update();
	boolean PortalConnection;
	boolean PortalDataReceived;
	unsigned long PortalTimeOut;
	boolean FoundIP;
	void PortalConnect();
	boolean IsPortalConnected();
	boolean IsMQTTConnected();

private:
//	boolean bIncomingR;
//	unsigned long timeoutR;
//	byte ConnectionRetry;
//	String uid;
	unsigned long MQTTReconnectmillis;
	unsigned long MQTTSendmillis;
	byte OldParamArrayByte[NumParamByte];
	int OldParamArrayInt[NumParamInt];
protected:
	size_t write(uint8_t c);
	size_t write(unsigned long n);
	size_t write(long n);
	size_t write(unsigned int n);
	size_t write(int n);
};
#endif  // ETH_WIZ5100
#endif /* RA_WIZNET5100_H_ */

