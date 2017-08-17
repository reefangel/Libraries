/*
 * Copyright 2010 Curt Binder
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

#ifndef __RA_CUSTOMCOLORS_H__
#define __RA_CUSTOMCOLORS_H__

#include <RA_Colors.h>

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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR

// Reef Touch Colors
#define COLOR_BLACK                 RGB565(0x00, 0x00, 0x00)
#define COLOR_WHITE                 RGB565(0xFF, 0xFF, 0xFF)
#define COLOR_MIDNIGHTBLUE          RGB565(0x00, 0x33, 0x66)
#define COLOR_ROYALBLUE             RGB565(0x45, 0x71, 0xda)
#define COLOR_LIGHTBLUE             RGB565(0xad, 0xd8, 0xe6)
#define COLOR_RED                   RGB565(0xFF, 0x00, 0x00)
#define COLOR_GREEN                 RGB565(0x00, 0xFF, 0x00)
#define COLOR_BLUE                  RGB565(0x00, 0x00, 0xFF)
#define COLOR_YELLOW                RGB565(0xFF, 0xFF, 0x00)
#define COLOR_MAGENTA               RGB565(0xFF, 0x00, 0xFF)
#define COLOR_CYAN                  RGB565(0x00, 0xFF, 0xFF)
#define COLOR_GRAY                  RGB565(0x80, 0x80, 0x40)
#define COLOR_SILVER                RGB565(0xA0, 0xA0, 0x80)
#define COLOR_GRAY88                RGB565(0xE0, 0xE0, 0xE0)
#define COLOR_GOLD                  RGB565(0xA0, 0xA0, 0x40)
#define COLOR_ORANGE				RGB565(0xFF, 0x80, 0x00)
#define TOPBAR_BC					COLOR_WHITE
#define TOPBAR_FC					COLOR_BLACK
#define BOTTOMBAR_BC				COLOR_WHITE
#define BOTTOMBAR_FC				COLOR_BLACK
#define BKCOLOR						COLOR_BLACK
#define WARNING_TEXT				COLOR_GOLD
#define DIVISION					RGB565(0x40, 0x40, 0x40)
#define RELAYBOXLABELBAR			RGB565(0xDC, 0xAC, 0xDE)
#define PWMLABELBAR					RGB565(0xF7, 0xBC, 0x54)
#define RFLABELBAR					RGB565(0x27, 0xA0, 0x78)
#define RFLABELBAR1					RGB565(0x46, 0xd1, 0xFF)
#define AILABELBAR					RGB565(0xFF, 0x8A, 0x00)
#define IOLABELBAR					RGB565(0x89, 0x21, 0x3a)
#define RELAYGREEN                  RGB565(0x00, 0xAA, 0x00)
#define DCLABELBAR					RGB565(0xA1, 0xC5, 0x59)
#define CVARLABELBAR				RGB565(0xF6, 0x03, 0xFF)
#define PWMWHITE					COLOR_ORANGE
#define PWMROYALBLUE				RGB565(0x0, 0x66, 0xCC)
#define PWMRED						COLOR_RED
#define PWMGREEN					COLOR_GREEN
#define PWMBLUE						COLOR_BLUE
#define PWMINTENSITY				COLOR_MAGENTA
#define DefaultBGColor				BKCOLOR

#else //  RA_TOUCH

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

#endif //  RA_TOUCH

#endif  // __RA_CUSTOMCOLORS_H__
