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
#include <avr/pgmspace.h>

#ifdef wifi
const prog_char XML_T1[] PROGMEM = "<RA><T1>";
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
const prog_char XML_REM[] PROGMEM = "</EM><REM>";
const prog_char XML_REM_END[] PROGMEM = "</REM>";

#ifdef DisplayLEDPWM
const prog_char XML_PWMA[] PROGMEM = "<PWMA>";
const prog_char XML_PWMD[] PROGMEM = "</PWMA><PWMD>";
const prog_char XML_PWMD_END[] PROGMEM = "</PWMD>";
#endif  // DisplayLEDPWM
#ifdef SALINITYEXPANSION
const prog_char XML_SAL[] PROGMEM = "<SAL>";
const prog_char XML_SAL_END[] PROGMEM = "</SAL>";
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
const prog_char XML_ORP[] PROGMEM = "<ORP>";
const prog_char XML_ORP_END[] PROGMEM = "</ORP>";
#endif  // ORPEXPANSION
#ifdef PWMEXPANSION
const prog_char XML_PWME[] PROGMEM = "<PWME";
const prog_char XML_PWME_END[] PROGMEM = "</PWME";
#endif  // PWMEXPANSION
#ifdef AI_LED
const prog_char XML_AIW[] PROGMEM = "<AIW>";
const prog_char XML_AIW_END[] PROGMEM = "</AIW><AIB>";
const prog_char XML_AIB_END[] PROGMEM = "</AIB><AIRB>";
const prog_char XML_AIRB_END[] PROGMEM = "</AIRB>";
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
const prog_char XML_RFI_END[] PROGMEM = "</RFI>";
#endif  // RFEXPANSION
#ifdef IOEXPANSION
const prog_char XML_IO[] PROGMEM = "<IO>";
const prog_char XML_IO_END[] PROGMEM = "</IO>";
#endif  // IOEXPANSION
#ifdef CUSTOM_VARIABLES
const prog_char XML_C[] PROGMEM = "<C";
const prog_char XML_C_END[] PROGMEM = "</C";
#endif  // CUSTOM_VARIABLES
#ifdef ENABLE_ATO_LOGGING
const prog_char XML_ATOLOW_LOG_OPEN[] PROGMEM = "<AL";
const prog_char XML_ATOLOW_LOG_CLOSE[] PROGMEM = "</AL";
const prog_char XML_ATOHIGH_LOG_OPEN[] PROGMEM = "<AH";
const prog_char XML_ATOHIGH_LOG_CLOSE[] PROGMEM = "</AH";
#endif  // ENABLE_ATO_LOGGING
const prog_char XML_END[] PROGMEM = "</RA>";
const prog_char XML_CLOSE_TAG[] PROGMEM = ">";


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

#define SERVER_DENY "HTTP/1.1 401 Access Denied" CRLF "WWW-Authenticate: Basic realm=Reef Angel Controller" SERVER_HEADER2 "0" SERVER_HEADER3
#define SERVER_HEADER1 "HTTP/1.1 200 OK" CRLF "Server: ReefAngel" CRLF "Cache-Control: no-store, no-cache, must-revalidate" CRLF "Pragma: no-cache" CRLF "Connection: close" CRLF "Content-Type: text/"
#define SERVER_HEADER2 CRLF "Content-Length: "
#define SERVER_HEADER3 CRLF CRLF
#define SERVER_HEADER_HTML SERVER_HEADER1 "html" SERVER_HEADER2
#define SERVER_HEADER_XML SERVER_HEADER1 "xml" SERVER_HEADER2
#define SERVER_DEFAULT "<h1>Reef Angel Controller Web Server</h1>"
#define SERVER_UKNOWN "<h1>Unknown Request</h1>"
#define SERVER_RELAY ""
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"
#define BAD "bad=1"

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
#define REQ_HTTP		127		// HTTP get request from  external server
#define REQ_UNKNOWN		128	 	// Unknown request

#define P(name)   static const prog_char name[] PROGMEM
const prog_char SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const prog_char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const prog_char BannerGET[] PROGMEM = "GET /status/submitp.asp?t1=";
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
const prog_char BannerREM[] PROGMEM = "&rem=";
const prog_char BannerCustom[] PROGMEM = "&c";
const prog_char BannerKey[] PROGMEM = "&key=";

#if defined DisplayLEDPWM && ! defined RemoveAllLights
	const prog_char BannerPWMA[] PROGMEM = "&pwma=";
	const prog_char BannerPWMD[] PROGMEM = "&pwmd=";
#endif  // DisplayLEDPWM && ! defined RemoveAllLights

#ifdef PWMEXPANSION
	const prog_char BannerPWME[] PROGMEM = "&pwme";
	#define PWMEbit		1
#else
	#define PWMEbit		0
#endif  // PWMEXPANSION

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
	#define RFEbit		2
#else
	#define RFEbit		0
#endif  // RFEXPANSION

#ifdef AI_LED
	const prog_char BannerAIW[] PROGMEM = "&aiw=";
	const prog_char BannerAIB[] PROGMEM = "&aib=";
	const prog_char BannerAIRB[] PROGMEM = "&airb=";
	#define AIbit		4
#else
	#define AIbit		0
#endif  // AI_LED

#ifdef SALINITYEXPANSION
	const prog_char BannerSal[] PROGMEM = "&sal=";
	#define Salbit		8
#else
	#define Salbit		0
#endif  // SALINITYEXPANSION

#ifdef ORPEXPANSION
	const prog_char BannerORP[] PROGMEM = "&orp=";
	#define ORPbit		16
#else
	#define ORPbit		0
#endif  // ORPEXPANSION

#ifdef IOEXPANSION
	const prog_char BannerIO[] PROGMEM = "&io=";
	#define IObit		32
#else
	#define IObit		0
#endif  // IOEXPANSION

#ifdef CUSTOM_VARIABLES
	const prog_char BannerCustomVar[] PROGMEM = "&c";
#endif //CUSTOM_VARIABLES


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

static char m_pushback[32];
static byte m_pushbackindex=0;
static byte reqtype=0;
static unsigned long timeout;
static boolean bIncoming=false;
static boolean auth=false;
static char authStr[32];
static int weboption=0;
static int weboption2=-1;
static int weboption3=-1;
static byte bHasSecondValue = false;
//static byte bHasComma = false;
static byte bCommaCount = 0;


void WebResponse (const prog_char *response, long strsize);
void pushbuffer(byte inStr);
void PrintHTTPHeader(int s);
char GetC(int c);
void ConvertC(char* strIn, char* strOut, byte len);
void WifiAuthentication(char* userpass);
void SendXMLData(bool fAtoLog = false);
void processHTTP();
void CheckWifi();
#endif  // wifi

void pingSerial();
void PROGMEMprint(const prog_char str[]);

#endif  // __RA_WIFI_H__
