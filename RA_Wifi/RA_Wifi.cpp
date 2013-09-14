/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#include <Globals.h>
#if defined wifi || defined RA_STAR
#include "RA_Wifi.h"
#include <DS1307RTC.h>
#include <ReefAngel.h>

RA_Wifi::RA_Wifi()
{
#if !defined RA_STAR
  _wifiSerial = &WIFI_SERIAL;
  _wifiSerial->begin(57600);
#endif  // wifi

  m_pushbackindex=0;
  reqtype=0;
  bIncoming=false;
  auth=false;
  weboption=0;
  weboption2=-1;
  weboption3=-1;
  bHasSecondValue = false;
//  bHasComma = false;
  bCommaCount = 0;
  webnegoption=false;
  portalusername="";
}

void RA_Wifi::WebResponse (const prog_char *response, long strsize)
{
//  P(WebHeaderMsg) = SERVER_HEADER_HTML;
//  printP(WebHeaderMsg);
	PrintHeader(strsize,0);
	PROGMEMprint(response);
}

void RA_Wifi::ModeResponse(bool fOk)
{
	uint8_t s;
	if ( fOk )
		s = 15;
	else
		s = 16;
	PrintHeader(s,1);
	PROGMEMprint(XML_MODE_OPEN);
	if ( fOk )
		PROGMEMprint(XML_OK);
	else
		PROGMEMprint(XML_ERR);
	PROGMEMprint(XML_MODE_CLOSE);
}

void RA_Wifi::PushBuffer(byte inStr)
{
	m_pushback[m_pushbackindex]=inStr;
	m_pushback[m_pushbackindex+1]=0;
	if (reqtype>0 && reqtype<128)
	{
		if (authStr[m_pushbackindex]==inStr) m_pushbackindex++; else m_pushbackindex=0;
		if (authStr[m_pushbackindex]==0) auth=true;
		//if (m_pushbackindex>0) Serial.println(m_pushbackindex,DEC);
		//if (m_pushbackindex>0) Serial.println(test,DEC);
	}
	else
	{
		m_pushbackindex++;
		if (m_pushbackindex==32) m_pushbackindex=0;
		if (reqtype>128)
		{
		    if (inStr==' ')
		    {
		        reqtype=256-reqtype;
		       if ( (reqtype == REQ_M_BYTE) || (reqtype == REQ_M_INT) || (reqtype == REQ_M_RAW || reqtype == REQ_OVERRIDE || reqtype == REQ_M_CVAR) )
		        {
		        	// must have a comma to have second value
		        	// verify that the last char was a digit
		        	if ( isdigit(m_pushback[m_pushbackindex-2]) )
		        	{
		        		// check for the comma to determine how we proceed
		        		if ( bCommaCount )
		        		{
		        			bHasSecondValue = true;
		        		}
		        		else
		        		{
		        			bHasSecondValue = false;
		        			weboption2 = weboption;
		        		}
		        	}
		        }
		        if ( reqtype == REQ_DATE )
		        {
		        	// last char must be a digit
					if ( isdigit(m_pushback[m_pushbackindex-2]) )
					{
						// comma count must be 2 otherwise it's an error
						// if not, set weboption to -1 to signify an error
						if ( bCommaCount != 2 )
						{
							weboption = -1;
						}
					}
					else
					{
						// last digit not a char and no commas means we need to
						// send the current date/time of the controller
						if ( bCommaCount == 0 )
						{
							weboption = -2;
						}
						else
						{
							weboption = -1;
						}
					}
		        }
		    }
		    else if (inStr == ',')
		    {
		    	// when we hit a comma, copy the first value (weboption) to weboption2
		    	// then skip over the comma and put the value to be written into weboption
		    	// second comma copies the value into weboption3
				bCommaCount++;
				if ( bCommaCount == 1 )
					weboption2 = weboption;
				else if ( bCommaCount == 2 )
					weboption3 = weboption;
		    	// reset weboption to 0
		    	weboption = 0;
		    }
		    else if (inStr == '-')
		    {
		    	webnegoption=true;
		    }
		    else if(isdigit(inStr))
		    {
		    	// process digits here
				weboption*=10;
				weboption+=inStr-'0';
		    }
		    // 3/14/11 - curt
		    //else all other chars are discarded
		    // consider further sanity checks to ensure that we don't get any malformed strings or commands
		    // right now, we can embed non digits in the parameters and as long as the last char is a digit,
		    // it is ok and the non digits are just skipped over
		    // we may want to signal an error or break out of processing the string to indicate there is an error
		    // maybe set weboption2 and weboption to -1 or 0
		    // could also flush the buffer and set reqtype to a REQ_ERROR or something
		    // need to give this more thought
		    //
		    // NOTES about too large of value being stored
		    //   if you exceed the storage limit for the variable, the negative value gets stored
		    //   so we should limit the value being saved from the client side
		    //   otherwise we would have to do additional checks in here for the size and that would
		    //   require more code
		    //   Users shouldn't be manually changing values from the web without an interface that
		    //   limits them, so we "should" be safe (in theory), but this may need to be revisited
		    //   in the future.  - curt (3/14/11)
		}
		else
		{
            if (strncmp("GET / ", m_pushback, 6)==0) reqtype = REQ_ROOT;
            else if (strncmp("GET /wifi", m_pushback, 9)==0) reqtype = REQ_WIFI;
            else if (strncmp("GET /r", m_pushback, 6)==0) reqtype = -REQ_RELAY;
            else if (strncmp("GET /mb", m_pushback, 7)==0) { reqtype = -REQ_M_BYTE; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
            else if (strncmp("GET /mi", m_pushback, 7)==0) { reqtype = -REQ_M_INT; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
//            else if (strncmp("GET /ma", m_pushback, 7)==0) reqtype = -REQ_M_ALL;
            else if (strncmp("GET /mr", m_pushback, 7)==0) { reqtype = -REQ_M_RAW; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
            else if (strncmp("GET /v", m_pushback, 6)==0) reqtype = -REQ_VERSION;
            else if (strncmp("GET /d", m_pushback, 6)==0) { reqtype = -REQ_DATE; weboption2 = -1; weboption3 = -1; bCommaCount = 0; }
            else if (strncmp("HTTP/1.", m_pushback, 7)==0) reqtype = -REQ_HTTP;
            else if (strncmp("GET /sr", m_pushback, 7)==0) reqtype = -REQ_R_STATUS;
            else if (strncmp("GET /sa", m_pushback, 7)==0) reqtype = -REQ_RA_STATUS;
            else if (strncmp("GET /bp", m_pushback, 7)==0) reqtype = -REQ_BTN_PRESS;
            else if (strncmp("GET /mf", m_pushback, 7)==0) reqtype = -REQ_FEEDING;
            else if (strncmp("GET /mw", m_pushback, 7)==0) reqtype = -REQ_WATER;
//            else if (strncmp("GET /cr", m_pushback, 7)==0) reqtype = -REQ_CAL_RELOAD;
            else if (strncmp("GET /mt", m_pushback, 7)==0) reqtype = -REQ_ALARM_ATO;
            else if (strncmp("GET /mo", m_pushback, 7)==0) reqtype = -REQ_ALARM_OVERHEAT;
            else if (strncmp("GET /ml", m_pushback, 7)==0) reqtype = -REQ_ALARM_LEAK;
            else if (strncmp("GET /l0", m_pushback, 7)==0) reqtype = -REQ_LIGHTSOFF;
            else if (strncmp("GET /l1", m_pushback, 7)==0) reqtype = -REQ_LIGHTSON;
            else if (strncmp("GET /boot", m_pushback, 9)==0) reqtype = REQ_REBOOT;
            else if (strncmp("GET /po", m_pushback, 7)==0) { reqtype = -REQ_OVERRIDE; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
#ifdef CUSTOM_VARIABLES
            else if (strncmp("GET /cvar", m_pushback, 9)==0) { reqtype = -REQ_M_CVAR; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
#endif
            //else reqtype = -REQ_UNKNOWN;
		}
	}
}

void RA_Wifi::ProcessHTTP()
{
  if (webnegoption) weboption*=-1;
	switch ( reqtype )
	{
		case REQ_ROOT:
		{
			WebResponse(SERVER_DEFAULT, sizeof(SERVER_DEFAULT) - 1);
			break;
		}  // REQ_ROOT
		case REQ_WIFI:
		{
			WebResponse(SERVER_RA, sizeof(SERVER_RA) - 1);
			break;
		}  // REQ_WIFI
		case REQ_RELAY:
		{
			if (weboption<10) break;
			byte o_relay=weboption/10;
			byte o_type=weboption%10;
			if (o_type==0)  // Turn port off
			{
				if ( o_relay < 9 )
				{
					bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
					bitClear(ReefAngel.Relay.RelayMaskOff,o_relay-1);
				}
#ifdef RelayExp
				if ( (o_relay > 10) && (o_relay < 89) )
				{
					byte EID = byte(o_relay/10);
					bitClear(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
					bitClear(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
				}
#endif  // RelayExp
			}
			else if (o_type==1)  // Turn port on
			{
				if ( o_relay < 9 )
				{
					bitSet(ReefAngel.Relay.RelayMaskOn,o_relay-1);
					bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
				}
#ifdef RelayExp
				if ( (o_relay > 10) && (o_relay < 89) )
				{
					byte EID = byte(o_relay/10);
					bitSet(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
					bitSet(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
				}
#endif  // RelayExp
			}
			else if (o_type==2)  // Set port back to Auto
			{
				if ( o_relay < 9 )
				{
					bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
					bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
				}
#ifdef RelayExp
				if ( (o_relay > 10) && (o_relay < 89) )
				{
					byte EID = byte(o_relay/10);
					bitClear(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
					bitSet(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
				}
#endif  // RelayExp
			}
#ifdef OVERRIDE_PORTS
			// Reset relay masks for ports we want always in their programmed states.
			ReefAngel.Relay.RelayMaskOn &= ~ReefAngel.OverridePorts;
			ReefAngel.Relay.RelayMaskOff |= ReefAngel.OverridePorts;
#ifdef RelayExp
				byte i;
				for ( i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
				{
						ReefAngel.Relay.RelayMaskOnE[i] &= ~ReefAngel.OverridePortsE[i];
						ReefAngel.Relay.RelayMaskOffE[i] |= ReefAngel.OverridePortsE[i];
				}
#endif  // RelayExp  
#endif  // OVERRIDE_PORTS
			ReefAngel.Relay.Write();
			// Force update of the Portal after relay change
//				ReefAngel.Timer[PORTAL_TIMER].ForceTrigger();
		}
		case REQ_RA_STATUS:
		case REQ_R_STATUS:
		{
			char temp[6];
			int s=170;
			//<RA><ID></ID><T1></T1><T2></T2><T3></T3><PH></PH><R></R><RON></RON><ROFF></ROFF><ATOLOW></ATOLOW><ATOHIGH></ATOHIGH><EM></EM><EM1></EM1><REM></REM><AF></AF><SF></SF></RA>
			s += strlen(portalusername);
			s += intlength(ReefAngel.Params.Temp[T1_PROBE]);
			s += intlength(ReefAngel.Params.Temp[T2_PROBE]);
			s += intlength(ReefAngel.Params.Temp[T3_PROBE]);
			s += intlength(ReefAngel.Params.PH);
			s += intlength(ReefAngel.EM);
			s += intlength(ReefAngel.EM1);
			s += intlength(ReefAngel.REM);
			s += intlength(ReefAngel.AlertFlags);
			s += intlength(ReefAngel.StatusFlags);
			s += 2;  // one digit for each ATO
			s += intlength(ReefAngel.Relay.RelayData);
			s += intlength(ReefAngel.Relay.RelayMaskOn);
			s += intlength(ReefAngel.Relay.RelayMaskOff);
#ifdef DisplayLEDPWM
			s += 56;
			//<PWMA></PWMA><PWMD></PWMD><PWMAO></PWMAO><PWMDO></PWMDO>
			s += intlength(ReefAngel.PWM.GetDaylightValue());
			s += intlength(ReefAngel.PWM.GetActinicValue());
			s += intlength(ReefAngel.PWM.GetDaylightOverrideValue());
			s += intlength(ReefAngel.PWM.GetActinicOverrideValue());
#endif  // DisplayLEDPWM
#ifdef RelayExp
			s += 296;
			//<R0></R0><RON0></RON0><ROFF0></ROFF0><R1></R1><RON1></RON1><ROFF1></ROFF1><R2></R2><RON2></RON2><ROFF2></ROFF2><R3></R3><RON3></RON3><ROFF3></ROFF3><R4></R4><RON4></RON4><ROFF4></ROFF4><R5></R5><RON5></RON5><ROFF5></ROFF5><R6></R6><RON6></RON6><ROFF6></ROFF6><R7></R7><RON7></RON7><ROFF7></ROFF7>
			for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
			{
				s += intlength(ReefAngel.Relay.RelayDataE[EID]);
				s += intlength(ReefAngel.Relay.RelayMaskOnE[EID]);
				s += intlength(ReefAngel.Relay.RelayMaskOffE[EID]);
			}
#endif  // RelayExp
#ifdef PWMEXPANSION
			s += 192;
			//<PWME0></PWME0><PWME1></PWME1><PWME2></PWME2><PWME3></PWME3><PWME4></PWME4><PWME5></PWME5><PWME0O></PWME0O><PWME1O></PWME1O><PWME2O></PWME2O><PWME3O></PWME3O><PWME4O></PWME4O><PWME5O></PWME5O>
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.ExpansionChannel[EID]);
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.ExpansionChannelOverride[EID]);
#endif  // PWMEXPANSION
#ifdef RFEXPANSION
			s += 181;
			//<RFM></RFM><RFS></RFS><RFD></RFD><RFW></RFW><RFRB></RFRB><RFR></RFR><RFG></RFG><RFB></RFB><RFI></RFI><RFWO></RFWO><RFRBO></RFRBO><RFRO></RFRO><RFGO></RFGO><RFBO></RFBO><RFIO></RFIO>
			for ( byte EID = 0; EID < RF_CHANNELS; EID++ ) s += intlength(ReefAngel.RF.GetChannel(EID));
			for ( byte EID = 0; EID < RF_CHANNELS; EID++ ) s += intlength(ReefAngel.RF.GetOverrideChannel(EID));
			s += intlength(ReefAngel.RF.Mode);
			s += intlength(ReefAngel.RF.Speed);
			s += intlength(ReefAngel.RF.Duration);
#endif  // RFEXPANSION
#ifdef AI_LED
			s += 76;
			//<AIW></AIW><AIB></AIB><AIRB></AIRB><AIWO></AIWO><AIBO></AIBO><AIRBO></AIRBO>
			for ( byte EID = 0; EID < AI_CHANNELS; EID++ ) s += intlength(ReefAngel.AI.GetChannel(EID));
			for ( byte EID = 0; EID < AI_CHANNELS; EID++ ) s += intlength(ReefAngel.AI.GetOverrideChannel(EID));
#endif  // AI_LED
#ifdef SALINITYEXPANSION
			s += 11;
			//<SAL></SAL>
			s += intlength(ReefAngel.Params.Salinity);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
			s += 11;
			//<ORP></ORP>
			s += intlength(ReefAngel.Params.ORP);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
			s += 11;
			//<PHE></PHE>
			s += intlength(ReefAngel.Params.PHExp);
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
			s += 53;
			//<WL></WL><WL1></WL1><WL2></WL2><WL3></WL3><WL4></WL4>
			s += intlength(ReefAngel.WaterLevel.GetLevel());
			for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ ) s += intlength(ReefAngel.WaterLevel.GetLevel(EID));
#endif  // WATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
			s += 11;
			//<HUM></HUM>
			s += intlength(ReefAngel.Humidity.GetLevel());
#endif  // HUMIDITYEXPANSION
#ifdef DCPUMPCONTROL
			s += 33;
			//<DCM></DCM><DCS></DCS><DCD></DCD>
			s += intlength(ReefAngel.DCPump.Mode);
			s += intlength(ReefAngel.DCPump.Speed);
			s += intlength(ReefAngel.DCPump.Duration);
#endif  // DCPUMPCONTROL
#ifdef IOEXPANSION
			s += 9;
			//<IO></IO>
			s += intlength(ReefAngel.IO.GetChannel());
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
			s += 72;
			//<C0></C0><C1></C1><C2></C2><C3></C3><C4></C4><C5></C5><C6></C6><C7></C7>
			for ( byte EID = 0; EID < 8; EID++ ) s += intlength(ReefAngel.CustomVar[EID]);
#endif  // CUSTOM_VARIABLES
#ifdef ENABLE_ATO_LOGGING
			if ( reqtype == REQ_RA_STATUS )
			{
				// we send all the ato logging events both high & low
				/*
				The XML code will be like this.  This is 1 event.  There are 8 events total (4 low / 4 high).
				<AL#ON>DWORD</AL#ON>
				<AL#OFF>DWORD</AL#OFF>
				Each event is 40 bytes (32 bytes for text, 8 bytes for dword values total)
				*/
				s += 320;
			}
#endif  // ENABLE_ATO_LOGGING
			PrintHeader(s,1);
#ifdef ENABLE_ATO_LOGGING
			if ( reqtype == REQ_RA_STATUS )
				SendXMLData(true);
			else
				SendXMLData();
#else
			SendXMLData();
#endif  // ENABLE_ATO_LOGGING
			break;
		}  // REQ_RELAY
		case REQ_M_BYTE:
		case REQ_M_INT:
		case REQ_M_CVAR:
		{
			int s;

			// if memory location is > 800 it means app is trying to pull/set old memory location.
			// we decrease 600 to start using new memory map
			// this is a temporary solution until we get all apps to point to new memory location
			int newweboption2=weboption2;
			if ( weboption2>=800 ) newweboption2-=600;

			// weboption2 is location
			// weboption is value
			if ( bHasSecondValue && (weboption2 >= 0) )
			{

				// if we have a second value, we write the value to memory
				if ( reqtype == REQ_M_BYTE )
					InternalMemory.write(newweboption2, weboption);
				else if ( reqtype == REQ_M_INT )
					InternalMemory.write_int(newweboption2, weboption);
#ifdef CUSTOM_VARIABLES
				else if ( reqtype == REQ_M_CVAR )
					if (newweboption2 < 8) ReefAngel.CustomVar[newweboption2]=weboption;
#endif
				// check if we have to reload any timers
				if ( weboption2 == Mem_I_FeedingTimer )
				{
					ReefAngel.Timer[FEEDING_TIMER].SetInterval(weboption);
				}
				else if ( weboption2 == Mem_I_LCDTimer )
				{
					ReefAngel.Timer[LCD_TIMER].SetInterval(weboption);
				}
//#ifdef DisplayLEDPWM
//					else if ( weboption2 == Mem_B_LEDPWMActinic )
//					{
//						ReefAngel.PWM.SetActinic(weboption);
//						// Force update of the Portal after change
//						ReefAngel.Timer[PORTAL_TIMER].ForceTrigger();
//					}
//					else if ( weboption2 == Mem_B_LEDPWMDaylight )
//					{
//						ReefAngel.PWM.SetDaylight(weboption);
//						// Force update of the Portal after change
//						ReefAngel.Timer[PORTAL_TIMER].ForceTrigger();
//					}
//#endif  // DisplayLEDPWM

				s = 9;  // <M>OK</M>
				// add in the location, twice
				s += (intlength(weboption2)*2);
				PrintHeader(s,1);

				PROGMEMprint(XML_M_OPEN);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
				PROGMEMprint(XML_OK);
				PROGMEMprint(XML_M_CLOSE);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
			}
			else if ( !bHasSecondValue && (weboption2 >= 0) && (bCommaCount==0) )
			{
				int memvalue=0;
				// get the length first
				s = 7;  // <M></M>
				// length of the memory location, twice since it's in the open & close tag
				s += (intlength(weboption2)*2);
				// length of the value from memory
				if ( reqtype == REQ_M_BYTE )
					memvalue=InternalMemory.read(newweboption2);
				else if ( reqtype == REQ_M_INT )
					memvalue=InternalMemory.read_int(newweboption2);
#ifdef CUSTOM_VARIABLES
				else if ( reqtype == REQ_M_CVAR )
					if (newweboption2 < 8) memvalue=ReefAngel.CustomVar[newweboption2];
#endif
				s += intlength(memvalue);
				PrintHeader(s,1);

				// no second value and no comma, so we read the value from memory
				PROGMEMprint(XML_M_OPEN);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
				print(memvalue,DEC);
				PROGMEMprint(XML_M_CLOSE);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
			}
			else
			{
				s = 10;  // <M>ERR</M>
				PrintHeader(s,1);
				PROGMEMprint(XML_M_OPEN);
				PROGMEMprint(XML_CLOSE_TAG);
				PROGMEMprint(XML_ERR);
				PROGMEMprint(XML_M_CLOSE);
				PROGMEMprint(XML_CLOSE_TAG);
			}
			break;
		}  // REQ_M_BYTE || REQ_M_INT
		case REQ_OVERRIDE:
		{
			int s;

			// weboption2 is channel
			// weboption is override value
			if ( bHasSecondValue && (weboption2 < OVERRIDE_CHANNELS) )
			{
				// Override channel
				// if channel is from an expansion module that is not enabled, the command will be accepted, but it will do nothing.
#ifdef DisplayLEDPWM					
				if (weboption2==0) ReefAngel.PWM.SetDaylightOverride(weboption);
				else if (weboption2==1) ReefAngel.PWM.SetActinicOverride(weboption);
#endif // DisplayLEDPWM					
#ifdef PWMEXPANSION
				if (weboption2>=2 && weboption2<=7) ReefAngel.PWM.SetChannelOverride(weboption2-2,weboption);
#endif // PWMEXPANSION
#ifdef AI_LED
				if (weboption2>=8 && weboption2<=10) ReefAngel.AI.SetChannelOverride(weboption2-8,weboption);
#endif // AI_LED
#ifdef RFEXPANSION
				if (weboption2>=11 && weboption2<=16) ReefAngel.RF.SetChannelOverride(weboption2-11,weboption);
#endif // RFEXPANSION
				s = 9;  // <P>OK</P>
				// add in the channel, twice
				s += (intlength(weboption2)*2);
				PrintHeader(s,1);
				PROGMEMprint(XML_P_OPEN);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
				PROGMEMprint(XML_OK);
				PROGMEMprint(XML_P_CLOSE);
				print(weboption2, DEC);
				PROGMEMprint(XML_CLOSE_TAG);
			}
			else
			{
				s = 10;  // <P>ERR</P>
				PrintHeader(s,1);
				PROGMEMprint(XML_P_OPEN);
				PROGMEMprint(XML_CLOSE_TAG);
				PROGMEMprint(XML_ERR);
				PROGMEMprint(XML_P_CLOSE);
				PROGMEMprint(XML_CLOSE_TAG);
			}
			break;
		}  // REQ_OVERRIDE
		case REQ_M_RAW:
		{
			int s = 11;  // start with the base size of the mem tags

			// default to Main memory locations
			int memStart = VarsStart;
			int memEnd = VarsEnd;
			if ( bHasSecondValue && (weboption2 >= 0) )
			{
				memStart = weboption2;
				memEnd = weboption;

				// Some sanity check here
				if (memStart > memEnd) {
					weboption = -1;
				} 
			}

				if (weboption == -1)
			{
				s = 14;  // <MEM>ERR</MEM>
				PrintHeader(s,1);
				PROGMEMprint(XML_MEM_OPEN);
				PROGMEMprint(XML_ERR);
			}
			else
			{
					s += (memEnd-memStart)*2;
				PrintHeader(s,1);
				PROGMEMprint(XML_MEM_OPEN);

				byte m;
				for ( int x = memStart; x < memEnd; x++ )
				{
					m=InternalMemory.read(x);
					if (m<16) print("0");
					print(m,HEX);
				}  // for x
			}
			PROGMEMprint(XML_MEM_CLOSE);
			break;
		}  // REQ_M_RAW
		case REQ_VERSION:
		{
			int s = 7;
			s += strlen(ReefAngel_Version);
			PrintHeader(s,1);
			print("<V>"ReefAngel_Version"</V>");
			break;
		}  // REQ_VERSION
		case REQ_DATE:
		{
			uint8_t s = 10;
			uint8_t hr, min, mon, mday;
			if ( weboption > -1 )
			{
				/*
				Numbers must be formatted as follows, add leading space if a single digit
				weboption contains YY (years since 2000)
				weboption2 contains HHMM
				weboption3 contains MMDD
				*/
				hr = weboption2 / 100;
				min = weboption2 % 100;
				mon = weboption3 / 100;
				mday = weboption3 % 100;

				/*
				Simple sanity checks.  Ensure that the values are within "normal" ranges.
				This will obviously fail if somebody tries to set the 30th day of February or
				with the months that only have 30 days.  We will rely on the client sending the request
				to ensure the date is in proper range.  We will just check for the simple checks.
				*/
				if ( ( hr > 23 || hr < 0 ) ||
					 ( min > 59 || min < 0 ) ||
					 ( mon > 12 || mon < 1 ) ||
					 ( mday > 31 || mday < 1 ) )
				{
					weboption = -1;
				}
				else
				{
					s--;
				}
			} else if ( weboption == -2 )
			{
				// sending controller date/time
				// 51 = rest of xml tags
				// 12 = data being sent (already have 3 extra in s)
				//  7  = base xml tags (open & close d)
				s += 60;
			}
			PrintHeader(s,1);
			PROGMEMprint(XML_DATE_OPEN);
			if ( weboption == -1 )
			{
				PROGMEMprint(XML_ERR);
			}
			else if ( weboption == -2 )
			{
				time_t n = now();
				print("<HR>");
				print(hour(n), DEC);
				print("</HR><MIN>");
				print(minute(n), DEC);
				print("</MIN><MON>");
				print(month(n), DEC);
				print("</MON><DAY>");
				print(day(n), DEC);
				print("</DAY><YR>");
				print(year(n), DEC);
				print("</YR>");
			}
			else
			{
				PROGMEMprint(XML_OK);
				setTime(hr, min, 0, mday, mon, weboption);
#ifdef RA_EVOLUTION
				now();
				ReefAngel.RTC_INTERNAL.set_time(hr, min, 0);
				ReefAngel.RTC_INTERNAL.set_date(mday, mon, weboption+2000);
#else // RA_EVOLUTION
				now();
				RTC.set(now());
#endif // RA_EVOLUTION
			}
			PROGMEMprint(XML_DATE_CLOSE);
			break;
		}  // REQ_DATE
		case REQ_HTTP:
		{
			// When using the WebBanner and sending to reefangel.com, ra.com replies back to us that the command was successful
			// If we process that command (which we will), we end up not recognizing it and send off an uknown request response
			// back to the server.  Then the server will send another response back to us and we end up getting in an almost
			// infinite loop.  We will mark it as an HTTP request and ignore it
			break;
		}
		case REQ_FEEDING:
		{
			// Start up the feeding mode only if we are on the home screen or from Water change
			if ( ReefAngel.DisplayedMenu == DEFAULT_MENU || ReefAngel.DisplayedMenu==WATERCHANGE_MODE )
			{
				ReefAngel.ChangeMode=FEEDING_MODE;
				ModeResponse(true);
			}
			else
			{
				ModeResponse(false);
			}
			break;
		}
		case REQ_WATER:
		{
			// Start up the water change mode only if we are on the home screen
			if ( ReefAngel.DisplayedMenu == DEFAULT_MENU )
			{
				ReefAngel.ChangeMode=WATERCHANGE_MODE;
				ModeResponse(true);
			}
			else
			{
				ModeResponse(false);
			}
			break;
		}
		case REQ_BTN_PRESS:
		{
			// Only accept button press for feeding and water change mode
			if ( ReefAngel.DisplayedMenu == FEEDING_MODE || ReefAngel.DisplayedMenu==WATERCHANGE_MODE )
			{
				// Simulate a button press to stop the modes
				ButtonPress++;
				ModeResponse(true);
			}
			else
			{
				ModeResponse(false);
			}
			break;
		}
//			case REQ_CAL_RELOAD:
//			{
//				// Reload calibration values from memory
//				ReefAngel.PHMin = InternalMemory.PHMin_read();
//				ReefAngel.PHMax = InternalMemory.PHMax_read();
//#ifdef SALINITYEXPANSION
//				ReefAngel.SalMax = InternalMemory.SalMax_read();
//#endif  // SALINITYEXPANSION
//#ifdef ORPEXPANSION
//				ReefAngel.ORPMin = InternalMemory.ORPMin_read();
//				ReefAngel.ORPMax = InternalMemory.ORPMax_read();
//#endif  // ORPEXPANSION
//			    PROGMEMprint(XML_OK);
//			    break;
//			}
		case REQ_ALARM_ATO:
		{
			ReefAngel.ATOClear();
			ModeResponse(true);
			break;
		}
		case REQ_ALARM_OVERHEAT:
		{
			ReefAngel.OverheatClear();
			ModeResponse(true);
			break;
		}
		case REQ_ALARM_LEAK:
		{
#ifdef LEAKDETECTOREXPANSION
			ReefAngel.LeakClear();
#endif // LEAKDETECTOREXPANSION
			ModeResponse(true);
			break;
		}
		case REQ_LIGHTSON:
		{
			// Turn Lights On
			ReefAngel.LightsOn();
			ModeResponse(true);
			break;
		}
		case REQ_LIGHTSOFF:
		{
			// Turn Lights Off
			ReefAngel.LightsOff();
			ModeResponse(true);
			break;
		}
		case REQ_REBOOT:
		{
			// Reboot
			ModeResponse(true);
			while(1);
			break;
		}
		default:
		case REQ_UNKNOWN:
		{
			//P(WebBodyMsg) = SERVER_UKNOWN;
			//WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
			break;
		}
	}  // switch reqtype
	_wifiSerial->flush();
	m_pushbackindex=0;
    reqtype=0;
    weboption=0;
    webnegoption=false;
}

void RA_Wifi::PrintHeader(int s, byte type)
{
	PROGMEMprint(SERVER_HEADER1);
	if (type)
		print("xml");
	else
		print("html");
	PROGMEMprint(SERVER_HEADER2);
	print(s, DEC);
	print("\r\n\r\n");
}

char RA_Wifi::GetC(int c)
{
	return pgm_read_byte(c+EncodingChars);
}

void RA_Wifi::ConvertC(char* strIn, char* strOut, byte len)
{
	strOut[0] = GetC(strIn[0]>>2);
	strOut[1] = GetC(((strIn[0]&0x03)<<4)|((strIn[1]&0xf0)>>4));
	strOut[2] = len>1?GetC(((strIn[1]&0x0f)<<2)|((strIn[2]&0xc0)>>6)):'=';
	strOut[3] = len>2?GetC(strIn[2]&0x3f ):'=';
}

void RA_Wifi::WifiAuthentication(char* userpass)
{
	char* authPtr;
	int len = strlen(userpass);
	int authPtrSize = ((len + 2) / 3) << 2;
	authPtr = (char*)malloc(authPtrSize + 1);
	char* authPtr1 = authPtr;
	while (len > 0)
	{
		ConvertC(userpass, authPtr1, min(len,3));
		authPtr1 += 4;
		userpass += 3;
		len -= 3;
	}
	*(authPtr + authPtrSize) = 0;
	strcpy(authStr,authPtr);
	free(authPtr);
	println(authStr);
}

void RA_Wifi::SendXMLData(bool fAtoLog /*= false*/)
{
	// This function is used for sending the XML data on the wifi interface
	// It prints the strings from program memory instead of RAM
	PROGMEMprint(XML_ID);
	print(portalusername);
	PROGMEMprint(XML_T1);
	print(ReefAngel.Params.Temp[T1_PROBE]);
	PROGMEMprint(XML_T2);
	print(ReefAngel.Params.Temp[T2_PROBE]);
	PROGMEMprint(XML_T3);
	print(ReefAngel.Params.Temp[T3_PROBE]);
	PROGMEMprint(XML_PH);
	print(ReefAngel.Params.PH);
	PROGMEMprint(XML_R);
	print(ReefAngel.Relay.RelayData,DEC);
	PROGMEMprint(XML_RON);
	print(ReefAngel.Relay.RelayMaskOn,DEC);
	PROGMEMprint(XML_ROFF);
	print(ReefAngel.Relay.RelayMaskOff,DEC);
	PROGMEMprint(XML_RE_CLOSE);
	PROGMEMprint(XML_RE_OFF);
	PROGMEMprint(XML_CLOSE_TAG);
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		// relay data
		PROGMEMprint(XML_RE_OPEN);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.Relay.RelayDataE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		// relay on mask
		PROGMEMprint(XML_RE_OPEN);
		PROGMEMprint(XML_RE_ON);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.Relay.RelayMaskOnE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		PROGMEMprint(XML_RE_ON);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		// relay off mask
		PROGMEMprint(XML_RE_OPEN);
		PROGMEMprint(XML_RE_OFF);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.Relay.RelayMaskOffE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		PROGMEMprint(XML_RE_OFF);
		print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // RelayExp
	PROGMEMprint(XML_ATOLOW);
	print(ReefAngel.LowATO.IsActive());
	PROGMEMprint(XML_ATOHIGH);
	print(ReefAngel.HighATO.IsActive());
	PROGMEMprint(XML_EM);
	print(ReefAngel.EM, DEC);
	PROGMEMprint(XML_EM1);
	print(ReefAngel.EM1, DEC);
	PROGMEMprint(XML_REM);
	print(ReefAngel.REM, DEC);
	PROGMEMprint(XML_ALERTFLAG);
	print(ReefAngel.AlertFlags, DEC);
	PROGMEMprint(XML_STATUSFLAG);
	print(ReefAngel.StatusFlags, DEC);
	PROGMEMprint(XML_STATUSFLAG_END);
#ifdef DisplayLEDPWM
	PROGMEMprint(XML_PWMA);
	print(ReefAngel.PWM.GetActinicValue(), DEC);
	PROGMEMprint(XML_PWMD);
	print(ReefAngel.PWM.GetDaylightValue(), DEC);
	PROGMEMprint(XML_PWMAO);
	print(ReefAngel.PWM.GetActinicOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO);
	print(ReefAngel.PWM.GetDaylightOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO_END);
#endif  // DisplayLEDPWM
#ifdef SALINITYEXPANSION
	PROGMEMprint(XML_SAL);
	print(ReefAngel.Params.Salinity, DEC);
	PROGMEMprint(XML_SAL_END);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
	PROGMEMprint(XML_ORP);
	print(ReefAngel.Params.ORP, DEC);
	PROGMEMprint(XML_ORP_END);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	PROGMEMprint(XML_PHEXP);
	print(ReefAngel.Params.PHExp, DEC);
	PROGMEMprint(XML_PHEXP_END);
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	PROGMEMprint(XML_WL);
	PROGMEMprint(XML_CLOSE_TAG);
	print(ReefAngel.WaterLevel.GetLevel(), DEC);
	PROGMEMprint(XML_WL_END);
	PROGMEMprint(XML_CLOSE_TAG);
	for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ )
	{
		PROGMEMprint(XML_WL);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.WaterLevel.GetLevel(EID), DEC);
		PROGMEMprint(XML_WL_END);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // WATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
	PROGMEMprint(XML_HUM);
	print(ReefAngel.Humidity.GetLevel(), DEC);
	PROGMEMprint(XML_HUM_END);
#endif  // HUMIDITYEXPANSION
#ifdef DCPUMPCONTROL
	PROGMEMprint(XML_DCM);
	print(ReefAngel.DCPump.Mode, DEC);
	PROGMEMprint(XML_DCM_END);
	print(ReefAngel.DCPump.Speed, DEC);
	PROGMEMprint(XML_DCS_END);
	print(ReefAngel.DCPump.Duration, DEC);
	PROGMEMprint(XML_DCD_END);
#endif  // DCPUMPCONTROL
#ifdef IOEXPANSION
	PROGMEMprint(XML_IO);
	print(ReefAngel.IO.GetChannel(), DEC);
	PROGMEMprint(XML_IO_END);
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		PROGMEMprint(XML_C);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.CustomVar[EID], DEC);
		PROGMEMprint(XML_C_END);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // CUSTOM_VARIABLES
#ifdef PWMEXPANSION
	for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
	{
		PROGMEMprint(XML_PWME);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.PWM.GetChannelValue(EID), DEC);
		PROGMEMprint(XML_PWME_END);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		PROGMEMprint(XML_PWME);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
		print(ReefAngel.PWM.GetChannelOverrideValue(EID), DEC);
		PROGMEMprint(XML_PWME_END);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // PWMEXPANSION
#ifdef AI_LED
	PROGMEMprint(XML_AIW);
	print(ReefAngel.AI.GetChannel(0), DEC);
	PROGMEMprint(XML_AIW_END);
	print(ReefAngel.AI.GetChannel(1), DEC);
	PROGMEMprint(XML_AIB_END);
	print(ReefAngel.AI.GetChannel(2), DEC);
	PROGMEMprint(XML_AIRB_END);
	print(ReefAngel.AI.GetOverrideChannel(0), DEC);
	PROGMEMprint(XML_AIWO_END);
	print(ReefAngel.AI.GetOverrideChannel(1), DEC);
	PROGMEMprint(XML_AIBO_END);
	print(ReefAngel.AI.GetOverrideChannel(2), DEC);
	PROGMEMprint(XML_AIRBO_END);
#endif  // AI_LED
#ifdef RFEXPANSION
	PROGMEMprint(XML_RFM);
	print(ReefAngel.RF.Mode, DEC);
	PROGMEMprint(XML_RFM_END);
	print(ReefAngel.RF.Speed, DEC);
	PROGMEMprint(XML_RFS_END);
	print(ReefAngel.RF.Duration, DEC);
	PROGMEMprint(XML_RFD_END);
	print(ReefAngel.RF.GetChannel(0), DEC);
	PROGMEMprint(XML_RFW_END);
	print(ReefAngel.RF.GetChannel(1), DEC);
	PROGMEMprint(XML_RFRB_END);
	print(ReefAngel.RF.GetChannel(2), DEC);
	PROGMEMprint(XML_RFR_END);
	print(ReefAngel.RF.GetChannel(3), DEC);
	PROGMEMprint(XML_RFG_END);
	print(ReefAngel.RF.GetChannel(4), DEC);
	PROGMEMprint(XML_RFB_END);
	print(ReefAngel.RF.GetChannel(5), DEC);
	PROGMEMprint(XML_RFI_END);
	print(ReefAngel.RF.GetOverrideChannel(0), DEC);
	PROGMEMprint(XML_RFWO_END);
	print(ReefAngel.RF.GetOverrideChannel(1), DEC);
	PROGMEMprint(XML_RFRBO_END);
	print(ReefAngel.RF.GetOverrideChannel(2), DEC);
	PROGMEMprint(XML_RFRO_END);
	print(ReefAngel.RF.GetOverrideChannel(3), DEC);
	PROGMEMprint(XML_RFGO_END);
	print(ReefAngel.RF.GetOverrideChannel(4), DEC);
	PROGMEMprint(XML_RFBO_END);
	print(ReefAngel.RF.GetOverrideChannel(5), DEC);
	PROGMEMprint(XML_RFIO_END);
#endif  // RFEXPANSION
#ifdef ENABLE_ATO_LOGGING
	if ( fAtoLog )
	{
		int loc;
		for ( byte b = 0; b < MAX_ATO_LOG_EVENTS; b++ )
		{
			// print ato low event
			// low start time
			loc = (b * ATOEventSize) + ATOEventStart;
			PROGMEMprint(XML_ATOLOW_LOG_OPEN);
			print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOLOW_LOG_CLOSE);
			print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// low stop time
			loc += ATOEventOffStart;
			PROGMEMprint(XML_ATOLOW_LOG_OPEN);
			print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOLOW_LOG_CLOSE);
			print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// print ato high event
			// high start time
			loc = (b * ATOEventSize) + ATOEventStart + (ATOEventSize * MAX_ATO_LOG_EVENTS);
			PROGMEMprint(XML_ATOHIGH_LOG_OPEN);
			print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOHIGH_LOG_CLOSE);
			print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// high stop time
			loc += ATOEventOffStart;
			PROGMEMprint(XML_ATOHIGH_LOG_OPEN);
			print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOHIGH_LOG_CLOSE);
			print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
		}
		AtoEventCount = 0;
	}
#endif  // ENABLE_ATO_LOGGING
	PROGMEMprint(XML_END);
}

void RA_Wifi::ProcessSerial()
{
  bIncoming=true;
  timeout=millis();
  while (bIncoming)
  {
    if (millis()-timeout>100)
    {
      bIncoming=false;
      //for (int a=0;a<32;a++) pushbuffer(0);
    }
    if (_wifiSerial->available()>0)
    {
      PushBuffer(_wifiSerial->read());
      timeout=millis();
#if defined WDT || defined WDT_FORCE
      wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
    }
  }

  ProcessHTTP();

  _wifiSerial->flush();
  m_pushbackindex=0;
}

void RA_Wifi::ReceiveData()
{
  if ( _wifiSerial->available() > 0 ) ProcessSerial();
}

void RA_Wifi::PROGMEMprint(const prog_char str[])
{
    char c;
    if(!str) return;
    while((c = pgm_read_byte(str++)))
    {
      write(c);
    }
}

void RA_Wifi::LoadWebBanner(int pointer, byte qty)
{
  //  webbannerpointer = pointer;
  //  webbannerqty = qty;
}

void RA_Wifi::Portal(char *username)
{
  /*
  static byte LastRelayData;
    byte TempRelay = Relay.RelayData;
    TempRelay &= Relay.RelayMaskOff;
    TempRelay |= Relay.RelayMaskOn;
    if (TempRelay!=LastRelayData)
    {
      Timer[PORTAL_TIMER].ForceTrigger();
      LastRelayData=TempRelay;
    }
#ifdef RelayExp
  static byte LastRelayDataE[MAX_RELAY_EXPANSION_MODULES];

    for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
  {
    TempRelay = Relay.RelayDataE[EID];
    TempRelay &= Relay.RelayMaskOffE[EID];
    TempRelay |= Relay.RelayMaskOnE[EID];
      if (TempRelay!=LastRelayDataE[EID])
      {
        Timer[PORTAL_TIMER].ForceTrigger();
        LastRelayDataE[EID]=TempRelay;
      }
  }
#endif  // RelayExp
   */
  if (ReefAngel.Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,"");
  portalusername=username;
}

void RA_Wifi::Portal(char *username, char *key)
{
  if (ReefAngel.Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,key);
  portalusername=username;
}

void RA_Wifi::SendPortal(char *username, char*key)
{
#ifdef RA_STAR
  ReefAngel.Network.PortalConnection=true;
  if (NetClient.connect(PortalServer, 80))
  {
#endif
  ReefAngel.Timer[PORTAL_TIMER].Start();
  PROGMEMprint(BannerGET);
  print(ReefAngel.Params.Temp[T1_PROBE], DEC);
  PROGMEMprint(BannerT2);
  print(ReefAngel.Params.Temp[T2_PROBE], DEC);
  PROGMEMprint(BannerT3);
  print(ReefAngel.Params.Temp[T3_PROBE], DEC);
  PROGMEMprint(BannerPH);
  print(ReefAngel.Params.PH, DEC);
  PROGMEMprint(BannerID);
  print(username);
  PROGMEMprint(BannerEM);
  print(ReefAngel.EM, DEC);
  PROGMEMprint(BannerEM1);
  print(ReefAngel.EM1, DEC);
  PROGMEMprint(BannerREM);
  print(ReefAngel.REM, DEC);
  PROGMEMprint(BannerKey);
  print(key);
  PROGMEMprint(BannerAlertFlag);
  print(ReefAngel.AlertFlags);
  PROGMEMprint(BannerStatusFlag);
  print(ReefAngel.StatusFlags);
  PROGMEMprint(BannerATOHIGH);
  print(ReefAngel.HighATO.IsActive(), DEC);
  PROGMEMprint(BannerATOLOW);
  print(ReefAngel.LowATO.IsActive(), DEC);
  PROGMEMprint(BannerRelayData);
  print("=");
  print(ReefAngel.Relay.RelayData, DEC);
  PROGMEMprint(BannerRelayMaskOn);
  print("=");
  print(ReefAngel.Relay.RelayMaskOn, DEC);
  PROGMEMprint(BannerRelayMaskOff);
  print("=");
  print(ReefAngel.Relay.RelayMaskOff, DEC);

#ifdef RelayExp
  for ( byte x = 0; x < InstalledRelayExpansionModules && x < MAX_RELAY_EXPANSION_MODULES; x++ )
  {
    PROGMEMprint(BannerRelayData);
    print(x+1, DEC);
    print("=");
    print(ReefAngel.Relay.RelayDataE[x], DEC);
    PROGMEMprint(BannerRelayMaskOn);
    print(x+1, DEC);
    print("=");
    print(ReefAngel.Relay.RelayMaskOnE[x], DEC);
    PROGMEMprint(BannerRelayMaskOff);
    print(x+1, DEC);
    print("=");
    print(ReefAngel.Relay.RelayMaskOffE[x], DEC);
  }  // for x
#endif  // RelayExp
#if defined DisplayLEDPWM && ! defined RemoveAllLights
  PROGMEMprint(BannerPWMA);
  print(ReefAngel.PWM.GetActinicValue(), DEC);
  PROGMEMprint(BannerPWMD);
  print(ReefAngel.PWM.GetDaylightValue(), DEC);
  PROGMEMprint(BannerPWMAO);
  print(ReefAngel.PWM.GetActinicOverrideValue(), DEC);
  PROGMEMprint(BannerPWMDO);
  print(ReefAngel.PWM.GetDaylightOverrideValue(), DEC);
#endif  // DisplayLEDPWM && ! defined RemoveAllLights
#ifdef PWMEXPANSION
  for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
  {
    PROGMEMprint(BannerPWME);
    print(EID, DEC);
    print("=");
    print(ReefAngel.PWM.GetChannelValue(EID), DEC);
    PROGMEMprint(BannerPWME);
    print(EID, DEC);
    print("O");
    print("=");
    print(ReefAngel.PWM.GetChannelOverrideValue(EID), DEC);
  }
#endif  // PWMEXPANSION
#ifdef RFEXPANSION
  PROGMEMprint(BannerRFM);
  print(ReefAngel.RF.Mode, DEC);
  PROGMEMprint(BannerRFS);
  print(ReefAngel.RF.Speed, DEC);
  PROGMEMprint(BannerRFD);
  print(ReefAngel.RF.Duration, DEC);
  PROGMEMprint(BannerRFW);
  print(ReefAngel.RF.GetChannel(0), DEC);
  PROGMEMprint(BannerRFRB);
  print(ReefAngel.RF.GetChannel(1), DEC);
  PROGMEMprint(BannerRFR);
  print(ReefAngel.RF.GetChannel(2), DEC);
  PROGMEMprint(BannerRFG);
  print(ReefAngel.RF.GetChannel(3), DEC);
  PROGMEMprint(BannerRFB);
  print(ReefAngel.RF.GetChannel(4), DEC);
  PROGMEMprint(BannerRFI);
  print(ReefAngel.RF.GetChannel(5), DEC);
#endif  // RFEXPANSION
#ifdef AI_LED
  PROGMEMprint(BannerAIW);
  print(ReefAngel.AI.GetChannel(0), DEC);
  PROGMEMprint(BannerAIB);
  print(ReefAngel.AI.GetChannel(1), DEC);
  PROGMEMprint(BannerAIRB);
  print(ReefAngel.AI.GetChannel(2), DEC);
#endif  // AI_LED
#ifdef SALINITYEXPANSION
  PROGMEMprint(BannerSal);
  print(ReefAngel.Params.Salinity, DEC);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
  PROGMEMprint(BannerORP);
  print(ReefAngel.Params.ORP, DEC);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
  PROGMEMprint(BannerPHE);
  print(ReefAngel.Params.PHExp, DEC);
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
  PROGMEMprint(BannerWL);
  print("=");
  print(ReefAngel.WaterLevel.GetLevel(), DEC);
  for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ )
  {
    PROGMEMprint(BannerWL);
    print(EID, DEC);
    print("=");
    print(ReefAngel.WaterLevel.GetLevel(EID), DEC);
  }
#endif  // WATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
  PROGMEMprint(BannerHumidity);
  print(ReefAngel.Humidity.GetLevel(), DEC);
#endif  // HUMIDITYEXPANSION
#ifdef DCPUMPCONTROL
  PROGMEMprint(BannerDCM);
  print(ReefAngel.DCPump.Mode, DEC);
  PROGMEMprint(BannerDCS);
  print(ReefAngel.DCPump.Speed, DEC);
  PROGMEMprint(BannerDCD);
  print(ReefAngel.DCPump.Duration, DEC);
#endif  // DCPUMPCONTROL
#ifdef IOEXPANSION
  PROGMEMprint(BannerIO);
  print(ReefAngel.IO.GetChannel(), DEC);
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
  for ( byte EID = 0; EID < 8; EID++ )
  {
    PROGMEMprint(BannerCustomVar);
    print(EID, DEC);
    print("=");
    print(ReefAngel.CustomVar[EID], DEC);
  }
#endif  // CUSTOM_VARIABLES
#ifdef RA_STAR
  PROGMEMprint(BannerHTTP11);
  PROGMEMprint(BannerHost);
  PROGMEMprint(BannerConnectionClose);
  ReefAngel.Network.PortalTimeOut=millis();
#endif
  println("\n\n");
#ifdef RA_STAR
  }
#endif
}
#endif  // wifi
