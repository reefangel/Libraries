//  Copyright (C) 2010 Georg Kaindl
//  http://gkaindl.com
//
//  This file is part of Arduino EthernetDNS.
//
//  EthernetDNS is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as
//  published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EthernetDNS is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EthernetDNS. If not, see
//  <http://www.gnu.org/licenses/>.
//

#if !defined(__ETHERNET_DNS_H__)
#define __ETHERNET_DNS_H__ 1

extern "C" {
   #include <inttypes.h>
}

typedef uint8_t byte;

typedef enum _DNSState_t {
   DNSStateIdle,
   DNSStateQuerySent
} DNSState_t;

typedef enum _DNSError_t {
   DNSTryLater = 3,
   DNSNothingToDo = 2,
   DNSSuccess  = 1,
   DNSInvalidArgument = -1,
   DNSOutOfMemory = -2,
   DNSSocketError = -3,
   DNSAlreadyProcessingQuery = -4,
   DNSNotFound = -5,
   DNSServerError = -6,
   DNSTimedOut = -7
} DNSError_t;

typedef DNSError_t DNSError;

typedef struct _DNSDataInternal_t {
   uint8_t  serverIpAddr[4];
   uint32_t xid;
   uint32_t lastQueryFirstXid;
} DNSDataInternal_t;

class EthernetDNSClass
{
private:
   DNSDataInternal_t    _dnsData;
   int                  _socket;
   DNSState_t           _state;
   unsigned long        _lastSendMillis;
   
   DNSError_t _sendDNSQueryPacket(const char* hostName);
   
   int _startDNSSession();
   int _closeDNSSession();
public:
   EthernetDNSClass();
   ~EthernetDNSClass();
   
   void setDNSServer(const byte dnsServerIpAddr[4]);
   
   DNSError_t resolveHostName(const char* hostName, byte ipAddr[4]);
   
   DNSError_t sendDNSQuery(const char* hostName);
   DNSError_t pollDNSReply(byte ipAddr[4]);
};

extern EthernetDNSClass EthernetDNS;

#endif // __ETHERNET_DNS_H__
