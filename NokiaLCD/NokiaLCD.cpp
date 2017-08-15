#include "WProgram.h"
#include "Time.h"
#include "NokiaLCD.h"
#include <Wire.h>
#include <EEPROM.h>


NokiaLCD::NokiaLCD()
{
#if defined(__AVR_ATmega2560__)
	pinMode(BL,OUTPUT);
	pinMode(CS,OUTPUT);
	pinMode(CLK,OUTPUT);
	pinMode(SDA,OUTPUT);
	pinMode(RESET,OUTPUT);
	digitalWrite(BL,HIGH);
	digitalWrite(CS,HIGH);
	digitalWrite(CLK,HIGH);
	digitalWrite(SDA,HIGH);
	digitalWrite(RESET,HIGH);
#else
  DDRD |= B01111100;   // Set SPI pins as output
  PORTD |= B01111000;  // Set SPI pins HIGH
#endif
}



void NokiaLCD::ShiftBits(byte b)
{
  byte Bit;

  for (Bit = 0; Bit < 8; Bit++)     // 8 Bit Write
  {
    CLK0          // Standby SCLK
    if((b&0x80)>>7)
    {
      SDA1
    }
    else
    {
      SDA0
    }
    CLK1          // Strobe signal bit
    b <<= 1;   // Next bit data
  }
}

void NokiaLCD::SendData(byte data) {
  CLK0
  SDA1
  CLK1
  ShiftBits(data);
}

void NokiaLCD::SendCMD(byte data) {
  CLK0
  SDA0
  CLK1
  ShiftBits(data);

}

void NokiaLCD::SetBox(byte x1, byte y1, byte x2, byte y2)
{
  SendCMD(CASET);   // page start/end ram
  SendData(x1);     // for some reason starts at 2
  SendData(x2);

  SendCMD(PASET);   // column start/end ram
  SendData(y1);
  SendData(y2);
}

void NokiaLCD::Clear(byte color, byte x1, byte y1, byte x2, byte y2)
{
  uint16_t xmin, xmax, ymin, ymax;
  uint16_t i;
  unsigned int icolor;
  icolor = ~color;

  // best way to create a filled rectangle is to define a drawing box
  // and loop two pixels at a time
  // calculate the min and max for x and y directions
  xmin = (x1 <= x2) ? x1 : x2;
  xmax = (x1 > x2) ? x1 : x2;
  ymin = (y1 <= y2) ? y1 : y2;
  ymax = (y1 > y2) ? y1 : y2;

  // specify the controller drawing box according to those limits
  // Row address set (command 0x2B)
  SendCMD(CASET);
  SendData(xmin);
  SendData(xmax);

  // Column address set (command 0x2A)
  SendCMD(PASET);
  SendData(ymin);
  SendData(ymax);

  // WRITE MEMORY
  SendCMD(RAMWR);

  // loop on total number of pixels / 2
  for (i = 0; i < ((xmax - xmin + 1) * (ymax - ymin + 1)) ; i++)
  {
    // use the color value to output three data bytes covering two pixels
    // For some reason, it has to send blue first then green and red
    //SendData((color << 4) | ((color & 0xF0) >> 4));
    //SendData(((color >> 4) & 0xF0) | (color & 0x0F));
    //SendData((color & 0xF0) | (color >> 8));
    SendData(icolor);
  }
}

void NokiaLCD::Init()
{
  // Initial state
  CS1
  CS0

  // Hardware Reset LCD
  RESET0
  delay(100);
  RESET1
  delay(100);

  //Software Reset
  SendCMD(SWRESET);

  //Sleep Out
  SendCMD(SLEEPOUT);

  //Booster ON
  SendCMD(BSTRON);


  //Display On
  SendCMD(DISPON);

  //Normal display mode
  SendCMD(NORON);

  //Display inversion on
  SendCMD(INVON);

  //Data order
  //SendCMD(0xBA);

  SendCMD(SETCON);
  SendData(0x38);

  //Memory data access control
  SendCMD(MADCTL);

 //SendData(8|64);   //rgb + MirrorX
 //SendData(8|128);   //rgb + MirrorY
  SendData(0xc0);

  SendCMD(COLMOD);
  SendData(2);   //16-Bit per Pixel

  Clear(0xff,0,0,131,131);

}

void NokiaLCD::Sleep()
{
  SendCMD(DISPOFF);
  SendCMD(SLEEPIN);
  BacklightOff();
}

void NokiaLCD::Wake()
{
  BacklightOn();
  SendCMD(SLEEPOUT);
  SendCMD(DISPON);
}

void NokiaLCD::BacklightOn()
{
  BL1
}

void NokiaLCD::BacklightOff()
{
  BL0
}


void NokiaLCD::DrawTextLine(byte fcolor, byte bcolor,byte x, byte y,char c)
{
  int i;
  SetBox(x,y,x,y+7);
  SendCMD(RAMWR);
  for(i=0;i<8;i++)
  {
    if (1<<i & c)
      SendData(~fcolor);
    else
      SendData(~bcolor);
  }
}
#ifdef bigfonts
void NokiaLCD::DrawLargeTextLine(byte fcolor, byte bcolor,byte x, byte y,char c)
{
  int i;
  SetBox(x,y,x+10,y);
  SendCMD(RAMWR);
  for(i=7;i>=0;i--)
  {
    if (1<<i & c)
      SendData(~fcolor);
    else
      SendData(~bcolor);
  }
}
#endif

#ifdef hugefonts
void NokiaLCD::DrawHugeTextLine(byte fcolor, byte bcolor, byte x, byte y, uint16_t c)
{
	int i;
	SetBox(x,y,x,y+15);  // 15 works
	SendCMD(RAMWR);
	for(i=0;i<16;i++)
	{
		if (1<<i & c)
			SendData(~fcolor);
		else
			SendData(~bcolor);
	}
}
#endif

void NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,char *text)
{
	char c;
	int t;
	while(*text != 0)
	{
		t=*text;
		t-=32;
		t*=5;
		for(int j = t; j < t+5; j++)
		{
			c = pgm_read_byte_near(font + j);
			DrawTextLine(fcolor, bcolor, x++, y, c);
		}
		DrawTextLine(fcolor, bcolor, x++, y, 0);
		text++;
	}

}

#ifdef bigfonts

void NokiaLCD::DrawLargeText(byte fcolor, byte bcolor, byte x, byte y,char *text)
{
	char c;
	int t;
	while(*text != 0)
	{
		t=*text;
		t-=32;
		t*=8;
		for(int j = t; j < t+8; j++)
		{
			c = pgm_read_byte_near(big_font + j);
			DrawLargeTextLine(fcolor, bcolor, x-1, y++, c);
		}
		text++;
		y-=8;
		x+=8;
	}

}
#endif

#ifdef hugefonts
void NokiaLCD::DrawHugeText(byte fcolor, byte bcolor, byte x, byte y, char* text)
{
	uint16_t c;
	int t;
	byte w = 16;
	byte x_w = 16;  // 16 works
	byte y_w = 16;
	const uint16_t *f = num_16x16;
	while(*text != 0)
	{
		t=*text;
		t-=46;
		t*=w;
		for(int j = t; j < t+w; j++)
		{
			c = pgm_read_word_near(f + j);
			DrawHugeTextLine(fcolor, bcolor, x++, y, c);
		}
		text++;
		//y-=y_w;
		//x+=x_w;
	}
}

#endif
void NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,int text)
{
	char temp[6];
	itoa(text,temp,10);
	if (text<10) strcat(text," ");
	if (text<100) strcat(text," ");
	DrawText(fcolor, bcolor, x, y,temp);
}

void NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,byte text)
{
	char temp[6];
	itoa(text,temp,10);
	if (text<10) strcat(text," ");
	if (text<100) strcat(text," ");
	DrawText(fcolor, bcolor, x, y,temp);
}

void NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,long text)
{
	char temp[20];
	ltoa(text,temp,10);
	DrawText(fcolor, bcolor, x, y,temp);
}

void NokiaLCD::PutPixel(byte color, byte x, byte y)
{
  unsigned int icolor;
  icolor = ~color;

  SendCMD(CASET);   // page start/end ram
  SendData(x);      // for some reason starts at 2
  SendData(x+1);

  SendCMD(PASET);   // column start/end ram
  SendData(y);
  SendData(y+1);
  SendCMD(RAMWR);
  SendData(icolor);
}

void NokiaLCD::SetContrast(byte Contrast)
{
  SendCMD(SETCON);
  SendData(Contrast);
}


void NokiaLCD::DrawDate(byte x, byte y)
{
  //byte iTimeHourOffset=0;
  char text[21];
  char temp[]="  ";
  strcpy(text,"");
  itoa(month(),temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  strcat(text,"/");
  itoa(day(),temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  strcat(text,"/");
  itoa(year()-2000,temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  strcat(text," ");
  //if (iTimeHour>12) iTimeHourOffset=12;
  itoa(hourFormat12(),temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  strcat(text,":");
  itoa(minute(),temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  strcat(text,":");
  itoa(second(),temp,10);
  if (temp[1]==0) strcat(text,"0");
  strcat(text,temp);
  if (isAM())
  {
  strcat(text," AM");
  }
  else
  {
  strcat(text," PM");
  }
  DrawText(RED,WHITE,x,y,text);
}

void NokiaLCD::DrawOutletBox(byte x, byte y,byte RelayData)
{
	Clear(94,x,y,x+104,y);
	Clear(94,x,y+12,x+104,y+12);
	for (int a=0;a<8;a++)
	{
	  byte bcolor=WHITE;
	  byte fcolor=BLACK;
	  char temp[]="  ";
	  if ((RelayData&(1<<a))==1<<a)
	  {
		bcolor=13;
		fcolor=WHITE;
	  }
	  Clear(bcolor,x+1+(a*13),y+1,x+14+(a*13),y+11);
	  itoa(a+1,temp,10);
	  DrawText(fcolor,bcolor,x+5+(a*13),y+3,temp);
	}
}

void NokiaLCD::DrawSingleMonitor(int Temp, byte fcolor, byte x, byte y, byte decimal)
{
  char text[7];
  char temptxt[3];
  if (Temp==0xFFFF) Temp=0;
  itoa(Temp/decimal,text,10);
  if (decimal>1)
  {
	  itoa(Temp%decimal,temptxt,10);
	  strcat(text , ".");
	  if (Temp%decimal<10 && decimal==100) strcat(text , "0");
	  strcat(text , temptxt);
  }
  Clear(WHITE,x,y,x+30,y+8);
  DrawText(fcolor,WHITE,x,y,text);
}

void NokiaLCD::DrawMonitor(byte x, byte y, ParamsStruct Params, byte DaylightPWMValue, byte ActnicPWMValue)
{
  DrawText(WaterTempColor,WHITE,x,y,"T1:");
  DrawSingleMonitor(Params.Temp1, WaterTempColor, x+18, y,10);
  DrawText(LightsTempColor,WHITE,x,y+10,"T2:");
  DrawSingleMonitor(Params.Temp2, LightsTempColor, x+18, y+10,10);
  DrawText(AmbientTempColor,WHITE,x,y+20,"T3:");
  DrawSingleMonitor(Params.Temp3, AmbientTempColor, x+18, y+20,10);
  DrawText(PHColor,WHITE,x+60,y,"PH:");
  DrawSingleMonitor(Params.PH, PHColor, x+78, y,100);
  DrawText(DPColor,WHITE,x+60,y+10,"DP:");
  DrawSingleMonitor(DaylightPWMValue, DPColor, x+78, y+10,1);
  DrawText(APColor,WHITE,x+60,y+20,"AP:");
  DrawSingleMonitor(ActnicPWMValue, APColor, x+78, y+20,1);
}

void NokiaLCD::DrawSingleGraph(byte color, byte x, byte y, int I2CAddr, int EEaddr)
{
	int start;
	for (int a=0;a<120;a++)
	{
		start=EEaddr+a;
		if (start > (int(EEaddr/120)+1)*120) start=start-120;
		Wire.beginTransmission(I2CAddr);
		Wire.send((int)(start >> 8));   // MSB
		Wire.send((int)(start & 0xFF)); // LSB
		Wire.endTransmission();
		Wire.requestFrom(I2CAddr,1);
		if (Wire.available()) PutPixel(color,x+a,y+50-Wire.receive());
	}

}

void NokiaLCD::DrawEEPromImage(int swidth, int sheight, byte x, byte y, int I2CAddr, int EEaddr)
{
  int count = 0;
  SetBox(x,y,swidth-1+x,sheight-1+y);
  SendCMD(0x2c);
  //for (int j = 0; j < sheight; j++)
  //{
  //  for (int i = 0; i < swidth; i++)
  //  {
      //e.lcd_put_pixel(readEEPROM(I2CEEPROM,count+start),i+x,j+y);
  //    count++;
  //  }
  //  WatchDogReset();
  //}
  do
  {
    Wire.beginTransmission(I2CAddr);
    Wire.send((int)(EEaddr+count >> 8));   // MSB
    Wire.send((int)(EEaddr+count & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(I2CAddr,30);
    for (int j = 0; j < 30; j++)
    {
      count+=1;
      if ((count<swidth*sheight) && Wire.available()) SendData(~Wire.receive());
    }
  }
  while (count < swidth*sheight);
}

void NokiaLCD::DrawGraph(byte x, byte y, int I2CAddr, int pointer)
{
  Clear(WHITE,0,y,131,y+50);
  Clear(BLACK,x,y,x,y+50);
  for (int i=6; i<=131; i+=3){
    PutPixel(0x49, i, y+25);
  }
  DrawSingleGraph(WaterTempColor,x,y,I2CAddr,EEPROM.read(pointer));
  DrawSingleGraph(LightsTempColor,x,y,I2CAddr,EEPROM.read(pointer)+120);
  DrawSingleGraph(AmbientTempColor,x,y,I2CAddr,EEPROM.read(pointer)+240);
  DrawSingleGraph(PHColor,x,y,I2CAddr,EEPROM.read(pointer)+360);
}
