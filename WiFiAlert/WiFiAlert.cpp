#include <Globals.h>

#if defined wifi || defined ETH_WIZ5100
#include "WiFiAlert.h"

WiFiAlert::WiFiAlert()
{
  AlertDelay=900;
  LastAlert=0;
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
  if (IsAlert() || force)
  {
    LastAlert=now();
    WiFiSendAlert(message);
  }
}

void WiFiAlert::WiFiSendAlert(char *message)
{
  Serial.print("GET /status/wifialert.aspx?id=");
  Serial.print(ReefAngel.Network.portalusername);
  Serial.print("&key=");
  Serial.print(ReefAngel.Network.portalkey);
  Serial.print("&msg=");
  Serial.println(message);
  Serial.println("\n\n");
}
#endif  // wifi || ETH_WIZ5100
