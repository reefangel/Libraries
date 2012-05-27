#ifndef	Phillips6610LCD_h

#define Phillips6610LCD_h

#include "WProgram.h"


class Phillips6610LCD
{

public:



Phillips6610LCD();

void lcd_init();
void lcd_Sleep();
void lcd_Wake();
void lcd_BacklightOn();
void lcd_BacklightOff();
void lcd_clear(byte color, byte x1, byte y1, byte x2, byte y2);
void lcd_draw_text(byte fcolor, byte bcolor, byte x, byte y,char *text);
void lcd_put_pixel(byte color, byte x, byte y);
void lcd_set_box(byte x1, byte y1, byte x2, byte y2);
void sendCMD(byte data);
void sendData(byte data);

private:
void shiftBits(byte b);
void draw_text_line(byte fcolor, byte bcolor,byte x, byte y,char c);

};
#endif
