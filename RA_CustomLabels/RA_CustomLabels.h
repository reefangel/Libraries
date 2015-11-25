/*
 * Copyright 2015 Reef Angel
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

#ifndef __RA_CUSTOMLABELS_H__
#define __RA_CUSTOMLABELS_H__

// Cloud authentication
// Max number of characters for username and password is 16
const prog_char CLOUD_USERNAME[] PROGMEM = "cloudtest";
const prog_char CLOUD_PASSWORD[] PROGMEM = "123456";

// Headers
const prog_char RELAY_BOX_LABEL[] PROGMEM = "Relay Box";
const prog_char EXP_RELAY_1_LABEL[] PROGMEM = "Exp. Relay Box 1";
const prog_char EXP_RELAY_2_LABEL[] PROGMEM = "Exp. Relay Box 2";
const prog_char EXP_RELAY_3_LABEL[] PROGMEM = "Exp. Relay Box 3";
const prog_char EXP_RELAY_4_LABEL[] PROGMEM = "Exp. Relay Box 4";
const prog_char EXP_RELAY_5_LABEL[] PROGMEM = "Exp. Relay Box 5";
const prog_char EXP_RELAY_6_LABEL[] PROGMEM = "Exp. Relay Box 6";
const prog_char EXP_RELAY_7_LABEL[] PROGMEM = "Exp. Relay Box 7";
const prog_char EXP_RELAY_8_LABEL[] PROGMEM = "Exp. Relay Box 8";
const prog_char PWM_EXPANSION_LABEL[] PROGMEM = "PWM Expansion";
const prog_char SIXTEENCH_PWM_EXPANSION_LABEL[] PROGMEM = "16 Ch PWM Expansion";
const prog_char RF_EXPANSION_LABEL[] PROGMEM = "RF Expansion";
const prog_char RF_EXPANSION_LABEL1[] PROGMEM = "RF Expansion";
const prog_char AI_LABEL[] PROGMEM = "Aqua Illumination";
const prog_char IO_EXPANSION_LABEL[] PROGMEM = "IO Expansion";
const prog_char DCPUMP_LABEL[] PROGMEM = "DC Pump";
const prog_char CVAR_LABEL[] PROGMEM = "Custom Variables";
const prog_char STATUS_LABEL[] PROGMEM = "Status";
const prog_char ALERT_LABEL[] PROGMEM = "Alert";
const prog_char ALERT_LIGHTS_ON_LABEL[] PROGMEM = "Lights On";
const prog_char ALERT_ATO_TIMEOUT_LABEL[] PROGMEM = "ATO Timeout";
const prog_char ALERT_OVERHEAT_LABEL[] PROGMEM = "Overheat";
const prog_char ALERT_BUSLOCK_LABEL[] PROGMEM = "Bus Lock";
const prog_char ALERT_LEAK_LABEL[] PROGMEM = "Water Leak";


// Labels
const prog_char LABEL_TEMP1[] PROGMEM = "Temp 1";
const prog_char LABEL_TEMP2[] PROGMEM = "Temp 2";
const prog_char LABEL_TEMP3[] PROGMEM = "Temp 3";
const prog_char LABEL_PH[] PROGMEM = "PH";

const prog_char LABEL_ATOLOW[] PROGMEM = "Low ATO";
const prog_char LABEL_ATOHIGH[] PROGMEM = "High ATO";
const prog_char LABEL_ALARM[] PROGMEM = "Alarm";
const prog_char LABEL_LEAK[] PROGMEM = "Leak";
const prog_char LABEL_DAYLIGHT[] PROGMEM = "Daylight";
const prog_char LABEL_DAYLIGHT2[] PROGMEM = "Daylight 2";
const prog_char LABEL_ACTINIC[] PROGMEM = "Actinic";
const prog_char LABEL_ACTINIC2[] PROGMEM = "Actinic 2";

const prog_char LABEL_SALINITY[] PROGMEM = "Salinity";
const prog_char LABEL_ORP[] PROGMEM = "ORP";
const prog_char LABEL_PHEXP[] PROGMEM = "PH Exp";
const prog_char LABEL_HUMIDITY[] PROGMEM = "Humidity";
const prog_char LABEL_PAR[] PROGMEM = "PAR";

const prog_char LABEL_CUSTOM_EXP0[] PROGMEM = "Exp 0";
const prog_char LABEL_CUSTOM_EXP1[] PROGMEM = "Exp 1";
const prog_char LABEL_CUSTOM_EXP2[] PROGMEM = "Exp 2";
const prog_char LABEL_CUSTOM_EXP3[] PROGMEM = "Exp 3";
const prog_char LABEL_CUSTOM_EXP4[] PROGMEM = "Exp 4";
const prog_char LABEL_CUSTOM_EXP5[] PROGMEM = "Exp 5";
const prog_char LABEL_CUSTOM_EXP6[] PROGMEM = "Exp 6";
const prog_char LABEL_CUSTOM_EXP7[] PROGMEM = "Exp 7";

const prog_char LABEL_WL0[] PROGMEM = "WL";
const prog_char LABEL_WL1[] PROGMEM = "WL 1";
const prog_char LABEL_WL2[] PROGMEM = "WL 2";
const prog_char LABEL_WL3[] PROGMEM = "WL 3";
const prog_char LABEL_WL4[] PROGMEM = "WL 4";

const prog_char LABEL_PWME0[] PROGMEM = "Channel 0";
const prog_char LABEL_PWME1[] PROGMEM = "Channel 1";
const prog_char LABEL_PWME2[] PROGMEM = "Channel 2";
const prog_char LABEL_PWME3[] PROGMEM = "Channel 3";
const prog_char LABEL_PWME4[] PROGMEM = "Channel 4";
const prog_char LABEL_PWME5[] PROGMEM = "Channel 5";

const prog_char LABEL_IO0[] PROGMEM = "I/O Channel 0";
const prog_char LABEL_IO1[] PROGMEM = "I/O Channel 1";
const prog_char LABEL_IO2[] PROGMEM = "I/O Channel 2";
const prog_char LABEL_IO3[] PROGMEM = "I/O Channel 3";
const prog_char LABEL_IO4[] PROGMEM = "I/O Channel 4";
const prog_char LABEL_IO5[] PROGMEM = "I/O Channel 5";

const prog_char LABEL_C0[] PROGMEM = "Custom Var 0";
const prog_char LABEL_C1[] PROGMEM = "Custom Var 1";
const prog_char LABEL_C2[] PROGMEM = "Custom Var 2";
const prog_char LABEL_C3[] PROGMEM = "Custom Var 3";
const prog_char LABEL_C4[] PROGMEM = "Custom Var 4";
const prog_char LABEL_C5[] PROGMEM = "Custom Var 5";
const prog_char LABEL_C6[] PROGMEM = "Custom Var 6";
const prog_char LABEL_C7[] PROGMEM = "Custom Var 7";

const prog_char LABEL_PORT1[] PROGMEM = "Port 1";
const prog_char LABEL_PORT2[] PROGMEM = "Port 2";
const prog_char LABEL_PORT3[] PROGMEM = "Port 3";
const prog_char LABEL_PORT4[] PROGMEM = "Port 4";
const prog_char LABEL_PORT5[] PROGMEM = "Port 5";
const prog_char LABEL_PORT6[] PROGMEM = "Port 6";
const prog_char LABEL_PORT7[] PROGMEM = "Port 7";
const prog_char LABEL_PORT8[] PROGMEM = "Port 8";
const prog_char LABEL_PORT11[] PROGMEM = "Port 11";
const prog_char LABEL_PORT12[] PROGMEM = "Port 12";
const prog_char LABEL_PORT13[] PROGMEM = "Port 13";
const prog_char LABEL_PORT14[] PROGMEM = "Port 14";
const prog_char LABEL_PORT15[] PROGMEM = "Port 15";
const prog_char LABEL_PORT16[] PROGMEM = "Port 16";
const prog_char LABEL_PORT17[] PROGMEM = "Port 17";
const prog_char LABEL_PORT18[] PROGMEM = "Port 18";
const prog_char LABEL_PORT21[] PROGMEM = "Port 21";
const prog_char LABEL_PORT22[] PROGMEM = "Port 22";
const prog_char LABEL_PORT23[] PROGMEM = "Port 23";
const prog_char LABEL_PORT24[] PROGMEM = "Port 24";
const prog_char LABEL_PORT25[] PROGMEM = "Port 25";
const prog_char LABEL_PORT26[] PROGMEM = "Port 26";
const prog_char LABEL_PORT27[] PROGMEM = "Port 27";
const prog_char LABEL_PORT28[] PROGMEM = "Port 28";
const prog_char LABEL_PORT31[] PROGMEM = "Port 31";
const prog_char LABEL_PORT32[] PROGMEM = "Port 32";
const prog_char LABEL_PORT33[] PROGMEM = "Port 33";
const prog_char LABEL_PORT34[] PROGMEM = "Port 34";
const prog_char LABEL_PORT35[] PROGMEM = "Port 35";
const prog_char LABEL_PORT36[] PROGMEM = "Port 36";
const prog_char LABEL_PORT37[] PROGMEM = "Port 37";
const prog_char LABEL_PORT38[] PROGMEM = "Port 38";
const prog_char LABEL_PORT41[] PROGMEM = "Port 41";
const prog_char LABEL_PORT42[] PROGMEM = "Port 42";
const prog_char LABEL_PORT43[] PROGMEM = "Port 43";
const prog_char LABEL_PORT44[] PROGMEM = "Port 44";
const prog_char LABEL_PORT45[] PROGMEM = "Port 45";
const prog_char LABEL_PORT46[] PROGMEM = "Port 46";
const prog_char LABEL_PORT47[] PROGMEM = "Port 47";
const prog_char LABEL_PORT48[] PROGMEM = "Port 48";
const prog_char LABEL_PORT51[] PROGMEM = "Port 51";
const prog_char LABEL_PORT52[] PROGMEM = "Port 52";
const prog_char LABEL_PORT53[] PROGMEM = "Port 53";
const prog_char LABEL_PORT54[] PROGMEM = "Port 54";
const prog_char LABEL_PORT55[] PROGMEM = "Port 55";
const prog_char LABEL_PORT56[] PROGMEM = "Port 56";
const prog_char LABEL_PORT57[] PROGMEM = "Port 57";
const prog_char LABEL_PORT58[] PROGMEM = "Port 58";
const prog_char LABEL_PORT61[] PROGMEM = "Port 61";
const prog_char LABEL_PORT62[] PROGMEM = "Port 62";
const prog_char LABEL_PORT63[] PROGMEM = "Port 63";
const prog_char LABEL_PORT64[] PROGMEM = "Port 64";
const prog_char LABEL_PORT65[] PROGMEM = "Port 65";
const prog_char LABEL_PORT66[] PROGMEM = "Port 66";
const prog_char LABEL_PORT67[] PROGMEM = "Port 67";
const prog_char LABEL_PORT68[] PROGMEM = "Port 68";
const prog_char LABEL_PORT71[] PROGMEM = "Port 71";
const prog_char LABEL_PORT72[] PROGMEM = "Port 72";
const prog_char LABEL_PORT73[] PROGMEM = "Port 73";
const prog_char LABEL_PORT74[] PROGMEM = "Port 74";
const prog_char LABEL_PORT75[] PROGMEM = "Port 75";
const prog_char LABEL_PORT76[] PROGMEM = "Port 76";
const prog_char LABEL_PORT77[] PROGMEM = "Port 77";
const prog_char LABEL_PORT78[] PROGMEM = "Port 78";
const prog_char LABEL_PORT81[] PROGMEM = "Port 81";
const prog_char LABEL_PORT82[] PROGMEM = "Port 82";
const prog_char LABEL_PORT83[] PROGMEM = "Port 83";
const prog_char LABEL_PORT84[] PROGMEM = "Port 84";
const prog_char LABEL_PORT85[] PROGMEM = "Port 85";
const prog_char LABEL_PORT86[] PROGMEM = "Port 86";
const prog_char LABEL_PORT87[] PROGMEM = "Port 87";
const prog_char LABEL_PORT88[] PROGMEM = "Port 88";

const prog_char LABEL_AI_WHITE[] PROGMEM = "White";
const prog_char LABEL_AI_BLUE[] PROGMEM = "Blue";
const prog_char LABEL_AI_ROYAL_BLUE[] PROGMEM = "R. Blue";

const prog_char LABEL_RF_WHITE[] PROGMEM = "White";
const prog_char LABEL_RF_ROYAL_BLUE[] PROGMEM = "R. Blue";
const prog_char LABEL_RF_RED[] PROGMEM = "Red";
const prog_char LABEL_RF_BLUE[] PROGMEM = "Green";
const prog_char LABEL_RF_GREEN[] PROGMEM = "Blue";
const prog_char LABEL_RF_INTENSITY[] PROGMEM = "Intensity";

// Buttons
const prog_char CUSTOMLABELOKBUTTON[] PROGMEM = "Ok";
const prog_char CUSTOMLABELCANCELBUTTON[] PROGMEM = "Cancel";

// Arrays
static PROGMEM const char *LABEL_PWME[] = {LABEL_PWME0,LABEL_PWME1,LABEL_PWME2,LABEL_PWME3,LABEL_PWME4,LABEL_PWME5};
static PROGMEM const char *LABEL_WL[] = {LABEL_WL0,LABEL_WL1,LABEL_WL2,LABEL_WL3,LABEL_WL4};
static PROGMEM const char *LABEL_IO[] = {LABEL_IO0,LABEL_IO1,LABEL_IO2,LABEL_IO3,LABEL_IO4,LABEL_IO5};
static PROGMEM const char *LABEL_C[] = {LABEL_C0,LABEL_C1,LABEL_C2,LABEL_C3,LABEL_C4,LABEL_C5,LABEL_C6,LABEL_C7};
static PROGMEM const char *LABEL_AI[] = {LABEL_AI_WHITE, LABEL_AI_BLUE, LABEL_AI_ROYAL_BLUE};
static PROGMEM const char *LABEL_RF[] = {LABEL_RF_WHITE, LABEL_RF_ROYAL_BLUE, LABEL_RF_RED, LABEL_RF_BLUE, LABEL_RF_GREEN, LABEL_RF_INTENSITY};
static PROGMEM const char *LABEL_RELAY[] = {RELAY_BOX_LABEL, EXP_RELAY_1_LABEL, EXP_RELAY_2_LABEL, EXP_RELAY_3_LABEL, EXP_RELAY_4_LABEL, EXP_RELAY_5_LABEL, EXP_RELAY_6_LABEL, EXP_RELAY_7_LABEL, EXP_RELAY_8_LABEL, PWM_EXPANSION_LABEL, RF_EXPANSION_LABEL, RF_EXPANSION_LABEL1, AI_LABEL, IO_EXPANSION_LABEL, DCPUMP_LABEL, CVAR_LABEL, STATUS_LABEL, ALERT_LABEL};
static PROGMEM const char *LABEL_PORT[] = {LABEL_PORT1,LABEL_PORT2,LABEL_PORT3,LABEL_PORT4,LABEL_PORT5,LABEL_PORT6,LABEL_PORT7,LABEL_PORT8,LABEL_PORT11,LABEL_PORT12,LABEL_PORT13,LABEL_PORT14,LABEL_PORT15,LABEL_PORT16,LABEL_PORT17,LABEL_PORT18,LABEL_PORT21,LABEL_PORT22,LABEL_PORT23,LABEL_PORT24,LABEL_PORT25,LABEL_PORT26,LABEL_PORT27,LABEL_PORT28,LABEL_PORT31,LABEL_PORT32,LABEL_PORT33,LABEL_PORT34,LABEL_PORT35,LABEL_PORT36,LABEL_PORT37,LABEL_PORT38,LABEL_PORT41,LABEL_PORT42,LABEL_PORT43,LABEL_PORT44,LABEL_PORT45,LABEL_PORT46,LABEL_PORT47,LABEL_PORT48,LABEL_PORT51,LABEL_PORT52,LABEL_PORT53,LABEL_PORT54,LABEL_PORT55,LABEL_PORT56,LABEL_PORT57,LABEL_PORT58,LABEL_PORT61,LABEL_PORT62,LABEL_PORT63,LABEL_PORT64,LABEL_PORT65,LABEL_PORT66,LABEL_PORT67,LABEL_PORT68,LABEL_PORT71,LABEL_PORT72,LABEL_PORT73,LABEL_PORT74,LABEL_PORT75,LABEL_PORT76,LABEL_PORT77,LABEL_PORT78,LABEL_PORT81,LABEL_PORT82,LABEL_PORT83,LABEL_PORT84,LABEL_PORT85,LABEL_PORT86,LABEL_PORT87,LABEL_PORT88};
static PROGMEM const char *LABEL_ALERT[] = {ALERT_ATO_TIMEOUT_LABEL,ALERT_OVERHEAT_LABEL,ALERT_BUSLOCK_LABEL,ALERT_LEAK_LABEL};
static PROGMEM const char *LABEL_CUSTOM_EXP[] = {LABEL_CUSTOM_EXP0,LABEL_CUSTOM_EXP1,LABEL_CUSTOM_EXP2,LABEL_CUSTOM_EXP3,LABEL_CUSTOM_EXP4,LABEL_CUSTOM_EXP5,LABEL_CUSTOM_EXP6,LABEL_CUSTOM_EXP7};

#endif  // __RA_CUSTOMLABELS_H__
