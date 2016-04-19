// Star

#include <RA_TouchLCD.h>
#include <RA_TFT.h>
#include <RA_TS.h>
#include <RA_Wiznet5100.h>
#include <RA_CustomLabels.h>
#include <Font.h>
#include <SPI.h>
#include <SD.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#define wdtenabled 1

#ifdef CUSTOM_MAIN
void DrawCustomMain();
void DrawCustomGraph();
#endif  // CUSTOM_MAIN

#ifdef CUSTOM_MENU
void MenuEntry1();
#if CUSTOM_MENU_ENTRIES >= 2
void MenuEntry2();
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
void MenuEntry3();
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
void MenuEntry4();
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
void MenuEntry5();
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
void MenuEntry6();
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
void MenuEntry7();
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
void MenuEntry8();
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
void MenuEntry9();
#endif  // CUSTOM_MENU_ENTRIES >= 9
#endif  // CUSTOM_MENU

class ButtonClass
{
	public:
		ButtonClass();
		void Create(int color, int textcolor, const char *str);
		void Create(int color, int textcolor, const char *str, const unsigned char *Ptr);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
//		void inline SetLabel(char *value) { str=value; Serial.println(str);};
//		void inline SetLabelP(const char *value) { char buff[16]; byte index=0; while( buff[index++] = pgm_read_byte(value++)); strcpy(str,buff); Serial.println(str);};
		void Show();
		void Hide(int bkcolor=BKCOLOR);
		boolean IsPressed();
	private:
		int color, x1, x2, y1, textcolor;
		const char *str;
		const unsigned char *Ptr;
		boolean visible;
};

class ProgressBarClass
{
	public:
		ProgressBarClass();
		void Create(int color, int bkcolor, int textcolor, char *str);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void SetCurrent(int value);
		int inline GetCurrent() { return current; };
		void inline SetLabel(char *value) { str=value; };
		void inline SetLabel(String value) { char buff[value.length()+2]; value.toCharArray(buff, value.length()+1); strcpy(str,buff); }; 
		void inline SetLabelP(const char *value) { char buff[16]; byte index=0; while( buff[index++] = pgm_read_byte(value++)); strcpy(str,buff); };
 		void inline SetColor(int value) { color=value; };
		void inline SetBackColor(int value) { bkcolor=value; };
		void inline SetTextColor(int value) { textcolor=value; };
		void Show();
		void Hide();
		boolean IsPressed();
		boolean NeedsRedraw;
	private:
		int color, bkcolor, x1, y1, textcolor;
		int min,max,current;
		char *str;
		boolean visible;
};

class SliderClass
{
	public:
		SliderClass();
		void Create(int color, int textcolor, char *str);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void inline SetMin(int value) { min=value; };
		void inline SetMax(int value) { max=value; };
		void inline SetCurrent(int value) { current=value; };
		void inline SetOverrideID(int value) { overrideid=value; };
		int inline GetMin() { return min; };
		int inline GetMax() { return max; };
		int inline GetCurrent() { return current; };
		int inline GetOverrideID() { return overrideid; };
		void inline SetColor(int value) { color=value; };
		void inline SetLabel(char *value) { str=value; };
		void inline SetLabel(String value) { char buff[value.length()+2]; value.toCharArray(buff, value.length()+1); strcpy(str,buff); };
		void inline SetLabelP(const char *value) { char buff[16]; byte index=0; while( buff[index++] = pgm_read_byte(value++)); strcpy(str,buff); };
		void DrawMarker();
		void Show();
		void Hide();
		boolean Refresh();
		boolean IsPlusPressed();
		boolean IsMinusPressed();
	private:
		int color, x1, y1, textcolor;
		int min,max,current;
		byte overrideid;
		char *str;
		boolean visible;
		boolean NeedsRedraw;
	
};
