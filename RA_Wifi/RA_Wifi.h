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

const char XML_ID[] PROGMEM = "<RA><ID>";
const char XML_T1[] PROGMEM = "</ID><T1>";
const char XML_T2[] PROGMEM = "</T1><T2>";
const char XML_T3[] PROGMEM = "</T2><T3>";
const char XML_T4[] PROGMEM = "</T3><T4>";
const char XML_T5[] PROGMEM = "</T4><T5>";
const char XML_T6[] PROGMEM = "</T5><T6>";
const char XML_PH[] PROGMEM = "</T6><PH>";
const char XML_R[] PROGMEM = "</PH><R>";
const char XML_RON[] PROGMEM = "</R><RON>";
const char XML_ROFF[] PROGMEM = "</RON><ROFF>";
const char XML_RE_OPEN[] PROGMEM = "<R";
const char XML_RE_CLOSE[] PROGMEM = "</R";
const char XML_RE_ON[] PROGMEM = "ON";
const char XML_RE_OFF[] PROGMEM = "OFF";
const char XML_ATOLOW[] PROGMEM = "<ATOLOW>";
const char XML_ATOHIGH[] PROGMEM = "</ATOLOW><ATOHIGH>";
const char XML_EM[] PROGMEM = "</ATOHIGH><EM>";
const char XML_EM1[] PROGMEM = "</EM><EM1>";
const char XML_REM[] PROGMEM = "</EM1><REM>";
const char XML_BOARDID[] PROGMEM = "</REM><BID>";
const char XML_ALERTFLAG[] PROGMEM = "</BID><AF>";
const char XML_STATUSFLAG[] PROGMEM = "</AF><SF>";
const char XML_STATUSFLAG_END[] PROGMEM = "</SF>";
const char XML_PWMA[] PROGMEM = "<PWMA>";
const char XML_PWMD[] PROGMEM = "</PWMA><PWMD>";
const char XML_PWMAO[] PROGMEM = "</PWMD><PWMAO>";
const char XML_PWMDO[] PROGMEM = "</PWMAO><PWMDO>";
const char XML_PWMDO_END[] PROGMEM = "</PWMDO>";
const char XML_ORP[] PROGMEM = "<ORP>";
const char XML_ORP_END[] PROGMEM = "</ORP>";
const char XML_SAL[] PROGMEM = "<SAL>";
const char XML_SAL_END[] PROGMEM = "</SAL>";
const char XML_PHEXP[] PROGMEM = "<PHE>";
const char XML_PHEXP_END[] PROGMEM = "</PHE>";
const char XML_WL[] PROGMEM = "<WL";
const char XML_WL_END[] PROGMEM = "</WL";
const char XML_HUM[] PROGMEM = "<HUM>";
const char XML_HUM_END[] PROGMEM = "</HUM>";
const char XML_PAR[] PROGMEM = "<PAR>";
const char XML_PAR_END[] PROGMEM = "</PAR>";
const char XML_DCM[] PROGMEM = "<DCM>";
const char XML_DCM_END[] PROGMEM = "</DCM><DCS>";
const char XML_DCS_END[] PROGMEM = "</DCS><DCD>";
const char XML_DCD_END[] PROGMEM = "</DCD><DCT>";
const char XML_DCT_END[] PROGMEM = "</DCT>";
const char XML_PWME[] PROGMEM = "<PWME";
const char XML_PWME_END[] PROGMEM = "</PWME";
const char XML_SCPWME[] PROGMEM = "<SCPWME";
const char XML_SCPWME_END[] PROGMEM = "</SCPWME";
const char XML_AIW[] PROGMEM = "<AIW>";
const char XML_AIW_END[] PROGMEM = "</AIW><AIB>";
const char XML_AIB_END[] PROGMEM = "</AIB><AIRB>";
const char XML_AIRB_END[] PROGMEM = "</AIRB><AIWO>";
const char XML_AIWO_END[] PROGMEM = "</AIWO><AIBO>";
const char XML_AIBO_END[] PROGMEM = "</AIBO><AIRBO>";
const char XML_AIRBO_END[] PROGMEM = "</AIRBO>";
const char XML_RFM[] PROGMEM = "<RFM>";
const char XML_RFM_END[] PROGMEM = "</RFM><RFS>";
const char XML_RFS_END[] PROGMEM = "</RFS><RFD>";
const char XML_RFD_END[] PROGMEM = "</RFD><RFW>";
const char XML_RFW_END[] PROGMEM = "</RFW><RFRB>";
const char XML_RFRB_END[] PROGMEM = "</RFRB><RFR>";
const char XML_RFR_END[] PROGMEM = "</RFR><RFG>";
const char XML_RFG_END[] PROGMEM = "</RFG><RFB>";
const char XML_RFB_END[] PROGMEM = "</RFB><RFI>";
const char XML_RFI_END[] PROGMEM = "</RFI><RFWO>";
const char XML_RFWO_END[] PROGMEM = "</RFWO><RFRBO>";
const char XML_RFRBO_END[] PROGMEM = "</RFRBO><RFRO>";
const char XML_RFRO_END[] PROGMEM = "</RFRO><RFGO>";
const char XML_RFGO_END[] PROGMEM = "</RFGO><RFBO>";
const char XML_RFBO_END[] PROGMEM = "</RFBO><RFIO>";
const char XML_RFIO_END[] PROGMEM = "</RFIO>";
const char XML_IO[] PROGMEM = "<IO>";
const char XML_IO_END[] PROGMEM = "</IO>";
const char XML_C[] PROGMEM = "<C";
const char XML_C_END[] PROGMEM = "</C";
const char XML_LEAK[] PROGMEM = "<LEAK>";
const char XML_LEAK_END[] PROGMEM = "</LEAK>";
const char XML_ALARM[] PROGMEM = "<ALARM>";
const char XML_ALARM_END[] PROGMEM = "</ALARM>";
const char XML_PWMA2[] PROGMEM = "<PWMA2>";
const char XML_PWMD2[] PROGMEM = "</PWMA2><PWMD2>";
const char XML_PWMA2O[] PROGMEM = "</PWMD2><PWMA2O>";
const char XML_PWMD2O[] PROGMEM = "</PWMA2O><PWMD2O>";
const char XML_PWMD2O_END[] PROGMEM = "</PWMD2O>";
const char XML_ATOLOW_LOG_OPEN[] PROGMEM = "<AL";
const char XML_ATOLOW_LOG_CLOSE[] PROGMEM = "</AL";
const char XML_ATOHIGH_LOG_OPEN[] PROGMEM = "<AH";
const char XML_ATOHIGH_LOG_CLOSE[] PROGMEM = "</AH";
const char XML_END[] PROGMEM = "</RA>";
const char XML_CLOSE_TAG[] PROGMEM = ">";
const char XML_P_OPEN[] PROGMEM = "<P";
const char XML_P_CLOSE[] PROGMEM = "</P";
const char XML_M_OPEN[] PROGMEM = "<M";
const char XML_M_CLOSE[] PROGMEM = "</M";
const char XML_MEM_OPEN[] PROGMEM = "<MEM>";
const char XML_MEM_CLOSE[] PROGMEM = "</MEM>";
const char XML_DATE_OPEN[] PROGMEM = "<D>";
const char XML_DATE_CLOSE[] PROGMEM = "</D>";
const char XML_MODE_OPEN[] PROGMEM = "<MODE>";
const char XML_MODE_CLOSE[] PROGMEM = "</MODE>";
const char XML_OK[] PROGMEM = "OK";
const char XML_ERR[] PROGMEM = "ERR";

const char SERVER_HEADER1[] PROGMEM = "HTTP/1.1 200 OK\r\nServer: ReefAngel\r\nCache-Control: no-store, no-cache, must-revalidate\r\nPragma: no-cache\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET\r\nConnection: close\r\nContent-Type: text/";
const char SERVER_HEADER2[] PROGMEM = "\r\nContent-Length: ";
const char SERVER_HEADER3[] PROGMEM = "\r\n\r\n";
const char SERVER_DENY[] PROGMEM = "HTTP/1.1 401 Access Denied\r\nWWW-Authenticate: Basic realm=Reef Angel Controller\r\nContent-Length: 0\r\n";
const char SERVER_DEFAULT[] PROGMEM = "<h1>Reef Angel Controller Web Server</h1>";

#ifndef RA_STANDARD
const char JSON_OPEN[] PROGMEM = "{\"json\":{";
const char JSON_ID[] PROGMEM = "ID";
const char JSON_T1[] PROGMEM = "T1";
const char JSON_T2[] PROGMEM = "T2";
const char JSON_T3[] PROGMEM = "T3";
const char JSON_T4[] PROGMEM = "T4";
const char JSON_T5[] PROGMEM = "T5";
const char JSON_T6[] PROGMEM = "T6";
const char JSON_PH[] PROGMEM = "PH";
const char JSON_R[] PROGMEM = "R";
const char JSON_RON[] PROGMEM = "RON";
const char JSON_ROFF[] PROGMEM = "ROFF";
const char JSON_R1[] PROGMEM = "R1";
const char JSON_RON1[] PROGMEM = "RON1";
const char JSON_ROFF1[] PROGMEM = "ROFF1";
const char JSON_R2[] PROGMEM = "R2";
const char JSON_RON2[] PROGMEM = "RON2";
const char JSON_ROFF2[] PROGMEM = "ROFF2";
const char JSON_R3[] PROGMEM = "R3";
const char JSON_RON3[] PROGMEM = "RON3";
const char JSON_ROFF3[] PROGMEM = "ROFF3";
const char JSON_R4[] PROGMEM = "R4";
const char JSON_RON4[] PROGMEM = "RON4";
const char JSON_ROFF4[] PROGMEM = "ROFF4";
const char JSON_R5[] PROGMEM = "R5";
const char JSON_RON5[] PROGMEM = "RON5";
const char JSON_ROFF5[] PROGMEM = "ROFF5";
const char JSON_R6[] PROGMEM = "R6";
const char JSON_RON6[] PROGMEM = "RON6";
const char JSON_ROFF6[] PROGMEM = "ROFF6";
const char JSON_R7[] PROGMEM = "R7";
const char JSON_RON7[] PROGMEM = "RON7";
const char JSON_ROFF7[] PROGMEM = "ROFF7";
const char JSON_R8[] PROGMEM = "R8";
const char JSON_RON8[] PROGMEM = "RON8";
const char JSON_ROFF8[] PROGMEM = "ROFF8";
const char JSON_ATOLOW[] PROGMEM = "ATOLOW";
const char JSON_ATOHIGH[] PROGMEM = "ATOHIGH";
const char JSON_EM[] PROGMEM = "EM";
const char JSON_EM1[] PROGMEM = "EM1";
const char JSON_REM[] PROGMEM = "REM";
const char JSON_ALERTFLAG[] PROGMEM = "AF";
const char JSON_STATUSFLAG[] PROGMEM = "SF";
const char JSON_BOARDID[] PROGMEM = "BID";
const char JSON_PWMA[] PROGMEM = "PWMA";
const char JSON_PWMD[] PROGMEM = "PWMD";
const char JSON_PWMAO[] PROGMEM = "PWMAO";
const char JSON_PWMDO[] PROGMEM = "PWMDO";
const char JSON_ORP[] PROGMEM = "ORP";
const char JSON_SAL[] PROGMEM = "SAL";
const char JSON_PHEXP[] PROGMEM = "PHE";
const char JSON_WL[] PROGMEM = "WL";
const char JSON_WL1[] PROGMEM = "WL1";
const char JSON_WL2[] PROGMEM = "WL2";
const char JSON_WL3[] PROGMEM = "WL3";
const char JSON_WL4[] PROGMEM = "WL4";
const char JSON_HUM[] PROGMEM = "HUM";
const char JSON_PAR[] PROGMEM = "PAR";
const char JSON_DCM[] PROGMEM = "DCM";
const char JSON_DCS[] PROGMEM = "DCS";
const char JSON_DCD[] PROGMEM = "DCD";
const char JSON_DCT[] PROGMEM = "DCT";
const char JSON_PWME[] PROGMEM = "PWME";
const char JSON_PWME0[] PROGMEM = "PWME0";
const char JSON_PWME1[] PROGMEM = "PWME1";
const char JSON_PWME2[] PROGMEM = "PWME2";
const char JSON_PWME3[] PROGMEM = "PWME3";
const char JSON_PWME4[] PROGMEM = "PWME4";
const char JSON_PWME5[] PROGMEM = "PWME5";
const char JSON_PWMEO[] PROGMEM = "PWMEO";
const char JSON_PWME0O[] PROGMEM = "PWME0O";
const char JSON_PWME1O[] PROGMEM = "PWME1O";
const char JSON_PWME2O[] PROGMEM = "PWME2O";
const char JSON_PWME3O[] PROGMEM = "PWME3O";
const char JSON_PWME4O[] PROGMEM = "PWME4O";
const char JSON_PWME5O[] PROGMEM = "PWME5O";
const char JSON_SCPWME[] PROGMEM = "SCPWME";
const char JSON_AIW[] PROGMEM = "AIW";
const char JSON_AIB[] PROGMEM = "AIB";
const char JSON_AIRB[] PROGMEM = "AIRB";
const char JSON_AIWO[] PROGMEM = "AIWO";
const char JSON_AIBO[] PROGMEM = "AIBO";
const char JSON_AIRBO[] PROGMEM = "AIRBO";
const char JSON_RFM[] PROGMEM = "RFM";
const char JSON_RFS[] PROGMEM = "RFS";
const char JSON_RFD[] PROGMEM = "RFD";
const char JSON_RFW[] PROGMEM = "RFW";
const char JSON_RFRB[] PROGMEM = "RFRB";
const char JSON_RFR[] PROGMEM = "RFR";
const char JSON_RFG[] PROGMEM = "RFG";
const char JSON_RFB[] PROGMEM = "RFB";
const char JSON_RFI[] PROGMEM = "RFI";
const char JSON_RFWO[] PROGMEM = "RFWO";
const char JSON_RFRBO[] PROGMEM = "RFRBO";
const char JSON_RFRO[] PROGMEM = "RFRO";
const char JSON_RFGO[] PROGMEM = "RFGO";
const char JSON_RFBO[] PROGMEM = "RFBO";
const char JSON_RFIO[] PROGMEM = "RFIO";
const char JSON_IO[] PROGMEM = "IO";
const char JSON_C[] PROGMEM = "C";
const char JSON_C0[] PROGMEM = "C0";
const char JSON_C1[] PROGMEM = "C1";
const char JSON_C2[] PROGMEM = "C2";
const char JSON_C3[] PROGMEM = "C3";
const char JSON_C4[] PROGMEM = "C4";
const char JSON_C5[] PROGMEM = "C5";
const char JSON_C6[] PROGMEM = "C6";
const char JSON_C7[] PROGMEM = "C7";
const char JSON_LEAK[] PROGMEM = "LEAK";
const char JSON_ALARM[] PROGMEM = "ALARM";
const char JSON_PWMA2[] PROGMEM = "PWMA2";
const char JSON_PWMD2[] PROGMEM = "PWMD2";
const char JSON_PWMA2O[] PROGMEM = "PWMA2O";
const char JSON_PWMD2O[] PROGMEM = "PWMD2O";
const char JSON_CEXP0[] PROGMEM = "CEXP0";
const char JSON_CEXP1[] PROGMEM = "CEXP1";
const char JSON_CEXP2[] PROGMEM = "CEXP2";
const char JSON_CEXP3[] PROGMEM = "CEXP3";
const char JSON_CEXP4[] PROGMEM = "CEXP4";
const char JSON_CEXP5[] PROGMEM = "CEXP5";
const char JSON_CEXP6[] PROGMEM = "CEXP6";
const char JSON_CEXP7[] PROGMEM = "CEXP7";
const char JSON_CLOSE[] PROGMEM = "}}";

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
#define REQ_FAVICON		26		// favicon
#define REQ_HTTP		127		// HTTP get request from  external server
#define REQ_UNKNOWN		128	 	// Unknown request

#define P(name)   static const char name[] PROGMEM
//const char SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const char SERVER_RA[] PROGMEM = "<object type=text/html data=http://www.reefangel.com/wifi3/content.html width=100% height=98%></object>";
const char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const char BannerGET[] PROGMEM = "GET /status/submitp.aspx?t1=";
const char BannerALERT[] PROGMEM = "GET /status/wifialert.aspx?id=";
const char BannerHTTP11[] PROGMEM = " HTTP/1.1\r\n";
const char BannerHost[] PROGMEM = "Host: forum.reefangel.com\r\n";
const char BannerConnectionClose[] PROGMEM = "Connection: close\r\n";
const char BannerT2[] PROGMEM = "&t2=";
const char BannerT3[] PROGMEM = "&t3=";
const char BannerPH[] PROGMEM = "&ph=";
const char BannerATOHIGH[] PROGMEM = "&atohigh=";
const char BannerATOLOW[] PROGMEM = "&atolow=";
const char BannerRelayData[] PROGMEM = "&r";
const char BannerRelayMaskOn[] PROGMEM = "&ron";
const char BannerRelayMaskOff[] PROGMEM = "&roff";
const char BannerID[] PROGMEM = "&id=";
const char BannerEM[] PROGMEM = "&em=";
const char BannerEM1[] PROGMEM = "&em1=";
const char BannerREM[] PROGMEM = "&rem=";
const char BannerCustom[] PROGMEM = "&c";
const char BannerKey[] PROGMEM = "&key=";
const char BannerAlertFlag[] PROGMEM = "&af=";
const char BannerStatusFlag[] PROGMEM = "&sf=";
const char BannerBoardID[] PROGMEM = "&bid=";
const char BannerSubdomain[] PROGMEM = "&ddns=";
const char BannerPWMA[] PROGMEM = "&pwma=";
const char BannerPWMD[] PROGMEM = "&pwmd=";
const char BannerPWMAO[] PROGMEM = "&pwmao=";
const char BannerPWMDO[] PROGMEM = "&pwmdo=";
const char BannerPWME[] PROGMEM = "&pwme";
const char BannerSCPWME[] PROGMEM = "&scpwme";
const char BannerRFM[] PROGMEM = "&rfm=";
const char BannerRFS[] PROGMEM = "&rfs=";
const char BannerRFD[] PROGMEM = "&rfd=";
const char BannerRFW[] PROGMEM = "&rfw=";
const char BannerRFRB[] PROGMEM = "&rfrb=";
const char BannerRFR[] PROGMEM = "&rfr=";
const char BannerRFG[] PROGMEM = "&rfg=";
const char BannerRFB[] PROGMEM = "&rfb=";
const char BannerRFI[] PROGMEM = "&rfi=";
const char BannerAIW[] PROGMEM = "&aiw=";
const char BannerAIB[] PROGMEM = "&aib=";
const char BannerAIRB[] PROGMEM = "&airb=";
const char BannerSal[] PROGMEM = "&sal=";
const char BannerORP[] PROGMEM = "&orp=";
const char BannerIO[] PROGMEM = "&io=";
const char BannerPHE[] PROGMEM = "&phe=";
const char BannerWL[] PROGMEM = "&wl";
const char BannerHumidity[] PROGMEM = "&hum=";
const char BannerPAR[] PROGMEM = "&par=";
const char BannerDCM[] PROGMEM = "&dcm=";
const char BannerDCS[] PROGMEM = "&dcs=";
const char BannerDCD[] PROGMEM = "&dcd=";
const char BannerDCT[] PROGMEM = "&dct=";
const char BannerCustomVar[] PROGMEM = "&c";
const char BannerLeak[] PROGMEM = "&leak=";
const char BannerAlarm[] PROGMEM = "&alarm=";
const char BannerPWMA2[] PROGMEM = "&pwma2=";
const char BannerPWMD2[] PROGMEM = "&pwmd2=";
const char BannerPWMA2O[] PROGMEM = "&pwma2o=";
const char BannerPWMD2O[] PROGMEM = "&pwmd2o=";

//const char probe1_tag[] PROGMEM = "t1n";
//const char probe2_tag[] PROGMEM = "t2n";
//const char probe3_tag[] PROGMEM = "t3n";
//const char relay1_tag[] PROGMEM = "r1n";
//const char relay2_tag[] PROGMEM = "r2n";
//const char relay3_tag[] PROGMEM = "r3n";
//const char relay4_tag[] PROGMEM = "r4n";
//const char relay5_tag[] PROGMEM = "r5n";
//const char relay6_tag[] PROGMEM = "r6n";
//const char relay7_tag[] PROGMEM = "r7n";
//const char relay8_tag[] PROGMEM = "r8n";
//#ifdef RelayExp
//#if InstalledRelayExpansionModules >= 1
//const char relay11_tag[] PROGMEM = "r11n";
//const char relay12_tag[] PROGMEM = "r12n";
//const char relay13_tag[] PROGMEM = "r13n";
//const char relay14_tag[] PROGMEM = "r14n";
//const char relay15_tag[] PROGMEM = "r15n";
//const char relay16_tag[] PROGMEM = "r16n";
//const char relay17_tag[] PROGMEM = "r17n";
//const char relay18_tag[] PROGMEM = "r18n";
//#endif  // InstalledRelayExpansionModules >= 1
//#if InstalledRelayExpansionModules >= 2
//const char relay21_tag[] PROGMEM = "r21n";
//const char relay22_tag[] PROGMEM = "r22n";
//const char relay23_tag[] PROGMEM = "r23n";
//const char relay24_tag[] PROGMEM = "r24n";
//const char relay25_tag[] PROGMEM = "r25n";
//const char relay26_tag[] PROGMEM = "r26n";
//const char relay27_tag[] PROGMEM = "r27n";
//const char relay28_tag[] PROGMEM = "r28n";
//#endif  // InstalledRelayExpansionModules >= 2
//#if InstalledRelayExpansionModules >= 3
//const char relay31_tag[] PROGMEM = "r31n";
//const char relay32_tag[] PROGMEM = "r32n";
//const char relay33_tag[] PROGMEM = "r33n";
//const char relay34_tag[] PROGMEM = "r34n";
//const char relay35_tag[] PROGMEM = "r35n";
//const char relay36_tag[] PROGMEM = "r36n";
//const char relay37_tag[] PROGMEM = "r37n";
//const char relay38_tag[] PROGMEM = "r38n";
//#endif  // InstalledRelayExpansionModules >= 3
//#if InstalledRelayExpansionModules >= 4
//const char relay41_tag[] PROGMEM = "r41n";
//const char relay42_tag[] PROGMEM = "r42n";
//const char relay43_tag[] PROGMEM = "r43n";
//const char relay44_tag[] PROGMEM = "r44n";
//const char relay45_tag[] PROGMEM = "r45n";
//const char relay46_tag[] PROGMEM = "r46n";
//const char relay47_tag[] PROGMEM = "r47n";
//const char relay48_tag[] PROGMEM = "r48n";
//#endif  // InstalledRelayExpansionModules >= 4
//#if InstalledRelayExpansionModules >= 5
//const char relay51_tag[] PROGMEM = "r51n";
//const char relay52_tag[] PROGMEM = "r52n";
//const char relay53_tag[] PROGMEM = "r53n";
//const char relay54_tag[] PROGMEM = "r54n";
//const char relay55_tag[] PROGMEM = "r55n";
//const char relay56_tag[] PROGMEM = "r56n";
//const char relay57_tag[] PROGMEM = "r57n";
//const char relay58_tag[] PROGMEM = "r58n";
//#endif  // InstalledRelayExpansionModules >= 5
//#if InstalledRelayExpansionModules >= 6
//const char relay61_tag[] PROGMEM = "r61n";
//const char relay62_tag[] PROGMEM = "r62n";
//const char relay63_tag[] PROGMEM = "r63n";
//const char relay64_tag[] PROGMEM = "r64n";
//const char relay65_tag[] PROGMEM = "r65n";
//const char relay66_tag[] PROGMEM = "r66n";
//const char relay67_tag[] PROGMEM = "r67n";
//const char relay68_tag[] PROGMEM = "r68n";
//#endif  // InstalledRelayExpansionModules >= 6
//#if InstalledRelayExpansionModules >= 7
//const char relay71_tag[] PROGMEM = "r71n";
//const char relay72_tag[] PROGMEM = "r72n";
//const char relay73_tag[] PROGMEM = "r73n";
//const char relay74_tag[] PROGMEM = "r74n";
//const char relay75_tag[] PROGMEM = "r75n";
//const char relay76_tag[] PROGMEM = "r76n";
//const char relay77_tag[] PROGMEM = "r77n";
//const char relay78_tag[] PROGMEM = "r78n";
//#endif  // InstalledRelayExpansionModules >= 7
//#if InstalledRelayExpansionModules >= 8
//const char relay81_tag[] PROGMEM = "r81n";
//const char relay82_tag[] PROGMEM = "r82n";
//const char relay83_tag[] PROGMEM = "r83n";
//const char relay84_tag[] PROGMEM = "r84n";
//const char relay85_tag[] PROGMEM = "r85n";
//const char relay86_tag[] PROGMEM = "r86n";
//const char relay87_tag[] PROGMEM = "r87n";
//const char relay88_tag[] PROGMEM = "r88n";
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
    void WebResponse (const char* response, long strsize);
    void ModeResponse(bool fOk);
    void PushBuffer(byte inStr);
    void PrintHeader(int s, byte type);
    char GetC(int c);
    void ConvertC(char* strIn, char* strOut, byte len);
    void WifiAuthentication(char* userpass);
    void SendXMLData(bool fAtoLog = false);
#ifndef RA_STANDARD
    void SendJSONData();
    void SendSingleJSON(const char str[], int value, char* suffix="");
    void SendSingleJSON(const char str[], char* value);
#endif // RA_STANDARD
    void ProcessHTTP();
    void ProcessSerial();
    void ReceiveData();
    void PROGMEMprint(const char *str);
    void LoadWebBanner(int pointer, byte qty);
    void Portal(char* username);
    void Portal(char* username, char* key);
    void DDNS(char* subdomain);
    void SendPortal(char* username, char* key);
    void SendAlert(char* username, char* key, char* msg);
    inline void CheckWifi(){};
    inline void pingSerial(){};
    char *portalusername;
    char *portalkey;
    char encodeduserpass[50];
    char *portalsubdomain;
    inline boolean BlockCloud() { return reqtype!=0; };
    
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
