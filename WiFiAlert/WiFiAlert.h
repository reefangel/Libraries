#include <Globals.h>

#if defined wifi || defined ETH_WIZ5100
#ifndef __WIFIALERT_H__
#define __WIFIALERT_H__

#include <Time.h>
#include <DS1307RTC.h>
#include <ReefAngel.h>

class WiFiAlert
{
public:
  WiFiAlert();
  void Send(char *message, boolean force);
  inline void Send(char *message) { Send(message,false); }
  inline void SetDelay(int delay) { AlertDelay=delay; }
private:
  int AlertDelay;
  time_t LastAlert;
  boolean IsAlert();
  void WiFiSendAlert(char *message);
};

#endif  // __WIFIALERT_H__
#endif  // wifi || ETH_WIZ5100
