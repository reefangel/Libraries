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

#if defined wifi || defined ETH_WIZ5100
#include "RA_Wifi.h"
#include <DS1307RTC.h>
#include <ReefAngel.h>

RA_Wifi::RA_Wifi()
{
#if !defined ETH_WIZ5100
  _wifiSerial = &WIFI_SERIAL;
  _wifiSerial->begin(57600);
  _wifiSerial->setTimeout(100);
#endif  // ETH_WIZ5100

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
  portalkey="";
  portalsubdomain="";
  usingAuth=false;
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
//		if (authStr[m_pushbackindex]==inStr) m_pushbackindex++; else m_pushbackindex=0;
//		if (authStr[m_pushbackindex]==0) auth=true;
		//if (m_pushbackindex>0) Serial.println(m_pushbackindex,DEC);
		//if (m_pushbackindex>0) Serial.println(test,DEC);
		if (usingAuth && !auth) auth=_wifiSerial->find(encodeduserpass);
		if (reqtype==10) auth=true;
		_wifiSerial->flush();
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
            else if (strncmp("GET /cal", m_pushback, 8)==0) { reqtype = -REQ_CALIBRATION; weboption2 = -1; bHasSecondValue = false; bCommaCount = 0; }
            else if (strncmp("GET /json", m_pushback, 9)==0) reqtype = REQ_JSON;
            //else reqtype = -REQ_UNKNOWN;
		}
	}
}

void RA_Wifi::ProcessHTTP()
{
  if (usingAuth && !auth)
  {
	  PROGMEMprint(SERVER_DENY);
	  m_pushbackindex=0;
	  reqtype=0;
	  weboption=0;
	  webnegoption=false;
	  return;
  }
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
			int s=181;
			//<RA><ID></ID><T1></T1><T2></T2><T3></T3><PH></PH><R></R><RON></RON><ROFF></ROFF><ATOLOW></ATOLOW><ATOHIGH></ATOHIGH><EM></EM><EM1></EM1><REM></REM><BID></BID><AF></AF><SF></SF></RA>
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
			s += intlength(ReefAngel.Board);
#ifdef DisplayLEDPWM
			s += 56;
			//<PWMA></PWMA><PWMD></PWMD><PWMAO></PWMAO><PWMDO></PWMDO>
#if defined(__SAM3X8E__)
			s += intlength(ReefAngel.VariableControl.GetDaylightValue());
			s += intlength(ReefAngel.VariableControl.GetActinicValue());
			s += intlength(ReefAngel.VariableControl.GetDaylightOverrideValue());
			s += intlength(ReefAngel.VariableControl.GetActinicOverrideValue());
#else
			s += intlength(ReefAngel.PWM.GetDaylightValue());
			s += intlength(ReefAngel.PWM.GetActinicValue());
			s += intlength(ReefAngel.PWM.GetDaylightOverrideValue());
			s += intlength(ReefAngel.PWM.GetActinicOverrideValue());
#endif
#if defined RA_STAR || defined RA_EVOLUTION
			s += 64;
			//<PWMA2></PWMA2><PWMD2></PWMD2><PWMA2O></PWMA2O><PWMD2O></PWMD2O>
#if defined(__SAM3X8E__)
			s += intlength(ReefAngel.VariableControl.GetDaylight2Value());
			s += intlength(ReefAngel.VariableControl.GetActinic2Value());
			s += intlength(ReefAngel.VariableControl.GetDaylight2OverrideValue());
			s += intlength(ReefAngel.VariableControl.GetActinic2OverrideValue());
#else
			s += intlength(ReefAngel.PWM.GetDaylight2Value());
			s += intlength(ReefAngel.PWM.GetActinic2Value());
			s += intlength(ReefAngel.PWM.GetDaylight2OverrideValue());
			s += intlength(ReefAngel.PWM.GetActinic2OverrideValue());
#endif
#endif  // RA_STAR
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
#if defined(__SAM3X8E__)
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.GetChannelValue(EID));
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.GetChannelOverrideValue(EID));
#else
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.GetChannelValue(EID));
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.GetChannelOverrideValue(EID));
#endif
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
			s += 664;
			//<SCPWME0></SCPWME0><SCPWME1></SCPWME1><SCPWME2></SCPWME2><SCPWME3></SCPWME3><SCPWME4></SCPWME4><SCPWME5></SCPWME5><SCPWME6></SCPWME6><SCPWME7></SCPWME7><SCPWME8></SCPWME8><SCPWME9></SCPWME9><SCPWME10></SCPWME10><SCPWME11></SCPWME11><SCPWME12></SCPWME12><SCPWME13></SCPWME13><SCPWME14></SCPWME14><SCPWME15></SCPWME15><SCPWME0O></SCPWME0O><SCPWME1O></SCPWME1O><SCPWME2O></SCPWME2O><SCPWME3O></SCPWME3O><SCPWME4O></SCPWME4O><SCPWME5O></SCPWME5O><SCPWME6O></SCPWME6O><SCPWME7O></SCPWME7O><SCPWME8O></SCPWME8O><SCPWME9O></SCPWME9O><SCPWME10O></SCPWME10O><SCPWME11O></SCPWME11O><SCPWME12O></SCPWME12O><SCPWME13O></SCPWME13O><SCPWME14O></SCPWME14O><SCPWME15O></SCPWME15O>
#if defined(__SAM3X8E__)
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.Get16ChannelValue(EID));
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.Get16ChannelOverrideValue(EID));
#else
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.Get16ChannelValue(EID));
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.Get16ChannelOverrideValue(EID));
#endif
#endif  // SIXTEENCHPWMEXPANSION
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
#if defined WATERLEVELEXPANSION
			s += 9;
			//<WL></WL>
			s += intlength(ReefAngel.WaterLevel.GetLevel());
#endif  // WATERLEVELEXPANSION
#if defined MULTIWATERLEVELEXPANSION
			s += 44;
			//<WL1></WL1><WL2></WL2><WL3></WL3><WL4></WL4>
			for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ ) s += intlength(ReefAngel.WaterLevel.GetLevel(EID));
#endif  // MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
			s += 11;
			//<HUM></HUM>
			s += intlength(ReefAngel.Humidity.GetLevel());
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
			s += 11;
			//<PAR></PAR>
			s += intlength(ReefAngel.PAR.GetLevel());
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
			s += 44;
			//<DCM></DCM><DCS></DCS><DCD></DCD><DCT></DCT>
			s += intlength(ReefAngel.DCPump.Mode);
			s += intlength(ReefAngel.DCPump.Speed);
			s += intlength(ReefAngel.DCPump.Duration);
			s += intlength(ReefAngel.DCPump.Threshold);
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
#ifdef LEAKDETECTOREXPANSION
			s += 13;
			//<LEAK></LEAK>
			s += intlength(ReefAngel.IsLeakDetected());
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
			s += 15;
			//<ALARM></ALARM>
			s += intlength(ReefAngel.AlarmInput.IsActive());
#endif  // RA_STAR
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
#if defined(__SAM3X8E__)
				if (weboption2==0) ReefAngel.VariableControl.SetDaylightOverride(weboption);
				else if (weboption2==1) ReefAngel.VariableControl.SetActinicOverride(weboption);
#else
				if (weboption2==0) ReefAngel.PWM.SetDaylightOverride(weboption);
				else if (weboption2==1) ReefAngel.PWM.SetActinicOverride(weboption);
#endif
#ifdef PWMEXPANSION
#if defined(__SAM3X8E__)
				if (weboption2>=2 && weboption2<=7) ReefAngel.VariableControl.SetChannelOverride(weboption2-2,weboption);
#else
				if (weboption2>=2 && weboption2<=7) ReefAngel.PWM.SetChannelOverride(weboption2-2,weboption);
#endif
#endif // PWMEXPANSION
#ifdef AI_LED
				if (weboption2>=8 && weboption2<=10) ReefAngel.AI.SetChannelOverride(weboption2-8,weboption);
#endif // AI_LED
#ifdef RFEXPANSION
				if (weboption2>=11 && weboption2<=16) ReefAngel.RF.SetChannelOverride(weboption2-11,weboption);
#endif // RFEXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
#if defined(__SAM3X8E__)
				if (weboption2==17) ReefAngel.VariableControl.SetDaylight2Override(weboption);
				else if (weboption2==18) ReefAngel.VariableControl.SetActinic2Override(weboption);
#else
				if (weboption2==17) ReefAngel.PWM.SetDaylight2Override(weboption);
				else if (weboption2==18) ReefAngel.PWM.SetActinic2Override(weboption);
#endif
#endif // RA_STAR
#ifdef SIXTEENCHPWMEXPANSION
#if defined(__SAM3X8E__)
				if (weboption2>=19 && weboption2<=34) ReefAngel.VariableControl.Set16ChannelOverride(weboption2-19,weboption);
#else
				if (weboption2>=19 && weboption2<=34) ReefAngel.PWM.Set16ChannelOverride(weboption2-19,weboption);
#endif
#endif // SIXTEENCHPWMEXPANSION
#endif // DisplayLEDPWM
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
				I2CRTC.set(now());
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
			// Only accept button press for feeding, water change or calibration modes
			if ( ReefAngel.DisplayedMenu == FEEDING_MODE ||
				ReefAngel.DisplayedMenu == WATERCHANGE_MODE ||
				ReefAngel.DisplayedMenu == PH_CALIBRATE_MENU ||
				ReefAngel.DisplayedMenu == SAL_CALIBRATE_MENU ||
				ReefAngel.DisplayedMenu == ORP_CALIBRATE_MENU ||
				ReefAngel.DisplayedMenu == PHE_CALIBRATE_MENU ||
				ReefAngel.DisplayedMenu == WL_CALIBRATE_MENU )
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
		case REQ_CALIBRATION:
		{
			ModeResponse(true);
			if (weboption==0)
				ReefAngel.ChangeMode=PH_CALIBRATE_MENU;
#ifdef SALINITYEXPANSION
			if (weboption==1)
				ReefAngel.ChangeMode=SAL_CALIBRATE_MENU;
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
			if (weboption==2)
				ReefAngel.ChangeMode=ORP_CALIBRATE_MENU;
#endif // ORPEXPANSION
#ifdef PHEXPANSION
			if (weboption==3)
				ReefAngel.ChangeMode=PHE_CALIBRATE_MENU;
#endif // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
			if (weboption==4)
				ReefAngel.ChangeMode=WL_CALIBRATE_MENU;
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
			break;
		}
#ifndef RA_STANDARD
		case REQ_JSON:
		{
			char temp[6];
			int s=152;
			//{"json":{"ID":"","T1":"","T2":"","T3":"","PH":"","R":"","RON":"","ROFF":"","ATOLOW":"","ATOHIGH":"","EM":"","EM1":"","REM":"","BID":"","AF":"","SF":""}}
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
			s += intlength(ReefAngel.Board);
#ifdef DisplayLEDPWM
			s += 42;
			//,"PWMA":"","PWMD":"","PWMAO":"","PWMDO":""
#if defined(__SAM3X8E__)
			s += intlength(ReefAngel.VariableControl.GetDaylightValue());
			s += intlength(ReefAngel.VariableControl.GetActinicValue());
			s += intlength(ReefAngel.VariableControl.GetDaylightOverrideValue());
			s += intlength(ReefAngel.VariableControl.GetActinicOverrideValue());
#else
			s += intlength(ReefAngel.PWM.GetDaylightValue());
			s += intlength(ReefAngel.PWM.GetActinicValue());
			s += intlength(ReefAngel.PWM.GetDaylightOverrideValue());
			s += intlength(ReefAngel.PWM.GetActinicOverrideValue());
#endif
#if defined RA_STAR || defined RA_EVOLUTION
			s += 46;
			//,"PWMA2":"","PWMD2":"","PWMA2O":"","PWMD2O":""
#if defined(__SAM3X8E__)
			s += intlength(ReefAngel.VariableControl.GetDaylight2Value());
			s += intlength(ReefAngel.VariableControl.GetActinic2Value());
			s += intlength(ReefAngel.VariableControl.GetDaylight2OverrideValue());
			s += intlength(ReefAngel.VariableControl.GetActinic2OverrideValue());
#else
			s += intlength(ReefAngel.PWM.GetDaylight2Value());
			s += intlength(ReefAngel.PWM.GetActinic2Value());
			s += intlength(ReefAngel.PWM.GetDaylight2OverrideValue());
			s += intlength(ReefAngel.PWM.GetActinic2OverrideValue());
#endif
#endif  // RA_STAR
#endif  // DisplayLEDPWM
#ifdef RelayExp
			s += 232;
			//,"R1":"","RON1":"","ROFF1":"","R2":"","RON2":"","ROFF2":"","R3":"","RON3":"","ROFF3":"","R4":"","RON4":"","ROFF4":"","R5":"","RON5":"","ROFF5":"","R6":"","RON6":"","ROFF6":"","R7":"","RON7":"","ROFF7":"","R8":"","RON8":"","ROFF8":""
			for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
			{
				s += intlength(ReefAngel.Relay.RelayDataE[EID]);
				s += intlength(ReefAngel.Relay.RelayMaskOnE[EID]);
				s += intlength(ReefAngel.Relay.RelayMaskOffE[EID]);
			}
#endif  // RelayExp
#ifdef PWMEXPANSION
			s += 138;
			//,"PWME0":"","PWME1":"","PWME2":"","PWME3":"","PWME4":"","PWME5":"","PWME0O":"","PWME1O":"","PWME2O":"","PWME3O":"","PWME4O":"","PWME5O":""
#if defined(__SAM3X8E__)
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.GetChannelValue(EID));
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.GetChannelOverrideValue(EID));
#else
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.GetChannelValue(EID));
			for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.GetChannelOverrideValue(EID));
#endif
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
			s += 444;
			//,"SCPWME0":"","SCPWME1":"","SCPWME2":"","SCPWME3":"","SCPWME4":"","SCPWME5":"","SCPWME6":"","SCPWME7":"","SCPWME8":"","SCPWME9":"","SCPWME10":"","SCPWME11":"","SCPWME12":"","SCPWME13":"","SCPWME14":"","SCPWME15":"","SCPWME0O":"","SCPWME1O":"","SCPWME2O":"","SCPWME3O":"","SCPWME4O":"","SCPWME5O":"","SCPWME6O":"","SCPWME7O":"","SCPWME8O":"","SCPWME9O":"","SCPWME10O":"","SCPWME11O":"","SCPWME12O":"","SCPWME13O":"","SCPWME14O":"","SCPWME15O":""
#if defined(__SAM3X8E__)
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.Get16ChannelValue(EID));
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.VariableControl.Get16ChannelOverrideValue(EID));
#else
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.Get16ChannelValue(EID));
			for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ ) s += intlength(ReefAngel.PWM.Get16ChannelOverrideValue(EID));
#endif
#endif  // SIXTEENCHPWMEXPANSION
#ifdef RFEXPANSION
			s += 143;
			//,"RFM":"","RFS":"","RFD":"","RFW":"","RFRB":"","RFR":"","RFG":"","RFB":"","RFI":"","RFWO":"","RFRBO":"","RFRO":"","RFGO":"","RFBO":"","RFIO":""
			for ( byte EID = 0; EID < RF_CHANNELS; EID++ ) s += intlength(ReefAngel.RF.GetChannel(EID));
			for ( byte EID = 0; EID < RF_CHANNELS; EID++ ) s += intlength(ReefAngel.RF.GetOverrideChannel(EID));
			s += intlength(ReefAngel.RF.Mode);
			s += intlength(ReefAngel.RF.Speed);
			s += intlength(ReefAngel.RF.Duration);
#endif  // RFEXPANSION
#ifdef AI_LED
			s += 59;
			//,"AIW":"","AIB":"","AIRB":"","AIWO":"","AIBO":"","AIRBO":""
			for ( byte EID = 0; EID < AI_CHANNELS; EID++ ) s += intlength(ReefAngel.AI.GetChannel(EID));
			for ( byte EID = 0; EID < AI_CHANNELS; EID++ ) s += intlength(ReefAngel.AI.GetOverrideChannel(EID));
#endif  // AI_LED
#ifdef SALINITYEXPANSION
			s += 9;
			//,"SAL":""
			s += intlength(ReefAngel.Params.Salinity);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
			s += 9;
			//,"ORP":""
			s += intlength(ReefAngel.Params.ORP);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
			s += 9;
			//,"PHE":""
			s += intlength(ReefAngel.Params.PHExp);
#endif  // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
			s += 44;
			//,"WL":"","WL1":"","WL2":"","WL3":"","WL4":""
			s += intlength(ReefAngel.WaterLevel.GetLevel());
			for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ ) s += intlength(ReefAngel.WaterLevel.GetLevel(EID));
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
			s += 9;
			//,"HUM":""
			s += intlength(ReefAngel.Humidity.GetLevel());
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
			s += 9;
			//,"PAR":""
			s += intlength(ReefAngel.PAR.GetLevel());
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
			s += 36;
			//,"DCM":"","DCS":"","DCD":"","DCT":""
			s += intlength(ReefAngel.DCPump.Mode);
			s += intlength(ReefAngel.DCPump.Speed);
			s += intlength(ReefAngel.DCPump.Duration);
			s += intlength(ReefAngel.DCPump.Threshold);
#endif  // DCPUMPCONTROL
#ifdef IOEXPANSION
			s += 8;
			//,"IO":""
			s += intlength(ReefAngel.IO.GetChannel());
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
			s += 64;
			//,"C0":"","C1":"","C2":"","C3":"","C4":"","C5":"","C6":"","C7":""
			for ( byte EID = 0; EID < 8; EID++ ) s += intlength(ReefAngel.CustomVar[EID]);
#endif  // CUSTOM_VARIABLES
#ifdef LEAKDETECTOREXPANSION
			s += 10;
			//,"LEAK":""
			s += intlength(ReefAngel.IsLeakDetected());
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
			s += 11;
			//,"ALARM":""
			s += intlength(ReefAngel.AlarmInput.IsActive());
#endif  // RA_STAR
			PrintHeader(s,2);
			SendJSONData();
			break;
		}  // REQ_JSON
#endif // RA_STANDARD

		default:
		case REQ_UNKNOWN:
		{
			//P(WebBodyMsg) = SERVER_UKNOWN;
			//WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
			break;
		}
	}  // switch reqtype
#if defined WDT || defined WDT_FORCE
	ReefAngel.WDTReset();
#endif  // defined WDT || defined WDT_FORCE

	m_pushbackindex=0;
    reqtype=0;
    weboption=0;
    webnegoption=false;
}

void RA_Wifi::PrintHeader(int s, byte type)
{
	PROGMEMprint(SERVER_HEADER1);
	switch(type)
	{
	  case 0:
	    print("html");
	    break;
	  case 1:
	    print("xml");
	    break;
	  case 2:
	    print("json");
	    break;
	}

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
	strcpy(encodeduserpass,authPtr);
	free(authPtr);
	usingAuth=true;
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
	PROGMEMprint(XML_BOARDID);
	print(ReefAngel.Board, DEC);
	PROGMEMprint(XML_ALERTFLAG);
	print(ReefAngel.AlertFlags, DEC);
	PROGMEMprint(XML_STATUSFLAG);
	print(ReefAngel.StatusFlags, DEC);
	PROGMEMprint(XML_STATUSFLAG_END);
#ifdef DisplayLEDPWM
#if defined(__SAM3X8E__)
	PROGMEMprint(XML_PWMA);
	print(ReefAngel.VariableControl.GetActinicValue(), DEC);
	PROGMEMprint(XML_PWMD);
	print(ReefAngel.VariableControl.GetDaylightValue(), DEC);
	PROGMEMprint(XML_PWMAO);
	print(ReefAngel.VariableControl.GetActinicOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO);
	print(ReefAngel.VariableControl.GetDaylightOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO_END);
#else
	PROGMEMprint(XML_PWMA);
	print(ReefAngel.PWM.GetActinicValue(), DEC);
	PROGMEMprint(XML_PWMD);
	print(ReefAngel.PWM.GetDaylightValue(), DEC);
	PROGMEMprint(XML_PWMAO);
	print(ReefAngel.PWM.GetActinicOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO);
	print(ReefAngel.PWM.GetDaylightOverrideValue(), DEC);
	PROGMEMprint(XML_PWMDO_END);
#endif
#if defined RA_STAR || defined RA_EVOLUTION
#if defined(__SAM3X8E__)
	PROGMEMprint(XML_PWMA2);
	print(ReefAngel.VariableControl.GetActinic2Value(), DEC);
	PROGMEMprint(XML_PWMD2);
	print(ReefAngel.VariableControl.GetDaylight2Value(), DEC);
	PROGMEMprint(XML_PWMA2O);
	print(ReefAngel.VariableControl.GetActinic2OverrideValue(), DEC);
	PROGMEMprint(XML_PWMD2O);
	print(ReefAngel.VariableControl.GetDaylight2OverrideValue(), DEC);
	PROGMEMprint(XML_PWMD2O_END);
#else
	PROGMEMprint(XML_PWMA2);
	print(ReefAngel.PWM.GetActinic2Value(), DEC);
	PROGMEMprint(XML_PWMD2);
	print(ReefAngel.PWM.GetDaylight2Value(), DEC);
	PROGMEMprint(XML_PWMA2O);
	print(ReefAngel.PWM.GetActinic2OverrideValue(), DEC);
	PROGMEMprint(XML_PWMD2O);
	print(ReefAngel.PWM.GetDaylight2OverrideValue(), DEC);
	PROGMEMprint(XML_PWMD2O_END);
#endif
#endif  // RA_STAR
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
#if defined WATERLEVELEXPANSION
	PROGMEMprint(XML_WL);
	PROGMEMprint(XML_CLOSE_TAG);
	print(ReefAngel.WaterLevel.GetLevel(), DEC);
	PROGMEMprint(XML_WL_END);
	PROGMEMprint(XML_CLOSE_TAG);
#endif  // WATERLEVELEXPANSION
#if defined MULTIWATERLEVELEXPANSION
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
#endif  // MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
	PROGMEMprint(XML_HUM);
	print(ReefAngel.Humidity.GetLevel(), DEC);
	PROGMEMprint(XML_HUM_END);
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
	PROGMEMprint(XML_PAR);
	print(ReefAngel.PAR.GetLevel(), DEC);
	PROGMEMprint(XML_PAR_END);
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
	PROGMEMprint(XML_DCM);
	print(ReefAngel.DCPump.Mode, DEC);
	PROGMEMprint(XML_DCM_END);
	print(ReefAngel.DCPump.Speed, DEC);
	PROGMEMprint(XML_DCS_END);
	print(ReefAngel.DCPump.Duration, DEC);
	PROGMEMprint(XML_DCD_END);
	print(ReefAngel.DCPump.Threshold, DEC);
	PROGMEMprint(XML_DCT_END);
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
#ifdef LEAKDETECTOREXPANSION
	PROGMEMprint(XML_LEAK);
	print(ReefAngel.IsLeakDetected(), DEC);
	PROGMEMprint(XML_LEAK_END);
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
	PROGMEMprint(XML_ALARM);
	print(ReefAngel.AlarmInput.IsActive(), DEC);
	PROGMEMprint(XML_ALARM_END);
#endif  // RA_STAR
#ifdef PWMEXPANSION
	for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
	{
		PROGMEMprint(XML_PWME);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
#if defined(__SAM3X8E__)
		print(ReefAngel.VariableControl.GetChannelValue(EID), DEC);
#else
		print(ReefAngel.PWM.GetChannelValue(EID), DEC);
#endif
		PROGMEMprint(XML_PWME_END);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		PROGMEMprint(XML_PWME);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
#if defined(__SAM3X8E__)
		print(ReefAngel.VariableControl.GetChannelOverrideValue(EID), DEC);
#else
		print(ReefAngel.PWM.GetChannelOverrideValue(EID), DEC);
#endif
		PROGMEMprint(XML_PWME_END);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
	for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ )
	{
		PROGMEMprint(XML_SCPWME);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
#if defined(__SAM3X8E__)
		print(ReefAngel.VariableControl.Get16ChannelValue(EID), DEC);
#else
		print(ReefAngel.PWM.Get16ChannelValue(EID), DEC);
#endif
		PROGMEMprint(XML_SCPWME_END);
		print(EID, DEC);
		PROGMEMprint(XML_CLOSE_TAG);
		PROGMEMprint(XML_SCPWME);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
#if defined(__SAM3X8E__)
		print(ReefAngel.VariableControl.Get16ChannelOverrideValue(EID), DEC);
#else
		print(ReefAngel.PWM.Get16ChannelOverrideValue(EID), DEC);
#endif
		PROGMEMprint(XML_SCPWME_END);
		print(EID, DEC);
		print("O");
		PROGMEMprint(XML_CLOSE_TAG);
	}
#endif  // SIXTEENCHPWMEXPANSION
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

#ifndef RA_STANDARD
void RA_Wifi::SendJSONData()
{
	// This function is used for sending the JSON data on the wifi interface
	// It prints the strings from program memory instead of RAM
	PROGMEMprint(JSON_OPEN);
	SendSingleJSON(JSON_ID,portalusername);
	SendSingleJSON(JSON_T1,ReefAngel.Params.Temp[T1_PROBE]);
	SendSingleJSON(JSON_T2,ReefAngel.Params.Temp[T2_PROBE]);
	SendSingleJSON(JSON_T3,ReefAngel.Params.Temp[T3_PROBE]);
	SendSingleJSON(JSON_PH,ReefAngel.Params.PH);
	SendSingleJSON(JSON_R,ReefAngel.Relay.RelayData);
	SendSingleJSON(JSON_RON,ReefAngel.Relay.RelayMaskOn);
	SendSingleJSON(JSON_ROFF,ReefAngel.Relay.RelayMaskOff);
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		// relay data
		char tid[2];
		tid[0]='0'+EID;
		tid[1]=0;
		SendSingleJSON(JSON_R,ReefAngel.Relay.RelayDataE[EID],tid);
		SendSingleJSON(JSON_RON,ReefAngel.Relay.RelayMaskOnE[EID],tid);
		SendSingleJSON(JSON_ROFF,ReefAngel.Relay.RelayMaskOffE[EID],tid);
	}
#endif  // RelayExp
	SendSingleJSON(JSON_ATOLOW,ReefAngel.LowATO.IsActive());
	SendSingleJSON(JSON_ATOHIGH,ReefAngel.HighATO.IsActive());
	SendSingleJSON(JSON_EM,ReefAngel.EM);
	SendSingleJSON(JSON_EM1,ReefAngel.EM1);
	SendSingleJSON(JSON_REM,ReefAngel.REM);
	SendSingleJSON(JSON_BOARDID,ReefAngel.Board);
	SendSingleJSON(JSON_ALERTFLAG,ReefAngel.AlertFlags);
	SendSingleJSON(JSON_STATUSFLAG,ReefAngel.StatusFlags);
#ifdef DisplayLEDPWM
#if defined(__SAM3X8E__)
	SendSingleJSON(JSON_PWMA,ReefAngel.VariableControl.GetActinicValue());
	SendSingleJSON(JSON_PWMD,ReefAngel.VariableControl.GetDaylightValue());
	SendSingleJSON(JSON_PWMA,ReefAngel.VariableControl.GetActinicOverrideValue(),"O");
	SendSingleJSON(JSON_PWMD,ReefAngel.VariableControl.GetDaylightOverrideValue(),"O");
#else
	SendSingleJSON(JSON_PWMA,ReefAngel.PWM.GetActinicValue());
	SendSingleJSON(JSON_PWMD,ReefAngel.PWM.GetDaylightValue());
	SendSingleJSON(JSON_PWMA,ReefAngel.PWM.GetActinicOverrideValue(),"O");
	SendSingleJSON(JSON_PWMD,ReefAngel.PWM.GetDaylightOverrideValue(),"O");
#endif
#if defined RA_STAR || defined RA_EVOLUTION
#if defined(__SAM3X8E__)
	SendSingleJSON(JSON_PWMA2,ReefAngel.VariableControl.GetActinic2Value());
	SendSingleJSON(JSON_PWMD2,ReefAngel.VariableControl.GetDaylight2Value());
	SendSingleJSON(JSON_PWMA2,ReefAngel.VariableControl.GetActinic2OverrideValue(),"O");
	SendSingleJSON(JSON_PWMD2,ReefAngel.VariableControl.GetDaylight2OverrideValue(),"O");
#else
	SendSingleJSON(JSON_PWMA2,ReefAngel.PWM.GetActinic2Value());
	SendSingleJSON(JSON_PWMD2,ReefAngel.PWM.GetDaylight2Value());
	SendSingleJSON(JSON_PWMA2,ReefAngel.PWM.GetActinic2OverrideValue(),"O");
	SendSingleJSON(JSON_PWMD2,ReefAngel.PWM.GetDaylight2OverrideValue(),"O");
#endif
#endif  // RA_STAR
#endif  // DisplayLEDPWM
#ifdef SALINITYEXPANSION
	SendSingleJSON(JSON_SAL,ReefAngel.Params.Salinity);
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
	SendSingleJSON(JSON_ORP,ReefAngel.Params.ORP);
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	SendSingleJSON(JSON_PHEXP,ReefAngel.Params.PHExp);
#endif  // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	SendSingleJSON(JSON_WL,ReefAngel.WaterLevel.GetLevel());
	for ( byte EID = 1; EID < WATERLEVEL_CHANNELS; EID++ )
	{
		char tid[2];
		tid[0]='0'+EID;
		tid[1]=0;
		SendSingleJSON(JSON_WL,ReefAngel.WaterLevel.GetLevel(EID),tid);
	}
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
	SendSingleJSON(JSON_HUM,ReefAngel.Humidity.GetLevel());
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
	SendSingleJSON(JSON_PAR,ReefAngel.PAR.GetLevel());
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
	SendSingleJSON(JSON_DCM,ReefAngel.DCPump.Mode);
	SendSingleJSON(JSON_DCS,ReefAngel.DCPump.Speed);
	SendSingleJSON(JSON_DCD,ReefAngel.DCPump.Duration);
	SendSingleJSON(JSON_DCT,ReefAngel.DCPump.Threshold);
#endif  // DCPUMPCONTROL
#ifdef IOEXPANSION
	SendSingleJSON(JSON_IO,ReefAngel.IO.GetChannel());
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
	for ( byte EID = 0; EID < 8; EID++ )
	{
		char tid[2];
		tid[0]='0'+EID;
		tid[1]=0;
		SendSingleJSON(JSON_C,ReefAngel.CustomVar[EID],tid);
	}
#endif  // CUSTOM_VARIABLES
#ifdef LEAKDETECTOREXPANSION
	SendSingleJSON(JSON_LEAK,ReefAngel.IsLeakDetected());
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
	SendSingleJSON(JSON_ALARM,ReefAngel.AlarmInput.IsActive());
#endif  // RA_STAR
#ifdef PWMEXPANSION
	for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
	{
		char tid[3];
		tid[0]='0'+EID;
		tid[1]=0;
#if defined(__SAM3X8E__)
		SendSingleJSON(JSON_PWME,ReefAngel.VariableControl.GetChannelValue(EID),tid);
#else
		SendSingleJSON(JSON_PWME,ReefAngel.PWM.GetChannelValue(EID),tid);
#endif
		tid[0]='0'+EID;
		tid[1]='O';
		tid[2]=0;
#if defined(__SAM3X8E__)
		SendSingleJSON(JSON_PWME,ReefAngel.VariableControl.GetChannelOverrideValue(EID),tid);
#else
		SendSingleJSON(JSON_PWME,ReefAngel.PWM.GetChannelOverrideValue(EID),tid);
#endif
	}
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
	for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ )
	{
		char tid[4];
		if (EID >= 10) {
      		tid[0]='1';
      		tid[1]='0'+(EID-10);
      		tid[2]=0;
      		} else {
      		tid[0]='0'+EID;
      		tid[1]=0;
      		}
#if defined(__SAM3X8E__)
		SendSingleJSON(JSON_SCPWME,ReefAngel.VariableControl.Get16ChannelValue(EID),tid);
#else
		SendSingleJSON(JSON_SCPWME,ReefAngel.PWM.Get16ChannelValue(EID),tid);
#endif
		if (EID >= 10) {
      		tid[0]='1';
      		tid[1]='0'+(EID-10);
      		tid[2]='O';
      		tid[3]=0;
      		} else {
      		tid[0]='0'+EID;
      		tid[1]='O';
      		tid[2]=0;
      		}
#if defined(__SAM3X8E__)
		SendSingleJSON(JSON_SCPWME,ReefAngel.VariableControl.Get16ChannelOverrideValue(EID),tid);
#else
		SendSingleJSON(JSON_SCPWME,ReefAngel.PWM.Get16ChannelOverrideValue(EID),tid);
#endif
	}
#endif  // SIXTEENCHPWMEXPANSION

#ifdef AI_LED
	SendSingleJSON(JSON_AIW,ReefAngel.AI.GetChannel(0));
	SendSingleJSON(JSON_AIB,ReefAngel.AI.GetChannel(0));
	SendSingleJSON(JSON_AIRB,ReefAngel.AI.GetChannel(0));
	SendSingleJSON(JSON_AIW,ReefAngel.AI.GetOverrideChannel(0),"O");
	SendSingleJSON(JSON_AIB,ReefAngel.AI.GetOverrideChannel(0),"O");
	SendSingleJSON(JSON_AIRB,ReefAngel.AI.GetOverrideChannel(0),"O");
#endif  // AI_LED
#ifdef RFEXPANSION
	SendSingleJSON(JSON_RFM,ReefAngel.RF.Mode);
	SendSingleJSON(JSON_RFS,ReefAngel.RF.Speed);
	SendSingleJSON(JSON_RFD,ReefAngel.RF.Duration);
	SendSingleJSON(JSON_RFW,ReefAngel.RF.GetChannel(0));
	SendSingleJSON(JSON_RFRB,ReefAngel.RF.GetChannel(1));
	SendSingleJSON(JSON_RFR,ReefAngel.RF.GetChannel(2));
	SendSingleJSON(JSON_RFG,ReefAngel.RF.GetChannel(3));
	SendSingleJSON(JSON_RFB,ReefAngel.RF.GetChannel(4));
	SendSingleJSON(JSON_RFI,ReefAngel.RF.GetChannel(5));
	SendSingleJSON(JSON_RFW,ReefAngel.RF.GetOverrideChannel(0),"O");
	SendSingleJSON(JSON_RFRB,ReefAngel.RF.GetOverrideChannel(1),"O");
	SendSingleJSON(JSON_RFR,ReefAngel.RF.GetOverrideChannel(2),"O");
	SendSingleJSON(JSON_RFG,ReefAngel.RF.GetOverrideChannel(3),"O");
	SendSingleJSON(JSON_RFB,ReefAngel.RF.GetOverrideChannel(4),"O");
	SendSingleJSON(JSON_RFI,ReefAngel.RF.GetOverrideChannel(5),"O");
#endif  // RFEXPANSION
	PROGMEMprint(JSON_CLOSE);
}

void RA_Wifi::SendSingleJSON(const prog_char str[], int value, char* suffix)
{
	print(",\"");
	PROGMEMprint(str);
	if (suffix!="")
		print(suffix);
	print("\":\"");
	print(value);
	print("\"");
}

void RA_Wifi::SendSingleJSON(const prog_char str[], char* value)
{
	print("\"");
	PROGMEMprint(str);
	print("\":\"");
	print(value);
	print("\"");
}

#endif // RA_STANDARD

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
      ReefAngel.WDTReset();
#endif  // defined WDT || defined WDT_FORCE
    }
  }

  ProcessHTTP();

  _wifiSerial->flush();
  m_pushbackindex=0;
  auth=false;
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
#ifdef ETH_WIZ5100
  if (ReefAngel.Network.PortalConnection && ReefAngel.Network.FoundIP) SendPortal(username,"");
#endif
  portalusername=username;
}

void RA_Wifi::Portal(char *username, char *key)
{
  if (ReefAngel.Timer[PORTAL_TIMER].IsTriggered()) SendPortal(username,key);
#ifdef ETH_WIZ5100
  if (ReefAngel.Network.PortalConnection && ReefAngel.Network.FoundIP) SendPortal(username,key); // Let's keep checking for Portal connection
#endif
  portalusername=username;
  portalkey=key;
}

void RA_Wifi::DDNS(char *subdomain)
{
	portalsubdomain=subdomain;
}

void RA_Wifi::SendPortal(char *username, char*key)
{
  ReefAngel.Timer[PORTAL_TIMER].Start();
#ifdef ETH_WIZ5100
  Serial.println("Portal Call");
  if (!ReefAngel.Network.FoundIP) return;
  if (!ReefAngel.Network.PortalConnection)
  {
	  ReefAngel.Network.PortalConnection=true;
	  PortalWaiting=false;
	  ReefAngel.Network.PortalConnect();
	  Serial.println("Connecting...");
  }
  else
  {
	if (ReefAngel.Network.IsPortalConnected() && !PortalWaiting) // Check for connection established
	{
		PortalWaiting=true;
		Serial.println("Connected");
#endif
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
  PROGMEMprint(BannerBoardID);
  print(ReefAngel.Board, DEC);
  PROGMEMprint(BannerKey);
  print(key);
  PROGMEMprint(BannerSubdomain);
  print(portalsubdomain);
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
#if defined(__SAM3X8E__)
  PROGMEMprint(BannerPWMA);
  print(ReefAngel.VariableControl.GetActinicValue(), DEC);
  PROGMEMprint(BannerPWMD);
  print(ReefAngel.VariableControl.GetDaylightValue(), DEC);
  PROGMEMprint(BannerPWMAO);
  print(ReefAngel.VariableControl.GetActinicOverrideValue(), DEC);
  PROGMEMprint(BannerPWMDO);
  print(ReefAngel.VariableControl.GetDaylightOverrideValue(), DEC);
#else
  PROGMEMprint(BannerPWMA);
  print(ReefAngel.PWM.GetActinicValue(), DEC);
  PROGMEMprint(BannerPWMD);
  print(ReefAngel.PWM.GetDaylightValue(), DEC);
  PROGMEMprint(BannerPWMAO);
  print(ReefAngel.PWM.GetActinicOverrideValue(), DEC);
  PROGMEMprint(BannerPWMDO);
  print(ReefAngel.PWM.GetDaylightOverrideValue(), DEC);
#endif
#endif  // DisplayLEDPWM && ! defined RemoveAllLights
#ifdef PWMEXPANSION
  for ( byte EID = 0; EID < PWM_EXPANSION_CHANNELS; EID++ )
  {
    PROGMEMprint(BannerPWME);
    print(EID, DEC);
    print("=");
#if defined(__SAM3X8E__)
    print(ReefAngel.VariableControl.GetChannelValue(EID), DEC);
#else
    print(ReefAngel.PWM.GetChannelValue(EID), DEC);
#endif
    PROGMEMprint(BannerPWME);
    print(EID, DEC);
    print("O");
    print("=");
#if defined(__SAM3X8E__)
    print(ReefAngel.VariableControl.GetChannelOverrideValue(EID), DEC);
#else
    print(ReefAngel.PWM.GetChannelOverrideValue(EID), DEC);
#endif
  }
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
  for ( byte EID = 0; EID < SIXTEENCH_PWM_EXPANSION_CHANNELS; EID++ )
  {
    PROGMEMprint(BannerSCPWME);
    print(EID, DEC);
    print("=");
#if defined(__SAM3X8E__)
    print(ReefAngel.VariableControl.Get16ChannelValue(EID), DEC);
#else
    print(ReefAngel.PWM.Get16ChannelValue(EID), DEC);
#endif
    PROGMEMprint(BannerSCPWME);
    print(EID, DEC);
    print("O");
    print("=");
#if defined(__SAM3X8E__)
    print(ReefAngel.VariableControl.Get16ChannelOverrideValue(EID), DEC);
#else
    print(ReefAngel.PWM.Get16ChannelOverrideValue(EID), DEC);
#endif
  }
#endif  // SIXTEENCHPWMEXPANSION
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
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
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
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
  PROGMEMprint(BannerHumidity);
  print(ReefAngel.Humidity.GetLevel(), DEC);
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
  PROGMEMprint(BannerPAR);
  print(ReefAngel.PAR.GetLevel(), DEC);
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
  PROGMEMprint(BannerDCM);
  print(ReefAngel.DCPump.Mode, DEC);
  PROGMEMprint(BannerDCS);
  print(ReefAngel.DCPump.Speed, DEC);
  PROGMEMprint(BannerDCD);
  print(ReefAngel.DCPump.Duration, DEC);
  PROGMEMprint(BannerDCT);
  print(ReefAngel.DCPump.Threshold, DEC);
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
#ifdef LEAKDETECTOREXPANSION
  PROGMEMprint(BannerLeak);
  print(ReefAngel.IsLeakDetected(), DEC);
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
  PROGMEMprint(BannerAlarm);
  print(ReefAngel.AlarmInput.IsActive(), DEC);
#if defined DisplayLEDPWM && ! defined RemoveAllLights
#if defined(__SAM3X8E__)
  PROGMEMprint(BannerPWMA2);
  print(ReefAngel.VariableControl.GetActinic2Value(), DEC);
  PROGMEMprint(BannerPWMD2);
  print(ReefAngel.VariableControl.GetDaylight2Value(), DEC);
  PROGMEMprint(BannerPWMA2O);
  print(ReefAngel.VariableControl.GetActinic2OverrideValue(), DEC);
  PROGMEMprint(BannerPWMD2O);
  print(ReefAngel.VariableControl.GetDaylight2OverrideValue(), DEC);
#else
  PROGMEMprint(BannerPWMA2);
  print(ReefAngel.PWM.GetActinic2Value(), DEC);
  PROGMEMprint(BannerPWMD2);
  print(ReefAngel.PWM.GetDaylight2Value(), DEC);
  PROGMEMprint(BannerPWMA2O);
  print(ReefAngel.PWM.GetActinic2OverrideValue(), DEC);
  PROGMEMprint(BannerPWMD2O);
  print(ReefAngel.PWM.GetDaylight2OverrideValue(), DEC);
#endif  
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
#endif  // RA_STAR
#ifdef ETH_WIZ5100
  PROGMEMprint(BannerHTTP11);
  PROGMEMprint(BannerHost);
  PROGMEMprint(BannerConnectionClose);
  ReefAngel.Network.PortalTimeOut=millis();
#endif // ETH_WIZ5100
  println("\n\n");
#ifdef ETH_WIZ5100
	Serial.println("Data Sent");
	}
  }
#endif // ETH_WIZ5100
}
#endif  // wifi
