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

#include "Font.h"
#include "RA_Wifi.h"

FontClass::FontClass()
{
	FC = COLOR_WHITE;
	BC = COLOR_BLACK;
	x = 0;
	y = 0;
	Transparent = false;
	CS=2;
	Fixed = true;	
}

void FontClass::SetColor(int iFC, int iBC, boolean iTransparent)
{
	FC=iFC;
	BC=iBC;
	Transparent=iTransparent;
}

void FontClass::SetPosition(int ix, int iy)
{
	x=ix;
	y=iy;
}

void FontClass::SetFont(uint8_t *iPtr)
{
	Ptr=iPtr;
	w = pgm_read_byte(Ptr + 2);
    h = pgm_read_byte(Ptr + 3);
    depth = pgm_read_byte(Ptr + 4);
    FirstC = pgm_read_byte(Ptr + 5);
    LastC = pgm_read_byte(Ptr + 6);
}

byte FontClass::CharWidth(char c)
{
	if(!Ptr) return 0; 
	if(c< FirstC || c>LastC) return 0;
	return  pgm_read_byte(Ptr + FONT_HEADER + c - FirstC);
}

int FontClass::TextWidth(char *str)
{
	int length=0;
	char c;
	while( c = *(str++))
	{
		length+=CharWidth(c);
		length+=2;
	}
	return length;
}

int FontClass::TextWidthP(const prog_char *str)
{
	int length=0;
	char c;
	while( c = pgm_read_byte(str++))
	{
		length+=CharWidth(c);
		length+=2;
	}
	return length;
}

void FontClass::DrawChar(int iFC, int iBC,int ix, int iy, char c)
{
	int TempFC,TempBC;
	
	TempFC=FC;
	TempBC=BC;
	FC=iFC;
	BC=iBC;
	x=ix;
	y=iy;
	DrawChar(c); 
	FC=TempFC;
	BC=TempBC;
	
}

void FontClass::DrawChar(int ix, int iy,char c)
{
	x=ix;
	y=iy;
	DrawChar(c); 
}


void FontClass::DrawChar(char c) 
{

    uint8_t *ptr;
    byte bit_num;
	byte font_data;
	byte i, j;
	int pix_x, pix_y;
    unsigned long index = 0;
  
    pingSerial();
    byte charwidth = CharWidth(c);
    if (charwidth == 0) return;
    byte width = charwidth + CS;
    if (Fixed && (width<w)) width = w;

//	if (x + charwidth  > TFT::GetWidth())
//	{
//		y+= h;
//		x = 0;
//        if ((y + h) > TFT::GetHeight()) y = 0;	
//	}
	
	if (depth & 0x80)
	//compressed font 
	{   	
		byte rle_table[4];
		byte padding, pixel_count = 0, bit_count=0,pixel_color;
		byte bitspixel;
		uint16_t bits = 0;
		
		bitspixel = depth & 0x7f;
		ptr = Ptr + FONT_HEADER + (LastC - FirstC +1);
		
		padding = pgm_read_byte(ptr++);
		
		rle_table[0] = 1;
		rle_table[1] = pgm_read_byte(ptr++);
		rle_table[2] = pgm_read_byte(ptr++);
		rle_table[3] = pgm_read_byte(ptr++);
	  
		for (i = FirstC; i < c; i++)
		{
			index += pgm_read_byte(ptr++);
		}	
		index *= padding;
		ptr = Ptr + FONT_HEADER + (LastC - FirstC +1)*2 + 4 + index;
		
		for(i=0;i<charwidth; i++)
		{
			pix_x = i + x;
		
			for(j=0; j<h; j++)
			{
//				if(LCD::GetOrientation() == LCD_HORIZONTAL)
					pix_y = j + y;
//				else
//					pix_y = (uint16_t)(j) + CursorY;
					
				if(pixel_count==0)
				{
					if(bit_count<=8)
					{
						bits |= (pgm_read_byte(ptr++))<<bit_count;
						bit_count +=8;
					}
					pixel_count = rle_table[bits & 0x3];
					bits>>=2;
					bit_count-=2;
					pixel_color = bits & (0xff>>(8-bitspixel));
					bits>>=bitspixel;
					bit_count-= bitspixel;
				}
				if(pixel_color)
				{
					RA_TFT::DrawPixel(FC, pix_x, pix_y);
				}
				else if(!Transparent)
				{
					RA_TFT::DrawPixel(BC, pix_x , pix_y);
				}
				pixel_count--;
			}
		}	
	}
	else   
	// uncompressed font
	{
		ptr = Ptr + FONT_HEADER;
		for (i = FirstC; i < c; i++)
		{
			index += pgm_read_byte(ptr++);
		}
  
		index *= h * depth;
	
		bit_num = index & 0x7;
		index = index>>3;
		ptr = Ptr + FONT_HEADER + (LastC - FirstC + 1) + index;
	
		font_data = pgm_read_byte(ptr++);
	
		for(i=0;i<charwidth; i++)
		{
			pix_x = x + i;
			
			for(j=0; j<h; j++)
			{
//				if(LCD::GetOrientation() == LCD_HORIZONTAL)
					pix_y = j + y;
//				else
//					pix_y = (uint16_t)(j)  + CursorY;
				
				if(font_data & (1<<bit_num))
				{
					RA_TFT::DrawPixel(FC, pix_x , pix_y);
				}
				else 
				{
					if(!Transparent) RA_TFT::DrawPixel(BC, pix_x , pix_y);
				}
				bit_num++;
				if(bit_num>7)
				{
					font_data = pgm_read_byte(ptr++);
					bit_num= 0;
				}
			}
		}
	}
	x += (charwidth + CS);
	if(!Transparent) RA_TFT::Clear(BC,x-CS,y,x,y+h);
}

//void FontClass::DrawText(int iFC, int iBC,int ix, int iy, const prog_char *str)
//{
//	int TempFC,TempBC;
//	
//	TempFC=FC;
//	TempBC=BC;
//	FC=iFC;
//	BC=iBC;
//	x=ix;
//	y=iy;
//	DrawText(str); 
//	FC=TempFC;
//	BC=TempBC;
//	
//}
//
//void FontClass::DrawText(int ix, int iy, const prog_char *str)
//{
//	x=ix;
//	y=iy;
//	DrawText(str); 
//}
//
//void FontClass::DrawText(const prog_char *str)
//{
//	char c;
//	while(c = pgm_read_byte(str++))	DrawChar(c);
//}

void FontClass::DrawText(int iFC, int iBC,int ix, int iy, char *str)
{
	int TempFC,TempBC;
	
	TempFC=FC;
	TempBC=BC;
	FC=iFC;
	BC=iBC;
	x=ix;
	y=iy;
	DrawText(str); 
	FC=TempFC;
	BC=TempBC;
	
}

void FontClass::DrawText(int ix, int iy, char *str)
{
	x=ix;
	y=iy;
	DrawText(str); 
}

void FontClass::DrawText(char *str)
{
	char c;
	while( c = *(str++))
		DrawChar(c);
}

void FontClass::DrawText(int iFC, int iBC,int ix, int iy, long n)
{
	int TempFC,TempBC;
	
	TempFC=FC;
	TempBC=BC;
	FC=iFC;
	BC=iBC;
	x=ix;
	y=iy;
	DrawText(n); 
	FC=TempFC;
	BC=TempBC;
	
}

void FontClass::DrawText(int ix, int iy, long n)
{
	x=ix;
	y=iy;
	DrawText(n); 
}

void FontClass::DrawText(long n)
{
   char buf[10];    
   uint8_t i=0;
   
   if(n==0)
   {
	   DrawChar('0');
   }
   else	if(n < 0)
   {
	   DrawChar('-');
	   n = -n;
   }
   while(n>0 && i <= 10)
   {
	   buf[i++] = (n % 10)+'0';  // n % base
	   n /= 10;   // n/= base
   }
   for(; i >0; i--)
   {
	   DrawChar(buf[i-1]);	  
   }
}

void FontClass::DrawText(int iFC, int iBC,int ix, int iy, String str)
{
	int TempFC,TempBC;
	char buff[str.length()+2];
	str.toCharArray(buff, str.length()+1);
	
	TempFC=FC;
	TempBC=BC;
	FC=iFC;
	BC=iBC;
	x=ix;
	y=iy;
	DrawText(buff); 
	FC=TempFC;
	BC=TempBC;
	
}

void FontClass::DrawText(int ix, int iy, String str)
{
	char buff[str.length()+2];
	str.toCharArray(buff, str.length()+1);
	x=ix;
	y=iy;
	DrawText(buff); 
}

void FontClass::DrawText(String str)
{
	char buff[str.length()+2];
	str.toCharArray(buff, str.length()+1);
	DrawText(buff);
}

void FontClass::DrawTextP(int iFC, int iBC,int ix, int iy, const prog_char *str)
{
	int TempFC,TempBC;
	
	TempFC=FC;
	TempBC=BC;
	FC=iFC;
	BC=iBC;
	x=ix;
	y=iy;
	DrawTextP(str); 
	FC=TempFC;
	BC=TempBC;	
}

void FontClass::DrawTextP(int ix, int iy, const prog_char *str)
{
	x=ix;
	y=iy;
	DrawTextP(str); 	
}

void FontClass::DrawTextP(const prog_char *str)
{
	char c;
	while( c = pgm_read_byte(str++))
		DrawChar(c);
}

void FontClass::DrawCenterText(int ix, int iy, char *str)
{
	int width=TextWidth(str);
	width/=2;
	DrawText(ix-width,iy,str);
}

void FontClass::DrawCenterText(int ix, int iy, String str)
{
	char buff[str.length()+2];
	str.toCharArray(buff, str.length()+1);
	DrawCenterText(ix, iy, buff);
}

void FontClass::DrawCenterTextP(int ix, int iy, const prog_char *str)
{
	int width=TextWidthP(str);
	width/=2;
	DrawTextP(ix-width,iy,str);
}

void FontClass::DrawCenterNumber(int ix, int iy, long n, byte decimal)
{
	char temp[6];
	int width;
	
	itoa(n,temp,10);
	width=TextWidth(temp);
	if (decimal!=0)
	{
		if (n<10) width+=TextWidth("0");
		width+=TextWidth(" . ");
		width/=2;
		DrawText(ix-width,iy," ");
		DrawText(n/decimal);
		DrawText(".");
		if (n%decimal<10 && decimal>99) DrawText("0");
		DrawText(n%decimal);
		DrawText(" ");
	}
	else
	{
		width+=TextWidth("  ");
		width/=2;
		DrawText(ix-width,iy," ");
		DrawText(n);
		DrawText(" ");
	}
}
