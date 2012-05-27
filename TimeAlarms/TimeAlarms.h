
#ifndef TimeAlarms_h
#define TimeAlarms_h

#include <inttypes.h>

#include "Time.h"

#define dtNBR_ALARMS 16

typedef struct  {
    uint8_t isAllocated            :1 ;  // the alarm is avialable for allocation if false
    uint8_t isEnabled              :1 ;  // the timer is only actioned if isEnabled is true 
    uint8_t isOneShot              :1 ;  // the timer will be de-allocated after trigger is processed 
    uint8_t isAlarm                :1 ;  // time of day alarm if true, period timer if false  
 }
    AlarmMode_t   ;

typedef uint8_t AlarmID_t;
typedef AlarmID_t AlarmId;  // Arduino friendly name
#define dtINVALID_ALARM_ID 255


class AlarmClass;  // forward reference
typedef void (*OnTick_t)(uint8_t);  // alarm callback function typedef 

// class defining an alarm instance, only used by dtAlarmsClass
class AlarmClass
{  
private:
public:
  AlarmClass();
  OnTick_t onTickHandler;
  void updateNextTrigger();
  time_t value;
  time_t nextTrigger;
  AlarmMode_t Mode;
  time_t repeatvalue;
  uint8_t repeatcount;
  uint8_t relayid;
};

// class containing the collection of alarms
class TimeAlarmsClass
{
private:
   AlarmClass Alarm[dtNBR_ALARMS];
   void serviceAlarms();
   uint8_t isServicing;
   AlarmID_t create( time_t value, time_t repeatvalue, uint8_t relayid, OnTick_t onTickHandler, uint8_t isAlarm, uint8_t isOneShot, uint8_t isEnabled=true);
   
public:
  TimeAlarmsClass();
  // functions to create alarms and timers
  AlarmID_t alarmRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid); // as above, with hms arguments
  AlarmID_t alarmRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid, time_t repeatvalue ); // as above, with hms arguments
  
  AlarmID_t timerOnce(time_t value, OnTick_t onTickHandler, uint8_t relayid);   // trigger once after the given number of seconds 
  AlarmID_t timerRepeat(time_t value, OnTick_t onTickHandler, uint8_t relayid); // trigger after the given number of seconds continuously
  
  void delay(unsigned long ms);
   
  // low level methods
  void clear();
  void enable(AlarmID_t ID);
  void disable(AlarmID_t ID);
//  void write(AlarmID_t ID, time_t value);    // write the value (and enable) the alarm with the given ID
//  time_t read(AlarmID_t ID);                 // return the value for the given timer 
  void StartUp();
};

extern TimeAlarmsClass Alarm;  // make an instance for the user

/*==============================================================================
 * MACROS
 *============================================================================*/

#define AlarmHMS(_hr_, _min_, _sec_) (_hr_ * SECS_PER_HOUR + _min_ * SECS_PER_MIN + _sec_)

#endif /* TimeAlarms_h */

