#include <WiFiAlert.h>

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

void WiFiAlert::Send(char *message)
{
  if (IsAlert())
  {
    LastAlert=now();
    WiFiSendAlert(message);
  }
}

void WiFiAlert::WiFiSendAlert(char *message)
{
  Serial.print("GET /status/wifialert.aspx?id=");
  Serial.print(ReefAngel.portalusername);
  Serial.print("&key=");
  Serial.print(ReefAngel.portalkey);
  Serial.print("&msg=");
  Serial.println(message);
  Serial.println("\n\n");
}
