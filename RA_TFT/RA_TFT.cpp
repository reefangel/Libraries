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

#include "RA_TFT.h"

void RA_TFT::Init()
{
#if defined(__SAM3X8E__)
	PIOC->PIO_OER |= 0x001FFFFE;
	PIOC->PIO_OWDR = 0xFFFE0001;
#endif
	CS0;
	RST1;
	delay(5); 
	RST0;
	delay(15);
	RST1;
	delay(15);

#if defined HX8347D || defined HX8347G
	//CMO2.8+HX8347D
	//Driving ability Setting
	SendComData(0x00EA,0x0000); //PTBA[15:8]
	SendComData(0x00EB,0x0020); //PTBA[7:0]
	SendComData(0x00EC,0x000C); //STBA[15:8]
	SendComData(0x00ED,0x00C4); //STBA[7:0]
	SendComData(0x00E8,0x0040); //OPON[7:0]
	SendComData(0x00E9,0x0038); //OPON1[7:0]
	SendComData(0x00F1,0x0001); //OTPS1B
	SendComData(0x00F2,0x0010); //GEN
	SendComData(0x0027,0x00A3);
	//Gamma 2.2 Setting
	SendComData(0x0040,0x0000); //
	SendComData(0x0041,0x0000); //
	SendComData(0x0042,0x0001); //
	SendComData(0x0043,0x0013); //
	SendComData(0x0044,0x0010); //
	SendComData(0x0045,0x0026); //
	SendComData(0x0046,0x0008); //
	SendComData(0x0047,0x0051); //
	SendComData(0x0048,0x0002); //
	SendComData(0x0049,0x0012); //
	SendComData(0x004A,0x0018); //
	SendComData(0x004B,0x0019); //
	SendComData(0x004C,0x0014); //
	SendComData(0x0050,0x0019); //
	SendComData(0x0051,0x002F); //
	SendComData(0x0052,0x002C); //
	SendComData(0x0053,0x003E); //
	SendComData(0x0054,0x003F); //
	SendComData(0x0055,0x003F); //
	SendComData(0x0056,0x002E); //
	SendComData(0x0057,0x0077); //
	SendComData(0x0058,0x000B); //
	SendComData(0x0059,0x0006); //
	SendComData(0x005A,0x0007); //
	SendComData(0x005B,0x000D); //
	SendComData(0x005C,0x001D); //
	SendComData(0x005D,0x00CC); //
	//Power Voltage Setting
	SendComData(0x001B,0x001B); //VRH=4.65V
	SendComData(0x001A,0x0001); //BT (VGH~15V,VGL~-10V,DDVDH~5V)
	SendComData(0x0024,0x002F); //VMH(VCOM High voltage ~3.2V)
	SendComData(0x0025,0x0057); //VML(VCOM Low voltage -1.2V)
	//****VCOM offset**///
	SendComData(0x0023,0x008d); //for Flicker adjust //can reload from OTP
	//Power on Setting
	SendComData(0x0018,0x0036); //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
	SendComData(0x0019,0x0001); //OSC_EN='1', start Osc
	SendComData(0x0001,0x0008); //DP_STB='0', out deep sleep
	SendComData(0x001F,0x0088);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	delay(5);
	SendComData(0x001F,0x0080);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	delay(5);
	SendComData(0x001F,0x0090);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	delay(5);
	SendComData(0x001F,0x00D0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	delay(5);
	//262k/65k color selection
	SendComData(0x0017,0x0005); //default 0x0006 262k color // 0x0005 65k color
	//SET PANEL
	SendComData(0x0036,0x0000); //SS_P, GS_P,REV_P,BGR_P
	//Display ON Setting
	SendComData(0x0028,0x0038); //GON=1, DTE=1, D=1000
	delay(40);
	SendComData(0x0028,0x003C); //GON=1, DTE=1, D=1100
	//Set GRAM Area
	//SendComData(0x0016,0x000f); // Normal Rotation
	//SendComData(0x0016,0x00af); // 90 Rotation
	//SendComData(0x0016,0x00cf); // 180 Rotation
	//SendComData(0x0016,0x006f); // 270 Rotation
	//Scroll settings
	SendComData(0x000e,0x0); // Top=0
	SendComData(0x000f,0x0);
	SendComData(0x0010,0x1); // Height=320
	SendComData(0x0011,0x40);
	SendComData(0x0012,0x0);  // Bottom=0
	SendComData(0x0013,0x0);
	
	
	
	SendComData(0x0002,0x0000);
	SendComData(0x0003,0x0000); //Column Start
	SendComData(0x0004,0x0000);
	SendComData(0x0005,0x00EF); //Column End
	SendComData(0x0006,0x0000);
	SendComData(0x0007,0x0000); //Row Start
	SendComData(0x0008,0x0001);
	SendComData(0x0009,0x003F); //Row End

#endif
	
#ifdef ILI9341
	  SendCom(0x00,0x01);
	  delay(10);
	  SendCom(0x00,0x28);

	  SendCom(0x00,0xcf);
	  SendData(0x00,0x00);
	  SendData(0x00,0x83);
	  SendData(0x00,0x30);

	  SendCom(0x00,0xed);
	  SendData(0x00,0x64);
	  SendData(0x00,0x03);
	  SendData(0x00,0x12);
	  SendData(0x00,0x81);

	  SendCom(0x00,0xe8);
	  SendData(0x00,0x85);
	  SendData(0x00,0x01);
	  SendData(0x00,0x79);

	  SendCom(0x00,0xcb);
	  SendData(0x00,0x39);
	  SendData(0x00,0x2c);
	  SendData(0x00,0x00);
	  SendData(0x00,0x34);
	  SendData(0x00,0x02);

	  SendCom(0x00,0xf7);
	  SendData(0x00,0x20);

	  SendCom(0x00,0xea);
	  SendData(0x00,0x00);
	  SendData(0x00,0x00);

	  SendCom(0x00,0xc0);
	  SendData(0x00,0x26);

	  SendCom(0x00,0xc1);
	  SendData(0x00,0x11);

	  SendCom(0x00,0xc5);
	  SendData(0x00,0x35);
	  SendData(0x00,0x3e);

	  SendCom(0x00,0xc7);
	  SendData(0x00,0xbe);

	  SendCom(0x00,0x36);
	  SendData(0x00,0x48);

	  SendCom(0x00,0x3a);
	  SendData(0x00,0x55);

	  SendCom(0x00,0xb1);
	  SendData(0x00,0x00);
	  SendData(0x00,0x1B);

	  SendCom(0x00,0xf2);
	  SendData(0x00,0x08);

	  SendCom(0x00,0x26);
	  SendData(0x00,0x01);

	  SendCom(0x00,0xe0);
	  SendData(0x00,0x1f);
	  SendData(0x00,0x1a);
	  SendData(0x00,0x18);
	  SendData(0x00,0x0a);
	  SendData(0x00,0x0f);
	  SendData(0x00,0x06);
	  SendData(0x00,0x45);
	  SendData(0x00,0x87);
	  SendData(0x00,0x32);
	  SendData(0x00,0x0a);
	  SendData(0x00,0x07);
	  SendData(0x00,0x02);
	  SendData(0x00,0x07);
	  SendData(0x00,0x05);
	  SendData(0x00,0x00);

	  SendCom(0x00,0xe1);
	  SendData(0x00,0x00);
	  SendData(0x00,0x25);
	  SendData(0x00,0x27);
	  SendData(0x00,0x05);
	  SendData(0x00,0x10);
	  SendData(0x00,0x09);
	  SendData(0x00,0x3a);
	  SendData(0x00,0x78);
	  SendData(0x00,0x4d);
	  SendData(0x00,0x05);
	  SendData(0x00,0x18);
	  SendData(0x00,0x0d);
	  SendData(0x00,0x38);
	  SendData(0x00,0x3a);
	  SendData(0x00,0x1f);

	  SendCom(0x00,0x2a);
	  SendData(0x00,0x00);
	  SendData(0x00,0x00);
	  SendData(0x00,0x00);
	  SendData(0x00,0xEF);

	  SendCom(0x00,0x2b);
	  SendData(0x00,0x00);
	  SendData(0x00,0x00);
	  SendData(0x00,0x01);
	  SendData(0x00,0x3F);

	  SendCom(0x00,0xb7);
	  SendData(0x00,0x07);

	  SendCom(0x00,0xb6);
	  SendData(0x00,0x0a);
	  SendData(0x00,0x82);
	  SendData(0x00,0x27);
	  SendData(0x00,0x00);

	  SendCom(0x00,0x11);
	  delay(100);
	  SendCom(0x00,0x29);
	  delay(100);

#endif
	
}

void RA_TFT::SendComData(int Com,int Data)
{
    SendCom(Com>>8,Com);
    SendData(Data>>8,Data);
}

void RA_TFT::SendCom(byte H,byte L)
{
	RS0;
	WriteBus(H,L);
}

void RA_TFT::SendData(byte H,byte L)
{
	RS1;
	WriteBus(H,L);
}

void RA_TFT::WriteBus(byte H,byte L)
{
	CS0;
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
	PORTA=H;
	PORTC=L;
#elif defined(__SAM3X8E__)
	PIOC->PIO_ODSR = H<<9 | L<<1;
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
	WR0;
	WR1;
	CS1;
}

void RA_TFT::Clear(int color, int x1, int y1, int x2, int y2)
{
	SetBox(x1,y1,x2,y2);
	SendData(color>>8,color&0x00ff);
	CS0;
	for(int i=0;i<(y2-y1+1);i++)
	{
		for (int j=0;j<(x2-x1+1);j++)
		{  
			WR0;
			WR1;
		}
	}
	CS1;
}


void RA_TFT::SetBox(int x1, int y1, int x2, int y2)
{
#if defined HX8347D || defined HX8347G
	SendComData(0x0002,x1>>8);     // Column address start2
	SendComData(0x0003,x1);    // Column address start1
	SendComData(0x0004,x2>>8);     // Column address end2
	SendComData(0x0005,x2);    // Column address end1
	SendComData(0x0006,y1>>8);     // Row address start2
	SendComData(0x0007,y1);    // Row address start1
	SendComData(0x0008,y2>>8);     // Row address end2
	SendComData(0x0009,y2);    // Row address end1
	SendCom(0x00,0x22);
#endif
#ifdef ILI9341
	SendCom(0x00,0x2A);                                                      /* Column Command address       */
	SendData(0x00,x1>>8);
	SendData(0x00,x1);
	SendData(0x00,x2>>8);	
	SendData(0x00,x2);	
	SendCom(0x00,0x2B);                                                      /* Column Command address       */
	SendData(0x00,y1>>8);
	SendData(0x00,y1);
	SendData(0x00,y2>>8);	
	SendData(0x00,y2);	
	SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif
	
}

void RA_TFT::DrawPixel(int color, int x, int y)
{
	SetBox(x, y, x, y);
	SendData(color>>8,color&0x00ff);
//	CS0;
//	WR0;
//	WR1;
//	CS1;	
}

void RA_TFT::Pixels(int count, const byte* d)
{
    byte slow = count & 0x03;
    if (slow)
    {
        do {
        	WriteBus(d[0],d[1]);
            d+=2;
        } while (--slow);
    }
    
    // x4 unrolled
    count >>= 2;
    if (count)
    {
        do {
        	SendData(d[0],d[1]);
        	SendData(d[2],d[3]);
        	SendData(d[4],d[5]);
        	SendData(d[6],d[7]);
            d += 8;
        } while (--count);
    }
}
void RA_TFT::Scroll(int offset)
{
	SendComData(0x0014,offset>>8);
	SendComData(0x0015,offset);
}

