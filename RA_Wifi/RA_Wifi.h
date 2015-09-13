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

#ifndef __RA_WIFI_H__
#define __RA_WIFI_H__

#include <Globals.h>

#if defined wifi || defined ETH_WIZ5100
#include <avr/pgmspace.h>
#include <avr/wdt.h>

const prog_char XML_ID[] PROGMEM = "<RA><ID>";
const prog_char XML_T1[] PROGMEM = "</ID><T1>";
const prog_char XML_T2[] PROGMEM = "</T1><T2>";
const prog_char XML_T3[] PROGMEM = "</T2><T3>";
const prog_char XML_PH[] PROGMEM = "</T3><PH>";
const prog_char XML_R[] PROGMEM = "</PH><R>";
const prog_char XML_RON[] PROGMEM = "</R><RON>";
const prog_char XML_ROFF[] PROGMEM = "</RON><ROFF>";
const prog_char XML_RE_OPEN[] PROGMEM = "<R";
const prog_char XML_RE_CLOSE[] PROGMEM = "</R";
const prog_char XML_RE_ON[] PROGMEM = "ON";
const prog_char XML_RE_OFF[] PROGMEM = "OFF";
const prog_char XML_ATOLOW[] PROGMEM = "<ATOLOW>";
const prog_char XML_ATOHIGH[] PROGMEM = "</ATOLOW><ATOHIGH>";
const prog_char XML_EM[] PROGMEM = "</ATOHIGH><EM>";
const prog_char XML_EM1[] PROGMEM = "</EM><EM1>";
const prog_char XML_REM[] PROGMEM = "</EM1><REM>";
const prog_char XML_BOARDID[] PROGMEM = "</REM><BID>";
const prog_char XML_ALERTFLAG[] PROGMEM = "</BID><AF>";
const prog_char XML_STATUSFLAG[] PROGMEM = "</AF><SF>";
const prog_char XML_STATUSFLAG_END[] PROGMEM = "</SF>";

#ifdef DisplayLEDPWM
const prog_char XML_PWMA[] PROGMEM = "<PWMA>";
const prog_char XML_PWMD[] PROGMEM = "</PWMA><PWMD>";
const prog_char XML_PWMAO[] PROGMEM = "</PWMD><PWMAO>";
const prog_char XML_PWMDO[] PROGMEM = "</PWMAO><PWMDO>";
const prog_char XML_PWMDO_END[] PROGMEM = "</PWMDO>";
#endif  // DisplayLEDPWM
#ifdef ORPEXPANSION
const prog_char XML_ORP[] PROGMEM = "<ORP>";
const prog_char XML_ORP_END[] PROGMEM = "</ORP>";
#endif  // ORPEXPANSION
#ifdef SALINITYEXPANSION
const prog_char XML_SAL[] PROGMEM = "<SAL>";
const prog_char XML_SAL_END[] PROGMEM = "</SAL>";
#endif  // SALINITYEXPANSION
#ifdef PHEXPANSION
const prog_char XML_PHEXP[] PROGMEM = "<PHE>";
const prog_char XML_PHEXP_END[] PROGMEM = "</PHE>";
#endif  // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
const prog_char XML_WL[] PROGMEM = "<WL";
const prog_char XML_WL_END[] PROGMEM = "</WL";
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
const prog_char XML_HUM[] PROGMEM = "<HUM>";
const prog_char XML_HUM_END[] PROGMEM = "</HUM>";
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
const prog_char XML_PAR[] PROGMEM = "<PAR>";
const prog_char XML_PAR_END[] PROGMEM = "</PAR>";
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
const prog_char XML_DCM[] PROGMEM = "<DCM>";
const prog_char XML_DCM_END[] PROGMEM = "</DCM><DCS>";
const prog_char XML_DCS_END[] PROGMEM = "</DCS><DCD>";
const prog_char XML_DCD_END[] PROGMEM = "</DCD><DCT>";
const prog_char XML_DCT_END[] PROGMEM = "</DCT>";
#endif  // DCPUMPCONTROL
#ifdef PWMEXPANSION
const prog_char XML_PWME[] PROGMEM = "<PWME";
const prog_char XML_PWME_END[] PROGMEM = "</PWME";
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
const prog_char XML_SCPWME[] PROGMEM = "<SCPWME";
const prog_char XML_SCPWME_END[] PROGMEM = "</SCPWME";
#endif  // SIXTEENCHPWMEXPANSION
#ifdef AI_LED
const prog_char XML_AIW[] PROGMEM = "<AIW>";
const prog_char XML_AIW_END[] PROGMEM = "</AIW><AIB>";
const prog_char XML_AIB_END[] PROGMEM = "</AIB><AIRB>";
const prog_char XML_AIRB_END[] PROGMEM = "</AIRB><AIWO>";
const prog_char XML_AIWO_END[] PROGMEM = "</AIWO><AIBO>";
const prog_char XML_AIBO_END[] PROGMEM = "</AIBO><AIRBO>";
const prog_char XML_AIRBO_END[] PROGMEM = "</AIRBO>";
#endif  // AI_LED
#ifdef RFEXPANSION
const prog_char XML_RFM[] PROGMEM = "<RFM>";
const prog_char XML_RFM_END[] PROGMEM = "</RFM><RFS>";
const prog_char XML_RFS_END[] PROGMEM = "</RFS><RFD>";
const prog_char XML_RFD_END[] PROGMEM = "</RFD><RFW>";
const prog_char XML_RFW_END[] PROGMEM = "</RFW><RFRB>";
const prog_char XML_RFRB_END[] PROGMEM = "</RFRB><RFR>";
const prog_char XML_RFR_END[] PROGMEM = "</RFR><RFG>";
const prog_char XML_RFG_END[] PROGMEM = "</RFG><RFB>";
const prog_char XML_RFB_END[] PROGMEM = "</RFB><RFI>";
const prog_char XML_RFI_END[] PROGMEM = "</RFI><RFWO>";
const prog_char XML_RFWO_END[] PROGMEM = "</RFWO><RFRBO>";
const prog_char XML_RFRBO_END[] PROGMEM = "</RFRBO><RFRO>";
const prog_char XML_RFRO_END[] PROGMEM = "</RFRO><RFGO>";
const prog_char XML_RFGO_END[] PROGMEM = "</RFGO><RFBO>";
const prog_char XML_RFBO_END[] PROGMEM = "</RFBO><RFIO>";
const prog_char XML_RFIO_END[] PROGMEM = "</RFIO>";
#endif  // RFEXPANSION
#ifdef IOEXPANSION
const prog_char XML_IO[] PROGMEM = "<IO>";
const prog_char XML_IO_END[] PROGMEM = "</IO>";
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
const prog_char XML_C[] PROGMEM = "<C";
const prog_char XML_C_END[] PROGMEM = "</C";
#endif  // CUSTOM_VARIABLES
#ifdef LEAKDETECTOREXPANSION
const prog_char XML_LEAK[] PROGMEM = "<LEAK>";
const prog_char XML_LEAK_END[] PROGMEM = "</LEAK>";
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
const prog_char XML_ALARM[] PROGMEM = "<ALARM>";
const prog_char XML_ALARM_END[] PROGMEM = "</ALARM>";
const prog_char XML_PWMA2[] PROGMEM = "<PWMA2>";
const prog_char XML_PWMD2[] PROGMEM = "</PWMA2><PWMD2>";
const prog_char XML_PWMA2O[] PROGMEM = "</PWMD2><PWMA2O>";
const prog_char XML_PWMD2O[] PROGMEM = "</PWMA2O><PWMD2O>";
const prog_char XML_PWMD2O_END[] PROGMEM = "</PWMD2O>";
#endif  // RA_STAR

#ifdef ENABLE_ATO_LOGGING
const prog_char XML_ATOLOW_LOG_OPEN[] PROGMEM = "<AL";
const prog_char XML_ATOLOW_LOG_CLOSE[] PROGMEM = "</AL";
const prog_char XML_ATOHIGH_LOG_OPEN[] PROGMEM = "<AH";
const prog_char XML_ATOHIGH_LOG_CLOSE[] PROGMEM = "</AH";
#endif  // ENABLE_ATO_LOGGING
const prog_char XML_END[] PROGMEM = "</RA>";
const prog_char XML_CLOSE_TAG[] PROGMEM = ">";


const prog_char XML_P_OPEN[] PROGMEM = "<P";
const prog_char XML_P_CLOSE[] PROGMEM = "</P";
const prog_char XML_M_OPEN[] PROGMEM = "<M";
const prog_char XML_M_CLOSE[] PROGMEM = "</M";
const prog_char XML_MEM_OPEN[] PROGMEM = "<MEM>";
const prog_char XML_MEM_CLOSE[] PROGMEM = "</MEM>";
const prog_char XML_DATE_OPEN[] PROGMEM = "<D>";
const prog_char XML_DATE_CLOSE[] PROGMEM = "</D>";
const prog_char XML_MODE_OPEN[] PROGMEM = "<MODE>";
const prog_char XML_MODE_CLOSE[] PROGMEM = "</MODE>";
const prog_char XML_OK[] PROGMEM = "OK";
const prog_char XML_ERR[] PROGMEM = "ERR";

const prog_char SERVER_HEADER1[] PROGMEM = "HTTP/1.1 200 OK\r\nServer: ReefAngel\r\nCache-Control: no-store, no-cache, must-revalidate\r\nPragma: no-cache\r\nConnection: close\r\nContent-Type: text/";
const prog_char SERVER_HEADER2[] PROGMEM = "\r\nContent-Length: ";
const prog_char SERVER_DENY[] PROGMEM = "HTTP/1.1 401 Access Denied\r\nWWW-Authenticate: Basic realm=Reef Angel Controller\r\nContent-Length: 0\r\n";
const prog_char SERVER_DEFAULT[] PROGMEM = "<h1>Reef Angel Controller Web Server</h1>";

#ifndef RA_STANDARD
const prog_char JSON_OPEN[] PROGMEM = "{\"json\":{";
const prog_char JSON_ID[] PROGMEM = "ID";
const prog_char JSON_T1[] PROGMEM = "T1";
const prog_char JSON_T2[] PROGMEM = "T2";
const prog_char JSON_T3[] PROGMEM = "T3";
const prog_char JSON_PH[] PROGMEM = "PH";
const prog_char JSON_R[] PROGMEM = "R";
const prog_char JSON_RON[] PROGMEM = "RON";
const prog_char JSON_ROFF[] PROGMEM = "ROFF";
const prog_char JSON_R1[] PROGMEM = "R1";
const prog_char JSON_RON1[] PROGMEM = "RON1";
const prog_char JSON_ROFF1[] PROGMEM = "ROFF1";
const prog_char JSON_R2[] PROGMEM = "R2";
const prog_char JSON_RON2[] PROGMEM = "RON2";
const prog_char JSON_ROFF2[] PROGMEM = "ROFF2";
const prog_char JSON_R3[] PROGMEM = "R3";
const prog_char JSON_RON3[] PROGMEM = "RON3";
const prog_char JSON_ROFF3[] PROGMEM = "ROFF3";
const prog_char JSON_R4[] PROGMEM = "R4";
const prog_char JSON_RON4[] PROGMEM = "RON4";
const prog_char JSON_ROFF4[] PROGMEM = "ROFF4";
const prog_char JSON_R5[] PROGMEM = "R5";
const prog_char JSON_RON5[] PROGMEM = "RON5";
const prog_char JSON_ROFF5[] PROGMEM = "ROFF5";
const prog_char JSON_R6[] PROGMEM = "R6";
const prog_char JSON_RON6[] PROGMEM = "RON6";
const prog_char JSON_ROFF6[] PROGMEM = "ROFF6";
const prog_char JSON_R7[] PROGMEM = "R7";
const prog_char JSON_RON7[] PROGMEM = "RON7";
const prog_char JSON_ROFF7[] PROGMEM = "ROFF7";
const prog_char JSON_R8[] PROGMEM = "R8";
const prog_char JSON_RON8[] PROGMEM = "RON8";
const prog_char JSON_ROFF8[] PROGMEM = "ROFF8";
const prog_char JSON_ATOLOW[] PROGMEM = "ATOLOW";
const prog_char JSON_ATOHIGH[] PROGMEM = "ATOHIGH";
const prog_char JSON_EM[] PROGMEM = "EM";
const prog_char JSON_EM1[] PROGMEM = "EM1";
const prog_char JSON_REM[] PROGMEM = "REM";
const prog_char JSON_ALERTFLAG[] PROGMEM = "AF";
const prog_char JSON_STATUSFLAG[] PROGMEM = "SF";
const prog_char JSON_BOARDID[] PROGMEM = "BID";

#ifdef DisplayLEDPWM
const prog_char JSON_PWMA[] PROGMEM = "PWMA";
const prog_char JSON_PWMD[] PROGMEM = "PWMD";
const prog_char JSON_PWMAO[] PROGMEM = "PWMAO";
const prog_char JSON_PWMDO[] PROGMEM = "PWMDO";
#endif  // DisplayLEDPWM
#ifdef ORPEXPANSION
const prog_char JSON_ORP[] PROGMEM = "ORP";
#endif  // ORPEXPANSION
#ifdef SALINITYEXPANSION
const prog_char JSON_SAL[] PROGMEM = "SAL";
#endif  // SALINITYEXPANSION
#ifdef PHEXPANSION
const prog_char JSON_PHEXP[] PROGMEM = "PHE";
#endif  // PHEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
const prog_char JSON_WL[] PROGMEM = "WL";
const prog_char JSON_WL1[] PROGMEM = "WL1";
const prog_char JSON_WL2[] PROGMEM = "WL2";
const prog_char JSON_WL3[] PROGMEM = "WL3";
const prog_char JSON_WL4[] PROGMEM = "WL4";
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef HUMIDITYEXPANSION
const prog_char JSON_HUM[] PROGMEM = "HUM";
#endif  // HUMIDITYEXPANSION
#ifdef PAREXPANSION
const prog_char JSON_PAR[] PROGMEM = "PAR";
#endif  // PAREXPANSION
#ifdef DCPUMPCONTROL
const prog_char JSON_DCM[] PROGMEM = "DCM";
const prog_char JSON_DCS[] PROGMEM = "DCS";
const prog_char JSON_DCD[] PROGMEM = "DCD";
const prog_char JSON_DCT[] PROGMEM = "DCT";
#endif  // DCPUMPCONTROL
#ifdef PWMEXPANSION
const prog_char JSON_PWME[] PROGMEM = "PWME";
const prog_char JSON_PWME0[] PROGMEM = "PWME0";
const prog_char JSON_PWME1[] PROGMEM = "PWME1";
const prog_char JSON_PWME2[] PROGMEM = "PWME2";
const prog_char JSON_PWME3[] PROGMEM = "PWME3";
const prog_char JSON_PWME4[] PROGMEM = "PWME4";
const prog_char JSON_PWME5[] PROGMEM = "PWME5";
const prog_char JSON_PWMEO[] PROGMEM = "PWMEO";
const prog_char JSON_PWME0O[] PROGMEM = "PWME0O";
const prog_char JSON_PWME1O[] PROGMEM = "PWME1O";
const prog_char JSON_PWME2O[] PROGMEM = "PWME2O";
const prog_char JSON_PWME3O[] PROGMEM = "PWME3O";
const prog_char JSON_PWME4O[] PROGMEM = "PWME4O";
const prog_char JSON_PWME5O[] PROGMEM = "PWME5O";
#endif  // PWMEXPANSION
#ifdef SIXTEENCHPWMEXPANSION
const prog_char JSON_SCPWME[] PROGMEM = "SCPWME";
#endif  // SIXTEENCHPWMEXPANSION
#ifdef AI_LED
const prog_char JSON_AIW[] PROGMEM = "AIW";
const prog_char JSON_AIB[] PROGMEM = "AIB";
const prog_char JSON_AIRB[] PROGMEM = "AIRB";
const prog_char JSON_AIWO[] PROGMEM = "AIWO";
const prog_char JSON_AIBO[] PROGMEM = "AIBO";
const prog_char JSON_AIRBO[] PROGMEM = "AIRBO";
#endif  // AI_LED
#ifdef RFEXPANSION
const prog_char JSON_RFM[] PROGMEM = "RFM";
const prog_char JSON_RFS[] PROGMEM = "RFS";
const prog_char JSON_RFD[] PROGMEM = "RFD";
const prog_char JSON_RFW[] PROGMEM = "RFW";
const prog_char JSON_RFRB[] PROGMEM = "RFRB";
const prog_char JSON_RFR[] PROGMEM = "RFR";
const prog_char JSON_RFG[] PROGMEM = "RFG";
const prog_char JSON_RFB[] PROGMEM = "RFB";
const prog_char JSON_RFI[] PROGMEM = "RFI";
const prog_char JSON_RFWO[] PROGMEM = "RFWO";
const prog_char JSON_RFRBO[] PROGMEM = "RFRBO";
const prog_char JSON_RFRO[] PROGMEM = "RFRO";
const prog_char JSON_RFGO[] PROGMEM = "RFGO";
const prog_char JSON_RFBO[] PROGMEM = "RFBO";
const prog_char JSON_RFIO[] PROGMEM = "RFIO";
#endif  // RFEXPANSION
#ifdef IOEXPANSION
const prog_char JSON_IO[] PROGMEM = "IO";
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
const prog_char JSON_C[] PROGMEM = "C";
const prog_char JSON_C0[] PROGMEM = "C0";
const prog_char JSON_C1[] PROGMEM = "C1";
const prog_char JSON_C2[] PROGMEM = "C2";
const prog_char JSON_C3[] PROGMEM = "C3";
const prog_char JSON_C4[] PROGMEM = "C4";
const prog_char JSON_C5[] PROGMEM = "C5";
const prog_char JSON_C6[] PROGMEM = "C6";
const prog_char JSON_C7[] PROGMEM = "C7";
#endif  // CUSTOM_VARIABLES
#ifdef LEAKDETECTOREXPANSION
const prog_char JSON_LEAK[] PROGMEM = "LEAK";
#endif  // LEAKDETECTOREXPANSION
#if defined RA_STAR || defined RA_EVOLUTION
const prog_char JSON_ALARM[] PROGMEM = "ALARM";
const prog_char JSON_PWMA2[] PROGMEM = "PWMA2";
const prog_char JSON_PWMD2[] PROGMEM = "PWMD2";
const prog_char JSON_PWMA2O[] PROGMEM = "PWMA2O";
const prog_char JSON_PWMD2O[] PROGMEM = "PWMD2O";
const prog_char JSON_CEXP0[] PROGMEM = "CEXP0";
const prog_char JSON_CEXP1[] PROGMEM = "CEXP1";
const prog_char JSON_CEXP2[] PROGMEM = "CEXP2";
const prog_char JSON_CEXP3[] PROGMEM = "CEXP3";
const prog_char JSON_CEXP4[] PROGMEM = "CEXP4";
const prog_char JSON_CEXP5[] PROGMEM = "CEXP5";
const prog_char JSON_CEXP6[] PROGMEM = "CEXP6";
const prog_char JSON_CEXP7[] PROGMEM = "CEXP7";

#endif  // RA_STAR
const prog_char JSON_CLOSE[] PROGMEM = "}}";

#endif // RA_STANDARD

// REQUEST TYPES
#define REQ_ROOT		1		// Default page
#define REQ_WIFI		2		// Wifi page
#define REQ_RELAY		3		// Relay status information
#define REQ_M_BYTE		4  		// Memory read/write byte
#define REQ_M_INT		5  		// Memory read/write int
#define REQ_M_ALL		6  		// All memory values
#define REQ_VERSION		7  		// Library version
#define REQ_DATE		8  		// Date and time
#define REQ_R_STATUS	9		// Relay status information
#define REQ_RA_STATUS	10		// Relay status information + ato logging
#define REQ_FEEDING		11		// Start feeding mode
#define REQ_WATER		12		// Start water change mode
#define REQ_BTN_PRESS	13		// Simulate a button press, to end feeding & water change modes
#define REQ_CAL_RELOAD	14		// Reload calibration values from memory
#define REQ_ALARM_ATO	15		// Clears the ATO alarm
#define REQ_ALARM_OVERHEAT	16	// Clears the Overheat alarm
#define REQ_M_RAW		17  	// All memory raw values
#define REQ_LIGHTSON	18		// Turn Lights On
#define REQ_LIGHTSOFF	19		// Turn Lights Off
#define REQ_ALARM_LEAK	20		// Clears the Leak alarm
#define REQ_OVERRIDE	21		// Channel Override
#define REQ_REBOOT		22		// Reboot
#define REQ_M_CVAR		23		// Custom Variable
#define REQ_CALIBRATION	24		// Calibration
#define REQ_JSON		25		// JSON export
#define REQ_HTTP		127		// HTTP get request from  external server
#define REQ_UNKNOWN		128	 	// Unknown request

#define P(name)   static const prog_char name[] PROGMEM
//const prog_char SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const prog_char SERVER_RA[] PROGMEM = "<!DOCTYPE html><script src='http://www.reefangel.com/wifi/ra2.js'></script>";
const prog_char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const prog_char BannerGET[] PROGMEM = "GET /status/submitp.aspx?t1=";
const prog_char BannerHTTP11[] PROGMEM = " HTTP/1.1\r\n";
const prog_char BannerHost[] PROGMEM = "Host: forum.reefangel.com\r\n";
const prog_char BannerConnectionClose[] PROGMEM = "Connection: close\r\n";
const prog_char BannerT2[] PROGMEM = "&t2=";
const prog_char BannerT3[] PROGMEM = "&t3=";
const prog_char BannerPH[] PROGMEM = "&ph=";
const prog_char BannerATOHIGH[] PROGMEM = "&atohigh=";
const prog_char BannerATOLOW[] PROGMEM = "&atolow=";
const prog_char BannerRelayData[] PROGMEM = "&r";
const prog_char BannerRelayMaskOn[] PROGMEM = "&ron";
const prog_char BannerRelayMaskOff[] PROGMEM = "&roff";
const prog_char BannerID[] PROGMEM = "&id=";
const prog_char BannerEM[] PROGMEM = "&em=";
const prog_char BannerEM1[] PROGMEM = "&em1=";
const prog_char BannerREM[] PROGMEM = "&rem=";
const prog_char BannerCustom[] PROGMEM = "&c";
const prog_char BannerKey[] PROGMEM = "&key=";
const prog_char BannerAlertFlag[] PROGMEM = "&af=";
const prog_char BannerStatusFlag[] PROGMEM = "&sf=";
const prog_char BannerBoardID[] PROGMEM = "&bid=";
const prog_char BannerSubdomain[] PROGMEM = "&ddns=";

#if defined DisplayLEDPWM && ! defined RemoveAllLights
	const prog_char BannerPWMA[] PROGMEM = "&pwma=";
	const prog_char BannerPWMD[] PROGMEM = "&pwmd=";
	const prog_char BannerPWMAO[] PROGMEM = "&pwmao=";
	const prog_char BannerPWMDO[] PROGMEM = "&pwmdo=";
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

#ifdef PWMEXPANSION
	const prog_char BannerPWME[] PROGMEM = "&pwme";
#endif  // PWMEXPANSION

#ifdef SIXTEENCHPWMEXPANSION
	const prog_char BannerSCPWME[] PROGMEM = "&scpwme";
#endif  // SIXTEENCHPWMEXPANSION

#ifdef RFEXPANSION
	const prog_char BannerRFM[] PROGMEM = "&rfm=";
	const prog_char BannerRFS[] PROGMEM = "&rfs=";
	const prog_char BannerRFD[] PROGMEM = "&rfd=";
	const prog_char BannerRFW[] PROGMEM = "&rfw=";
	const prog_char BannerRFRB[] PROGMEM = "&rfrb=";
	const prog_char BannerRFR[] PROGMEM = "&rfr=";
	const prog_char BannerRFG[] PROGMEM = "&rfg=";
	const prog_char BannerRFB[] PROGMEM = "&rfb=";
	const prog_char BannerRFI[] PROGMEM = "&rfi=";
#endif  // RFEXPANSION

#ifdef AI_LED
	const prog_char BannerAIW[] PROGMEM = "&aiw=";
	const prog_char BannerAIB[] PROGMEM = "&aib=";
	const prog_char BannerAIRB[] PROGMEM = "&airb=";
#endif  // AI_LED

#ifdef SALINITYEXPANSION
	const prog_char BannerSal[] PROGMEM = "&sal=";
#endif  // SALINITYEXPANSION

#ifdef ORPEXPANSION
	const prog_char BannerORP[] PROGMEM = "&orp=";
#endif  // ORPEXPANSION

#ifdef IOEXPANSION
	const prog_char BannerIO[] PROGMEM = "&io=";
#endif  // IOEXPANSION

#ifdef PHEXPANSION
	const prog_char BannerPHE[] PROGMEM = "&phe=";
#endif  // PHEXPANSION

#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	const prog_char BannerWL[] PROGMEM = "&wl";
#endif  // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION

#ifdef HUMIDITYEXPANSION
	const prog_char BannerHumidity[] PROGMEM = "&hum=";
#endif  // HUMIDITYEXPANSION

#ifdef PAREXPANSION
	const prog_char BannerPAR[] PROGMEM = "&par=";
#endif  // PAREXPANSION

#ifdef DCPUMPCONTROL
	const prog_char BannerDCM[] PROGMEM = "&dcm=";
	const prog_char BannerDCS[] PROGMEM = "&dcs=";
	const prog_char BannerDCD[] PROGMEM = "&dcd=";
	const prog_char BannerDCT[] PROGMEM = "&dct=";
#endif  // DCPUMPCONTROL

#ifdef CUSTOM_VARIABLES
	const prog_char BannerCustomVar[] PROGMEM = "&c";
#endif //CUSTOM_VARIABLES

#ifdef LEAKDETECTOREXPANSION
	const prog_char BannerLeak[] PROGMEM = "&leak=";
#endif  // LEAKDETECTOREXPANSION

#if defined RA_STAR || defined RA_EVOLUTION
	const prog_char BannerAlarm[] PROGMEM = "&alarm=";
	const prog_char BannerPWMA2[] PROGMEM = "&pwma2=";
	const prog_char BannerPWMD2[] PROGMEM = "&pwmd2=";
	const prog_char BannerPWMA2O[] PROGMEM = "&pwma2o=";
	const prog_char BannerPWMD2O[] PROGMEM = "&pwmd2o=";
#endif  // RA_STAR

//const prog_char probe1_tag[] PROGMEM = "t1n";
//const prog_char probe2_tag[] PROGMEM = "t2n";
//const prog_char probe3_tag[] PROGMEM = "t3n";
//const prog_char relay1_tag[] PROGMEM = "r1n";
//const prog_char relay2_tag[] PROGMEM = "r2n";
//const prog_char relay3_tag[] PROGMEM = "r3n";
//const prog_char relay4_tag[] PROGMEM = "r4n";
//const prog_char relay5_tag[] PROGMEM = "r5n";
//const prog_char relay6_tag[] PROGMEM = "r6n";
//const prog_char relay7_tag[] PROGMEM = "r7n";
//const prog_char relay8_tag[] PROGMEM = "r8n";
//#ifdef RelayExp
//#if InstalledRelayExpansionModules >= 1
//const prog_char relay11_tag[] PROGMEM = "r11n";
//const prog_char relay12_tag[] PROGMEM = "r12n";
//const prog_char relay13_tag[] PROGMEM = "r13n";
//const prog_char relay14_tag[] PROGMEM = "r14n";
//const prog_char relay15_tag[] PROGMEM = "r15n";
//const prog_char relay16_tag[] PROGMEM = "r16n";
//const prog_char relay17_tag[] PROGMEM = "r17n";
//const prog_char relay18_tag[] PROGMEM = "r18n";
//#endif  // InstalledRelayExpansionModules >= 1
//#if InstalledRelayExpansionModules >= 2
//const prog_char relay21_tag[] PROGMEM = "r21n";
//const prog_char relay22_tag[] PROGMEM = "r22n";
//const prog_char relay23_tag[] PROGMEM = "r23n";
//const prog_char relay24_tag[] PROGMEM = "r24n";
//const prog_char relay25_tag[] PROGMEM = "r25n";
//const prog_char relay26_tag[] PROGMEM = "r26n";
//const prog_char relay27_tag[] PROGMEM = "r27n";
//const prog_char relay28_tag[] PROGMEM = "r28n";
//#endif  // InstalledRelayExpansionModules >= 2
//#if InstalledRelayExpansionModules >= 3
//const prog_char relay31_tag[] PROGMEM = "r31n";
//const prog_char relay32_tag[] PROGMEM = "r32n";
//const prog_char relay33_tag[] PROGMEM = "r33n";
//const prog_char relay34_tag[] PROGMEM = "r34n";
//const prog_char relay35_tag[] PROGMEM = "r35n";
//const prog_char relay36_tag[] PROGMEM = "r36n";
//const prog_char relay37_tag[] PROGMEM = "r37n";
//const prog_char relay38_tag[] PROGMEM = "r38n";
//#endif  // InstalledRelayExpansionModules >= 3
//#if InstalledRelayExpansionModules >= 4
//const prog_char relay41_tag[] PROGMEM = "r41n";
//const prog_char relay42_tag[] PROGMEM = "r42n";
//const prog_char relay43_tag[] PROGMEM = "r43n";
//const prog_char relay44_tag[] PROGMEM = "r44n";
//const prog_char relay45_tag[] PROGMEM = "r45n";
//const prog_char relay46_tag[] PROGMEM = "r46n";
//const prog_char relay47_tag[] PROGMEM = "r47n";
//const prog_char relay48_tag[] PROGMEM = "r48n";
//#endif  // InstalledRelayExpansionModules >= 4
//#if InstalledRelayExpansionModules >= 5
//const prog_char relay51_tag[] PROGMEM = "r51n";
//const prog_char relay52_tag[] PROGMEM = "r52n";
//const prog_char relay53_tag[] PROGMEM = "r53n";
//const prog_char relay54_tag[] PROGMEM = "r54n";
//const prog_char relay55_tag[] PROGMEM = "r55n";
//const prog_char relay56_tag[] PROGMEM = "r56n";
//const prog_char relay57_tag[] PROGMEM = "r57n";
//const prog_char relay58_tag[] PROGMEM = "r58n";
//#endif  // InstalledRelayExpansionModules >= 5
//#if InstalledRelayExpansionModules >= 6
//const prog_char relay61_tag[] PROGMEM = "r61n";
//const prog_char relay62_tag[] PROGMEM = "r62n";
//const prog_char relay63_tag[] PROGMEM = "r63n";
//const prog_char relay64_tag[] PROGMEM = "r64n";
//const prog_char relay65_tag[] PROGMEM = "r65n";
//const prog_char relay66_tag[] PROGMEM = "r66n";
//const prog_char relay67_tag[] PROGMEM = "r67n";
//const prog_char relay68_tag[] PROGMEM = "r68n";
//#endif  // InstalledRelayExpansionModules >= 6
//#if InstalledRelayExpansionModules >= 7
//const prog_char relay71_tag[] PROGMEM = "r71n";
//const prog_char relay72_tag[] PROGMEM = "r72n";
//const prog_char relay73_tag[] PROGMEM = "r73n";
//const prog_char relay74_tag[] PROGMEM = "r74n";
//const prog_char relay75_tag[] PROGMEM = "r75n";
//const prog_char relay76_tag[] PROGMEM = "r76n";
//const prog_char relay77_tag[] PROGMEM = "r77n";
//const prog_char relay78_tag[] PROGMEM = "r78n";
//#endif  // InstalledRelayExpansionModules >= 7
//#if InstalledRelayExpansionModules >= 8
//const prog_char relay81_tag[] PROGMEM = "r81n";
//const prog_char relay82_tag[] PROGMEM = "r82n";
//const prog_char relay83_tag[] PROGMEM = "r83n";
//const prog_char relay84_tag[] PROGMEM = "r84n";
//const prog_char relay85_tag[] PROGMEM = "r85n";
//const prog_char relay86_tag[] PROGMEM = "r86n";
//const prog_char relay87_tag[] PROGMEM = "r87n";
//const prog_char relay88_tag[] PROGMEM = "r88n";
//#endif  // InstalledRelayExpansionModules >= 8
//#endif  // RelayExp
//static PROGMEM const char *webbanner_tags[] = {
//	id_tag, probe1_tag, probe2_tag, probe3_tag,
//	relay1_tag, relay2_tag, relay3_tag, relay4_tag, relay5_tag, relay6_tag, relay7_tag, relay8_tag,
//#ifdef RelayExp
//#if InstalledRelayExpansionModules >= 1
//	relay11_tag, relay12_tag, relay13_tag, relay14_tag, relay15_tag, relay16_tag, relay17_tag, relay18_tag,
//#endif  // InstalledRelayExpansionModules >= 1
//#if InstalledRelayExpansionModules >= 2
//	relay21_tag, relay22_tag, relay23_tag, relay24_tag, relay25_tag, relay26_tag, relay27_tag, relay28_tag,
//#endif  // InstalledRelayExpansionModules >= 2
//#if InstalledRelayExpansionModules >= 3
//	relay31_tag, relay32_tag, relay33_tag, relay34_tag, relay35_tag, relay36_tag, relay37_tag, relay38_tag,
//#endif  // InstalledRelayExpansionModules >= 3
//#if InstalledRelayExpansionModules >= 4
//	relay41_tag, relay42_tag, relay43_tag, relay44_tag, relay45_tag, relay46_tag, relay47_tag, relay48_tag,
//#endif  // InstalledRelayExpansionModules >= 4
//#if InstalledRelayExpansionModules >= 5
//	relay51_tag, relay52_tag, relay53_tag, relay54_tag, relay55_tag, relay56_tag, relay57_tag, relay58_tag,
//#endif  // InstalledRelayExpansionModules >= 5
//#if InstalledRelayExpansionModules >= 6
//	relay61_tag, relay62_tag, relay63_tag, relay64_tag, relay65_tag, relay66_tag, relay67_tag, relay68_tag,
//#endif  // InstalledRelayExpansionModules >= 6
//#if InstalledRelayExpansionModules >= 7
//	relay71_tag, relay72_tag, relay73_tag, relay74_tag, relay75_tag, relay76_tag, relay77_tag, relay78_tag,
//#endif  // InstalledRelayExpansionModules >= 7
//#if InstalledRelayExpansionModules >= 8
//	relay81_tag, relay82_tag, relay83_tag, relay84_tag, relay85_tag, relay86_tag, relay87_tag, relay88_tag,
//#endif  // InstalledRelayExpansionModules >= 8
//#endif  // RelayExp
//};
#endif  // wifi

class RA_Wifi: public Print
{
  public:
	  RA_Wifi();
    void WebResponse (const prog_char* response, long strsize);
    void ModeResponse(bool fOk);
    void PushBuffer(byte inStr);
    void PrintHeader(int s, byte type);
    char GetC(int c);
    void ConvertC(char* strIn, char* strOut, byte len);
    void WifiAuthentication(char* userpass);
    void SendXMLData(bool fAtoLog = false);
#ifndef RA_STANDARD
    void SendJSONData();
    void SendSingleJSON(const prog_char str[], int value, char* suffix="");
    void SendSingleJSON(const prog_char str[], char* value);
#endif // RA_STANDARD
    void ProcessHTTP();
    void ProcessSerial();
    void ReceiveData();
    void PROGMEMprint(const prog_char str[]);
    void LoadWebBanner(int pointer, byte qty);
    void Portal(char* username);
    void Portal(char* username, char* key);
    void DDNS(char* subdomain);
    void SendPortal(char* username, char* key);
    inline void CheckWifi(){};
    inline void pingSerial(){};
    char *portalusername;
    char *portalkey;
    char encodeduserpass[50];
    char *portalsubdomain;
    
#ifndef ETH_WIZ5100
    using Print::write;
    inline size_t write(uint8_t c) { return _wifiSerial->write((uint8_t)c); }
    inline size_t write(unsigned long n) { return _wifiSerial->write((uint8_t)n); }
    inline size_t write(long n) { return _wifiSerial->write((uint8_t)n); }
    inline size_t write(unsigned int n) { return _wifiSerial->write((uint8_t)n); }
    inline size_t write(int n) { return _wifiSerial->write((uint8_t)n); }
#endif // ETH_WIZ5100

  protected:
    char m_pushback[32];
    byte m_pushbackindex;
    byte reqtype;
    unsigned long timeout;
    boolean bIncoming;
    boolean auth;
    boolean usingAuth;
    int weboption;
    int weboption2;
    int weboption3;
    byte bHasSecondValue;
    //static byte bHasComma;
    byte bCommaCount;
    boolean webnegoption;

  private:
#if defined(__SAM3X8E__)
    UARTClass *_wifiSerial;
#else
    HardwareSerial* _wifiSerial;
#endif

};

#endif  // __RA_WIFI_H__
