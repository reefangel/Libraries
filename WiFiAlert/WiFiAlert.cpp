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

void WiFiAlert::Send(char *message, boolean force)
{
  if (force) { 
	TriggerAlert();
  }

  if (IsAlert())
  {
    LastAlert=now();
    AlertActive=true;
    WiFiSendAlert(message);
  }

  Send();
}

void WiFiAlert::WiFiSendAlert(char *message)
{
  AlertMsg=message;
  Serial.print("GET /status/wifialert.aspx?id=");
  Serial.print(ReefAngel.Network.portalusername);
  Serial.print("&key=");
  Serial.print(ReefAngel.Network.portalkey);
  Serial.print("&msg=");
  Serial.println(AlertMsg);
  Serial.println("\n\n");
}

void WiFiAlert::Send() {
#ifdef ETH_WIZ5100
	static boolean alert = false;
	static boolean alertConnection = false;

	if (AlertActive) 
	{
		if (millis()%15000<500)
		{
			Serial.println("Sending Alert...");

			if (!alert) 
			{
				Serial.println("connecting...");
				AlertClient.noblockconnect(PortalServer, 80);
				alert=true;
			}
		}

		if (AlertClient.checkconnect()==0x17 && !alertConnection)
		{
			Serial.println("connected");
			AlertClient.print("GET /status/wifialert.aspx?id=");
			AlertClient.print(ReefAngel.Network.portalusername);
			AlertClient.print("&key=");
			AlertClient.print(ReefAngel.Network.portalkey);
			AlertClient.print("&msg=");
			AlertClient.println(AlertMsg);
			AlertClient.println("\n\n");
			alertConnection=true;
		}
				
		while (AlertClient.available()) {
			char c = AlertClient.read();
			Serial.print(c);
			wdt_reset();
		}

		if (!AlertClient.connected() && alert) {
			Serial.println();
			Serial.println("disconnecting.");
			AlertClient.stop();
			alert=false;
			alertConnection=false;
			AlertActive=false;
		}
	}
#else
	AlertActive=false;
#endif // ETH_WIZ5100
}

#endif  // wifi || ETH_WIZ5100
