#ifndef __WIFIALERT_H__
#define __WIFIALERT_H__

#include <Time.h>
#include <DS1307RTC.h>
#include <ReefAngel.h>

class WiFiAlert
{
public:
  WiFiAlert();
  void Send(char *message);
  inline void SetDelay(int delay) { AlertDelay=delay; }
private:
  int AlertDelay;
  time_t LastAlert;
  boolean IsAlert();
  void WiFiSendAlert(char *message);  
};

#endif __WIFIALERT_H__
