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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <ReefAngel_Features.h>
#ifdef COLORS_PDE
#include <RA_CustomColors.h>
#endif  // COLORS_PDE
#include <Arduino.h>
#include <Time.h>
#include <OneWire.h>

#include <avr/pgmspace.h>

const prog_char NoIMCheck[] PROGMEM = "No Internal Memory";
const prog_char NoIMCheck1[] PROGMEM = "Found";


#ifdef __PLUS_SPECIAL_WIFI__
#define WIFI_SERIAL Serial1
#else
#define WIFI_SERIAL Serial
#endif // __PLUS_SPECIAL_WIFI__


// Outlets on Relay box
#define Port8   8
#define Port7   7
#define Port6   6
#define Port5   5
#define Port4   4
#define Port3   3
#define Port2   2
#define Port1   1

// Port bits
#define Port8Bit   1<<7
#define Port7Bit   1<<6
#define Port6Bit   1<<5
#define Port5Bit   1<<4
#define Port4Bit   1<<3
#define Port3Bit   1<<2
#define Port2Bit   1<<1
#define Port1Bit   1<<0

// Relay Box Modules
#define MAX_RELAY_EXPANSION_MODULES     8
#define PWM_EXPANSION_CHANNELS     		6
#define AI_CHANNELS     				3
#define RF_CHANNELS						6

#ifdef RelayExp
// Relay Expansion is defined in Features file
// Check if the user specified how many expansion modules they have
// if they have not or specified 0, assume they have 1 expansion module
#if !defined InstalledRelayExpansionModules || defined InstalledRelayExpansionModules == 0
#undef InstalledRelayExpansionModules
#define InstalledRelayExpansionModules	1
#endif  // InstalledRelayExpansionModules
#endif  // RelayExp

// Expansion Box Module Port Defines
// Box0 is the main / original box
// Boxes 1 - 8 are the expansion boxes
#define Box0_Port1			Port1
#define Box0_Port2			Port2
#define Box0_Port3			Port3
#define Box0_Port4			Port4
#define Box0_Port5			Port5
#define Box0_Port6			Port6
#define Box0_Port7			Port7
#define Box0_Port8			Port8
#define Box1_Port1			11
#define Box1_Port2			12
#define Box1_Port3			13
#define Box1_Port4			14
#define Box1_Port5			15
#define Box1_Port6			16
#define Box1_Port7			17
#define Box1_Port8			18
#define Box2_Port1			21
#define Box2_Port2			22
#define Box2_Port3			23
#define Box2_Port4			24
#define Box2_Port5			25
#define Box2_Port6			26
#define Box2_Port7			27
#define Box2_Port8			28
#define Box3_Port1			31
#define Box3_Port2			32
#define Box3_Port3			33
#define Box3_Port4			34
#define Box3_Port5			35
#define Box3_Port6			36
#define Box3_Port7			37
#define Box3_Port8			38
#define Box4_Port1			41
#define Box4_Port2			42
#define Box4_Port3			43
#define Box4_Port4			44
#define Box4_Port5			45
#define Box4_Port6			46
#define Box4_Port7			47
#define Box4_Port8			48
#define Box5_Port1			51
#define Box5_Port2			52
#define Box5_Port3			53
#define Box5_Port4			54
#define Box5_Port5			55
#define Box5_Port6			56
#define Box5_Port7			57
#define Box5_Port8			58
#define Box6_Port1			61
#define Box6_Port2			62
#define Box6_Port3			63
#define Box6_Port4			64
#define Box6_Port5			65
#define Box6_Port6			66
#define Box6_Port7			67
#define Box6_Port8			68
#define Box7_Port1			71
#define Box7_Port2			72
#define Box7_Port3			73
#define Box7_Port4			74
#define Box7_Port5			75
#define Box7_Port6			76
#define Box7_Port7			77
#define Box7_Port8			78
#define Box8_Port1			81
#define Box8_Port2			82
#define Box8_Port3			83
#define Box8_Port4			84
#define Box8_Port5			85
#define Box8_Port6			86
#define Box8_Port7			87
#define Box8_Port8			88

//Analog I/O
#define VPin                0
#define HPin                1
#define PHPin               6
#define Piezo               16

//Digital I/O
#define ledPin              7
#define tempPin             8
#define actinicPWMPin       9
#define daylightPWMPin      10
#define lowATOPin           11
#define highATOPin          12
#define okPin               13

// I2C Addresses
#define I2CEEPROM1          0x50
#define I2CEEPROM2          0x54
#define I2CClock            0x68
#define I2CExpander1        0x20
#define I2CExpander2        0x21
#define I2CExpModule        0x38 // 0x38-3f
#define I2CORP				0X4c
#define I2CSalinity			0X4d
#define I2CPWM				0x08
#define I2CIO				0x09
#define I2CRF				0X10

// I2C Images Addresses
#define I2CEEPROM2_Main              0     //0-2999
#define I2CEEPROM2_Feeding           9919  //3000-4999
#define I2CEEPROM2_Water_Change      11919  //15300-18104

// Timer Indices
#define FEEDING_TIMER			0
#define LCD_TIMER				3
#define PORTAL_TIMER			4
#define STORE_PARAMS_TIMER		5

// Temp Sensor References
#define T1_PROBE		1
#define T2_PROBE		2
#define T3_PROBE		3

/*
EEPROM locations

To Read/Write to these locations, use the InternalMemory variable.
Most (if not all) of these memory locations have a read/write function already created.

If a memory location does not have a specific function to read/write to it, you may use the
read/write or read_int/write_int functions to read/write to the memory location.

Byte variables need 1 block
Int variables need 2 blocks

When adding more variables, use the previous value plus 1 or 2
  depending on the previous values storage requirement
*/
#define ATO_Single_Exceed_Flag	  733	//747
#define ATO_Exceed_Flag			  734	//748
#define Overheat_Exceed_Flag	  735	//749

#define MAX_ATO_LOG_EVENTS		  4
#define ATOEventSize			  8
#define ATOEventOffStart		  4
#define ATOEventStart			  736	//750
#define ATOEvent1_On			  ATOEventStart
#define ATOEvent1_Off			  ATOEventStart+4
#define ATOEvent2_On			  ATOEventStart+8
#define ATOEvent2_Off			  ATOEventStart+12
#define ATOEvent3_On			  ATOEventStart+16
#define ATOEvent3_Off			  ATOEventStart+20
#define ATOEvent4_On			  ATOEventStart+24
#define ATOEvent4_Off			  ATOEventStart+28
#define ATOHighEvent1_On			  ATOEventStart+32
#define ATOHighEvent1_Off			  ATOEventStart+36
#define ATOHighEvent2_On			  ATOEventStart+40
#define ATOHighEvent2_Off			  ATOEventStart+44
#define ATOHighEvent3_On			  ATOEventStart+48
#define ATOHighEvent3_Off			  ATOEventStart+52
#define ATOHighEvent4_On			  ATOEventStart+56
#define ATOHighEvent4_Off			  ATOEventStart+60

#define VarsStart                 800
#define Mem_B_MHOnHour            VarsStart
#define Mem_B_MHOnMinute          VarsStart+1
#define Mem_B_MHOffHour           VarsStart+2
#define Mem_B_MHOffMinute         VarsStart+3
#define Mem_B_StdLightsOnHour     VarsStart+4
#define Mem_B_StdLightsOnMinute   VarsStart+5
#define Mem_B_StdLightsOffHour    VarsStart+6
#define Mem_B_StdLightsOffMinute  VarsStart+7
#define Mem_I_WM1Timer            VarsStart+8
#define Mem_I_WM2Timer            VarsStart+10
#define Mem_B_DP1Timer            VarsStart+12
#define Mem_B_DP2Timer            VarsStart+13
#define Mem_I_FeedingTimer        VarsStart+14
#define Mem_I_LCDTimer            VarsStart+16
#define Mem_I_OverheatTemp        VarsStart+18
#define Mem_B_LEDPWMDaylight      VarsStart+20
#define Mem_B_LEDPWMActinic       VarsStart+21
#define Mem_I_HeaterTempOn        VarsStart+22
#define Mem_I_HeaterTempOff       VarsStart+24
#define Mem_I_ChillerTempOn       VarsStart+26
#define Mem_I_ChillerTempOff      VarsStart+28
#define Mem_B_ATOTimeout          VarsStart+30
#define Mem_I_PHMax               VarsStart+31
#define Mem_I_PHMin               VarsStart+33
#define Mem_B_MHDelay             VarsStart+35
#define Mem_B_DP1OnHour           VarsStart+36
#define Mem_B_DP1OnMinute         VarsStart+37
#define Mem_B_DP2OnHour           VarsStart+38
#define Mem_B_DP2OnMinute         VarsStart+39
#define Mem_B_ATOHourInterval     VarsStart+40
#define Mem_B_ATOHighHourInterval VarsStart+41
#define Mem_B_ATOHighTimeout      VarsStart+42
#define Mem_I_DP1RepeatInterval	  VarsStart+43
#define Mem_I_DP2RepeatInterval	  VarsStart+45
#define Mem_I_SalMax			  VarsStart+47
#define Mem_B_PWMSlopeStartD      VarsStart+49
#define Mem_B_PWMSlopeEndD	      VarsStart+50
#define Mem_B_PWMSlopeDurationD   VarsStart+51
#define Mem_B_PWMSlopeStartA      VarsStart+52
#define Mem_B_PWMSlopeEndA	      VarsStart+53
#define Mem_B_PWMSlopeDurationA   VarsStart+54
#define Mem_B_RFMode			  VarsStart+55
#define Mem_B_RFSpeed			  VarsStart+56
#define Mem_B_RFDuration		  VarsStart+57
#define Mem_B_PWMSlopeStart0      VarsStart+58
#define Mem_B_PWMSlopeEnd0	      VarsStart+59
#define Mem_B_PWMSlopeDuration0   VarsStart+60
#define Mem_B_PWMSlopeStart1      VarsStart+61
#define Mem_B_PWMSlopeEnd1	      VarsStart+62
#define Mem_B_PWMSlopeDuration1   VarsStart+63
#define Mem_B_PWMSlopeStart2      VarsStart+64
#define Mem_B_PWMSlopeEnd2	      VarsStart+65
#define Mem_B_PWMSlopeDuration2   VarsStart+66
#define Mem_B_PWMSlopeStart3      VarsStart+67
#define Mem_B_PWMSlopeEnd3	      VarsStart+68
#define Mem_B_PWMSlopeDuration3   VarsStart+69
#define Mem_B_PWMSlopeStart4      VarsStart+70
#define Mem_B_PWMSlopeEnd4	      VarsStart+71
#define Mem_B_PWMSlopeDuration4   VarsStart+72
#define Mem_B_PWMSlopeStart5      VarsStart+73
#define Mem_B_PWMSlopeEnd5	      VarsStart+74
#define Mem_B_PWMSlopeDuration5   VarsStart+75
#define Mem_I_ATOExtendedTimeout  VarsStart+76
#define Mem_I_ATOHighExtendedTimeout  VarsStart+78
#define Mem_I_ORPMin			  VarsStart+80
#define Mem_I_ORPMax			  VarsStart+82
#define Mem_B_ActinicOffset		  VarsStart+84
#define Mem_I_CO2ControlOn        VarsStart+85
#define Mem_I_CO2ControlOff       VarsStart+87
#define Mem_I_PHControlOn         VarsStart+89
#define Mem_I_PHControlOff        VarsStart+91
#define Mem_B_AISlopeStartW       VarsStart+93
#define Mem_B_AISlopeEndW	      VarsStart+94
#define Mem_B_AISlopeDurationW    VarsStart+95
#define Mem_B_AISlopeStartB       VarsStart+96
#define Mem_B_AISlopeEndB	      VarsStart+97
#define Mem_B_AISlopeDurationB    VarsStart+98
#define Mem_B_AISlopeStartRB      VarsStart+99
#define Mem_B_AISlopeEndRB	      VarsStart+100
#define Mem_B_AISlopeDurationRB   VarsStart+101
#define Mem_B_RadionSlopeStartW       VarsStart+102
#define Mem_B_RadionSlopeEndW	      VarsStart+103
#define Mem_B_RadionSlopeDurationW    VarsStart+104
#define Mem_B_RadionSlopeStartRB      VarsStart+105
#define Mem_B_RadionSlopeEndRB	      VarsStart+106
#define Mem_B_RadionSlopeDurationRB   VarsStart+107
#define Mem_B_RadionSlopeStartR       VarsStart+108
#define Mem_B_RadionSlopeEndR	      VarsStart+109
#define Mem_B_RadionSlopeDurationR    VarsStart+110
#define Mem_B_RadionSlopeStartG       VarsStart+111
#define Mem_B_RadionSlopeEndG	      VarsStart+112
#define Mem_B_RadionSlopeDurationG    VarsStart+113
#define Mem_B_RadionSlopeStartB       VarsStart+114
#define Mem_B_RadionSlopeEndB	      VarsStart+115
#define Mem_B_RadionSlopeDurationB    VarsStart+116
#define Mem_B_RadionSlopeStartI       VarsStart+117
#define Mem_B_RadionSlopeEndI	      VarsStart+118
#define Mem_B_RadionSlopeDurationI    VarsStart+119
#define Mem_B_DelayedStart            VarsStart+120

#define VarsEnd                 VarsStart+121
// Next value starts VarsStart+121

// Internal Memory Check Pointer
#define IMPointer			600

// EEProm Pointers
#define PH_Min		        949
#define PH_Max		        951
#define T1Pointer			953

#define bit9600Delay 		101
#define KeyPressRate		250
#define DEGREE_F            0
#define DEGREE_C            1
#define DEGREE_F_LOW_TEMP			700
#define DEGREE_F_HIGH_TEMP			900
#define DEGREE_F_OVERHEAT_LOW_TEMP	800
#define DEGREE_F_OVERHEAT_HIGH_TEMP	2000
#define DEGREE_C_LOW_TEMP			211
#define DEGREE_C_HIGH_TEMP			323
#define DEGREE_C_OVERHEAT_LOW_TEMP	266
#define DEGREE_C_OVERHEAT_HIGH_TEMP	933
#define CONVERT_TO_C(x)	((x) - 320)/1.8
#define CONVERT_TO_F(x)	((x) * 1.8) + 320

#define MAX_TEMP_SWING      50  // Maximum value of temp change that can occur in 1 second, 100 = 10.0 F Degrees, 50 = 5.0 F Degrees
// WEB_BANNER_QTY - Quantity of Parameters when sending the web banner, default is 12 but changes as we have more relays installed
#ifdef RelayExp
#if InstalledRelayExpansionModules == 1
#define WEB_BANNER_QTY		20
#elif InstalledRelayExpansionModules == 2
#define WEB_BANNER_QTY		28
#elif InstalledRelayExpansionModules == 3
#define WEB_BANNER_QTY		36
#elif InstalledRelayExpansionModules == 4
#define WEB_BANNER_QTY		44
#elif InstalledRelayExpansionModules == 5
#define WEB_BANNER_QTY		52
#elif InstalledRelayExpansionModules == 6
#define WEB_BANNER_QTY		60
#elif InstalledRelayExpansionModules == 7
#define WEB_BANNER_QTY		68
#else
#define WEB_BANNER_QTY		72
#endif  // InstalledRelayExpansionModules
#else
#define WEB_BANNER_QTY		12  // Quantity of parameters when sending the web banner: userid, 3 probe labels, 8 relay labels
#endif  // RelayExp

// Menu Defines
#if defined SIMPLE_MENU || defined CUSTOM_MENU
// This overrides everything else with the menus
// If it's enabled there is only 1 menu
#define Total_Menus			1

#ifdef CUSTOM_MENU
// ensure proper number of menu entries are defined, force at least 1 entry
#if !defined CUSTOM_MENU_ENTRIES || defined CUSTOM_MENU_ENTRIES == 0
#undef CUSTOM_MENU_ENTRIES
#define CUSTOM_MENU_ENTRIES	1
#endif  // CUSTOM_MENU_ENTRIES
#endif  // CUSTOM_MENU

#else  // SIMPLE_MENU
// Standard Menu Checks
#if defined SetupExtras || defined ATOSetup
	#ifndef RemoveAllLights
		#define Total_Menus         5    // total number of menus
	#else  // RemoveAllLights
		#define Total_Menus         4
	#endif  // RemoveAllLights
#else  // if defined SetupExtras || defined ATOSetup
	#ifndef RemoveAllLights
		#define Total_Menus         4    // total number of menus
	#else // RemoveAllLights
		#define Total_Menus         3
	#endif  // RemoveAllLights
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // SIMPLE_MENU || CUSTOM_MENU


#define MENU_TIMEOUT        10   // timeout for menu to return to main screen
#define MENU_HEADING_SIZE   25   // pixels used for Menu Heading
#define MENU_ITEM_HEIGHT    9    // pixel height of a line of text
#define MENU_START_COL      7
#define MENU_START_ROW      10
#define MENU_END_COL        124
#define MAX_Y               128
#define MAX_X               128
#define DEFAULT_MENU        255
#define EXCEED_TIMEOUT_MENU 254
#define FEEDING_MODE		253
#define WATERCHANGE_MODE	252
#define ALT_SCREEN_MODE		251
#define RETURN_MAIN_MODE	250
#define DEFAULT_MENU_ITEM   0     // default menu item, first item on menu
#define MAIN_MENU           0

// PWM Expansion Modes
#define PWM_DATA_DEFAULT		0	// Default data
#define PWM_CMD_CLOUDS			6	// Clouds command
#define PWM_DATA_CLOUDS_SLOW	1	// Slow clouds, must be used with CLOUDS cmd
#define PWM_DATA_CLOUDS_FAST	15  // Fast clouds, must be used with CLOUDS cmd
#define PWM_CMD_TSTORM1			7	// Thunderstorm 1 command, use data default
#define PWM_CMD_TSTORM2			8	// Thunderstorm 2 command, use data default

#ifndef COLORS_PDE
//  Global Colors
#define COLOR_BLACK                 0x00
#define COLOR_NAVY                  0x02
#define COLOR_BLUE                  0x03
#define COLOR_DARKGREEN             0x0C
#define COLOR_GREEN                 0x10
#define COLOR_TEAL                  0x12
#define COLOR_LIGHT_BLUE            0x13
#define COLOR_DEEPSKYBLUE           0x17
#define COLOR_DARKTURQUOISE         0x1A
#define COLOR_LIME                  0x1C
#define COLOR_SPRINGGREEN           0x1D
#define COLOR_MEDIUMSPRINGGREEN     0x1E
#define COLOR_CYAN                  0x1F
#define COLOR_MIDNIGHTBLUE          0x25
#define COLOR_DARKSLATEGREY         0x29
#define COLOR_FORESTGREEN           0x30
#define COLOR_SEAGREEN              0x31
#define COLOR_DODGERBLUE            0x33
#define COLOR_LIGHTSEAGREEN         0x36
#define COLOR_LIMEGREEN             0x39
#define COLOR_INDIGO                0x42
#define COLOR_DARKSLATEBLUE         0x4A
#define COLOR_DARKOLIVEGREEN        0x4D
#define COLOR_ROYALBLUE             0x4F
#define COLOR_STEELBLUE             0x52
#define COLOR_MEDIUMSEAGREEN        0x55
#define COLOR_TURQUOISE             0x5A
#define COLOR_SLATEBLUE             0x6A
#define COLOR_DIMGREY               0x6D
#define COLOR_MEDIUMSLATEBLUE       0x6F
#define COLOR_OLIVEDRAB             0x70
#define COLOR_SLATEGREY             0x72
#define COLOR_CORNFLOWERBLUE        0x73
#define COLOR_MEDIUMAQUAMARINE      0x7A
#define COLOR_LAWNGREEN             0x7C
#define COLOR_AQUAMARINE            0x7E
#define COLOR_MAROON                0x80
#define COLOR_PURPLE                0x82
#define COLOR_DARKORCHID            0x86
#define COLOR_BLUEVIOLET            0x87
#define COLOR_SADDLEBROWN           0x88
#define COLOR_SIENNA                0x89
#define COLOR_MEDIUMPURPLE          0x8F
#define COLOR_OLIVE                 0x90
#define COLOR_GRAY                  0x92
#define COLOR_DARKSEAGREEN          0x96
#define COLOR_YELLOWGREEN           0x99
#define COLOR_SKYBLUE               0x9B
#define COLOR_LIGHTGREEN            0x9E
#define COLOR_BROWN                 0xA4
#define COLOR_MEDIUMVIOLETRED       0xA6
#define COLOR_MEDIUMORCHID          0xAA
#define COLOR_DARKGOLDENROD         0xB0
#define COLOR_ROSYBROWN             0xB2
#define COLOR_DARKKHAKI             0xB5
#define COLOR_SILVER                0xB6
#define COLOR_LIGHTSTEELBLUE        0xB7
#define COLOR_POWDERBLUE            0xBB
#define COLOR_GREENYELLOW           0xBD
#define COLOR_PALETURQUOISE         0xBF
#define COLOR_CRIMSON               0xC5
#define COLOR_CHOCOLATE             0xCC
#define COLOR_INDIANRED             0xCD
#define COLOR_PALEVIOLETRED         0xCE
#define COLOR_ORCHID                0xCF
#define COLOR_DARKSALMON            0xD1
#define COLOR_PLUM                  0xD3
#define COLOR_GOLDENROD             0xD4
#define COLOR_TAN                   0xD6
#define COLOR_THISTLE               0xD7
#define COLOR_LIGHTGRAY             0xDA
#define COLOR_LAVENDER              0xDB
#define COLOR_LIGHTCYAN             0xDF
#define COLOR_RED                   0xE0
#define COLOR_MAGENTA               0xE3
#define COLOR_DEEPPINK              0xE6
#define COLOR_ORANGERED             0xE8
#define COLOR_TOMATO                0xED
#define COLOR_HOTPINK               0xEE
#define COLOR_DARKORANGE            0xF0
#define COLOR_SALMON                0xF1
#define COLOR_LIGHTCORAL            0xF2
#define COLOR_VIOLET                0xF3
#define COLOR_ORANGE                0xF4
#define COLOR_SANDYBROWN            0xF5
#define COLOR_PINK                  0xF6
#define COLOR_GOLD                  0xF8
#define COLOR_KHAKI                 0xFA
#define COLOR_ANTIQUEWHITE          0xFB
#define COLOR_YELLOW                0xFC
#define COLOR_LIGHTGOLDENRODYELLOW  0xFE
#define COLOR_WHITE                 0xFF

/*
Ability to completely customize all colors of the screen
To change an aspect of the screen, you simply need to change the color defined for the variable.
The variables are labeled intuitively.  Please note that when changing the entire screen color
to another color, you must make sure that all the corresponding BG colors (background colors) match
otherwise you will have a non-consistent background screen.  This is primarily the case when displaying
the main screen / homepage.

Also note that if you intend to change the colors, you will want to experiment to find a good color
combination.  If the background color is changed, you will most likely need to alter all of the other
colors in order for the screen to be easily readable.

You can use any of the colors specified above OR you can specify your own colors in hex code.
The values can range from 0x00 to 0xFF.  Most of the common colors are specified above, so the use
of your own hex code (number) is most likely not needed.  If you want to see what the colors will look like,
on the ReefAngel Google Groups page is a Color Chart image that will show you the colors.
*/

#define T1TempColor         COLOR_RED  // Text color for the T1 temp probe (homescreen)
#define T2TempColor         COLOR_CHOCOLATE  // Text color for the T2 temp probe (homescreen)
#define T3TempColor         COLOR_MEDIUMORCHID  // Text color for the T3 temp probe (homescreen)
#define PHColor             COLOR_SEAGREEN  //originally 0x34, Text color for the PH probe (homescreen)
#define DPColor             COLOR_SADDLEBROWN // Text color for Daylight PWM value (homescreen)
#define APColor             COLOR_DARKSLATEBLUE  //originally 0x49, Text color for the Actinic PWM value (homescreen)
#define CalibrateColor      COLOR_RED  // color of the PH calibration text values
#define ModeScreenColor     COLOR_BLUE  // color of the text on Feeding Mode, Water Change Mode and Version screens
#define DateTextColor       COLOR_RED  // color of the date on home screen
#define OutletBorderColor   0x54  // Border around the outlet box on home screen
#define OutletOnBGColor     COLOR_GREEN  // Outlet box background color when outlet is on (homescreen)
#define OutletOnFGColor     COLOR_WHITE  // Outlet box text (number) color when outlet is on (homescreen)
#define OutletOffBGColor    COLOR_RED  // Outlet box background color when outlet is off (homescreen)
#define OutletOffFGColor    COLOR_WHITE  // Outlet box text (number) color when outlet is off (homescreen)
#define BtnBorderColor      COLOR_BLACK  // OK & Cancel button border color (setup screens)
#define BtnActiveColor      COLOR_GRAY  // OK & Cancel button ACTIVE (selected) background color (seup screens)
#define BtnInactiveColor    COLOR_LIGHTGRAY  // OK & Cancel button INACTIVE (not selected) background color (setup screens)
#define SelectionBGColor    COLOR_BLUE  // Background color for selected items (Menu & Setup screens)
#define SelectionFGColor    COLOR_WHITE  // Foreground (text) color for selected items (Menu & Setup screens)
#define DefaultBGColor      COLOR_WHITE  // Default background color
#define DefaultFGColor      COLOR_BLACK  // Default text color
#define GraphDotLineColor   0x49    // color of the dotted line in the middle of the graph

#endif  // COLORS_PDE

/*
Graph Range values

Below are listed the ranges for each one of the graph lines.  The range is the specified value for being
"low" and "high".  So the "low" value is the bottom of the graph and the "high" value is the top of the graph.
If the actual value observed is above or below the "high" and "low" value respectively, it will be capped
at the appropriate value.  Meaning, if the T1 high value is set to 90.0 F and the controller actually reads
95.0 F, the highest value reported in the graph will be 90.0 F.

The defaults should work just fine more everybody, but some people may want to alter the values.  This would be
necessary if you didn't want T2 to be used for the lights temperature.

The defaults are as follows:
formate is:  Probe: LOW - HIGH

T1: 70.0F - 90.0F
T2: 65.0F - 150.0F
T3: 65.0F - 92.0F
PH: 7 - 10
*/
#define T1LOW   700
#define T1HIGH  900
#define T2LOW   650
#define T2HIGH  1500
#define T3LOW   650
#define T3HIGH  920
#define PHLOW   550
#define PHHIGH  840

/*
Vortech Modes

Used by the RF Expansion Module
 */
#define Constant      0
#define Lagoon		  1
#define Random1       1 // Lagoonal
#define ReefCrest	  2
#define Random2       2 // Reef Crest
#define ShortPulse	  3
#define ShortWave     3
#define LongPulse	  4
#define LongWave      4
#define NutrientTransport	5
#define Smart_NTM     5 // Nutrient Transport Mode
#define TidalSwell	  6
#define Smart_TSM     6 // Tidal Swell Mode
#define Feeding_Start 7
#define Feeding_Stop  8
#define Night_Stop	  8
#define Night         9
#define Storm         10
#define Custom        11
#define Slave_Start   97
#define Slave_Stop    98
#define None          99
#define Radion        100

// Radion Channels
#define Radion_White      0
#define Radion_RoyalBlue  1
#define Radion_Red        2
#define Radion_Green      3
#define Radion_Blue       4
#define Radion_Intensity  5
#define Radion_Ready      9

/*
AI Channels

Used by the AI Functions
 */
#define White			0
#define Blue			1
#define RoyalBlue		2
const byte RawChannel[]={67,65,66};

// Parameters structure, moved from RA_NokiaLCD.h to a more central location
typedef struct  {
  int Temp[4];
  int PH;
  int Salinity;
  int ORP;
} ParamsStruct;

// Temperature units
#define Celsius		1
#define Fahrenheit	0

//ReefTouch Block

typedef struct Calibration
{
	int XMin;
	int XMax;
	int YMin;
	int YMax;
} CALIBRATION ;

typedef struct Compensation
{
	int XOff;
	int YOff;
} COMPENSATION ;

typedef struct {
    byte sig[4];
    long hdrSize;
    long width;
    long height;
    byte format;
    byte reserved0;
    byte colors;
    byte restartInterval;
    long reserved1;
} Img2;

// Reef Touch Colors
#define BLACK                       RGB565(0x00, 0x00, 0x00)
#define WHITE                       RGB565(0xFF, 0xFF, 0xFF)
#define RED                         RGB565(0xFF, 0x00, 0x00)
#define GREEN                       RGB565(0x00, 0xFF, 0x00)
#define BLUE                        RGB565(0x00, 0x00, 0xFF)
#define YELLOW                      RGB565(0xFF, 0xFF, 0x00)
#define MAGENTA                     RGB565(0xFF, 0x00, 0xFF)
#define CYAN                        RGB565(0x00, 0xFF, 0xFF)
#define GRAY                        RGB565(0x80, 0x80, 0x40)
#define SILVER                      RGB565(0xA0, 0xA0, 0x80)
#define GOLD                        RGB565(0xA0, 0xA0, 0x40)
#define ORANGE						RGB565(0xFF, 0x80, 0x00)
#define TOPBAR_BC					RGB565(0x3A, 0x8C, 0x98)
#define TOPBAR_FC					WHITE
#define BKCOLOR						BLACK
#define DIVISION					RGB565(0x40, 0x40, 0x40)
#define RELAYBOXLABELBAR			RGB565(0xDC, 0xAC, 0xDE)
#define PWMLABELBAR					RGB565(0xF7, 0xBC, 0x54)
#define RFLABELBAR					RGB565(0xF6, 0x03, 0xFF)
#define AILABELBAR					RGB565(0xFF, 0x8A, 0x00)
#define RELAYGREEN                  RGB565(0x00, 0xAA, 0x00)

//Internal EEPROM
#define TS_CALIBRATION_ADDRESS 		0x0
#define TT_COMPENSATION_ADDRESS 	0x10
#define	Probe1Name					0x20
#define	Probe2Name					0x30
#define	Probe3Name					0x40

#define	PWMChannel1					0x50
#define	PWMChannel2					0x60
#define	PWMChannel3					0x70
#define	PWMChannel4					0x80
#define	PWMChannel5					0x90
#define	PWMChannel6					0xa0

#define	R1Name						0x500
#define	R2Name						0x510
#define	R3Name						0x520
#define	R4Name						0x530
#define	R5Name						0x540
#define	R6Name						0x550
#define	R7Name						0x560
#define	R8Name						0x570
#define	R11Name						0x580
#define	R12Name						0x590
#define	R13Name						0x5a0
#define	R14Name						0x5b0
#define	R15Name						0x5c0
#define	R16Name						0x5d0
#define	R17Name						0x5e0
#define	R18Name						0x5f0
#define	R21Name						0x600
#define	R22Name						0x610
#define	R23Name						0x620
#define	R24Name						0x630
#define	R25Name						0x640
#define	R26Name						0x650
#define	R27Name						0x660
#define	R28Name						0x670
#define	R31Name						0x680
#define	R32Name						0x690
#define	R33Name						0x6a0
#define	R34Name						0x6b0
#define	R35Name						0x6c0
#define	R36Name						0x6d0
#define	R37Name						0x6e0
#define	R38Name						0x6f0
#define	R41Name						0x700
#define	R42Name						0x710
#define	R43Name						0x720
#define	R44Name						0x730
#define	R45Name						0x740
#define	R46Name						0x750
#define	R47Name						0x760
#define	R48Name						0x770
#define	R51Name						0x780
#define	R52Name						0x790
#define	R53Name						0x7a0
#define	R54Name						0x7b0
#define	R55Name						0x7c0
#define	R56Name						0x7d0
#define	R57Name						0x7e0
#define	R58Name						0x7f0
#define	R61Name						0x800
#define	R62Name						0x810
#define	R63Name						0x820
#define	R64Name						0x830
#define	R65Name						0x840
#define	R66Name						0x850
#define	R67Name						0x860
#define	R68Name						0x870
#define	R71Name						0x880
#define	R72Name						0x890
#define	R73Name						0x8a0
#define	R74Name						0x8b0
#define	R75Name						0x8c0
#define	R76Name						0x8d0
#define	R77Name						0x8e0
#define	R78Name						0x8f0
#define	R81Name						0x900
#define	R82Name						0x910
#define	R83Name						0x920
#define	R84Name						0x930
#define	R85Name						0x940
#define	R86Name						0x950
#define	R87Name						0x960
#define	R88Name						0x970

// IsRelayPresent function from Don Edvalson
#define MAIN_RELAY 0xff
#define EXP1_RELAY 0
#define EXP2_RELAY 1
#define EXP3_RELAY 2
#define EXP4_RELAY 3
#define EXP5_RELAY 4
#define EXP6_RELAY 5
#define EXP7_RELAY 6
#define EXP8_RELAY 7

//Main Screens
#define MAIN_SCREEN			0
#define RELAY_BOX			1
#define EXP_BOX_1			2
#define EXP_BOX_2			3
#define EXP_BOX_3			4
#define EXP_BOX_4			5
#define EXP_BOX_5			6
#define EXP_BOX_6			7
#define EXP_BOX_7			8
#define EXP_BOX_8			9
#define PWM_SCREEN			10
#define RF_SCREEN			11
#define AI_SCREEN			12

#define TT_SENSITIVITY					30
#define MAX_APP_BUFFER 					768
#define SplashDuration					10
#define TouchSample						5
#define TouchSlideDelta					20
#define TouchPressure					900
#define MAX_RELAY_EXPANSION_MODULES		8
#define FONT_HEADER 					7
#define TS_CALIBRATION_XMIN				365
#define TS_CALIBRATION_XMAX				3825
#define TS_CALIBRATION_YMIN				218
#define TS_CALIBRATION_YMAX				3571
#define TS_CALIBRATION_DELTA			250
#define MAX_SCREENS						12 // Highest ID for main screens

// Global macros
#define SIZE(array) (sizeof(array) / sizeof(*array))
// color definition
#define RGB565(r, g, b) ((uint16_t)(((r >> 3) << 11)| ((g >> 2) << 5)| (b >> 3)))

// external globally defined variables
extern byte ButtonPress;  // Defined in ReefAngel.cpp, used for joystick button presses
extern OneWire ds;  // Defined in TempSensor.cpp, used for TempSensorClass
// delayed on variables, Defined in Relay.cpp
extern uint32_t LastStart;
extern byte DelayedOnPorts;
#ifdef RelayExp
extern byte DelayedOnPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp
#ifdef ENABLE_ATO_LOGGING
extern byte AtoEventCount;  // Defined in RA_ATO.cpp
#endif  // ENABLE_ATO_LOGGING

// globally usable functions
byte intlength(int intin);
int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute);
bool IsLeapYear(int year);
byte PWMSlope(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte Duration, byte oldValue);
byte PWMParabola(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte oldValue);
byte MoonPhase();
void ConvertNumToString(char* string, int num, byte decimal);

// 16bit color alpha blend
int alphaBlend(int fgcolor, byte a);
int alphaBlend(int fgcolor, int bgcolor, byte a);

// for virtual functions
extern "C" void __cxa_pure_virtual(void);
// other fixes
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

#endif  // __GLOBALS_H__
