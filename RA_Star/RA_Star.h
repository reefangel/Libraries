/*
 * RA_Star.h
 *
 *  Created on: Sep 8, 2013
 *      Author: Benjamin
 */

#ifndef RA_STAR_H_
#define RA_STAR_H_
#include <RA_Wifi.h>
#include <avr/wdt.h>

class RA_Star : public RA_Wifi
{
  public:
    RA_Star();
    void ReceiveData();
    void ProcessEthernet();

  protected:
#ifdef RA_STAR
    inline size_t write(uint8_t c) { return NetClient->write((uint8_t)c); }
    inline size_t write(unsigned long n) { return NetClient->write((uint8_t)n); }
    inline size_t write(long n) { return NetClient->write((uint8_t)n); }
    inline size_t write(unsigned int n) { return NetClient->write((uint8_t)n); }
    inline size_t write(int n) { return NetClient->write((uint8_t)n); }
#endif
};

#endif /* RA_STAR_H_ */
