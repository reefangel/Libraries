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
 

#include "ReefTouch.h"

ReefTouchClass::ReefTouchClass()
{
	DDRL=0xff; //PORTL (Control Pins Output)
	DDRA=0xff; //PORTA (Data Output - D8-15)
	DDRC=0xff; //PORTC (Data Output - D0-7)
	PORTA=0xff; //PORTA pull up
	PORTC=0xff; //PORTC pull up
	PORTL=0xdf; //PORTL pull up
	DDRL|=(1<<0); //PL0 (Output) - SD Chip Select
	PORTL|=(1<<0); //PL0 pull up
	DDRD&=(0<<5); //PD5 (Input) - TP Interrupt
	PORTD|=(1<<5); //PD5 pull up
	DDRD|=(1<<7); //PD7 (Output) - TP Chip Select
	PORTD|=(1<<7); //PD7 pull up
	DDRL|=(1<<7); //PL7 (Output) - ACC Chip Select
	PORTL|=(1<<7); //PL7 pull up
	DDRH|=(1<<4); //PH4 (Output) - Status LED
	//PORTH|=(1<<4); //PH4 pull up
	SPCR=0x50;
}

void ReefTouchClass::Init()
{
	Serial.begin(57600);
	SPI.begin();
	Wire.begin();
	LCD.Init();
	SmallFont.SetFont(f8x8);
	Font.SetFont(f12x12);
	LargeFont.SetFont(ArialBold20);
	OkButton.Create(RGB565(0xA0, 0xFF, 0xA0),RED,"Ok");
	TS.Init();
	MMC_Init();
	Tilt.Init();
//	FAT_Init(_buffer,MMC_ReadSector);
//	File.Open("splash.im2");
//	LCD.DrawImage(File,0,0);

//	LCD.FullClear(WHITE);
//	LCD.DrawLogo(57,35,logo1);
//	LCD.DrawLogo(23,200,logo);
	
	BL1;
	Splash=true;
	MilitaryTime=false;
	DisplayedScreen=MAIN_SCREEN;
	ExpansionFlag=B00000011;
	RelayExpansionFlag=B01000000;
	setTime(1308161400);  // Unix time
	now();
}

void ReefTouchClass::Refresh()
{
	if ((millis()>SplashDuration) && Splash)
	{
		Splash=false;
		if (TS.IsCalibrationNeeded()) CalibrateTouchScreen();
		LCD.FullClear(BKCOLOR);
	}
	if (!Splash)
	{
		Tilt.Refresh();
	}
}

void ReefTouchClass::ShowInterface()
{
	Refresh();
	if (!Splash)
	{
		if (!TS.IsTouched())
		{
			int i,j;
			char tempname[15];

			//Draw Top Bar
			if (LCD.GetOrientation()%2==0) i=0; else i=12;
			if (LastOrientation != LCD.GetOrientation())
			{
				LastOrientation = LCD.GetOrientation();
				NeedsRedraw=true;
				//Top Bar
				LCD.DrawRectangle(TOPBAR_BC,0,0,LCD.GetWidth(),27,true);
				for (int a=0;a<=5;a++)
				{
					LCD.DrawLine(alphaBlend(WHITE,(5-a)*10),0,28+a,LCD.GetWidth(),28+a);
				}
			}			
			LCD.DrawDateTime(5,10,MilitaryTime);
			
			if (DisplayedScreen==MAIN_SCREEN)
			{
				//Draw Top Bar
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					
					//Temperature
					j=68+i;
					Font.SetColor(GOLD,BKCOLOR,true);
					eeprom_read_block((void*)&tempname, (void*)Probe1Name, sizeof(tempname));
					Font.DrawCenterText((LCD.GetWidth()/4)-20,j,tempname);
					eeprom_read_block((void*)&tempname, (void*)Probe2Name, sizeof(tempname));
					Font.DrawCenterText(LCD.GetWidth()/2,j,tempname);
					eeprom_read_block((void*)&tempname, (void*)Probe3Name, sizeof(tempname));
					Font.DrawCenterText((LCD.GetWidth()*3/4)+20,j,tempname);
		
					//pH, Salinity and ORP
					j+=50+i;
					Font.SetColor(GOLD,BKCOLOR,true);
					Font.DrawCenterTextP((LCD.GetWidth()/4)-20,j,PH);
					Font.DrawCenterTextP(LCD.GetWidth()/2,j,SALINITY);
					Font.DrawCenterTextP((LCD.GetWidth()*3/4)+20,j,ORP);
					
					j+=18+i;
					//Yellow Bar
					for (int a=0;a<=5;a++)
					{
						LCD.DrawLine(alphaBlend(MAGENTA,(5-a)*10),a,33,a,j);
					}
		
					//Division
					LCD.DrawLine(DIVISION,0,j,LCD.GetWidth(),j);
		
					//Green Bar
					for (int a=0;a<=5;a++)
					{
						LCD.DrawLine(alphaBlend(GREEN,(5-a)*10),a,j,a,j+70+(i*5/2));
					}
					j+=10+i;
		
					//PWM Bars
					LCD.Clear(WHITE,10,j,130,j+20);
					for (int a=0;a<10;a++) LCD.DrawPixel(RED,78,j+(a*2));
					Font.DrawTextP(BLACK,WHITE,15,j+5,ACTINIC);
					j+=30+(i/2);
					LCD.Clear(WHITE,10,j,130,j+20);
					for (int a=0;a<10;a++) LCD.DrawPixel(RED,78,j+(a*2));
					Font.DrawTextP(BLACK,WHITE,15,j+5,DAYLIGHT);
		
					j+=30+i;
					//Division
					LCD.DrawLine(DIVISION,0,j,LCD.GetWidth(),j);
		
					//Red Bar
					for (int a=0;a<=5;a++)
					{
						LCD.DrawLine(alphaBlend(RED,(5-a)*10),a,j,a,LCD.GetHeight());
					}
					
					j+=13+(i/2);
					
					//ATO Ports
					Font.DrawTextP(WHITE,BLACK,(LCD.GetWidth()/10)+25,j,ATOHIGHPORT);
					Font.DrawTextP(WHITE,BLACK,(LCD.GetWidth()*6/10)+25,j,ATOLOWPORT);
				}
				
				//Temperature
				j=35+i;
				LargeFont.SetColor(WHITE,BKCOLOR,true);
				LargeFont.DrawCenterNumber((LCD.GetWidth()/4)-20,j,Params.Temp1,10);
				LargeFont.DrawCenterNumber(LCD.GetWidth()/2,j,Params.Temp2,10);
				LargeFont.DrawCenterNumber((LCD.GetWidth()*3/4)+20,j,Params.Temp3,10);
		
				//pH, Salinity and ORP
				j+=50+i;
				LargeFont.SetColor(WHITE,BKCOLOR,true);
				LargeFont.DrawCenterNumber((LCD.GetWidth()/4)-20,j,Params.PH,100);
				LargeFont.DrawCenterNumber(LCD.GetWidth()/2,j,Params.Sal,10);
				LargeFont.DrawCenterNumber((LCD.GetWidth()*3/4)+20,j,Params.ORP,10);
				
				// PWM Bars
				Font.SetColor(BLACK,WHITE,false);
				j+=61+(i*2);
				for (int a=0;a<(LCD.GetWidth()-139)*PWM.GetActinicValue()/100;a++) LCD.DrawLine(alphaBlend(BLUE,WHITE,(a*100)/(LCD.GetWidth()-139)),130+a,j,130+a,j+20);
				LCD.Clear(BLACK,((LCD.GetWidth()-139)*PWM.GetActinicValue()/100)+130,j,LCD.GetWidth(),j+20);
				Font.DrawText(85,j+5,PWM.GetActinicValue());
				Font.DrawText("%   ");
				j+=30+(i/2);
				for (int a=0;a<(LCD.GetWidth()-139)*PWM.GetDaylightValue()/100;a++) LCD.DrawLine(alphaBlend(ORANGE,WHITE,(a*100)/(LCD.GetWidth()-139)),130+a,j,130+a,j+20);
				LCD.Clear(BLACK,((LCD.GetWidth()-139)*PWM.GetDaylightValue()/100)+130,j,LCD.GetWidth(),j+20);
				Font.DrawText(85,j+5,PWM.GetDaylightValue());
				Font.DrawText("%   ");
				
				
				// ATO Buttons
				j+=40+(i*3/2);
				LCD.DrawBMP(LCD.GetWidth()/10,j,REDBUTTON);
				LCD.DrawBMP(LCD.GetWidth()*6/10,j,GREENBUTTON);
			}
			else if (DisplayedScreen>=RELAY_BOX && DisplayedScreen<=EXP_BOX_8)
			{
				byte TempRelay,TempRelayOn,TempRelayOff;
				if (DisplayedScreen==RELAY_BOX)
				{
					TempRelay=Relay.RelayData;
					TempRelayOn=Relay.RelayMaskOn;
					TempRelayOff=Relay.RelayMaskOff;
				}
				else
				{
					TempRelay=Relay.RelayDataE[DisplayedScreen-2];
					TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-2];
					TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-2];
				}
				TempRelay&=TempRelayOff;
				TempRelay|=TempRelayOn;
				
				if (LastRelayData!=TempRelay) NeedsRedraw=true;
				LastRelayData=TempRelay;

				if (NeedsRedraw)
				{
					NeedsRedraw=false;

					//Gray Bar
					for (int a=0;a<=25;a++) LCD.DrawLine(alphaBlend(RELAYBOXLABELBAR/DisplayedScreen,a*4),0,40+a,LCD.GetWidth(),40+a);
					LargeFont.SetColor(WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((LCD.GetWidth()/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
					
					// Relay Ports
					
					j=50+(i/2);
					Font.SetColor(WHITE,BKCOLOR,true);
					
					int l=LCD.GetWidth();
					byte k=l/4;
					for (int a=1;a<9;a++)
					{
						if (a==5)
						{
							j=50+(i/2);
							k*=3;
						}
						j+=40+i;
						if (a<5)
						{
							if (bitRead(TempRelay,a-1))
							{
								for (int c=0;c<=100;c++) LCD.DrawLine(alphaBlend(RELAYGREEN,c),c,j-10,c,j+20);
								LCD.Clear(RELAYGREEN,100,j-10,(l/2)-10,j+20);
							}
							else
							{
								for (int c=0;c<=100;c++) LCD.DrawLine(alphaBlend(RED,c),c,j-10,c,j+20);
								LCD.Clear(RED,100,j-10,(l/2)-10,j+20);
							}
							if (bitRead(TempRelayOn,a-1) || !bitRead(TempRelayOff,a-1)) LCD.DrawRectangle(YELLOW,(l/2)-15,j-10,(l/2)-10,j+20,true);
						}
						else
						{
							if (bitRead(TempRelay,a-1))
							{
								for (int c=0;c<=100;c++) LCD.DrawLine(alphaBlend(RELAYGREEN,c),l-c,j-10,l-c,j+20);
								LCD.Clear(RELAYGREEN,(l/2)+10,j-10,l-100,j+20);
							}
							else
							{
								for (int c=0;c<=100;c++) LCD.DrawLine(alphaBlend(RED,c),l-c,j-10,l-c,j+20);
								LCD.Clear(RED,(l/2)+10,j-10,l-100,j+20);
							}
							if (bitRead(TempRelayOn,a-1) || !bitRead(TempRelayOff,a-1)) LCD.DrawRectangle(YELLOW,(l/2)+10,j-10,(l/2)+15,j+20,true);
						}
						eeprom_read_block((void*)&tempname, (void*)(R1Name+((a-1)*0x10)+((DisplayedScreen-1)*0x80)), sizeof(tempname));
						Font.DrawCenterText(k,j,tempname);
					}
				}
			}
#ifdef PWMEXPANSION
			else if(DisplayedScreen==PWM_SCREEN)
			{
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					//Gray Bar
					for (int a=0;a<=25;a++) LCD.DrawLine(alphaBlend(PWMLABELBAR,a*4),0,40+a,LCD.GetWidth(),40+a);
					LargeFont.SetColor(WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((LCD.GetWidth()/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));

					j=50+(i/2);
					Font.SetColor(BKCOLOR,WHITE,true);
					for (int a=1;a<7;a++)
					{
						j+=25+i;
						LCD.Clear(WHITE,10,j,130,j+20);
						for (int a=0;a<10;a++) LCD.DrawPixel(RED,88,j+(a*2));
						eeprom_read_block((void*)&tempname, (void*)(PWMChannel1+((a-1)*0x10)), sizeof(tempname));
						Font.DrawText(15,j+5,tempname);
					}					
				}	
				
				// PWM Bars
				j=50+(i/2);
				Font.SetColor(BLACK,WHITE,false);
				for (int p=0;p<6;p++)
				{
					j+=25+i;
					for (int a=0;a<(LCD.GetWidth()-139)*PWM.ExpansionChannel[p]/100;a++) LCD.DrawLine(alphaBlend(GREEN,WHITE,(a*100)/(LCD.GetWidth()-139)),130+a,j,130+a,j+20);
					LCD.Clear(BLACK,((LCD.GetWidth()-139)*PWM.ExpansionChannel[p]/100)+130,j,LCD.GetWidth(),j+20);
					Font.DrawText(95,j+5,PWM.ExpansionChannel[p]);
					Font.DrawText("%   ");
				}
			}
#endif // PWMEXPANSION
#ifdef RFEXPANSION			
			else if(DisplayedScreen==RF_SCREEN)
			{
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					//Gray Bar
					for (int a=0;a<=25;a++) LCD.DrawLine(alphaBlend(RFLABELBAR,a*4),0,40+a,LCD.GetWidth(),40+a);
					LargeFont.SetColor(WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((LCD.GetWidth()/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
					Font.SetColor(GOLD,BKCOLOR,true);
					
					j=100+(i*2);
					Font.DrawCenterTextP((LCD.GetWidth()/2),j,MODE);
					j+=55+(i*2);
					Font.DrawCenterTextP((LCD.GetWidth()/2),j,SPEED);
					j+=55+(i*2);
					Font.DrawCenterTextP((LCD.GetWidth()/2),j,DURATION);
				}
				j=67+(i*2);
				LargeFont.DrawCenterTextP((LCD.GetWidth()/2),j,(char * )pgm_read_word(&(rf_items[0])));
				j+=55+(i*2);
				LargeFont.DrawCenterNumber((LCD.GetWidth()/2),j,100,0);
				j+=55+(i*2);
				LargeFont.DrawCenterNumber((LCD.GetWidth()/2),j,4,0);
			}
#endif // RFEXPANSION
#ifdef AI_LED	
			else if(DisplayedScreen==AI_SCREEN)
			{
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					//Gray Bar
					for (int a=0;a<=25;a++) LCD.DrawLine(alphaBlend(AILABELBAR,a*4),0,40+a,LCD.GetWidth(),40+a);
					LargeFont.SetColor(WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((LCD.GetWidth()/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));

					j=35;
					Font.SetColor(BKCOLOR,WHITE,true);
					for (int a=1;a<4;a++)
					{
						j+=50+(i*2);
						LCD.Clear(WHITE,10,j,130,j+20);
						for (int a=0;a<10;a++) LCD.DrawPixel(RED,93,j+(a*2));
					}					
					j=85+(i*2);
					Font.DrawTextP(15,j+5,AI_WHITE);
					j+=50+(i*2);
					Font.DrawTextP(15,j+5,AI_BLUE);
					j+=50+(i*2);
					Font.DrawTextP(15,j+5,AI_ROYAL_BLUE);

				}	
				
				// PWM Bars
				j=35;
				Font.SetColor(BLACK,WHITE,false);
				for (int p=0;p<3;p++)
				{
					j+=50+(i*2);
					for (int a=0;a<(LCD.GetWidth()-139)*AI.GetChannel(p)/100;a++) LCD.DrawLine(alphaBlend(GREEN,WHITE,(a*100)/(LCD.GetWidth()-139)),130+a,j,130+a,j+20);
					LCD.Clear(BLACK,((LCD.GetWidth()-139)*AI.GetChannel(p)/100)+130,j,LCD.GetWidth(),j+20);
					Font.DrawText(100,j+5,AI.GetChannel(p));
					Font.DrawText("%   ");
				}
			}
#endif // AI_LED
		}
	}
	//for ( byte a = 0; a < 6; a++ ) PWM.Expansion(a, int(2.55*PWM.ExpansionChannel[a]));
}
void ReefTouchClass::CalibrateTouchScreen()
{
	boolean calibrated=false;
	long CalibrationPoints[4]={24,32,213,287};
	long TouchedPoints[4];
	int eindex=0;

	// Initialize EEPROM strings
	char tempname[15];
	char temp[3];
	
	// Temperature Probes
	for (int a=0; a<3; a++)
	{
		strcpy_P(tempname,TEMP);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(Probe1Name+(a*0x10)), sizeof(tempname));
	}
	
	// Relays
	for (int a=0; a<=8; a++)
	{
		for (int b=0; b<8; b++)
		{
			strcpy_P(tempname,RELAY);
			if (a>0)
			{
				itoa(a,temp,10);
				strcat(tempname,temp);
			}
			itoa(b+1,temp,10);
			strcat(tempname,temp);
			eeprom_write_block((void*)&tempname, (void*)(R1Name+(eindex*0x10)), sizeof(tempname));
			eindex++;
		}
	}

	//PWM Expansion Channels
	for (int a=0; a<6; a++)
	{
		strcpy_P(tempname,CHANNEL);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(PWMChannel1+(a*0x10)), sizeof(tempname));
	}
	
	
	LCD.SetOrientation(1);
	while (!calibrated)
	{
		LCD.FullClear(WHITE);
		LargeFont.SetColor(GOLD,WHITE,true);
		LargeFont.DrawCenterTextP(120,45,CALI1);
		LargeFont.DrawCenterTextP(120,75,CALI2);
		LargeFont.DrawCenterTextP(120,105,CALI3);
		LargeFont.DrawCenterTextP(120,135,CALI4);
		Font.SetColor(BLACK,WHITE,true);
		Font.DrawCenterTextP(120,190,CALI5);
		Font.DrawCenterTextP(120,205,CALI6);
		Font.DrawCenterTextP(120,220,CALI7);
		for(byte a = 0; a<2; a++)
		{
			LCD.DrawCircle(RED,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 5, false);
			while (!TS.IsTouched());
			TS.GetTouch();
//			Serial.println(TS.uX,DEC);
//			Serial.println(TS.uY,DEC);
			if (a==0)
			{
				SmallFont.DrawText(SILVER,WHITE,40,25,TS.uX);			
				SmallFont.DrawText(SILVER,WHITE,40,35,TS.uY);
			}
			else
			{
				SmallFont.DrawText(SILVER,WHITE,175,280,TS.uX);			
				SmallFont.DrawText(SILVER,WHITE,175,290,TS.uY);
			}
			TouchedPoints[a*2] = TS.uX;
			TouchedPoints[(a*2)+1] = TS.uY;
			LCD.DrawCircle(BLUE,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 4, true);
			delay(500);
		}
		TS.calibration.XMin=map(0,CalibrationPoints[0],CalibrationPoints[2],TouchedPoints[0],TouchedPoints[2]);
		TS.calibration.XMax=map(239,CalibrationPoints[0],CalibrationPoints[2],TouchedPoints[0],TouchedPoints[2]);
		TS.calibration.YMin=map(0,CalibrationPoints[1],CalibrationPoints[3],TouchedPoints[1],TouchedPoints[3]);
		TS.calibration.YMax=map(319,CalibrationPoints[1],CalibrationPoints[3],TouchedPoints[1],TouchedPoints[3]);
		if (
			TS.calibration.XMin > TS_CALIBRATION_XMIN - TS_CALIBRATION_DELTA && 
			TS.calibration.XMin < TS_CALIBRATION_XMIN + TS_CALIBRATION_DELTA &&
			TS.calibration.XMax > TS_CALIBRATION_XMAX - TS_CALIBRATION_DELTA && 
			TS.calibration.XMax < TS_CALIBRATION_XMAX + TS_CALIBRATION_DELTA &&
			TS.calibration.YMin > TS_CALIBRATION_YMIN - TS_CALIBRATION_DELTA &&
			TS.calibration.YMin < TS_CALIBRATION_YMIN + TS_CALIBRATION_DELTA &&
			TS.calibration.YMax > TS_CALIBRATION_YMAX - TS_CALIBRATION_DELTA &&
			TS.calibration.YMax < TS_CALIBRATION_YMAX + TS_CALIBRATION_DELTA
			) calibrated=true;
//		calibrated=true;
	}
    TS.SaveCalibration();
    Tilt.CompensateAccelerometer();
	LCD.Clear(WHITE,0,180,239,270);
	OkButton.SetPosition(88,200);
	OkButton.Show();
	do TS.GetTouch(); while(!ReefTouch.OkButton.IsPressed());
	OkButton.Hide();
	LCD.FullClear(WHITE);
	
}

void ReefTouchClass::ChangeDisplayedScreen(signed char index)
{
	NeedsRedraw=true;
	DisplayedScreen+=index;
	if (DisplayedScreen<0) DisplayedScreen=MAX_SCREENS;
	if (DisplayedScreen>RELAY_BOX)
		if (index>0)
		{
			for (int a=DisplayedScreen;a<=MAX_SCREENS;a++)
			{
				if (a<10)
				{
					if (bitRead(RelayExpansionFlag,a-2)) break;
				}
				else
				{
					if (bitRead(ExpansionFlag,a-10)) break;
				}
				DisplayedScreen++;
			}
		}
		else
		{
			for (int a=DisplayedScreen;a>=2;a--)
			{
				if (a<10)
				{
					if (bitRead(RelayExpansionFlag,a-2)) break;
				}
				else
				{
					if (bitRead(ExpansionFlag,a-10)) break;
				}
				DisplayedScreen--;
			}
		}
	if (DisplayedScreen>MAX_SCREENS) DisplayedScreen=0;
	LCD.Clear(BLACK,0,33,LCD.GetWidth(),LCD.GetHeight());
}

void TFT::Init()
{
	CS0;
	RST1;
	delay(5); 
	RST0;
	delay(15);
	RST1;
	delay(15);


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

	SendCom(0x00,0x22);   
}

void TFT::SendComData(int Com,int Data)
{
    SendCom(Com>>8,Com);
    SendData(Data>>8,Data);
}

void TFT::SendCom(byte H,byte L)
{
	RS0;
	WriteBus(H,L);
}

void TFT::SendData(byte H,byte L)
{
	RS1;
	WriteBus(H,L);
}

void TFT::WriteBus(byte H,byte L)
{
	CS0;
	PORTA=H;
	PORTC=L;
	WR0;
	WR1;
	CS1;
}

void TFT::Clear(int color, int x1, int y1, int x2, int y2)
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


void TFT::SetBox(int x1, int y1, int x2, int y2)
{
	SendComData(0x0002,x1>>8);     // Column address start2
	SendComData(0x0003,x1);    // Column address start1
	SendComData(0x0004,x2>>8);     // Column address end2
	SendComData(0x0005,x2);    // Column address end1
	SendComData(0x0006,y1>>8);     // Row address start2
	SendComData(0x0007,y1);    // Row address start1
	SendComData(0x0008,y2>>8);     // Row address end2
	SendComData(0x0009,y2);    // Row address end1
	SendCom(0x00,0x22);      
}

void TFT::DrawPixel(int color, int x, int y)
{
	SetBox(x, y, x, y);
	SendData(color>>8,color&0x00ff);
//	CS0;
//	WR0;
//	WR1;
//	CS1;	
}

void TFT::Pixels(int count, const byte* d)
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
void TFT::Scroll(int offset)
{
	SendComData(0x0014,offset>>8);
	SendComData(0x0015,offset);
}

LCDClass::LCDClass()
{
}

void LCDClass::Init()
{
	TFT::Init();
	SetOrientation(1);
}

void LCDClass::SetBacklight(byte bl)
{
	analogWrite(44,bl*2.55);
}

void LCDClass::SetOrientation(byte O)
{
	if (orientation!=O)
	{
		
		orientation=O;
		switch (O)
		{
		case 1:
			TFT::SendComData(0x0016,0x000f); // Normal Rotation
			w=239;
			h=319;
			break;
		case 2:
			TFT::SendComData(0x0016,0x00af); // 90 Rotation
			w=319;
			h=239;
			break;
		case 3:
			TFT::SendComData(0x0016,0x00cf); // 180 Rotation
			w=239;
			h=319;
			break;
		case 4:
			TFT::SendComData(0x0016,0x006f); // 270 Rotation
			w=319;
			h=239;
			break;
		}
		FullClear(BKCOLOR);
	}
}


byte LCDClass::GetOrientation()
{
	return orientation;
}

int LCDClass::GetWidth()
{
	return w;
}

int LCDClass::GetHeight()
{
	return h;
}

void LCDClass::Scroll(int offset)
{
	TFT::Scroll(offset);
}

void LCDClass::FullClear(int color)
{
	TFT::Clear(color,0,0,GetWidth(),GetHeight());
}
void LCDClass::Clear(int color, int x1, int y1, int x2, int y2)
{
	TFT::Clear(color,x1,y1,x2,y2);
}

void LCDClass::DrawPixel(int color, int x, int y)
{
	TFT::DrawPixel(color,x,y);
}

void LCDClass::DrawLine(int color, int x1, int y1, int x2, int y2)
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

void LCDClass::DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill)
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

void LCDClass::DrawCircle(int color, int x, int y, int radius, boolean fill)
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

void LCDClass::DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill)
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

void LCDClass::DrawBMP(int ix, int iy, const prog_uchar *iPtr)
{
	byte i,j;
	int w = pgm_read_byte_near(iPtr++)+1;
	int h  =pgm_read_byte_near(iPtr++)+1;
	iPtr+=3;
	TFT::SetBox(ix, iy, w+ix-1, h+iy-1);
	for (int a=0; a<w*h; a++)
	{
//		if ((i=pgm_read_byte_near(iPtr++))==0) i=255;
//		if ((j=pgm_read_byte_near(iPtr++))==0) j=255;
//		TFT::SendData(i,j);
		TFT::SendData(pgm_read_byte_near(iPtr++),pgm_read_byte_near(iPtr++));
	}
}

void LCDClass::DrawImage(SDFile& f, int x, int y, int scroll, int lines)
{
	//File f;
	//char s[256];
	//sprintf(s,"Draw %d %d %d %d\n",x,y,scroll,lines);
	//print(s);

    Img2 hdr;    
    if (!f.Read(&hdr,sizeof(Img2)) || hdr.sig[0] != 'i' || hdr.sig[3] != '2')
        return;
	//	Palettes are bad news on small machines
	//	Removing and replacing with new runlength format

    //  Load palette
   // byte palette[512];    
    int paletteLen = (hdr.colors*2 + 3) & ~3;
	f.Skip(paletteLen);   // Align to 4
    //f.Read(palette+2,paletteLen);   // Align to 4
    
    int width = hdr.width;
    if (lines == 0)
        lines = hdr.height;
    int maxy = min(hdr.height,lines) + y;
    
    //  Skip restart indexes
    if (hdr.restartInterval)
    {
        int restarts = (hdr.height + hdr.restartInterval - 1)/hdr.restartInterval + 1;
        if (scroll > 0)   // 
        {
            int i = min(scroll/hdr.restartInterval,restarts-1);
            scroll -= i*hdr.restartInterval;
            maxy -= i*hdr.restartInterval;
            f.Skip(i*4);
            unsigned long restart;
            f.Read(&restart,4);
            f.SetPos(restart);
        } else
            f.Skip(restarts*4);
    }
    
    //  Skip restart intervals
    if (maxy > 320)
        maxy = 320;
	TFT::SetBox(x,y,hdr.width,maxy-y); 
            
    byte format = hdr.format;
    if (format & 0x80)
    {
        f.SetPos(512 + ((long)scroll)*512);
        while (y < maxy)
        {
            // 512 bytes in buffer
            int count;
            const byte* b = f.GetBuffer(&count);
            //LCD.Blit(b,width);
            TFT::Pixels(width,b);
            f.Skip(512);
            y++;
        }
        return;
	}

	//	Other format deprecated because it was silly
}

void LCDClass::DrawDateTime(int x, int y, boolean militarytime)
{
	char text[9];
	char temp[3];
	byte m,s;
	
	ReefTouch.Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
	if (!militarytime && hour()>12)
	{
		ReefTouch.Font.DrawText(x,y,hour()-12);
	}
	else
	{
		ReefTouch.Font.DrawText(x,y,hour());
	}
	ReefTouch.Font.DrawText(":");
	m=minute();
	if (m<10) ReefTouch.Font.DrawText("0");
	ReefTouch.Font.DrawText(m);
	ReefTouch.Font.DrawText(":");
	s=second();
	if (s<10) ReefTouch.Font.DrawText("0");
	ReefTouch.Font.DrawText(s);
	if (!militarytime) if(hour()>=12) ReefTouch.Font.DrawText(x+50,y,"PM"); else ReefTouch.Font.DrawText(x+50,y,"AM");
	strcpy(text,"");
	if (month()<10) strcat(text,"0");
	itoa(month(),temp,10);
	strcat(text,temp);
	strcat(text,"/");
	if (day()<10) strcat(text,"0");
	itoa(day(),temp,10);
	strcat(text,temp);
	strcat(text,"/");
	if (year()<2010) strcat(text,"0");
	itoa(year()-2000,temp,10);
	strcat(text,temp);
	ReefTouch.Font.DrawText(GetWidth()-ReefTouch.Font.TextWidth(text),y,text);
}


FontClass::FontClass()
{
	FC = WHITE;
	BC = BLACK;
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
					TFT::DrawPixel(FC, pix_x, pix_y);
				}
				else if(!Transparent)
				{
					TFT::DrawPixel(BC, pix_x , pix_y);
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
					TFT::DrawPixel(FC, pix_x , pix_y);
				}
				else 
				{
					if(!Transparent) TFT::DrawPixel(BC, pix_x , pix_y);
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
	if(!Transparent) TFT::Clear(BC,x-CS,y,x,y+h);
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
		width+=TextWidth(".");
		if (decimal<10) width+=TextWidth("0");
		width/=2;
		DrawText(ix-width,iy,n/decimal);
		DrawText(".");
		DrawText(n%decimal);
	}
	else
	{
		width/=2;
		DrawText(ix-width,iy,n);
	}
}

TouchScreen::TouchScreen()
{
}

void TouchScreen::Init()
{
	X=0;Y=0;uX=0;uY=0;
	CalibrationNeeded=false;
	ApplyCalibration();
}

void TouchScreen::ApplyCalibration()
{
	eeprom_read_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
	if (calibration.XMin<TS_CALIBRATION_XMIN - TS_CALIBRATION_DELTA || calibration.XMin>TS_CALIBRATION_XMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.XMax<TS_CALIBRATION_XMAX - TS_CALIBRATION_DELTA || calibration.XMax>TS_CALIBRATION_XMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMin<TS_CALIBRATION_YMIN - TS_CALIBRATION_DELTA || calibration.YMin>TS_CALIBRATION_YMIN + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
	if (calibration.YMax<TS_CALIBRATION_YMAX - TS_CALIBRATION_DELTA || calibration.YMax>TS_CALIBRATION_YMAX + TS_CALIBRATION_DELTA) CalibrationNeeded=true;
}	

void TouchScreen::SaveCalibration()
{
	eeprom_write_block((void*)&calibration, (void*)TS_CALIBRATION_ADDRESS, sizeof(CALIBRATION));
}

void TouchScreen::GetTouch()
{
	int a,b;
	int uZ1,uZ2;
	
	SPCR=0x53;
	uX=0;
	uY=0;
	TP0;
	SPI.transfer(0xd0);
	a= SPI.transfer(0);
	b= SPI.transfer(0);
	uX=(a<<5|b>>3);

	SPI.transfer(0x90);
	a= SPI.transfer(0);
	b= SPI.transfer(0);
	uY=(a<<5|b>>3);

	SPI.transfer(0xb0);
	a= SPI.transfer(0);
	b= SPI.transfer(0);
	uZ1=(a<<5|b>>3);

	SPI.transfer(0xc0);
	a= SPI.transfer(0);
	b= SPI.transfer(0);
	uZ2=(a<<5|b>>3);

	double pressure;
	pressure=uZ2;
	pressure/=uZ1;
	pressure*=100;

	if (pressure>TouchPressure) uX=0;

	TP1;
	SPCR=0x50;
	if (uX==0) uY=0;

	switch (ReefTouch.LCD.GetOrientation())
	{
	case 1:
		X=map(uX,calibration.XMin,calibration.XMax,0,239);
		Y=map(uY,calibration.YMin,calibration.YMax,0,319);
		X=constrain(X,0,239);
		Y=constrain(Y,0,319);
		break;
	case 2:
		X=map(uY,calibration.YMax,calibration.YMin,0,319);
		Y=map(uX,calibration.XMin,calibration.XMax,0,239);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
		break;
	case 3:
		X=map(uX,calibration.XMax,calibration.XMin,0,239);
		Y=map(uY,calibration.YMax,calibration.YMin,0,319);
		X=constrain(X,0,239);
		Y=constrain(Y,0,319);
		break;
	case 4:
		X=map(uY,calibration.YMin,calibration.YMax,0,319);
		Y=map(uX,calibration.XMax,calibration.XMin,0,239);
		X=constrain(X,0,319);
		Y=constrain(Y,0,239);
		break;
	}
	if (X <= 0) X = 0;
	if (Y <= 0) Y = 0;
}

boolean TouchScreen::IsTouched()
{
	boolean t=!(PIND&(1<<5));
	if (t) 
	{
		GetTouch();
		if (FirstX==0 && FirstY==0)
		{
			FirstX=X;
			FirstY=Y;
		}
		if (X-FirstX>TouchSlideDelta) SlideIndex++;
		if (FirstX-X>TouchSlideDelta) SlideIndex--;
	}
	else
	{
		FirstX=FirstY=0;
		if (SlideIndex>0) ReefTouch.ChangeDisplayedScreen(1);
		if (SlideIndex<0) ReefTouch.ChangeDisplayedScreen(-1);
		SlideIndex=0;
	}
	return t;
}

boolean TouchScreen::IsCalibrationNeeded()
{
	return CalibrationNeeded;
}

TiltClass::TiltClass()
{
}

void TiltClass::Init()
{
	Write(0x16,0x5);
	ApplyCompensation();
}

void TiltClass::Write(byte reg, byte data)
{
	ACC0;
	SPI.transfer(0x80 | (reg << 1));
	SPI.transfer(data);
	ACC1;
}

byte TiltClass::Read(byte data)
{
	SPI.transfer(data << 1);
	return SPI.transfer(0);
}

void TiltClass::Refresh()
{
	ACC0;
	x=Read(0x6);
	y=Read(0x7);
	ACC1;
	//if (Tilt.GetX()>-TT_SENSITIVITY && Tilt.GetX()<TT_SENSITIVITY && Tilt.GetY()>-TT_SENSITIVITY && Tilt.GetY()<TT_SENSITIVITY) LCD.SetOrientation(1);
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y>TT_SENSITIVITY) ReefTouch.LCD.SetOrientation(1);
	if (x>TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) ReefTouch.LCD.SetOrientation(2);
	if (x>-TT_SENSITIVITY && x<TT_SENSITIVITY && y<-TT_SENSITIVITY) ReefTouch.LCD.SetOrientation(3);
	if (x<-TT_SENSITIVITY && y>-TT_SENSITIVITY && y<TT_SENSITIVITY) ReefTouch.LCD.SetOrientation(4);
}

void TiltClass::CompensateAccelerometer()
{
	int xoffd, yoffd;
	compensation.XOff = 0;
	compensation.YOff = 0;
	do
	{
		Refresh();
	    xoffd = -2.1*(x);
	    yoffd = -2.1*(y);
	    compensation.XOff += xoffd;
	    compensation.YOff += yoffd;
	    Write(0x10, 0xFF&compensation.XOff);
	    Write(0x11, 0xFF&(compensation.XOff>>8));
	    Write(0x12, 0xFF&compensation.YOff);
	    Write(0x13, 0xFF&(compensation.YOff>>8));
	    delay(10);
	} 
	while (abs(xoffd) > 2 || abs(yoffd) > 2);
	eeprom_write_block((void*)&compensation, (void*)TT_COMPENSATION_ADDRESS, sizeof(COMPENSATION));
}

void TiltClass::ApplyCompensation()
{
	eeprom_read_block((void*)&compensation, (void*)TT_COMPENSATION_ADDRESS, sizeof(COMPENSATION));
    Write(0x10, 0xFF&compensation.XOff);
    Write(0x11, 0xFF&(compensation.XOff>>8));
    Write(0x12, 0xFF&compensation.YOff);
    Write(0x13, 0xFF&(compensation.YOff>>8));
    delay(10);
}	

ButtonClass::ButtonClass()
{
}

void ButtonClass::Create(int icolor, int itextcolor, char *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void ButtonClass::SetPosition(int ix1, int iy1)
{
	x1=ix1;
	y1=iy1;
}

void ButtonClass::Show()
{
	visible=true;
	ReefTouch.LargeFont.DrawText(x1+25,y1+11,str);
	x2=ReefTouch.LargeFont.GetX()+25;
	ReefTouch.LCD.DrawRoundRect(RGB565(0xD0, 0xD0, 0xD0),x1+1,y1+1,x2+1,y1+41,10,false);
	ReefTouch.LCD.DrawRoundRect(BLACK,x1,y1,x2,y1+40,10,false);
	ReefTouch.LCD.DrawRoundRect(color,x1+1,y1+1,x2-1,y1+39,10,true);
	ReefTouch.LargeFont.SetColor(BLACK,SILVER,true);
	ReefTouch.LargeFont.DrawText(x1+26,y1+10,str);
	ReefTouch.LargeFont.SetColor(textcolor,SILVER,true);
	ReefTouch.LargeFont.DrawText(x1+25,y1+9,str);
}

void ButtonClass::Hide()
{
	visible=false;
	ReefTouch.LCD.Clear(WHITE,x1,y1,x2+1,y1+41);
}

boolean ButtonClass::IsPressed()
{
	return (ReefTouch.TS.X > x1 && ReefTouch.TS.X < x2 && ReefTouch.TS.Y > y1 && ReefTouch.TS.Y < y1+40);
}

ReefTouchClass ReefTouch = ReefTouchClass();