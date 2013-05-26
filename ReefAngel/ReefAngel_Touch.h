void ReefAngelClass::SetOrientation(byte o)
{
	if (orientation!=o) NeedsRedraw=true;
	orientation=o;
	TouchLCD.SetOrientation(o);
	TS.SetOrientation(o);
}

void ReefAngelClass::CalibrateTouchScreen()
{
	boolean calibrated=false;
	long CalibrationPoints[4]={24,32,213,287};
	long TouchedPoints[4];

	SetOrientation(1);
	while (!calibrated)
	{
		wdt_reset();
		TouchLCD.FullClear(COLOR_WHITE);
		LargeFont.SetColor(COLOR_GOLD,COLOR_WHITE,true);
		LargeFont.DrawCenterTextP(120,45,CALI1);
		LargeFont.DrawCenterTextP(120,75,CALI2);
		Font.SetColor(COLOR_BLACK,COLOR_WHITE,true);
		Font.DrawCenterTextP(120,190,CALI3);
		Font.DrawCenterTextP(120,205,CALI4);
		for(byte a = 0; a<2; a++)
		{
			TouchLCD.DrawCircle(COLOR_RED,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 5, false);
			do wdt_reset(); while (!TS.IsTouched());
			TS.GetTouch();
//			Serial.println(TS.uX,DEC);
//			Serial.println(TS.uY,DEC);
			if (a==0)
			{
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,40,25,TS.uX);			
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,40,35,TS.uY);
			}
			else
			{
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,175,280,TS.uX);			
				SmallFont.DrawText(COLOR_SILVER,COLOR_WHITE,175,290,TS.uY);
			}
			TouchedPoints[a*2] = TS.uX;
			TouchedPoints[(a*2)+1] = TS.uY;
			TouchLCD.DrawCircle(COLOR_BLUE,CalibrationPoints[a*2], CalibrationPoints[(a*2)+1], 4, true);
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
    TouchLCD.Clear(COLOR_WHITE,0,180,239,270);
	OkButton.SetPosition(78,200);
	OkButton.Show();
	do
	{
		wdt_reset();
		TS.GetTouch();
	}
	while(!OkButton.IsPressed());
	OkButton.Hide();
	TouchLCD.FullClear(COLOR_WHITE);
}

void ReefAngelClass::SaveInitialSettings()
{
	// Initialize EEPROM strings
	char tempname[15];
	char temp[3];
	int eindex=0;
	
	// Temperature Probes
	for (int a=0; a<3; a++)
	{
		strcpy_P(tempname,LABEL_TEMP);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(Probe1Name+(a*0x10)), sizeof(tempname));
	}
	
	// Relays
	for (int a=0; a<=8; a++)
	{
		for (int b=0; b<8; b++)
		{
			strcpy_P(tempname,LABEL_RELAY);
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
		strcpy_P(tempname,LABEL_CHANNEL);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(PWMChannel1+(a*0x10)), sizeof(tempname));
	}

	//IO Expansion Channels
	for (int a=0; a<6; a++)
	{
		strcpy_P(tempname,LABEL_IOPORT);
		itoa(a+1,temp,10);
		strcat(tempname,temp);
		eeprom_write_block((void*)&tempname, (void*)(IOChannel1+(a*0x10)), sizeof(tempname));
	}		
}

void ReefAngelClass::ChangeDisplayedScreen(signed char index)
{
	NeedsRedraw=true;
	DisplayedScreen+=index;
	if (DisplayedScreen<0) DisplayedScreen=MAX_SCREENS-1;
	if (DisplayedScreen>RELAY_BOX)
		if (index>0)
		{
			for (int a=DisplayedScreen;a<MAX_SCREENS;a++)
			{
				if (a<10)
				{
					if (bitRead(REM,a-2)) break;
				}
				else
				{
					if (a==10 && bitRead(EM,0)) break;
					if (a==11 && bitRead(EM,1)) break;
					if (a==12 && bitRead(EM,1)) break;
					if (a==13 && bitRead(EM,2)) break;
					if (a==14 && bitRead(EM,5)) break;
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
					if (bitRead(REM,a-2)) break;
				}
				else
				{
					if (a==10 && bitRead(EM,0)) break;
					if (a==11 && bitRead(EM,1)) break;
					if (a==12 && bitRead(EM,1)) break;
					if (a==13 && bitRead(EM,2)) break;
					if (a==14 && bitRead(EM,5)) break;
//					if (bitRead(EM,a-10)) break;
				}
				DisplayedScreen--;
			}
		}
	if (DisplayedScreen>=MAX_SCREENS) DisplayedScreen=0;
}

void ReefAngelClass::MainScreen()
{
	ExitMenu();
	NeedsRedraw=true;	
}

void ReefAngelClass::ResetScreenSaver()
{
	// turn the backlight on
	TouchLCD.SetBacklight(100);
	Timer[LCD_TIMER].SetInterval(InternalMemory.LCDTimer_read());  // LCD Sleep Mode timer
	Timer[LCD_TIMER].Start();  // start timer

}

void ReefAngelClass::SetupTouchDateTime()
{
	int y,twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	DisplayedMenu=DATE_TIME_MENU;
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2+1, 6, MENU_BUTTON_DATETIME);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(TouchLCD.GetWidth()/2, 5, MENU_BUTTON_DATETIME);

	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	y=(theight/4);
	LargeFont.DrawText(twidth*3/8-15,y,"/");
	LargeFont.DrawText(twidth*5/8+5,y,"/");

	y=(theight*3/5);
	LargeFont.DrawText(twidth*3/8-15,y,":");

	Font.SetColor(COLOR_SILVER, COLOR_WHITE,false);
	y=(theight/4)-35;
	Font.DrawCenterText(twidth/6,y,"Month");
	Font.DrawCenterText(twidth/2,y,"Day");
	Font.DrawCenterText(twidth*5/6,y,"Year");
	y=(theight*3/5)-35;
	Font.DrawCenterText(twidth/6,y,"Hour");
	Font.DrawCenterText(twidth/2,y,"Minute");
	Font.DrawCenterText(twidth*5/6,y,"AM/PM");

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();

	NeedsRedraw=true;
	newnow=now();
}

void ReefAngelClass::SetupTouchCalibratePH()
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	DisplayedMenu=PH_CALIBRATE_MENU;
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_PH);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_PH);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
	Font.DrawCenterTextP(twidth/2- 10,theight*5/12,MENU_BUTTON_PH);
	Font.DrawCenterNumber(twidth/2+ 10,theight*5/12,7,1);
	Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
}

void ReefAngelClass::SetupTouchCalibrateSal()
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	DisplayedMenu=SAL_CALIBRATE_MENU;
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_SALINITY);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_SALINITY);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
#ifdef SALINITYEXPANSION
	Font.DrawCenterTextP(twidth/2,theight/3,SAL_CALI1);
	Font.DrawCenterNumber(twidth/2- 10,theight*5/12,35,0);
	Font.DrawCenterTextP(twidth/2+ 15,theight*5/12,SAL_CALI2);
	Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/3,NO_SAL1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,NO_SAL2);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
}

void ReefAngelClass::SetupTouchCalibrateORP()
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	DisplayedMenu=ORP_CALIBRATE_MENU;
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_ORP);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_ORP);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
#ifdef ORPEXPANSION
	Font.DrawCenterTextP(twidth/2,theight/3,ORP_CALI1);
	Font.DrawCenterNumber(twidth/2- 15,theight*5/12,470,0);
	Font.DrawCenterTextP(twidth/2+ 18,theight*5/12,ORP_CALI2);
	Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/3,NO_ORP1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,NO_SAL2);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
}

void ReefAngelClass::CheckMenuTimeout()
{
	if ( (now() - menutimeout) > MENU_TIMEOUT )
	{
		SetDisplayedMenu(DEFAULT_MENU);
		DisplayedScreen=MAIN_SCREEN;
		NeedsRedraw=true;
		ResetScreenSaver();
#ifdef REEFTOUCHDISPLAY
		SendMaster(MESSAGE_MENU,DEFAULT_MENU,DEFAULT_MENU); 	// Change Menu
#endif // REEFTOUCHDISPLAY
	}
}

void ReefAngelClass::ShowTouchMenu()
{
	SetDisplayedMenu(TOUCH_MENU);
	DisplayedScreen=MAIN_MENU_SCREEN;
	NeedsRedraw=true;
}

void ReefAngelClass::ShowTouchInterface()
{
	Refresh();
	int twidth=TouchLCD.GetWidth();
	int theight=TouchLCD.GetHeight();
	if (LastOrientation != orientation)
	{
		LastOrientation = orientation;
		twidth=TouchLCD.GetWidth();
		theight=TouchLCD.GetHeight();
		NeedsRedraw=true;
	}
	byte numexp=0;
	if ((EM&(1<<3))!=0) numexp++;
	if ((EM&(1<<4))!=0) numexp++;
	if ((EM&(1<<6))!=0) numexp++;
	if ((EM&(1<<7))!=0) numexp++;

	switch ( DisplayedMenu )
	{
		case MAIN_MENU:
		case DEFAULT_MENU:  // This is the home screen
		{
			// process screensaver timeout
			if ( Timer[LCD_TIMER].IsTriggered() )
			{
				// Screensaver timeout expired
				TouchLCD.SetBacklight(0);
				Sleeping=true;
				DisplayedScreen==MAIN_SCREEN;
				NeedsRedraw=true;
			}
#ifdef CUSTOM_MAIN
			DrawCustomMain();
#else  // CUSTOM_MAIN
			if (!Splash)
			{
				int i,j;
				char tempname[15];
				byte TempRelay,TempRelayOn,TempRelayOff;

				if (!TS.IsTouched())
				{
					LongTouch=0;
					if (!Sleeping)
					{
						TouchEnabled=true;
						//Draw Top Bar
						if (orientation%2==0) i=0; else i=12;
						if (NeedsRedraw)
						{
							//Top Bar
							TouchLCD.DrawRectangle(TOPBAR_BC,0,0,twidth,27,true);
							for (int a=0;a<=5;a++)
							{
								TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,28+a,twidth,28+a);
							}
							//Bottom Bar
							TouchLCD.DrawRectangle(BOTTOMBAR_BC,0,theight-27,twidth,theight,true);
							for (int a=0;a<=5;a++)
							{
								TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,theight-28-a,twidth,theight-28-a);
							}
							//Logo
							TouchLCD.DrawBMP(5,2,ICONLOGO);
							//Arrows
							TouchLCD.DrawBMP(10,theight-25,ARROWLEFT);
							TouchLCD.DrawBMP(twidth-10-23,theight-25,ARROWRIGHT);
							//Menu Dividers and arrow
							TouchLCD.DrawBMP(twidth/2-50,theight-25,DIVIDER);
							TouchLCD.DrawBMP(twidth/2+50,theight-25,DIVIDER);
							TouchLCD.DrawBMP(twidth/2-3,theight-25,ARROWMENU);
							Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
							Font.DrawCenterText(twidth/2,theight-15,"Menu");
						}
						TouchLCD.DrawDateTime(38,9,MilitaryTime,Font);
						TouchLCD.DrawFlags(Flags);
						if (DisplayedScreen==MAIN_SCREEN)
						{
							int x=0;

							if (NeedsRedraw)
							{
								NeedsRedraw=false;

								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
								if (i==12 || i==4) // Orientation is portrait
								{
									x=twidth*3/16;
									if (numexp>2) i=4;
									//Temperature
									j=60+i;
									Font.SetColor(COLOR_GOLD,BKCOLOR,true);
									eeprom_read_block((void*)&tempname, (void*)Probe1Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/16;
									eeprom_read_block((void*)&tempname, (void*)Probe2Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/16;
									eeprom_read_block((void*)&tempname, (void*)Probe3Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);

									//pH
									x=twidth*3/16;
									j+=45+i;
									Font.DrawCenterTextP(x,j,LABEL_PH);
									x+=twidth*5/16;
									//Salinity
									if ((EM&(1<<3))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_SALINITY);
										x+=twidth*5/16;
									}
									//ORP
									if ((EM&(1<<4))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_ORP);
										x+=twidth*5/16;
									}
									//pH Exp
									if ((EM&(1<<6))!=0)
									{
										if (x>twidth*14/16)
										{
											x=twidth*3/16;
											j+=45+i;
										}
										Font.DrawCenterTextP(x,j,LABEL_PHE);
										x+=twidth*5/16;
									}
									//Water Level
									if ((EM&(1<<7))!=0)
									{
										if (x>twidth*14/16)
										{
											x=twidth*3/16;
											j+=45+i;
										}
										Font.DrawCenterTextP(x,j,LABEL_WL);
									}
								}
								else // Orientation is landscape
								{
									//Temperature, pH
									x=twidth*3/21;
									if (numexp==0) i=7;
									j=64+i;
									Font.SetColor(COLOR_GOLD,BKCOLOR,true);
									eeprom_read_block((void*)&tempname, (void*)Probe1Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									eeprom_read_block((void*)&tempname, (void*)Probe2Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									eeprom_read_block((void*)&tempname, (void*)Probe3Name, sizeof(tempname));
									Font.DrawCenterText(x,j,tempname);
									x+=twidth*5/21;
									Font.DrawCenterTextP(x,j,LABEL_PH);

									x=twidth*3/21;
									if (numexp>0) j+=43+i;
									//Salinity
									if ((EM&(1<<3))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_SALINITY);
										x+=twidth*5/21;
									}
									//ORP
									if ((EM&(1<<4))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_ORP);
										x+=twidth*5/21;
									}
									//pH Exp
									if ((EM&(1<<6))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_PHE);
										x+=twidth*5/21;
									}
									//Water Level
									if ((EM&(1<<7))!=0)
									{
										Font.DrawCenterTextP(x,j,LABEL_WL);
									}
								}

								j+=18+i;

								//Division
								TouchLCD.DrawLine(DIVISION,0,j,twidth,j);

								j+=5+i;
								PB[0].NeedsRedraw=true;
								j+=30+(i/2);
								PB[1].NeedsRedraw=true;
								j+=25+i;
								//Division
								TouchLCD.DrawLine(DIVISION,0,j,twidth,j);

								j+=8+(i*3/4);

								//ATO Ports
								Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,(twidth/10)+25,j,LABEL_ATOHIGHPORT);
								Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,(twidth*6/10)+25,j,LABEL_ATOLOWPORT);
							}

							// Draw Parameter values
							if (i==12 || i==4) // Orientation is portrait
							{
								if (numexp>2) i=4;
								//Temperature
								x=twidth*3/16;
								j=27+i;
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								x+=twidth*5/16;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								x+=twidth*5/16;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);

								//pH
								x=twidth*3/16;
								j+=43+i;
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);
								x+=twidth*5/16;
#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									x+=twidth*5/16;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,1);
									x+=twidth*5/16;
								}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
								//pH Exp
								if ((EM&(1<<6))!=0)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									x+=twidth*5/16;
								}
#endif // PHEXPANSION
#ifdef WATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(),0);
									x+=twidth*5/16;
								}
#endif // WATERLEVELEXPANSION
							}
							else // Orientation is landscape
							{
								x=twidth*3/21;
								if (numexp==0) i=7;
								//Temperature
								j=32+i;
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
								x+=twidth*5/21;
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);

								x=twidth*3/21;
								if (numexp>0) j+=43+i;
#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									x+=twidth*5/21;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,10);
									x+=twidth*5/21;
								}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
								//pH Exp
								if ((EM&(1<<6))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									x+=twidth*5/21;
								}
#endif // PHEXPANSION
#ifdef WATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(),0);
								}
#endif // WATERLEVELEXPANSION
							}

							// Progress Bars
							j+=55+(i*2);
							PB[0].SetPosition(10,j);
							PB[0].SetColor(COLOR_ORANGE);
							PB[0].SetLabel("Daylight");
							PB[0].SetCurrent(PWM.GetDaylightValue());
							PB[0].Show();
							j+=29+(i/2);
							PB[1].SetPosition(10,j);
							PB[1].SetColor(COLOR_ROYALBLUE);
							PB[1].SetLabel("Actinic");
							PB[1].SetCurrent(PWM.GetActinicValue());
							PB[1].Show();

							j+=19+i;
							if (i==4) // Orientation is portrait
							{
								j+=3;
							}

							// ATO Buttons
							j+=11+(i*3/4);
							if (HighATO.IsActive())
								TouchLCD.DrawBMP(twidth/10,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(twidth/10,j,REDBUTTON);
							if (LowATO.IsActive())
								TouchLCD.DrawBMP(twidth*6/10,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(twidth*6/10,j,REDBUTTON);
							if (i==4) i=12;
							if (i==7) i=0;

						}
						else if (DisplayedScreen>=RELAY_BOX && DisplayedScreen<=EXP_BOX_8)
						{
							int l=twidth;
							byte k;
							int rx;

							if (DisplayedScreen==RELAY_BOX)
							{
								TempRelay=Relay.RelayData;
								TempRelayOn=Relay.RelayMaskOn;
								TempRelayOff=Relay.RelayMaskOff;
							}
							else
							{
#ifdef RelayExp
								TempRelay=Relay.RelayDataE[DisplayedScreen-2];
								TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-2];
								TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-2];
#endif // RelayExp
							}
							TempRelay&=TempRelayOff;
							TempRelay|=TempRelayOn;

							if (NeedsRedraw)
							{
								NeedsRedraw=false;

								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RELAYBOXLABELBAR/DisplayedScreen,a*3),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
							}

							// Relay Ports Status
							j=36;
							k=l*5/16;
							rx=10;
							if (i==0) // Orientation is landscape
							{
								j=42;
								k=l/4;
							}
							for (int a=1;a<9;a++)
							{
								if (a==5)
								{
									j=36;
									k=l*11/16;
									if (i==0) // Orientation is landscape
									{
										j=42;
										k=l*3/4;
									}
									rx=l-40;
								}
								if (i==12) j+=52; else j+=34;
								eeprom_read_block((void*)&tempname, (void*)(R1Name+((a-1)*0x10)+((DisplayedScreen-1)*0x80)), sizeof(tempname));
								Font.SetColor(COLOR_WHITE,BKCOLOR,true);
								Font.DrawCenterText(k,j+3,tempname);
								SmallFont.SetColor(COLOR_YELLOW,BKCOLOR,true);
								if (bitRead(TempRelayOn,a-1) || !bitRead(TempRelayOff,a-1))
									SmallFont.DrawCenterTextP(k,j+18,LABEL_OVERRIDE);
								else
									TouchLCD.Clear(BKCOLOR,k-23,j+18,k+23,j+26);
								TouchLCD.DrawRelayStatus(rx,j,bitRead(TempRelay,a-1));
							}				
						}
#ifdef PWMEXPANSION
						else if(DisplayedScreen==PWM_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(PWMLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));

								for (int a=0;a<6;a++)
									PB[a].NeedsRedraw=true;
							}	
							
							// Progress Bars
							j=50+(i/2);
							Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								eeprom_read_block((void*)&tempname, (void*)(PWMChannel1+((a)*0x10)), sizeof(tempname));
								PB[a].SetPosition(10,j);
								PB[a].SetColor(COLOR_GREEN);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(PWM.GetChannelValue(a));
								PB[a].Show();
							}
						}
#endif // PWMEXPANSION
#ifdef RFEXPANSION
						else if(DisplayedScreen==RF_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RFLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));					
								j=60;
								Font.SetColor(COLOR_GOLD,BKCOLOR,false);
								j+=40+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_MODE);
								j+=45+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_SPEED);
								j+=45+(i*2);
								Font.DrawCenterTextP((twidth/2),j,LABEL_DURATION);
							}
							j=27;
							j+=40+(i*2);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							char temp[25];
							char rf_mode[35];
							strcpy_P(temp,(char * )pgm_read_word(&(rf_items[RF.Mode])));
							strcpy(rf_mode,"   ");
							strcat(rf_mode,temp);
							strcat(rf_mode,"   ");
							LargeFont.DrawCenterText((twidth/2),j,rf_mode);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Speed,0);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Duration,0);
						}
						else if(DisplayedScreen==RF_SCREEN1)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(RFLABELBAR1,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));					
								for (int a=0;a<6;a++)
									PB[a].NeedsRedraw=true;
							}
							// Progress Bars
							j=50+(i/2);
							int rfcolor[] = {COLOR_ORANGE,COLOR_ROYALBLUE,COLOR_RED,COLOR_GREEN,COLOR_LIGHTBLUE,COLOR_MAGENTA};
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								int ptr = pgm_read_word(&(LABEL_RF[a]));								
								strcpy_P(tempname, (char *)ptr);
								PB[a].SetPosition(10,j);
								PB[a].SetColor(rfcolor[a]);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(RF.GetChannel(a));
								PB[a].Show();
							}							

						}	
#endif // RFEXPANSION
#ifdef AI_LED
						else if(DisplayedScreen==AI_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(AILABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								for (int a=0;a<3;a++)
									PB[a].NeedsRedraw=true;
							}	
							
							// Progress Bars
							j=35;
							int aicolor[] = {COLOR_ORANGE,COLOR_LIGHTBLUE,COLOR_ROYALBLUE};
							for (int a=0;a<3;a++)
							{
								j+=40+(i*2);
								int ptr = pgm_read_word(&(LABEL_AI[a]));								
								strcpy_P(tempname, (char *)ptr);
								PB[a].SetPosition(10,j);
								PB[a].SetColor(aicolor[a]);
								PB[a].SetLabel(tempname);
								PB[a].SetCurrent(AI.GetChannel(a));
								PB[a].Show();							
							}
						}
#endif //  AI_LED
#ifdef IOEXPANSION
						else if(DisplayedScreen==IO_SCREEN)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);					
								//Gray Bar
								for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(IOLABELBAR,a*4),0,40+a,twidth,40+a);
								LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));
								LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
								LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-1])));	
								Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
								j=50+(i/2);
								for (int a=0;a<6;a++)
								{
									j+=22+i;
									Font.DrawCenterTextP((twidth/2)-10,j+5,LABEL_IOPORT);
									Font.DrawText(a);
								}								
							}
							j=50+(i/2);
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								int iocolor;
								if (IO.GetChannel(a))
									TouchLCD.DrawBMP(twidth/6,j+3,GREENBUTTON);
								else
									TouchLCD.DrawBMP(twidth/6,j+3,REDBUTTON);
							}								

						}
#endif //  IOEXPANSION
						else if(DisplayedScreen==DIMMING_OVERRIDE)
						{
							if (NeedsRedraw)
							{
								NeedsRedraw=false;
								TouchLCD.Clear(COLOR_WHITE,0,28,twidth,theight);	
								Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
								Font.DrawTextP(10,105,PWM_OVERRIDE_LABEL1);
								Font.DrawTextP(10,117,PWM_OVERRIDE_LABEL2);
								OkButton.SetPosition(twidth/2-40,theight*5/9);
								OkButton.Show();
								CancelButton.SetPosition(twidth/2-65,theight*7/9);
								CancelButton.Show();
								Slider.Show();
//									Slider.Refresh();
							}
						}
					}
				}
				else // if touched
				{
					// Check for Soft Reset of screen
					// If top left corner is long touched, we reinitialize the LCD. 
					if (TS.X<50 && TS.Y<50) // top left corner
					{
						if (LongTouch++==255) // 255 count is about 6 seconds
						{	
							LongTouch=0;
							NeedsRedraw=true;
							TouchLCD.Init();
						}
					}
					if (TouchEnabled && Sleeping)
					{
						TouchEnabled=false;
						Sleeping=false;
					}
					else
					{
						if (TouchEnabled && DisplayedScreen<MAX_SCREENS)
						{
							int j;
							TouchEnabled=false;
							if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
								ChangeDisplayedScreen(-1);
							if (TS.X>twidth-50 && TS.Y>theight-30)
								ChangeDisplayedScreen(1);
							if (TS.X<twidth-80 && TS.X>80 && TS.Y>theight-30)
								ShowTouchMenu();
							if(DisplayedScreen==MAIN_SCREEN)
							{
								if (orientation%2==0)
								{
									//landscape
									if (numexp>0)
										j=127;
									else
										j=105;
								}
								else
								{
									//portrait
									if (numexp>2)
										j=187;
									else
										j=172;
										
								}
								if (TS.IsTouchedInside(0,j-5,twidth,j+25))
								{
									RecallScreen=DisplayedScreen;
									DisplayedScreen=DIMMING_OVERRIDE;
									NeedsRedraw=true;
									Slider.SetColor(COLOR_ORANGE);							
									Slider.SetCurrent(PWM.GetDaylightValue());
									Slider.SetOverrideID(OVERRIDE_DAYLIGHT);
									Slider.SetLabel("Daylight");
								}
								j+=35;
								if (TS.IsTouchedInside(0,j-5,twidth,j+25))
								{
									RecallScreen=DisplayedScreen;
									NeedsRedraw=true;
									DisplayedScreen=DIMMING_OVERRIDE;
									Slider.SetColor(COLOR_ROYALBLUE);
									Slider.SetCurrent(PWM.GetActinicValue());
									Slider.SetOverrideID(OVERRIDE_ACTINIC);
									Slider.SetLabel("Actinic");
								}
							}
							else if (DisplayedScreen>=RELAY_BOX && DisplayedScreen<=EXP_BOX_8)
							{
								int k,l,x,x1,h;
								
								if (DisplayedScreen==RELAY_BOX)
								{
									TempRelay=Relay.RelayData;
									TempRelayOn=Relay.RelayMaskOn;
									TempRelayOff=Relay.RelayMaskOff;
								}
								else
								{
#ifdef RelayExp
									TempRelay=Relay.RelayDataE[DisplayedScreen-2];
									TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-2];
									TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-2];
#endif // RelayExp
								}
								TempRelay&=TempRelayOff;
								TempRelay|=TempRelayOn;
								
								if (orientation%2==0) // Orientation is landscape
								{
									j=53;
									l=34;
									h=16;
								}
								else
								{
									j=43;
									l=52;
									h=24;
								}
								k=j;
								x=0;
								x1=45;
								for (int a=1;a<9;a++)
								{

									if (a==5)
									{
										k=j;
										x=twidth-45;
										x1=twidth-118;
									}
									k+=l;
									if (TS.IsTouchedInside(x,k-h,x+45,k+h))
									{
										if (bitRead(TempRelayOn,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,0);
#endif // REEFTOUCHDISPLAY
											Relay.Override(a+(DisplayedScreen-1)*10,0);
										}
										else if (!bitRead(TempRelayOff,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,1);
#endif // REEFTOUCHDISPLAY
											Relay.Override(a+(DisplayedScreen-1)*10,1);
										}
										else if (!bitRead(TempRelayOn,a-1) && bitRead(TempRelayOff,a-1))
										{
#ifdef REEFTOUCHDISPLAY
											SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,!bitRead(TempRelay,a-1));
#endif // REEFTOUCHDISPLAY
											if (bitRead(TempRelay,a-1))
											{
												Relay.Override(a+(DisplayedScreen-1)*10,0);
											}
											else
											{
												Relay.Override(a+(DisplayedScreen-1)*10,1);
											}
										}
									}
									if (TS.IsTouchedInside(x1,k-h,x1+74,k+h))
									{
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-1)*10,2);
#endif // REEFTOUCHDISPLAY
										Relay.Override(a+(DisplayedScreen-1)*10,2);
									}
								}										
							}
#ifdef PWMEXPANSION
							else if (DisplayedScreen==PWM_SCREEN)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=60;
									h=10;
									k=22;
								}
								else
								{
									//portrait
									j=65;
									h=15;
									k=34;
								}									
								for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(COLOR_GREEN);
										Slider.SetCurrent(PWM.GetChannelValue(a));
										Slider.SetOverrideID(OVERRIDE_CHANNEL0+a);
										eeprom_read_block((void*)&tempname, (void*)(PWMChannel1+((a)*0x10)), sizeof(tempname));
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // PWMEXPANSION
#ifdef AI_LED
							else if (DisplayedScreen==AI_SCREEN)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=45;
									h=17;
									k=40;
								}
								else
								{
									//portrait
									j=45;
									h=20;
									k=64;
								}									
								int aicolor[] = {COLOR_ORANGE,COLOR_LIGHTBLUE,COLOR_ROYALBLUE};
								for (int a=0;a<AI_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(aicolor[a]);
										Slider.SetCurrent(AI.GetChannel(a));
										Slider.SetOverrideID(OVERRIDE_AI_WHITE+a);
										int ptr = pgm_read_word(&(LABEL_AI[a]));								
										strcpy_P(tempname, (char *)ptr);
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // AI_LED
#ifdef RFEXPANSION
							else if (DisplayedScreen==RF_SCREEN1)
							{
								int j,h,k;
								if (orientation%2==0)
								{
									//landscape
									j=60;
									h=10;
									k=22;
								}
								else
								{
									//portrait
									j=65;
									h=15;
									k=34;
								}									
								int rfcolor[] = {COLOR_ORANGE,COLOR_ROYALBLUE,COLOR_RED,COLOR_GREEN,COLOR_LIGHTBLUE,COLOR_MAGENTA};
								for (int a=0;a<RF_CHANNELS;a++)
								{
									j+=k;
									if (TS.IsTouchedInside(0,j-h,twidth,j+h))
									{
										RecallScreen=DisplayedScreen;
										NeedsRedraw=true;
										DisplayedScreen=DIMMING_OVERRIDE;
										Slider.SetColor(rfcolor[a]);
										Slider.SetCurrent(RF.GetChannel(a));
										Slider.SetOverrideID(OVERRIDE_RF_WHITE+a);
										int ptr = pgm_read_word(&(LABEL_RF[a]));								
										strcpy_P(tempname, (char *)ptr);
										Slider.SetLabel(tempname);
									}
								}
							}
#endif // RFEXPANSION
						}
						else
						{
							// if we are displaying overrides, refresh sliders
							if(DisplayedScreen==DIMMING_OVERRIDE)
							{
								if (TouchEnabled)
								{
									TouchEnabled=false;
									bool bDone=false;
									TS.GetTouch();
									byte oid=Slider.GetOverrideID();
									byte ovalue=Slider.GetCurrent();
									if (OkButton.IsPressed()) 
									{
										bDone=true;
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,ovalue); 	// Send Override Request
#endif // REEFTOUCHDISPLAY
										if (oid<=OVERRIDE_CHANNEL5)
											ReefAngel.PWM.Override(oid,ovalue);
#ifdef AI_LED
										if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_BLUE)
											ReefAngel.AI.Override(oid-OVERRIDE_AI_WHITE,ovalue);
#endif // AI_LED
#ifdef RFEXPANSION
										if (oid>=OVERRIDE_RF_WHITE && oid<=OVERRIDE_RF_INTENSITY)
											ReefAngel.RF.Override(oid-OVERRIDE_RF_WHITE,ovalue);
#endif // RFEXPANSION
									}
									if (CancelButton.IsPressed()) 
									{
										bDone=true;
#ifdef REEFTOUCHDISPLAY
										SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,255); 	// Send Cancel Override Request
#endif // REEFTOUCHDISPLAY
										if (oid<=OVERRIDE_CHANNEL5)
											ReefAngel.PWM.Override(oid,255);
#ifdef AI_LED
										if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_BLUE)
											ReefAngel.AI.Override(oid-OVERRIDE_AI_WHITE,255);
#endif // AI_LED
#ifdef RFEXPANSION
										if (oid>=OVERRIDE_RF_WHITE && oid<=OVERRIDE_RF_INTENSITY)
											ReefAngel.RF.Override(oid-OVERRIDE_RF_WHITE,255);
#endif // RFEXPANSION
									}
									if (bDone)
									{
										TouchEnabled=false;
										DisplayedScreen=RecallScreen;
										NeedsRedraw=true;
									}
								}
								Slider.Refresh();
							}
						}
					}
				}
			}	
#endif //  CUSTOM_MAIN
			
			wdt_reset();
			if (TS.IsTouched()) ResetScreenSaver();
			break;
		}  // DEFAULT_MENU
		case FEEDING_MODE:
		{
			int t;
			byte y;
			bool bDone = false;
			t = Timer[FEEDING_TIMER].Trigger - now();
			if ( (t >= 0) && ! Timer[FEEDING_TIMER].IsTriggered() )
			{
				if (orientation%2==0) y=0; else y=40;
				wdt_reset();
				if (NeedsRedraw)
				{
					delay(50);
					TouchLCD.FullClear(BKCOLOR);
					TouchLCD.SetBacklight(0);
					if (orientation%2==0)
					{
						if (SDFound) TouchLCD.DrawSDRawImage("feed_l.raw",0,80,320,160);
					}
					else
					{
						if (SDFound) TouchLCD.DrawSDRawImage("feed_p.raw",0,160,240,160);
					}
					TouchLCD.SetBacklight(100);
					LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
					LargeFont.DrawCenterTextP(twidth/2, 10+y, FEEDING_LABEL);	
					NeedsRedraw=false;
				}
				LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
				LargeFont.DrawCenterNumber(twidth/2, 40+y, t, 0);
			}
			else
			{
				bDone = true;
			}

			LastStart = now();  // Set the time normal mode is started
			if ( TS.IsTouched() )
			{
				// screen was touched, so we stop the feeding mode
				bDone = true;
				TouchEnabled=false;
			}
			if ( bDone )
			{
				// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
				Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

				// turn on ports
				Relay.RelayMaskOff |= FeedingModePorts;
				// Compare the delayed on ports with the previous port states
				Relay.RelayData &= ~(FeedingModePorts & DelayedOnPorts);
#ifdef RelayExp
				for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
				{
					Relay.RelayMaskOffE[i] |= FeedingModePortsE[i];
					Relay.RelayDataE[i] &= ~(FeedingModePortsE[i] & DelayedOnPortsE[i]);
				}
#endif  // RelayExp
#ifdef RFEXPANSION
				RF.SetMode(Feeding_Stop,0,0);
#endif  // RFEXPANSION
				NeedsRedraw=true;
#ifdef REEFTOUCHDISPLAY
				SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // REEFTOUCHDISPLAY
				ExitMenu();
			}
//				Relay.Write();
			break;
		}
		case WATERCHANGE_MODE:
		{
			byte y;
			bool bDone = false;
			if (orientation%2==0) y=0; else y=40;
			wdt_reset();
			if (NeedsRedraw)
			{
				TouchLCD.FullClear(BKCOLOR);
				TouchLCD.SetBacklight(0);
				if (orientation%2==0)
				{
					if (SDFound) TouchLCD.DrawSDRawImage("water_l.raw",0,40,320,200);
				}
				else
				{
					if (SDFound) TouchLCD.DrawSDRawImage("water_p.raw",0,170,240,150);
				}
				TouchLCD.SetBacklight(100);
				NeedsRedraw=false;
			}
			LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
			LargeFont.DrawCenterTextP(twidth/2, 10+y, WATER_CHANGE_LABEL);
			LastStart = now();  // Set the time normal mode is started
			if ( TS.IsTouched() )
			{
				// screen was touched, so we stop the feeding mode
				bDone = true;
				TouchEnabled=false;
			}
			if ( bDone )
			{
				// we're finished, so let's clear the screen and return
#ifdef SaveRelayState
				Relay.RelayData = CurrentRelayState;
#endif  // SaveRelayState

				// turn on ports
				Relay.RelayMaskOff |= WaterChangePorts;
				// Compare the delayed on ports with the previous port states
				Relay.RelayData &= ~(WaterChangePorts & DelayedOnPorts);
#ifdef RelayExp
				for ( byte i = 0; i < MAX_RELAY_EXPANSION_MODULES; i++ )
				{
					Relay.RelayMaskOffE[i] |= WaterChangePortsE[i];
					Relay.RelayDataE[i] &= ~(WaterChangePortsE[i] & DelayedOnPortsE[i]);
				}
#endif  // RelayExp
				NeedsRedraw=true;
#ifdef REEFTOUCHDISPLAY
				SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // REEFTOUCHDISPLAY
				ExitMenu();
			}
//				Relay.Write();
			break;
		}
		case TOUCH_MENU:
		{
			int ch;
			int border;
			if (orientation%2==0)
			{
				ch=35;
				border=0;
			}
			else
			{
				ch=48;
				border=2;
			}
			wdt_reset();
			if (!TS.IsTouched())
			{
				TouchEnabled=true;
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					TouchLCD.Clear(COLOR_WHITE,0,0,twidth,theight); // Clear screen
					//Arrows
					TouchLCD.DrawBMP(10,theight-25,ARROWLEFT);
					TouchLCD.DrawBMP(twidth-10-23,theight-25,ARROWRIGHT);
					Font.SetColor(RGB565(0xBD, 0x13, 0x00),COLOR_WHITE,true);
					switch ( DisplayedScreen )
					{
						case MAIN_MENU_SCREEN:
						{
							for (int a=0;a<6;a++)
							{
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),5,6+(ch*a),(twidth/2)-6,ch*(a+1),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,5+border,6+border+(ch*a),(twidth/2)-6-border,(ch*(a+1))-border,4,false);
								if (a==4)
								{
									if (bitRead(Flags,LightsOnFlag))
										Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),MENU_BUTTON_CANCEL);
									else
										Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
								}
								else
								{
									Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
								}
								Font.DrawCenterTextP(twidth/4,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[(a*2)+1])));
			
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,6+(ch*a),twidth-6,ch*(a+1),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+5+border,6+border+(ch*a),twidth-6-border,(ch*(a+1))-border,4,false);
								Font.DrawCenterTextP(twidth/4*3,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[a*2])));
								Font.DrawCenterTextP(twidth/4*3,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[(a*2)+1])));
							}
							break;
						}
						case SETUP_MENU_SCREEN:
						{
							for (int a=0;a<6;a++)
							{
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),5,6+(ch*a),(twidth/2)-6,ch*(a+1),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,5+border,6+border+(ch*a),(twidth/2)-6-border,(ch*(a+1))-border,4,false);
								Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items3[a*2])));
								Font.DrawCenterTextP(twidth/4,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items3[(a*2)+1])));
			
								TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,6+(ch*a),twidth-6,ch*(a+1),4,true);
								TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+5+border,6+border+(ch*a),twidth-6-border,(ch*(a+1))-border,4,false);
								Font.DrawCenterTextP(twidth/4*3,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items4[a*2])));
								Font.DrawCenterTextP(twidth/4*3,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items4[(a*2)+1])));
							}
							break;							
						}
					}
				}
				if (MenuFunctionPtr!=&ReefAngelClass::Touch) // check to see if any menu choice was touched
				{
					(this->*MenuFunctionPtr)(); // call function
					NeedsRedraw=true;
					MenuFunctionPtr=&ReefAngelClass::Touch; // put pointer back to default
				}
			}
			else
			{
				menutimeout=now();
				if (TouchEnabled && DisplayedScreen<MAX_SCREENS)
				{
					TouchEnabled=false;
					if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
						ChangeDisplayedScreen(-1);
					if (TS.X>twidth-50 && TS.Y>theight-30)
						ChangeDisplayedScreen(1);
					switch ( DisplayedScreen )
					{
						case MAIN_MENU_SCREEN:
						{
							for (int a=0;a<6;a++)
							{
								if (TS.IsTouchedInside(0,5+(47*a),twidth/2,5+ch+(47*a)))
									MenuFunctionPtr=menu_button_functions1[a]; // Assing function pointers when buttons are touched
								if (TS.IsTouchedInside(twidth/2,5+(47*a),twidth,5+ch+(47*a)))
									MenuFunctionPtr=menu_button_functions2[a]; // Assing function pointers when buttons are touched
							}
							break;
						}
					}
				}
			}
			CheckMenuTimeout();
			break;
		}
		case DATE_TIME_MENU:
		{
			int y;
			byte mo,d,yr,h,mi;

			mo=month(newnow);
			d=day(newnow);
			yr=year(newnow)-2000;
			h=hour(newnow);
			mi=minute(newnow);

			if (NeedsRedraw)
			{
		    	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
		    	y=(theight/4);
		    	TouchLCD.DrawSetupDateTime(twidth/6,y,mo,LargeFont);
		    	TouchLCD.DrawSetupDateTime(twidth/2,y,d,LargeFont);
		    	TouchLCD.DrawSetupDateTime(twidth*5/6,y,yr,LargeFont);

		    	y=(theight*3/5);
		    	TouchLCD.DrawSetupDateTime(twidth/2,y,mi,LargeFont);
		    	if (h>12)
		    	{
			    	TouchLCD.DrawSetupDateTime(twidth/6,y,h-12,LargeFont);
		    	}
		    	else
		    	{
		    		if (hour(newnow)==0) h=12;
			    	TouchLCD.DrawSetupDateTime(twidth/6,y,h,LargeFont);
		    	}
		    	if (h<12 || hour(newnow)==0)
		    		TouchLCD.DrawSetupDateTime(twidth*5/6,y,"AM",LargeFont);
		    	else
		    		TouchLCD.DrawSetupDateTime(twidth*5/6,y,"PM",LargeFont);

		    	NeedsRedraw=false;
		    	menutimeout=now();
			}
			if (TS.IsTouched())
			{
				TimeElements tme;
				breakTime(newnow,tme);
				y=(theight/4)-23;
//				TouchLCD.Clear(COLOR_RED,twidth/6-27,y,twidth/6+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth/2-27,y,twidth/2+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth*5/6-27,y,twidth*5/6+27,y+25);
				if (TS.IsTouchedInside(twidth/6-27,y,twidth/6+27,y+25)) { if (++tme.Month==13) tme.Month=1; if (tme.Day>monthDays[tme.Month-1]) tme.Day=monthDays[tme.Month-1];}
				if (TS.IsTouchedInside(twidth/2-27,y,twidth/2+27,y+25))	{ if (++tme.Day>monthDays[tme.Month-1]) tme.Day=1; }
				if (TS.IsTouchedInside(twidth*5/6-27,y,twidth*5/6+27,y+25)) { if (++tme.Year==130) tme.Year=30; }
				y=(theight/4)+32;
//				TouchLCD.Clear(COLOR_RED,twidth/6-27,y,twidth/6+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth/2-27,y,twidth/2+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth*5/6-27,y,twidth*5/6+27,y+25);
				if (TS.IsTouchedInside(twidth/6-27,y,twidth/6+27,y+25))	{ if (--tme.Month==0) tme.Month=12; if (tme.Day>monthDays[tme.Month-1]) tme.Day=monthDays[tme.Month-1];}
				if (TS.IsTouchedInside(twidth/2-27,y,twidth/2+27,y+25))	{ if (--tme.Day==0) tme.Day=monthDays[tme.Month-1]; }
				if (TS.IsTouchedInside(twidth*5/6-27,y,twidth*5/6+27,y+25)) { if (--tme.Year==29) tme.Year=129; }

		    	y=(theight*3/5)-23;
//				TouchLCD.Clear(COLOR_RED,twidth/6-27,y,twidth/6+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth/2-27,y,twidth/2+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth*5/6-27,y,twidth*5/6+27,y+25);
				if (TS.IsTouchedInside(twidth/6-27,y,twidth/6+27,y+25))	{ if (++tme.Hour>23) tme.Hour=0; }
				if (TS.IsTouchedInside(twidth/2-27,y,twidth/2+27,y+25)) { if (++tme.Minute==60) tme.Minute=0; }
				if (TS.IsTouchedInside(twidth*5/6-27,y,twidth*5/6+27,y+25)) { tme.Hour+=isPM(newnow)?-12:12; }
		    	y=(theight*3/5)+32;
//				TouchLCD.Clear(COLOR_RED,twidth/6-27,y,twidth/6+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth/2-27,y,twidth/2+27,y+25);
//				TouchLCD.Clear(COLOR_RED,twidth*5/6-27,y,twidth*5/6+27,y+25);
				if (TS.IsTouchedInside(twidth/6-27,y,twidth/6+27,y+25))	{ if (--tme.Hour==255) tme.Hour=23; }
				if (TS.IsTouchedInside(twidth/2-27,y,twidth/2+27,y+25)) { if (--tme.Minute==255) tme.Minute=59; }
				if (TS.IsTouchedInside(twidth*5/6-27,y,twidth*5/6+27,y+25)) { tme.Hour+=isPM(newnow)?-12:12; }
				newnow=makeTime(tme);
		    	menutimeout=now();
		    	NeedsRedraw=true;

				if (CancelButton.IsPressed())
					ShowTouchMenu();
				if (OkButton.IsPressed())
				{
					setTime(newnow);
					now();
					RTC.set(now());
				}
			}
			CheckMenuTimeout();
			break;
		}
		case PH_CALIBRATE_MENU:
		{
			unsigned long t=newnow-now();
			unsigned long p=0;

			if (!TS.IsTouched()) TouchEnabled=true;
			if (TouchEnabled && TS.IsTouched())
			{
				if (CancelButton.IsPressed())
					ShowTouchMenu();
			}
			switch(CalStep)
			{
			case 0:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						OkButton.Hide(COLOR_WHITE);
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI4);
						Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI5);
						newnow=now()+CALIBRATION_TIMER;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 1:
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight/2,t,0);
				if (t==0)
				{
					Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
					for (int a=0;a<60;a++)
					{
						p+=analogRead(PHPin);
						delay(50);
						wdt_reset();
					}
					p/=60;
					CalVal1=p;
					CalStep++;
				}
				break;
			case 2:
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI7);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI8);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI9);
				Font.SetColor(COLOR_RED, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight*5/12,CalVal1,0);
				OkButton.Show();
				CalStep++;
				break;
			case 3:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI10);
						Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI11);
						Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI3);
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 4:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
						Font.DrawCenterTextP(twidth/2- 15,theight*5/12,MENU_BUTTON_PH);
						Font.DrawCenterNumber(twidth/2+ 15,theight*5/12,10,1);
						Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
						Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 5:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						OkButton.Hide(COLOR_WHITE);
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI4);
						Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI5);
						newnow=now()+CALIBRATION_TIMER;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 6:
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight/2,t,0);
				if (t==0)
				{
					Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
					for (int a=0;a<60;a++)
					{
						p+=analogRead(PHPin);
						delay(50);
						wdt_reset();
					}
					p/=60;
					CalVal2=p;
					CalStep++;
				}
				break;
			case 7:
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI7);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI8);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI9);
				Font.SetColor(COLOR_RED, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight*5/12,CalVal2,0);
				OkButton.Show();
				CalStep++;
				break;
			case 8:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI12);
						Font.DrawTextP(twidth/4,theight/2,MENU_BUTTON_PH);
						Font.DrawText(twidth/4+ 20,theight/2,7);
						Font.DrawTextP(twidth/4,theight*7/12,MENU_BUTTON_PH);
						Font.DrawText(twidth/4+ 20,theight*7/12,10);
						Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
						Font.SetColor(COLOR_RED, COLOR_WHITE,false);
						Font.DrawText(twidth*3/4,theight/2,CalVal1);
						Font.DrawText(twidth*3/4,theight*7/12,CalVal2);
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 9:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				        InternalMemory.PHMin_write(CalVal1);
				        PHMin = CalVal1;
				        InternalMemory.PHMax_write(CalVal2);
						PHMax = CalVal2;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 10:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
						ShowTouchMenu();
					TouchEnabled=false;
				}
				break;
			}
			break;
		}
		case SAL_CALIBRATE_MENU:
		{
			if (!TS.IsTouched()) TouchEnabled=true;
			if (TouchEnabled && TS.IsTouched())
			{
				if (CancelButton.IsPressed())
					ShowTouchMenu();
			}
#ifdef SALINITYEXPANSION
			unsigned long t=newnow-now();
			unsigned long p=0;

			switch(CalStep)
			{
			case 0:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						OkButton.Hide(COLOR_WHITE);
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI4);
						Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI5);
						newnow=now()+CALIBRATION_TIMER;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 1:
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight/2,t,0);
				if (t==0)
				{
					Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
					for (int a=0;a<60;a++)
					{
						p+=Salinity.Read();
						delay(50);
						wdt_reset();
					}
					p/=60;
					CalVal1=p;
					CalStep++;
				}
				break;
			case 2:
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI7);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI8);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI9);
				Font.SetColor(COLOR_RED, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight*5/12,CalVal1,0);
				OkButton.Show();
				CalStep++;
				break;
			case 3:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI12);
						Font.DrawText(twidth/4- 20,theight/2,35);
						Font.DrawTextP(twidth/4,theight/2,SAL_CALI2);
						Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
						Font.SetColor(COLOR_RED, COLOR_WHITE,false);
						Font.DrawText(twidth*3/4,theight/2,CalVal1);
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 4:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				        InternalMemory.SalMax_write(CalVal1);
						SalMax = CalVal1;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 5:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
						ShowTouchMenu();
					TouchEnabled=false;
				}
				break;
			}
#endif
			break;
		}
		case ORP_CALIBRATE_MENU:
		{
			if (!TS.IsTouched()) TouchEnabled=true;
			if (TouchEnabled && TS.IsTouched())
			{
				if (CancelButton.IsPressed())
					ShowTouchMenu();
			}
#ifdef ORPEXPANSION
			unsigned long t=newnow-now();
			unsigned long p=0;

			switch(CalStep)
			{
			case 0:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						OkButton.Hide(COLOR_WHITE);
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI4);
						Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI5);
						newnow=now()+CALIBRATION_TIMER;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 1:
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight/2,t,0);
				if (t==0)
				{
					Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
					for (int a=0;a<60;a++)
					{
						p+=ORP.Read();
						delay(50);
						wdt_reset();
					}
					p/=60;
					CalVal1=p;
					CalStep++;
				}
				break;
			case 2:
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI7);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI8);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI9);
				Font.SetColor(COLOR_RED, COLOR_WHITE,false);
				Font.DrawCenterNumber(twidth/2,theight*5/12,CalVal1,0);
				OkButton.Show();
				CalStep++;
				break;
			case 3:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI12);
						Font.DrawText(twidth/4- 27,theight/2,470);
						Font.DrawTextP(twidth/4,theight/2,ORP_CALI2);
						Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
						Font.SetColor(COLOR_RED, COLOR_WHITE,false);
						Font.DrawText(twidth*3/4,theight/2,CalVal1);
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 4:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
					{
						TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
						Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
						Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				        InternalMemory.ORPMax_write(CalVal1);
						ORPMax = CalVal1;
						CalStep++;
					}
					TouchEnabled=false;
				}
				break;
			case 5:
				if (TouchEnabled && TS.IsTouched())
				{
					if (OkButton.IsPressed())
						ShowTouchMenu();
					TouchEnabled=false;
				}
				break;
			}
#endif
			break;
		}
#ifdef CUSTOM_MENU
		case ALT_SCREEN_MODE:
		{
			/*
			for Custom Menus if displaying an alternate screen, to force the menu to be
			redrawn when exiting the screen.  Used primarily for SetupCalibratePH
			and with SetupDateTime.
			Or used when you draw your own screen and stay inside the function and want to
			return to the menu when your function exits
			If not used, then you will have to press the button 2 times to return to the
			main menu.
			*/
			redrawmenu = true;
			showmenu = true;
			SetDisplayedMenu(MAIN_MENU);
			break;
		}
		case RETURN_MAIN_MODE:
		{
			ExitMenu();
			break;
		}
#endif  // CUSTOM_MENU
		default:
		{
			// we are viewing another screen, primarily the DisplayMenuEntry screen
			if ( TS.IsTouched() )
			{
				// button is pressed, so we gotta exit out, show the menu & redraw it too
				redrawmenu = true;
				showmenu = true;
				Timer[FEEDING_TIMER].ForceTrigger();
				Timer[LCD_TIMER].Start();
			}
			// if displayed screen is less than 156, it means we need to redraw because we just came from a I2C receive interrupt
			if (DisplayedMenu<156)
			{
				DisplayedMenu+=100;
				NeedsRedraw=true;
			}
			break;
		}
	}  // switch DisplayedMenu
}

ButtonClass::ButtonClass()
{
	x1=0;
	y1=0;
	Ptr=0;
}

void ButtonClass::Create(int icolor, int itextcolor, char *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void ButtonClass::Create(int icolor, int itextcolor, char *istr, const prog_uchar *iPtr)
{
	Ptr=iPtr;
	Create(icolor,itextcolor,istr);
}

void ButtonClass::Show()
{
	visible=true;
	ReefAngel.Font.SetColor(COLOR_GRAY,COLOR_WHITE,true);
	ReefAngel.Font.DrawText(x1+26,y1+9,str); // This drawing is just to calculate the width of str
	x2=ReefAngel.Font.GetX()+25; // we add 25px in each side of str
//	ReefAngel.TouchLCD.DrawRoundRect(RGB565(0xD0, 0xD0, 0xD0),x1+1,y1+1,x2+1,y1+41,10,false);
	ReefAngel.TouchLCD.DrawRoundRect(COLOR_GRAY,x1,y1,x2,y1+26,8,false);
	ReefAngel.TouchLCD.DrawRoundRect(color,x1+1,y1+1,x2-1,y1+25,8,true);
	if (Ptr!=0)
		ReefAngel.TouchLCD.DrawBMP(x1+5,y1+5,Ptr);
	ReefAngel.Font.SetColor(COLOR_GRAY88,COLOR_WHITE,true);
	ReefAngel.Font.DrawText(x1+26,y1+9,str);
	ReefAngel.Font.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.Font.DrawText(x1+25,y1+8,str);
}

void ButtonClass::Hide(int bkcolor)
{
	visible=false;
	ReefAngel.TouchLCD.Clear(bkcolor,x1,y1,x2+1,y1+41);
}

boolean ButtonClass::IsPressed()
{
	return (ReefAngel.TS.X > x1 && ReefAngel.TS.X < x2 && ReefAngel.TS.Y > y1 && ReefAngel.TS.Y < y1+40);
}

ProgressBarClass::ProgressBarClass()
{
	min=0;
	max=100;
	current=0;
	x1=0;
	y1=0;
	NeedsRedraw=true;
}

void ProgressBarClass::Create(int icolor, int ibkcolor, int itextcolor, char *istr)
{
	color=icolor;
	bkcolor=ibkcolor;
	textcolor=itextcolor;
	str=istr;
}

void ProgressBarClass::SetCurrent(int value)
{
	if (current!=value) NeedsRedraw=true;
	current=value;
}

void ProgressBarClass::Show()
{
	if (NeedsRedraw)
	{
		visible=true;
		NeedsRedraw=false;
		ReefAngel.TouchLCD.Clear(bkcolor,x1,y1,130,y1+20);
		for (int a=0;a<10;a++) ReefAngel.TouchLCD.DrawPixel(COLOR_RED,x1+68,y1+(a*2));
		ReefAngel.Font.SetColor(textcolor,bkcolor,false);
		ReefAngel.Font.DrawText(x1+5,y1+5,str);
		ReefAngel.Font.DrawText(x1+75,y1+5,current);
		ReefAngel.Font.DrawText("%   ");
		for (int a=0;a<(ReefAngel.TouchLCD.GetWidth()-139)*current/100;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-139)),130+a,y1,130+a,y1+20);
		ReefAngel.TouchLCD.Clear(BKCOLOR,((ReefAngel.TouchLCD.GetWidth()-139)*current/100)+130,y1,ReefAngel.TouchLCD.GetWidth(),y1+20);
	}
}

void ProgressBarClass::Hide()
{
	visible=false;
	ReefAngel.TouchLCD.Clear(BKCOLOR,x1,y1,ReefAngel.TouchLCD.GetWidth()-10,y1+20);
}

boolean ProgressBarClass::IsPressed()
{
	return (ReefAngel.TS.X > x1 && ReefAngel.TS.Y > y1 && ReefAngel.TS.Y < y1+20);
}

SliderClass::SliderClass()
{
	min=0;
	max=100;
	current=0;
	x1=0;
	y1=0;
	NeedsRedraw=true;
}

void SliderClass::Create(int icolor, int itextcolor, char *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void SliderClass::DrawMarker()
{
	int currentX=map(current,min,max,41,ReefAngel.TouchLCD.GetWidth()-41);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX-2,y1+31,currentX-2,y1+32);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX-2,y1+59,currentX-2,y1+60);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX+2,y1+31,currentX+2,y1+32);
//	ReefAngel.TouchLCD.Clear(BKCOLOR,currentX+2,y1+59,currentX+2,y1+60);
//	for (int a=currentX-2;a<=currentX+2;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,BKCOLOR,((a-40)*100)/(ReefAngel.TouchLCD.GetWidth()-80)),a,y1+33,a,y1+58);
	for (int a=0;a<ReefAngel.TouchLCD.GetWidth()-80;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-80)),40+a,y1+33,40+a,y1+58);
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,0,y1+31,ReefAngel.TouchLCD.GetWidth(),y1+32);
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,0,y1+59,ReefAngel.TouchLCD.GetWidth(),y1+60);
	ReefAngel.TouchLCD.Clear(COLOR_GRAY,currentX-1,y1+31,currentX+1,y1+60);
	ReefAngel.TouchLCD.Clear(COLOR_RED,currentX,y1+32,currentX,y1+59);
	ReefAngel.LargeFont.SetColor(textcolor,COLOR_WHITE,false);
	char c[10];
	char temp[10];
	itoa(current,temp,10);
	strcpy(c," ");
	strcat(c,temp);
	strcat(c," ");
	ReefAngel.LargeFont.DrawCenterText(ReefAngel.TouchLCD.GetWidth()/2,y1,c);
}

void SliderClass::Show()
{
	visible=true;
	NeedsRedraw=false;
	ReefAngel.TouchLCD.Clear(COLOR_WHITE,40,y1,ReefAngel.TouchLCD.GetWidth()-40,y1+75);
	ReefAngel.Font.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.Font.DrawCenterText(ReefAngel.TouchLCD.GetWidth()/2,y1+63,str);
	for (int a=0;a<ReefAngel.TouchLCD.GetWidth()-80;a++) ReefAngel.TouchLCD.DrawLine(alphaBlend(color,COLOR_WHITE,(a*100)/(ReefAngel.TouchLCD.GetWidth()-80)),40+a,y1+33,40+a,y1+58);
	ReefAngel.LargeFont.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.LargeFont.DrawCenterNumber(ReefAngel.TouchLCD.GetWidth()/2,y1,current,0);
	ReefAngel.TouchLCD.DrawBMP(10,y1+33,MINUS);
	ReefAngel.TouchLCD.DrawBMP(ReefAngel.TouchLCD.GetWidth()-35,y1+33,PLUS);
	DrawMarker();
}

void SliderClass::Hide()
{
	visible=false;
	ReefAngel.TouchLCD.Clear(BKCOLOR,0,y1,ReefAngel.TouchLCD.GetWidth(),y1+72);
}

boolean SliderClass::Refresh()
{
    wdt_reset();
	if (IsPlusPressed())
	{
		current++;
		if (current>max) current=max;
		DrawMarker();
		delay(100);
	}
	if (IsMinusPressed())
	{
		current--;
		if (current<min) current=min;
		DrawMarker();
		delay(100);
	}
	if (ReefAngel.TS.X > 40 && ReefAngel.TS.X < (ReefAngel.TouchLCD.GetWidth()-40) && ReefAngel.TS.Y > y1+25 && ReefAngel.TS.Y < y1+60)
	{
		current=map(ReefAngel.TS.X,40,ReefAngel.TouchLCD.GetWidth()-40,min,max);
		DrawMarker();
	}
	if (NeedsRedraw)
	{
		Show();
	}
}

boolean SliderClass::IsPlusPressed()
{
	return (ReefAngel.TS.X > (ReefAngel.TouchLCD.GetWidth()-35) && ReefAngel.TS.Y > y1+28 && ReefAngel.TS.Y < y1+63);
}

boolean SliderClass::IsMinusPressed()
{
	return (ReefAngel.TS.X < 35 && ReefAngel.TS.Y > y1+28 && ReefAngel.TS.Y < y1+63);
}
