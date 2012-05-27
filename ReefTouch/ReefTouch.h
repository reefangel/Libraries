/*
 * Copyright 2011 Reef Angel / Roberto Imai
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
 
#ifndef ReefTouch_h
#define ReefTouch_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <Wire.h>
#include <SPI.h>
#include <Time.h>
#include <mmc.h>
#include <f8x8.h>
#include <f12x12.h>
#include <ArialBold20.h>
#include <SDFile.h>
#include <ReefAngel_Logo.h>
#include <Green.h>
#include <Red.h>
#include <Globals.h>
#include <OneWire.h>
#include <RT_PWM.h>
#include <Relay.h>
#include <AI.h>

// ReefTouch PROGMEM Strings
// Calibration
const prog_char CALI1[] PROGMEM = "Touch Screen";
const prog_char CALI2[] PROGMEM = "Calibration";
const prog_char CALI3[] PROGMEM = "and Tilt";
const prog_char CALI4[] PROGMEM = "Compensation";
const prog_char CALI5[] PROGMEM = "Please place the screen on";
const prog_char CALI6[] PROGMEM = "top of a flat surface and";
const prog_char CALI7[] PROGMEM = "touch the red circle";

// Labels
const prog_char TEMP[] PROGMEM = "Temp ";
const prog_char RELAY[] PROGMEM = "Relay ";
const prog_char CHANNEL[] PROGMEM = "Channel ";
const prog_char PH[] PROGMEM = "pH";
const prog_char SALINITY[] PROGMEM = "Salinity";
const prog_char ORP[] PROGMEM = "ORP";
const prog_char ACTINIC[] PROGMEM = "Actinic";
const prog_char DAYLIGHT[] PROGMEM = "Daylight";
const prog_char AI_WHITE[] PROGMEM = "White";
const prog_char AI_BLUE[] PROGMEM = "Blue";
const prog_char AI_ROYAL_BLUE[] PROGMEM = "Royal Blue";
const prog_char ATOHIGHPORT[] PROGMEM = "ATO Low";
const prog_char ATOLOWPORT[] PROGMEM = "ATO High";
const prog_char MODE[] PROGMEM = "Mode";
const prog_char DURATION[] PROGMEM = "Duration";
const prog_char SPEED[] PROGMEM = "Max. Speed";

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
const prog_char RF_EXPANSION_LABEL[] PROGMEM = "RF Expansion";
const prog_char AI_LABEL[] PROGMEM = "Aqua Illumination";
static PROGMEM const char *relay_items[] = {RELAY_BOX_LABEL, EXP_RELAY_1_LABEL, EXP_RELAY_2_LABEL, EXP_RELAY_3_LABEL, EXP_RELAY_4_LABEL, EXP_RELAY_5_LABEL, EXP_RELAY_6_LABEL, EXP_RELAY_7_LABEL, EXP_RELAY_8_LABEL, PWM_EXPANSION_LABEL, RF_EXPANSION_LABEL, AI_LABEL};

// RF Modes
const prog_char RF_CONSTANT[] PROGMEM = "Constant";
const prog_char RF_LAGOONAL[] PROGMEM = "Lagoonal";
const prog_char RF_REEFCREST[] PROGMEM = "Reef Crest";
const prog_char RF_SHORTWAVE[] PROGMEM = "Short Wave";
const prog_char RF_LONGWAVE[] PROGMEM = "Long Wave";
const prog_char RF_NTM[] PROGMEM = "Nutrient Transport";
const prog_char RF_TSM[] PROGMEM = "Tidal Swell";
const prog_char RF_FEEDING[] PROGMEM = "Feeding";
const prog_char RF_NIGHT[] PROGMEM = "Night";
const prog_char RF_SLAVE[] PROGMEM = "Slave Check";
const prog_char RF_None[] PROGMEM = "None";
static PROGMEM const char *rf_items[] = {RF_CONSTANT, RF_LAGOONAL, RF_REEFCREST, RF_SHORTWAVE, RF_LONGWAVE, RF_NTM, RF_TSM, RF_FEEDING, RF_FEEDING, RF_NIGHT};

// Hardware Ports
#define sbi(port,bitnum)		port |= _BV(bitnum)
#define cbi(port,bitnum)		port &= ~(_BV(bitnum))

#define CS0 cbi(PORTL,1)
#define CS1 sbi(PORTL,1)
#define RS0 cbi(PORTL,2)
#define RS1 sbi(PORTL,2)
#define WR0 cbi(PORTL,3)
#define WR1 sbi(PORTL,3)
#define RST0 cbi(PORTL,6)
#define RST1 sbi(PORTL,6)
#define BL0 cbi(PORTL,5)
#define BL1 sbi(PORTL,5)
//#define MMC0 cbi(PORTL, 0)
//#define MMC1 sbi(PORTL, 0)
#define TP0	cbi(PORTD, 7)
#define TP1 sbi(PORTD, 7)
#define ACC0 cbi(PORTL, 7)
#define ACC1 sbi(PORTL, 7)
#define LED0 cbi(PORTH, 4)
#define LED1 sbi(PORTH, 4)


typedef struct  { 
  int Temp1; 
  int Temp2;
  int Temp3;
  int PH;
  int Sal;
  int ORP;
} ParamsStruct;


class FontClass
{
	public:
		FontClass();
		void SetColor(int iFC, int iBC, boolean iTransparent);
		void SetPosition(int ix, int iy);
		void SetFont(uint8_t *iPtr);
		byte CharWidth(char c);
		int TextWidth(char *str);
		int TextWidthP(const prog_char *str);
//		void DrawText(const prog_char *str);
//		void DrawText(int ix, int iy, const prog_char *str);
//		void DrawText(int iFC, int iBC,int ix, int iy, const prog_char *str);
		void DrawChar(char c); 
		void DrawChar(int ix, int iy,char c);
		void DrawChar(int iFC, int iBC,int ix, int iy,char c);
		void DrawText(char *str);
		void DrawText(int ix, int iy, char *str);
		void DrawText(int iFC, int iBC,int ix, int iy, char *str);
		void DrawText(long n);
		void DrawText(int ix, int iy, long n);
		void DrawText(int iFC, int iBC,int ix, int iy, long n);
		void DrawTextP(const prog_char *str);
		void DrawTextP(int ix, int iy, const prog_char *str);
		void DrawTextP(int iFC, int iBC,int ix, int iy, const prog_char *str);
		void DrawCenterText(int ix, int iy, char *str);
		void DrawCenterTextP(int ix, int iy, const prog_char *str);
		void DrawCenterNumber(int ix, int iy, long n, byte decimal);
		int inline GetX() { return x; }


	private:
		uint8_t *Ptr;
		byte w, h, depth;
    	char FirstC, LastC;
		boolean Fixed, Transparent;
		int FC, BC;
		int  x, y;
		int  CS;
		
};

class TFT
{
	public:
		static void Init();
		static void SendComData(int Com,int Data);
		static void SendCom(byte H,byte L);
		static void SendData(byte H,byte L);
		static void WriteBus(byte H,byte L);
		static void Clear(int color, int x1, int y1, int x2, int y2);
		static void SetBox(int x1, int y1, int x2, int y2);
		static void Pixels(int count, const byte* d);
		static void DrawPixel(int color, int x, int y);
		static void Scroll(int offset);
};

class TouchScreen
{
	public:

		int X,Y,uX,uY;
		CALIBRATION calibration;
		TouchScreen();
		void Init();
		void SaveCalibration();
		void GetTouch();
		boolean IsTouched();
		boolean IsCalibrationNeeded();
		int FirstX, FirstY;
		signed char SlideIndex;
	private:
		boolean CalibrationNeeded;
		void ApplyCalibration();
};

class LCDClass
{
	public:
		LCDClass();
		void Init();
		void SetBacklight(byte bl);
		void SetOrientation(byte O);
		byte GetOrientation();
		int GetWidth();
		int GetHeight();
		void Scroll(int offset);
		void FullClear(int color);
		void Clear(int color, int x1, int y1, int x2, int y2);
		void DrawPixel(int color, int x, int y);
		void DrawLine(int color, int x1, int y1, int x2, int y2);
		void DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill);
		void DrawCircle(int color, int x, int y, int radius, boolean fill);
		void DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill);
		void DrawBMP(int ix, int iy, const prog_uchar *iPtr);
		void DrawImage(SDFile& f, int x, int y, int scroll = 0, int lines = 0);
		void DrawDateTime(int x, int y, boolean militarytime);
	private:
		int w,h;
		byte orientation;	
};

class TiltClass
{
	public:
		TiltClass();
		void Init();
		void Refresh();
		void CompensateAccelerometer();
		void ApplyCompensation();
		inline signed char  GetX() { return x; }
		inline signed char  GetY() { return y; }
	private:
		signed char x,y;
		COMPENSATION compensation;
		void Write(byte reg, byte data);
		byte Read(byte data);
};

class ButtonClass
{
	public:
		ButtonClass();
		void Create(int color, int textcolor, char *str);
		void SetPosition(int x1, int y1);
		void Show();
		void Hide();
		boolean IsPressed();
	private:
		int color, x1, x2, y1, textcolor;
		char *str;
		boolean visible;
};

class ReefTouchClass
{

	public:
		ReefTouchClass();
		ParamsStruct Params;
		LCDClass LCD;
		FontClass Font;
		TouchScreen TS;
		FontClass SmallFont;
		FontClass LargeFont;
		TiltClass Tilt;
		ButtonClass OkButton;
		RT_PWMClass PWM;
		RelayClass Relay;
#ifdef AI_LED
		AIClass AI;
#endif
		byte _buffer[MAX_APP_BUFFER];	// Applications run in this buffer
		SDFile File;
		boolean Splash;
		byte LastOrientation;
		byte LastRelayData;
		boolean MilitaryTime;
		signed char DisplayedScreen;
		boolean NeedsRedraw;
		void Init();
		void Refresh();
		void ShowInterface();
		void CalibrateTouchScreen();
		void ChangeDisplayedScreen(signed char index);
	private:
		byte ExpansionFlag;
		// Expansion Flag
		// 00000111
		//      |||_ PWM Expansion
		//      ||__ RF Expansion
		//      |___ AI Expansion
		
		byte RelayExpansionFlag;
		// Relay Expansion Flag
		// 11111111
		// ||||||||_ Exp Relay Box 1
		// |||||||__ Exp Relay Box 2
		// ||||||___ Exp Relay Box 3
		// |||||____ Exp Relay Box 4
		// ||||_____ Exp Relay Box 5
		// |||______ Exp Relay Box 6
		// ||_______ Exp Relay Box 7
		// |________ Exp Relay Box 8
		
};

extern ReefTouchClass ReefTouch;  // make an instance for the user


#endif 