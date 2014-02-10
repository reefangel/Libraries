// Evolution

#include <RA_Wifi.h>
#include <RA_TouchLCD.h>
#include <RA_TFT.h>
#include <RA_TS.h>
#include <RA_Tilt.h>
#include <Font.h>
#include <SPI.h>
#include <SD.h>
#include <avr/pgmspace.h>

#ifdef CUSTOM_MAIN
void DrawCustomMain();
void DrawCustomGraph();
#endif  // CUSTOM_MAIN

class ButtonClass
{
	public:
		ButtonClass();
		void Create(int color, int textcolor, char *str);
		void Create(int color, int textcolor, char *str, const prog_uchar *Ptr);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void inline SetLabel(char *value) { str=value; };
		void Show();
		void Hide(int bkcolor=BKCOLOR);
		boolean IsPressed();
	private:
		int color, x1, x2, y1, textcolor;
		char *str;
		const prog_uchar *Ptr;
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