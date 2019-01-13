#include <Globals.h>

#if defined wifi || defined ETH_WIZ5100
#include "WiFiAlert.h"

WiFiAlert::WiFiAlert()
{
  AlertDelay=900;
  LastAlert=0;
  AlertActive=false;
  AlertMsg="";
}

boolean WiFiAlert::IsAlert()
{
  if (now()-LastAlert >= AlertDelay)
  {
    return true;
  }
  return false;
}

void WiFiAlert::Send(const __FlashStringHelper* message) 
{
	Send(message,false);	
}

void WiFiAlert::Send(const __FlashStringHelper* message, boolean force) 
{
	Send(getString(message),force);
}

char* WiFiAlert::getString(const __FlashStringHelper* str)
{
	char stringBuffer[50];
	strcpy_P(stringBuffer, (char*)str);
	return stringBuffer;
}

void WiFiAlert::Send(char *message, boolean force)
{
  static boolean connected = false;
  static boolean messageSent = false;

  if (IsAlert() || force)
  {
    LastAlert=now();
    AlertActive=true;
	AlertMsg=message;

  	Serial.print("GET /status/wifialert.aspx?id=");
	Serial.print(ReefAngel.Network.portalusername);
	Serial.print("&key=");
	Serial.print(ReefAngel.Network.portalkey);
	Serial.print("&msg=");
	Serial.println(AlertMsg);
	Serial.println("\n\n");
  }

#ifdef ETH_WIZ5100
  if (AlertActive) 
  {
//	Serial.println(AlertMsg);
	if (millis()%15000<500 && !connected)
	{
		Serial.println("connecting...");
		AlertClient.noblockconnect(PortalServer, 80);
		connected=true;
	}

	if (AlertClient.checkconnect()==0x17 && !messageSent)
	{
		Serial.println("connected");
		AlertClient.print("GET /status/wifialert.aspx?id=");
		AlertClient.print(ReefAngel.Network.portalusername);
		AlertClient.print("&key=");
		AlertClient.print(ReefAngel.Network.portalkey);
		AlertClient.print("&msg=");
		AlertClient.println(AlertMsg);
		AlertClient.println("\n\n");
		messageSent=true;
	}
				
	while (AlertClient.available()) {
		char c = AlertClient.read();
		Serial.print(c);
		wdt_reset();
	}

	if (!AlertClient.connected() && connected) {
		Serial.println();
		Serial.println("disconnecting.");
		AlertClient.stop();
		connected=false;
		messageSent=false;
		AlertActive=false;
	}
  }
#else
  AlertActive=false;
#endif // ETH_WIZ5100
}

#endif  // wifi || ETH_WIZ5100

