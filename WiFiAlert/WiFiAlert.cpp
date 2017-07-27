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
  ReefAngel.Network.SendAlert(ReefAngel.Network.portalusername, ReefAngel.Network.portalkey, message);
}
#endif  // wifi || ETH_WIZ5100
