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
  inline void Send() { Send(AlertMsg); };
  inline void Send(char *message) { Send(message,false); }
  void Send(char *message, boolean force);

  void Send(const __FlashStringHelper* message);
  void Send(const __FlashStringHelper* message, boolean force);
  char* getString(const __FlashStringHelper* str);

  inline void SetDelay(int delay) { AlertDelay=delay; }
  inline int GetDelay() { return AlertDelay; }
  inline void ResetAlert() { LastAlert=now(); }
  inline void TriggerAlert() { LastAlert=now()-AlertDelay; }
  inline time_t GetLastAlert() { return LastAlert; }
  inline boolean IsActive() { return AlertActive; }
  inline char *GetMessage() { return AlertMsg; }
  boolean IsAlert();
private:
  int AlertDelay;
  int AlertActive;
  char *AlertMsg;
  time_t LastAlert;
  EthernetClient AlertClient;

  void WiFiSendAlert();
};

#endif  // __WIFIALERT_H__
#endif  // wifi || ETH_WIZ5100
