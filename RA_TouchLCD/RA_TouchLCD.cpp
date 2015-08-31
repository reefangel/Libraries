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

#include "RA_TouchLCD.h"

RA_TouchLCD::RA_TouchLCD()
{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
	DDRA=0xff; //PORTA (Data Output - D8-15)
	DDRC=0xff; //PORTC (Data Output - D0-7)
	PORTA=0xff; //PORTA pull up
	PORTC=0xff; //PORTC pull up
	DDRE|=(1<<3); //PE3 (Output) - LDC Data/Command
	PORTE|=(1<<3); //PE3 pull up
	DDRE|=(1<<4); //PE4 (Output) - LDC Backlight
	PORTE&=~(1<<4); //PE4 pull down
	DDRE|=(1<<5); //PE5 (Output) - LDC Chip Select
	PORTE|=(1<<5); //PE5 pull up
	DDRE|=(1<<6); //PE6 (Output) - LDC Write
	PORTE|=(1<<6); //PE6 pull up
	DDRE|=(1<<7); //PE7 (Output) - LDC Read
	PORTE|=(1<<7); //PE7 pull up
	DDRH|=(1<<3); //PH3 (Output) - LDC Reset
	PORTH|=(1<<3); //PH3 pull up
#elif defined(__SAM3X8E__)
//	PIOC->PIO_OER |= 0x001FFFFE;
//	PIOC->PIO_OWER = 0x0001FFFE;
//	PIOC->PIO_OWDR = 0xFFFE0001;
	
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
}

void RA_TouchLCD::Init()
{
	RA_TFT::Init();
	SetOrientation(1);
}

void RA_TouchLCD::SetBacklight(byte bl)
{
	analogWrite(TouchBL,bl*2.55);
}

void RA_TouchLCD::SetOrientation(byte O)
{
	if (orientation!=O)
	{
		
		orientation=O;
		switch (O)
		{
		case 1:
#ifdef HX8347G
			RA_TFT::SendComData(0x0016,0x0048); // Normal Rotation
#endif
#ifdef HX8347D	
			RA_TFT::SendComData(0x0016,0x000f); // Normal Rotation
#endif
#ifdef ILI9341
			RA_TFT::SendComData(0x0036,0x0048); // Normal Rotation
#endif
			w=239;
			h=319;
			break;
		case 2:
#ifdef HX8347G
			RA_TFT::SendComData(0x0016,0x00e8); // 90 Rotation
#endif
#ifdef HX8347D	
			RA_TFT::SendComData(0x0016,0x00af); // 90 Rotation
#endif
#ifdef ILI9341
			RA_TFT::SendComData(0x0036,0x0028); // 90 Rotation
#endif
			w=319;
			h=239;
			break;
		case 3:
#ifdef HX8347G
			RA_TFT::SendComData(0x0016,0x0088); // 180 Rotation
#endif
#ifdef HX8347D	
			RA_TFT::SendComData(0x0016,0x00cf); // 180 Rotation
#endif
#ifdef ILI9341
			RA_TFT::SendComData(0x0036,0x0088); // 180 Rotation
#endif
			w=239;
			h=319;
			break;
		case 4:
#ifdef HX8347G
			RA_TFT::SendComData(0x0016,0x0028); // 270 Rotation
#endif
#ifdef HX8347D	
			RA_TFT::SendComData(0x0016,0x006f); // 270 Rotation
#endif
#ifdef ILI9341
			RA_TFT::SendComData(0x0036,0x00e8); // 270 Rotation
#endif
			w=319;
			h=239;
			break;
		}
		FullClear(BKCOLOR);
	}
}


byte RA_TouchLCD::GetOrientation()
{
	return orientation;
}

int RA_TouchLCD::GetWidth()
{
	return w;
}

int RA_TouchLCD::GetHeight()
{
	return h;
}

void RA_TouchLCD::Scroll(int offset)
{
	RA_TFT::Scroll(offset);
}

void RA_TouchLCD::FullClear(int color)
{
	RA_TFT::Clear(color,0,0,GetWidth(),GetHeight());
}
void RA_TouchLCD::Clear(int color, int x1, int y1, int x2, int y2)
{
	RA_TFT::Clear(color,x1,y1,x2,y2);
}

void RA_TouchLCD::DrawPixel(int color, int x, int y)
{
	RA_TFT::DrawPixel(color,x,y);
}

void RA_TouchLCD::DrawLine(int color, int x1, int y1, int x2, int y2)
{
	int dx, dy, sx, sy, f;
	
	if(x1==x2)  // vertical line
	{
		Clear(color, x1,y1,x2, y2);
	}
	else if(y1==y2)  // horizontal line  
	{
		Clear(color, x1,y1,x2, y2);
	}
	else // Bresenham line
	{
		dy = y2 - y1; 
		dx = x2 - x1; 
		if (dy < 0)
		{
			dy = -dy;
			sy = -1;
		}
		else
		{
			sy = 1;
		} 
		if (dx < 0)
		{
			dx = -dx;
			sx = -1;
		}
		else
		{
			sx = 1;
		} 
 
		dy <<= 1;         
		dx <<= 1;         
 
		DrawPixel(color, x1, y1); 
		
 
		if (dx > dy)
		{ 					
			f = dy - (dx >> 1);  
			while (x1!= x2)
			{ 
				if (f >= 0)
				{ 
					y1 += sy; 
					f -= dx;   
				} 
				x1 += sx; 
				f += dy;    
				DrawPixel(color, x1, y1); 
			} 
		} 
		else 
		{ 							
			f = dx - (dy >> 1); 
			while (y1 != y2)
			{ 
				if (f >= 0) 
				{ 
					x1 += sx; 
					f -= dy; 
				} 
				y1 += sy; 
				f += dx; 
				DrawPixel(color, x1, y1); 
			}
		} 
 	} 

 }

void RA_TouchLCD::DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill)
{
	if (fill)
	{
		Clear(color,x1,y1,x2,y2);
	}
	else
	{
		DrawLine(color,x1,y1,x2,y1);
		DrawLine(color,x2,y1,x2,y2);
		DrawLine(color,x1,y2,x2,y2);
		DrawLine(color,x1,y1,x1,y2);
	}

}

void RA_TouchLCD::DrawCircle(int color, int x, int y, int radius, boolean fill)
{
	int xo, yo, r;
	
	xo = 0;
	yo = radius;
	r = -radius;
	
	while (xo <= yo) {
		if(fill)
		{ 
			DrawLine(color, x-xo, y+yo, x+xo, y+yo);
			DrawLine(color, x-xo, y-yo, x+xo, y-yo);
			DrawLine(color, x-yo, y+xo, x+yo, y+xo);
			DrawLine(color, x-yo, y-xo, x+yo, y-xo);
		}
		else
		{
			DrawPixel(color, x+xo, y+yo);
			DrawPixel(color, x-xo, y+yo);
			DrawPixel(color, x-xo, y-yo);
			DrawPixel(color, x+xo, y-yo);
			DrawPixel(color, x+yo, y+xo);
			DrawPixel(color, x-yo, y+xo);
			DrawPixel(color, x-yo, y-xo);
			DrawPixel(color, x+yo, y-xo);
		}
			
		if ((r += xo++ + xo)>= 0)
		{
			r-= 2*(yo-1);
			yo--;
		}
	}
}

void RA_TouchLCD::DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill)
{

	
	if(radius) 
	{
		int xo, yo, r;
		int x1r, y1r, x2r, y2r;
		xo = 0;
		yo = radius;
		r = -radius;
		
		x1r = x1+radius;
		y1r = y1+radius;
		x2r = x2-radius;
		y2r = y2-radius;
	
		if(fill)
		{
			Clear(color, x1, y1r, x2, y2r);
		}
		else
		{
			DrawLine(color, x1r, y1, x2r, y1);
			DrawLine(color, x2, y1r, x2, y2r);
			DrawLine(color, x1r, y2, x2r, y2);
			DrawLine(color, x1, y1r, x1, y2r);
		}
		
		while (xo <= yo)
		{
			if(fill)
			{
				DrawLine(color, x1r-xo, y2r+yo, x2r+xo, y2r+yo);
				DrawLine(color, x1r-xo, y1r-yo, x2r+xo, y1r-yo);
				DrawLine(color, x1r-yo, y2r+xo, x2r+yo, y2r+xo);
				DrawLine(color, x1r-yo, y1r-xo, x2r+yo, y1r-xo);
			}
			else
			{
				DrawPixel(color, x2r+xo, y2r+yo);
				DrawPixel(color, x1r-xo, y2r+yo);
				DrawPixel(color, x1r-xo, y1r-yo);
				DrawPixel(color, x2r+xo, y1r-yo);
				DrawPixel(color, x2r+yo, y2r+xo);
				DrawPixel(color, x1r-yo, y2r+xo);
				DrawPixel(color, x1r-yo, y1r-xo);
				DrawPixel(color, x2r+yo, y1r-xo);
			}
			
			if ((r += xo++ + xo)>= 0) {
				r-= 2* (yo-1);
				yo--;
			}
		}		
	}
	else
	{
		if(fill)
		{
			Clear(color,x1,y1,x2,y2);
		}
		else
		{
			DrawRectangle(color,x1,y1,x2,y2,false);
		}
	}

}

void RA_TouchLCD::DrawBMP(int ix, int iy, const prog_uchar *iPtr)
{
	byte i,j;
	int w = pgm_read_byte_near(iPtr++)+1;
	int h  =pgm_read_byte_near(iPtr++)+1;
	iPtr+=3;
	RA_TFT::SetBox(ix, iy, w+ix-1, h+iy-1);
#ifdef ILI9341
		RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif		
	for (int a=0; a<w*h; a++)
	{
//		if ((i=pgm_read_byte_near(iPtr++))==0) i=255;
//		if ((j=pgm_read_byte_near(iPtr++))==0) j=255;
//		RA_TFT::SendData(i,j);
		RA_TFT::SendData(pgm_read_byte_near(iPtr++),pgm_read_byte_near(iPtr++));
	}
}

void RA_TouchLCD::DrawBMP(int ix, int iy, const prog_uchar *iPtr, byte overridecolor_msb, byte overridecolor_lsb)
{
	// This is to workaround avrdude bug that doesn't allow us to write chunk blocks of 0xff into memory.
	// So, we change override color to white.
	byte i,j;
	int w = pgm_read_byte_near(iPtr++)+1;
	int h  =pgm_read_byte_near(iPtr++)+1;
	iPtr+=3;
	RA_TFT::SetBox(ix, iy, w+ix-1, h+iy-1);
#ifdef ILI9341
		RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif
	for (int a=0; a<w*h; a++)
	{
		i=pgm_read_byte_near(iPtr++);
		j=pgm_read_byte_near(iPtr++);
		if (i==overridecolor_msb && j==overridecolor_lsb)
		{
			i=255;
			j=255;
		}
		RA_TFT::SendData(i,j);
//		RA_TFT::SendData(pgm_read_byte_near(iPtr++),pgm_read_byte_near(iPtr++));
	}
}
void RA_TouchLCD::DrawSDImage(char *bmp, int x, int y)
{
	int bmpWidth, bmpHeight;
	uint8_t bmpImageoffset;
	
	boolean bmpGood=true;
	File dataFile = SD.open(bmp);
	if (dataFile) {
		if (read16(dataFile) == 0x4D42) {
			  // read and ignore file size & creator bytes
			  read32(dataFile);  
			  read32(dataFile);
			  //read data offset
			  bmpImageoffset=read32(dataFile);
			  // read and ignore DIB header
			  read32(dataFile);
			  // read width & height
			  bmpWidth = read32(dataFile);
			  bmpHeight = read32(dataFile);
			  bmpHeight = abs(bmpHeight);
			  
			  if (read16(dataFile) != 1)
				  bmpGood=false;
			  // read bmp depth. Needs to be 24bit
			  if (read16(dataFile) != 24)
				  bmpGood=false;
			  // read compression. We can't read compressed bmp
			  if (read32(dataFile) != 0) {
				  bmpGood=false;
			  }
			  if (bmpGood)
			  {
				  uint16_t p;
				  uint8_t g, b;
				  int i, j;

				  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
				  uint8_t buffidx = 3*BUFFPIXEL;

				  dataFile.seek(bmpImageoffset);

				  RA_TFT::SetBox(x, y,x+bmpWidth-1, y+bmpHeight-1);
#ifdef ILI9341
				  RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif	
				  for (i=0; i< bmpHeight; i++) 
				  {

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
					  wdt_reset();
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					  for (j=0; j<bmpWidth; j++) 
					  {
						  // read more pixels
						  if (buffidx >= 3*BUFFPIXEL) 
						  {
							  dataFile.read(sdbuffer, 3*BUFFPIXEL);
							  buffidx = 0;
						  }

						  // convert pixel from 888 to 565
						  b = sdbuffer[buffidx++];     // blue
						  g = sdbuffer[buffidx++];     // green
						  p = sdbuffer[buffidx++];     // red

						  p >>= 3;
						  p <<= 6;

						  g >>= 2;
						  p |= g;
						  p <<= 5;

						  b >>= 3;
						  p |= b;

						  // write out the 16 bits of color
						  RA_TFT::SendData(p>>8,p);
					  }
				  }				  
			  }
		}
		dataFile.close();
	}     
}

void RA_TouchLCD::DrawSDRawImage(char *bmp, int x, int y, int w, int h)
{
	File dataFile = SD.open(bmp);
	
	if (dataFile) {
		uint16_t p;
		uint8_t g, b;
		unsigned long i, j;
		uint8_t sdbuffer[2 * BUFFPIXEL];  // 2 * pixels to buffer
		uint8_t buffidx = 2*BUFFPIXEL;

		RA_TFT::SetBox(x, y,x+w-1, y+h-1);
#ifdef ILI9341
		RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif	
		RS1;
		CS0;
		j=h;
		j*=w;
		for (i=0; i<j; i++) 
		{
			// read more pixels
			if (buffidx >= 2*BUFFPIXEL) 
			{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
					  wdt_reset();
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
				dataFile.read(sdbuffer, 2*BUFFPIXEL);
				buffidx = 0;
			}
			// write out the 16 bits of color
			// RA_TFT::SendData(sdbuffer[buffidx++],sdbuffer[buffidx++]);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			PORTA=sdbuffer[buffidx++];
			PORTC=sdbuffer[buffidx++];
#elif defined(__SAM3X8E__)
			PIOC->PIO_ODSR = sdbuffer[buffidx++]<<9 | sdbuffer[buffidx++]<<1;
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
			WR0;
			WR1;
		}
		CS1;
		dataFile.close();
	}	
}


void RA_TouchLCD::DrawDateTime(unsigned long t, int x, int y, boolean militarytime, FontClass Font)
{
	char text[15];
	
	//Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
	sprintf(text,"%02d/%02d/%02d ",month(t),day(t),year(t)-2000);
	Font.DrawText(x,y,text);
	if (militarytime)
	{
		sprintf(text,"%02d:%02d:%02d ",hour(t),minute(t),second(t));
	}
	else
	{
		if(hour()>=12)
			sprintf(text,"%02d:%02d:%02d PM",hour(t)-12,minute(t),second(t));
		else
			sprintf(text,"%02d:%02d:%02d AM",hour(t),minute(t),second(t));
	}
	Font.DrawText(text);
}

void RA_TouchLCD::DrawSetupDateTime(int x, int y, char *value, FontClass Font)
{
	DrawBMP(x-12,y-20,ARROWUP);
	Font.DrawCenterText(x,y,value);
	DrawBMP(x-12,y+34,ARROWDOWN);
}

void RA_TouchLCD::DrawSetupDateTime(int x, int y, byte value, FontClass Font)
{
	char sdate[5];
	sprintf(sdate," %02d ",value);
	DrawSetupDateTime(x,y,sdate,Font);
}

void RA_TouchLCD::DrawRelayStatus(int x, int y, boolean status)
{
	int color;
	if (status) color=RELAYGREEN; else color=COLOR_RED; 
	for (int c=0;c<=6;c++) DrawLine(alphaBlend(color,(10-c)*10),x+c+4,y,x+c+4,y+17);
	for (int c=4;c<=10;c++) DrawLine(alphaBlend(color,c*10),x+c+14,y,x+c+14,y+17);
	Clear(alphaBlend(color,40),x+11,y,x+17,y+17);
	DrawLine(COLOR_SILVER,x+1,y-1,x+27,y-1);
	DrawLine(COLOR_SILVER,x+1,y+18,x+27,y+18);
//	DrawLine(COLOR_SILVER,x+34,y,x+34,y+17);
	for (int c=0;c<=3;c++) DrawLine(alphaBlend(COLOR_SILVER,80-(c*10)),x+c,y,x+c,y+17);
	for (int c=0;c<=3;c++) DrawLine(alphaBlend(COLOR_SILVER,(c*10)+50),x+25+c,y,x+25+c,y+17);
//	Clear(color,x+10,y,x+15,y+17);
	
}

char* RA_TouchLCD::ConvertDigitsNumber(int number)
{
	char text[9];
	char temp[3];
	
	strcpy(text,"");
	if (number<10) strcat(text,"0");
	itoa(number,temp,10);
	strcat(text,temp);
	return text;
}
