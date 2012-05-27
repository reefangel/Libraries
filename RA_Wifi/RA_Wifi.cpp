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

#include "RA_Wifi.h"
#include <Globals.h>


#ifdef wifi

#include <DS1307RTC.h>
#include <ReefAngel.h>

void WebResponse (const prog_char *response, long strsize)
{
//  P(WebHeaderMsg) = SERVER_HEADER_HTML;
//  printP(WebHeaderMsg);
	PrintHTTPHeader(strsize);
	PROGMEMprint(response);
}

void ModeResponse(bool fOk)
{
	uint8_t s;
	if ( fOk )
		s = 15;
	else
		s = 16;
	PrintHTTPHeader(s);
	PROGMEMprint(XML_MODE_OPEN);
	if ( fOk )
		PROGMEMprint(XML_OK);
	else
		PROGMEMprint(XML_ERR);
	PROGMEMprint(XML_MODE_CLOSE);
}

void pushbuffer(byte inStr)
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
		        if ( (reqtype == REQ_M_BYTE) || (reqtype == REQ_M_INT) )
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
            else if (strncmp("GET /ma", m_pushback, 7)==0) reqtype = -REQ_M_ALL;
            else if (strncmp("GET /mr", m_pushback, 7)==0) reqtype = -REQ_M_RAW;
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
            //else reqtype = -REQ_UNKNOWN;
		}
	}
}

void processHTTP()
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
		if (WIFI_SERIAL.available()>0)
		{
			pushbuffer(WIFI_SERIAL.read());
			timeout=millis();
		}
    }
	if (authStr[0]==0) auth=true;
    if (auth)
    {
		auth=false;
		switch ( reqtype )
		{
			case REQ_ROOT:
			{
				P(WebBodyMsg) = SERVER_DEFAULT;
				WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
				break;
			}  // REQ_ROOT
			case REQ_WIFI:
			{
				WebResponse(SERVER_RA, sizeof(SERVER_RA) - 1);
				break;
			}  // REQ_WIFI
			case REQ_RELAY:
			{
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
				ReefAngel.Relay.Write();
				// Force update of the Portal after relay change
//				ReefAngel.Timer[PORTAL_TIMER].ForceTrigger();
			}
			case REQ_RA_STATUS:
			case REQ_R_STATUS:
			{
				char temp[6];
				int s=132;
				//<RA><T1></T1><T2></T2><T3></T3><PH></PH><R></R><RON></RON><ROFF></ROFF><ATOLOW></ATOLOW><ATOHIGH></ATOHIGH><EM></EM><REM></REM></RA>
				s += intlength(ReefAngel.Params.Temp[T1_PROBE]);
				s += intlength(ReefAngel.Params.Temp[T2_PROBE]);
				s += intlength(ReefAngel.Params.Temp[T3_PROBE]);
				s += intlength(ReefAngel.Params.PH);
				s += intlength(ReefAngel.EM);
				s += intlength(ReefAngel.REM);
				s += 2;  // one digit for each ATO
				s += intlength(ReefAngel.Relay.RelayData);
				s += intlength(ReefAngel.Relay.RelayMaskOn);
				s += intlength(ReefAngel.Relay.RelayMaskOff);
#ifdef DisplayLEDPWM
				s += 26;
				//<PWMA></PWMA><PWMD></PWMD>
				s += intlength(ReefAngel.PWM.GetDaylightValue());
				s += intlength(ReefAngel.PWM.GetActinicValue());
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
				s += 90;
				//<PWME0></PWME0><PWME1></PWME1><PWME2></PWME2><PWME3></PWME3><PWME4></PWME4><PWME5></PWME5>
				for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.ExpansionChannel[EID]);
#endif  // PWMEXPANSION
#ifdef RFEXPANSION
				s += 101;
				//<RFM></RFM><RFS></RFS><RFD></RFD><RFW></RFW><RFRB></RFRB><RFR></RFR><RFG></RFG><RFB></RFB><RFI></RFI>
				for ( byte EID = 0; EID < RF_CHANNELS; EID++ ) s += intlength(ReefAngel.RF.GetChannel(EID));
				s += intlength(ReefAngel.RF.Mode);
				s += intlength(ReefAngel.RF.Speed);
				s += intlength(ReefAngel.RF.Duration);
#endif  // RFEXPANSION
#ifdef AI_LED
				s += 35;
				//<AIW></AIW><AIB></AIB><AIRB></AIRB>
				for ( byte EID = 0; EID < AI_CHANNELS; EID++ ) s += intlength(ReefAngel.AI.GetChannel(EID));
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
				PrintHTTPHeader(s);
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
			{
				int s;

				// weboption2 is location
				// weboption is value
				if ( bHasSecondValue && (weboption2 >= 0) )
				{
					// if we have a second value, we write the value to memory
					if ( reqtype == REQ_M_BYTE )
						InternalMemory.write(weboption2, weboption);
					else
						InternalMemory.write_int(weboption2, weboption);

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
					PrintHTTPHeader(s);

					PROGMEMprint(XML_M_OPEN);
					WIFI_SERIAL.print(weboption2, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					PROGMEMprint(XML_OK);
					PROGMEMprint(XML_M_CLOSE);
					WIFI_SERIAL.print(weboption2, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				else if ( !bHasSecondValue && (weboption2 >= 0) && (bCommaCount==0) )
				{
					// get the length first
					s = 7;  // <M></M>
					// length of the memory location, twice since it's in the open & close tag
					s += (intlength(weboption2)*2);
					// length of the value from memory
					if ( reqtype == REQ_M_BYTE )
						s += intlength(InternalMemory.read(weboption2));
					else
						s += intlength(InternalMemory.read_int(weboption2));

					PrintHTTPHeader(s);

					// no second value and no comma, so we read the value from memory
					PROGMEMprint(XML_M_OPEN);
					WIFI_SERIAL.print(weboption2, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					if ( reqtype == REQ_M_BYTE )
						WIFI_SERIAL.print(InternalMemory.read(weboption2),DEC);
					else
						WIFI_SERIAL.print(InternalMemory.read_int(weboption2),DEC);
					PROGMEMprint(XML_M_CLOSE);
					WIFI_SERIAL.print(weboption2, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				else
				{
					s = 10;  // <M>ERR</M>
					PrintHTTPHeader(s);
					PROGMEMprint(XML_M_OPEN);
					PROGMEMprint(XML_CLOSE_TAG);
					PROGMEMprint(XML_ERR);
					PROGMEMprint(XML_M_CLOSE);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				break;
			}  // REQ_M_BYTE || REQ_M_INT
			case REQ_M_RAW:
			{
				int s = 11;  // start with the base size of the mem tags
				s += (VarsEnd-VarsStart)*2;
				PrintHTTPHeader(s);
				PROGMEMprint(XML_MEM_OPEN);
				byte m; 
				for ( int x = VarsStart; x < VarsEnd; x++ )
				{
					m=InternalMemory.read(x);
					if (m<17) WIFI_SERIAL.print("0");
					WIFI_SERIAL.print(m,HEX);
				}  // for x
				PROGMEMprint(XML_MEM_CLOSE);
				break;
			}  // REQ_M_RAW
			case REQ_M_ALL:
			{
				// TODO update all memory function
				//int s = 123;  // start with the base size of the headers plus the mem tags
				int s = 11;  // start with the base size of the mem tags
				/*
				Send all the data to the client requesting it.  The values will be sent as follows:
					- wrapped in <MEM></MEM> XML tags
					- individual memory values wrapped in their location XML tags
						Memory 800, value 20 - <M800>20</M800>

				An example would be:
					<MEM>
						<M800>20</M800>
						<M801>0</M801>
						<M802>16</M802>
						...
					</MEM>
				*/
				uint8_t offsets[] = {1,1,1,1,1,1,1,1,2,2,1,1,2,2,2,1,1,2,2,2,2,1,2,2,1,1,1,1,1,1,1,1,2,2};
				uint8_t num = sizeof(offsets)/sizeof(uint8_t);
				// add in the memory location sizes, 13 bytes if the memory location is 3 digits <MXXX></MXXX>
				s += num*13;
				uint16_t count = VarsStart;
				uint8_t x;
				for ( x = 0; x < num; x++ )
				{
					if ( offsets[x] == 1 )
						s += intlength(InternalMemory.read(count));
					else
						s += intlength(InternalMemory.read_int(count));
					count += offsets[x];
				}  // for x

				PrintHTTPHeader(s);
				PROGMEMprint(XML_MEM_OPEN);
				/*
				Loop through all the memory locations starting at VarsStart
				Check the offset for each location to tell us how many bytes to read and how much to increment
				the memory locations.  Currently there are only 1 and 2 byte locations, so when we check
				we just check for 1 and read a byte, otherwise we read 2 bytes (an int)
				Then we increment the memory position by the offset.
				We could be thrown off if somebody accidentally put a value larger than 2 in the offset array
				*/
				for ( x = 0, count = VarsStart; x < num; x++ )
				{
					PROGMEMprint(XML_M_OPEN);
					WIFI_SERIAL.print(count,DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					if ( offsets[x] == 1 )
						WIFI_SERIAL.print(InternalMemory.read(count),DEC);
					else
						WIFI_SERIAL.print(InternalMemory.read_int(count),DEC);
					PROGMEMprint(XML_M_CLOSE);
					WIFI_SERIAL.print(count,DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					count += offsets[x];
				}  // for x
				PROGMEMprint(XML_MEM_CLOSE);
				break;
			}  // REQ_M_ALL
			case REQ_VERSION:
			{
				int s = 7;
				s += strlen(ReefAngel_Version);
				PrintHTTPHeader(s);
				WIFI_SERIAL.print("<V>"ReefAngel_Version"</V>");
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
				PrintHTTPHeader(s);
				PROGMEMprint(XML_DATE_OPEN);
				if ( weboption == -1 )
				{
					PROGMEMprint(XML_ERR);
				}
				else if ( weboption == -2 )
				{
					time_t n = now();
					WIFI_SERIAL.print("<HR>");
					WIFI_SERIAL.print(hour(n), DEC);
					WIFI_SERIAL.print("</HR><MIN>");
					WIFI_SERIAL.print(minute(n), DEC);
					WIFI_SERIAL.print("</MIN><MON>");
					WIFI_SERIAL.print(month(n), DEC);
					WIFI_SERIAL.print("</MON><DAY>");
					WIFI_SERIAL.print(day(n), DEC);
					WIFI_SERIAL.print("</DAY><YR>");
					WIFI_SERIAL.print(year(n), DEC);
					WIFI_SERIAL.print("</YR>");
				}
				else
				{
					PROGMEMprint(XML_OK);
					setTime(hr, min, 0, mday, mon, weboption);
					now();
					RTC.set(now());
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
				// Start up the feeding mode only if we are on the home screen
				if ( ReefAngel.DisplayedMenu == DEFAULT_MENU )
				{
					//ReefAngel.ClearScreen(DefaultBGColor);
					ReefAngel.FeedingModeStart();
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
					//ReefAngel.ClearScreen(DefaultBGColor);
					ReefAngel.WaterChangeModeStart();
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
				// Simulate a button press to stop the modes
				ButtonPress++;
				ModeResponse(true);
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
			default:
			case REQ_UNKNOWN:
			{
				//P(WebBodyMsg) = SERVER_UKNOWN;
				//WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
				break;
			}
		}  // switch reqtype
    }
    else
    {
		if (reqtype>0)
		{
		  P(WebBodyMsg) = SERVER_DENY;
		  PROGMEMprint(WebBodyMsg);
		}
    }
	WIFI_SERIAL.flush();
	m_pushbackindex=0;
    reqtype=0;
    weboption=0;
}

void PrintHTTPHeader(int s)
{
	P(WebBodyMsg) = SERVER_HEADER_XML;
	PROGMEMprint(WebBodyMsg);
	WIFI_SERIAL.print(s, DEC);
	P(WebBodyMsg1) = SERVER_HEADER3;
	PROGMEMprint(WebBodyMsg1);
}

char GetC(int c)
{
	return pgm_read_byte(c+EncodingChars);
}

void ConvertC(char* strIn, char* strOut, byte len)
{
	strOut[0] = GetC(strIn[0]>>2);
	strOut[1] = GetC(((strIn[0]&0x03)<<4)|((strIn[1]&0xf0)>>4));
	strOut[2] = len>1?GetC(((strIn[1]&0x0f)<<2)|((strIn[2]&0xc0)>>6)):'=';
	strOut[3] = len>2?GetC(strIn[2]&0x3f ):'=';
}

void WifiAuthentication(char* userpass)
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
	WIFI_SERIAL.println(authStr);
}

void SendXMLData(bool fAtoLog /*= false*/)
{
	// This function is used for sending the XML data on the wifi interface
	// It prints the strings from program memory instead of RAM
	PROGMEMprint(XML_T1);
	WIFI_SERIAL.print(ReefAngel.Params.Temp[T1_PROBE]);
	PROGMEMprint(XML_T2);
	WIFI_SERIAL.print(ReefAngel.Params.Temp[T2_PROBE]);
	PROGMEMprint(XML_T3);
	WIFI_SERIAL.print(ReefAngel.Params.Temp[T3_PROBE]);
	PROGMEMprint(XML_PH);
	WIFI_SERIAL.print(ReefAngel.Params.PH);
	PROGMEMprint(XML_R);
	WIFI_SERIAL.print(ReefAngel.Relay.RelayData,DEC);
	PROGMEMprint(XML_RON);
	WIFI_SERIAL.print(ReefAngel.Relay.RelayMaskOn,DEC);
	PROGMEMprint(XML_ROFF);
	WIFI_SERIAL.print(ReefAngel.Relay.RelayMaskOff,DEC);
	PROGMEMprint(XML_RE_CLOSE);
	PROGMEMprint(XML_RE_OFF);
	PROGMEMprint(XML_CLOSE_TAG);
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		// relay data
		PROGMEMprint(XML_RE_OPEN);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		WIFI_SERIAL.print(ReefAngel.Relay.RelayDataE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		// relay on mask
		PROGMEMprint(XML_RE_OPEN);
		PROGMEMprint(XML_RE_ON);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		WIFI_SERIAL.print(ReefAngel.Relay.RelayMaskOnE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		PROGMEMprint(XML_RE_ON);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		// relay off mask
		PROGMEMprint(XML_RE_OPEN);
		PROGMEMprint(XML_RE_OFF);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		WIFI_SERIAL.print(ReefAngel.Relay.RelayMaskOffE[EID],DEC);
		PROGMEMprint(XML_RE_CLOSE);
		PROGMEMprint(XML_RE_OFF);
		WIFI_SERIAL.print(EID+1, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // RelayExp
	PROGMEMprint(XML_ATOLOW);
	WIFI_SERIAL.print(ReefAngel.LowATO.IsActive());
	PROGMEMprint(XML_ATOHIGH);
	WIFI_SERIAL.print(ReefAngel.HighATO.IsActive());
	PROGMEMprint(XML_EM);
	WIFI_SERIAL.print(ReefAngel.EM, DEC);
	PROGMEMprint(XML_REM);
	WIFI_SERIAL.print(ReefAngel.REM, DEC);
	PROGMEMprint(XML_REM_END);
#ifdef DisplayLEDPWM
	PROGMEMprint(XML_PWMA);
	WIFI_SERIAL.print(ReefAngel.PWM.GetActinicValue(), DEC);
	PROGMEMprint(XML_PWMD);
	WIFI_SERIAL.print(ReefAngel.PWM.GetDaylightValue(), DEC);
	PROGMEMprint(XML_PWMD_END);
#endif  // DisplayLEDPWM
#ifdef SALINITYEXPANSION
	PROGMEMprint(XML_SAL);
	WIFI_SERIAL.print(ReefAngel.Params.Salinity, DEC);
	PROGMEMprint(XML_SAL_END);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
	PROGMEMprint(XML_ORP);
	WIFI_SERIAL.print(ReefAngel.Params.ORP, DEC);
	PROGMEMprint(XML_ORP_END);
#endif  // ORPEXPANSION
#ifdef IOEXPANSION
	PROGMEMprint(XML_IO);
	WIFI_SERIAL.print(ReefAngel.IO.GetChannel(), DEC);
	PROGMEMprint(XML_IO_END);
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		PROGMEMprint(XML_C);
		WIFI_SERIAL.print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		WIFI_SERIAL.print(ReefAngel.CustomVar[EID], DEC);
		PROGMEMprint(XML_C_END);
		WIFI_SERIAL.print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // CUSTOM_VARIABLES
#ifdef PWMEXPANSION
	for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
	{
		PROGMEMprint(XML_PWME);
		WIFI_SERIAL.print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		WIFI_SERIAL.print(ReefAngel.PWM.GetChannelValue(EID), DEC);
		PROGMEMprint(XML_PWME_END);
		WIFI_SERIAL.print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // PWMEXPANSION
#ifdef AI_LED
	PROGMEMprint(XML_AIW);
	WIFI_SERIAL.print(ReefAngel.AI.GetChannel(0), DEC);
	PROGMEMprint(XML_AIW_END);
	WIFI_SERIAL.print(ReefAngel.AI.GetChannel(1), DEC);
	PROGMEMprint(XML_AIB_END);
	WIFI_SERIAL.print(ReefAngel.AI.GetChannel(2), DEC);
	PROGMEMprint(XML_AIRB_END);
#endif  // AI_LED
#ifdef RFEXPANSION
	PROGMEMprint(XML_RFM);
	WIFI_SERIAL.print(ReefAngel.RF.Mode, DEC);
	PROGMEMprint(XML_RFM_END);
	WIFI_SERIAL.print(ReefAngel.RF.Speed, DEC);
	PROGMEMprint(XML_RFS_END);
	WIFI_SERIAL.print(ReefAngel.RF.Duration, DEC);
	PROGMEMprint(XML_RFD_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(0), DEC);
	PROGMEMprint(XML_RFW_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(1), DEC);
	PROGMEMprint(XML_RFRB_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(2), DEC);
	PROGMEMprint(XML_RFR_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(3), DEC);
	PROGMEMprint(XML_RFG_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(4), DEC);
	PROGMEMprint(XML_RFB_END);
	WIFI_SERIAL.print(ReefAngel.RF.GetChannel(5), DEC);
	PROGMEMprint(XML_RFI_END);
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
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			WIFI_SERIAL.print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOLOW_LOG_CLOSE);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// low stop time
			loc += ATOEventOffStart;
			PROGMEMprint(XML_ATOLOW_LOG_OPEN);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			WIFI_SERIAL.print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOLOW_LOG_CLOSE);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// print ato high event
			// high start time
			loc = (b * ATOEventSize) + ATOEventStart + (ATOEventSize * MAX_ATO_LOG_EVENTS);
			PROGMEMprint(XML_ATOHIGH_LOG_OPEN);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			WIFI_SERIAL.print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOHIGH_LOG_CLOSE);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_ON);
			PROGMEMprint(XML_CLOSE_TAG);
			// zero out memory after sent
			InternalMemory.write_dword(loc, 0);
			// high stop time
			loc += ATOEventOffStart;
			PROGMEMprint(XML_ATOHIGH_LOG_OPEN);
			WIFI_SERIAL.print(b,DEC);
			PROGMEMprint(XML_RE_OFF);
			PROGMEMprint(XML_CLOSE_TAG);
			WIFI_SERIAL.print(InternalMemory.read_dword(loc), DEC);
			PROGMEMprint(XML_ATOHIGH_LOG_CLOSE);
			WIFI_SERIAL.print(b,DEC);
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

void CheckWifi(){};

#endif  // wifi

void pingSerial()
{
#ifdef wifi
    if ( WIFI_SERIAL.available() > 0 ) processHTTP();
#endif  // wifi
}

void PROGMEMprint(const prog_char str[])
{
    char c;
    if(!str) return;
    while((c = pgm_read_byte(str++)))
        WIFI_SERIAL.write(c);
}


