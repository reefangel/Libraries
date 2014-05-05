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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
			do wdt_reset(); while (!TS.IsTouched());
#elif defined(__SAM3X8E__)
			do WDT_Restart( WDT ); while (!TS.IsTouched());
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
			delay(100);
			TS.GetTouch();
			delay(100);
			TS.GetTouch();
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
	}
	TS.SaveCalibration();
	TouchLCD.Clear(COLOR_WHITE,0,180,239,270);
	OkButton.SetPosition(78,200);
	OkButton.Show();
	do
	{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
		TS.GetTouch();
	}
	while(!OkButton.IsPressed());
	OkButton.Hide();
	TouchLCD.FullClear(COLOR_WHITE);
}

void ReefAngelClass::InitCustomLabels()
{
	for (int a=1;a<=8;a++)
		CustomLabels[a-1]=String("Port " + String(a));
#ifdef RelayExp
	for (int a=1;a<=MAX_RELAY_EXPANSION_MODULES;a++)
		for (int b=1;b<=8;b++)
			CustomLabels[(a*8)+b-1]=String("Port "+ String(a) + String(b));
	for (int a=1;a<=3;a++)
		CustomLabels[Temp1Label+a-1]=String("Temp " + String(a));
	CustomLabels[PHLabel]="pH";
	CustomLabels[ATOLowLabel]="ATO Low";
	CustomLabels[ATOHighLabel]="ATO High";
	CustomLabels[AlarmLabel]="Alarm";
	CustomLabels[DaylightLabel]="Daylight";
	CustomLabels[ActinicLabel]="Actinic";
	CustomLabels[Daylight2Label]="Daylight 2";
	CustomLabels[Actinic2Label]="Actinic 2";
	for (int a=0;a<PWM_EXPANSION_CHANNELS;a++)
		CustomLabels[DimmingChannel0Label+a]=String("Channel " + String(a));
	for (int a=0;a<IO_EXPANSION_CHANNELS;a++)
		CustomLabels[IOChannel0Label+a]=String("I/O Channel " + String(a));
	CustomLabels[SalinityLabel]="Salinity";
	CustomLabels[ORPLabel]="ORP";
	CustomLabels[PHExpLabel]="pH Exp";
	for (int a=0;a<5;a++)
		CustomLabels[WaterLevelLabel+a]=String("WL " + String(a));
	CustomLabels[HumidityLabel]="Humidity";
	CustomLabels[LeakLabel]="Leak";
	for (int a=0; a<8; a++)
		CustomLabels[CustomVar0Label+a]=String("Cvar " + String(a));
#endif // RelayExp
}

void ReefAngelClass::ChangeDisplayedScreen(signed char index)
{
	NeedsRedraw=true;
	DisplayedScreen+=index;
	if (DisplayedMenu==TOUCH_MENU)
	{
		if (DisplayedScreen<0) DisplayedScreen=MAX_MENU_SCREENS-1;
		if (DisplayedScreen>=MAX_MENU_SCREENS) DisplayedScreen=0;
	}
	else
	{
		int cvarcheck=0;
		if (DisplayedScreen<0) DisplayedScreen=MAX_SCREENS-1;
		if (Board==RAStar && DisplayedScreen==RELAY_BOX) DisplayedScreen+=index;
#ifdef CUSTOM_VARIABLES
		for ( byte EID = 0; EID < 8; EID++ )
			cvarcheck+=CustomVar[EID];
#endif //CUSTOM_VARIABLES
		
		if (DisplayedScreen>RELAY_BOX)
			if (index>0)
			{
				for (int a=DisplayedScreen;a<MAX_SCREENS;a++)
				{
					if (a<12)
					{
						if (bitRead(REM,a-3)) break;
					}
					else
					{
						if (a==11 && bitRead(EM,0)) break;
						if (a==12 && bitRead(EM,1)) break;
						if (a==13 && bitRead(EM,1)) break;
						if (a==14 && bitRead(EM,2)) break;
						if (a==15 && bitRead(EM,5)) break;
						if (a==16 && bitRead(EM1,1)) break;
						if (a==17 && cvarcheck) break;
					}
					DisplayedScreen++;
				}
			}
			else
			{
				for (int a=DisplayedScreen;a>=3;a--)
				{
					if (a<12)
					{
						if (bitRead(REM,a-3)) break;
					}
					else
					{
						if (a==11 && bitRead(EM,0)) break;
						if (a==12 && bitRead(EM,1)) break;
						if (a==13 && bitRead(EM,1)) break;
						if (a==14 && bitRead(EM,2)) break;
						if (a==15 && bitRead(EM,5)) break;
						if (a==16 && bitRead(EM1,1)) break;
						if (a==17 && cvarcheck) break;
					}
					DisplayedScreen--;
				}
			}
		if (DisplayedScreen>=MAX_SCREENS) DisplayedScreen=0;

	}
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
	SetDisplayedMenu(DATE_TIME_MENU);
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

	if (orientation%2==1)
	{
		Font.SetColor(COLOR_SILVER, COLOR_WHITE,false);
		y=(theight/4)-35;
		Font.DrawCenterText(twidth/6,y,"Month");
		Font.DrawCenterText(twidth/2,y,"Day");
		Font.DrawCenterText(twidth*5/6,y,"Year");
		y=(theight*3/5)-35;
		Font.DrawCenterText(twidth/6,y,"Hour");
		Font.DrawCenterText(twidth/2,y,"Minute");
		Font.DrawCenterText(twidth*5/6,y,"AM/PM");
	}
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
	SetDisplayedMenu(PH_CALIBRATE_MENU);
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
	SetDisplayedMenu(SAL_CALIBRATE_MENU);
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
	SetDisplayedMenu(ORP_CALIBRATE_MENU);
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
	Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI3);

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

void ReefAngelClass::SetupTouchCalibratePHExp()
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	SetDisplayedMenu(PHE_CALIBRATE_MENU);
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_PHE);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_PHE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
#ifdef PHEXPANSION
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
#else
	Font.DrawCenterTextP(twidth/2,theight/3,NO_PHE1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,NO_SAL2);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
}

void ReefAngelClass::SetupTouchCalibrateWL()
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	SetDisplayedMenu(WL_CALIBRATE_MENU);
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_WL);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_WL);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
	Font.DrawCenterTextP(twidth/2,theight/3,WL_CALI1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,WL_CALI2);
	Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/3,NO_WL1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,NO_SAL2);
#endif //  WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
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
#ifdef RA_TOUCHDISPLAY
		SendMaster(MESSAGE_MENU,DEFAULT_MENU,DEFAULT_MENU); 	// Change Menu
#endif // RA_TOUCHDISPLAY
	}
}

void ReefAngelClass::ShowTouchMenu()
{
	menutimeout=now();
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
			byte TempRelay,TempRelayOn,TempRelayOff;

			if (orientation%2==0) i=0; else i=12;
			if (!TS.IsTouched())
			{
				LongTouch=0;
				if (!Sleeping)
				{
					TouchEnabled=true;
					//Draw Top Bar
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
							LastParams.Temp[T1_PROBE]=-1;
							LastParams.Temp[T2_PROBE]=-1;
							LastParams.Temp[T3_PROBE]=-1;
							LastParams.PH=0;
#ifdef SALINITYEXPANSION
							LastParams.Salinity=-1;
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
							LastParams.ORP=-1;
#endif // ORPEXPANSION
#ifdef PHEXPANSION
							LastParams.PHExp=-1;
#endif // PHEXPANSION
#ifdef HUMIDITYEXPANSION
							Humidity.LastLevel=-1;
#endif // HUMIDITYEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
							for (int a=0; a<WATERLEVEL_CHANNELS; a++)
								WaterLevel.LastLevel[a]=-1;
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION

							TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
							if (i==12 || i==4) // Orientation is portrait
							{
								x=twidth*3/16;
								if (numexp>2) i=4;
								//Temperature
								j=65+i;
								Font.SetColor(COLOR_GOLD,BKCOLOR,true);								
								Font.DrawCenterText(x,j,CustomLabels[Temp1Label]);
								x+=twidth*5/16;
								Font.DrawCenterText(x,j,CustomLabels[Temp2Label]);
								x+=twidth*5/16;
								Font.DrawCenterText(x,j,CustomLabels[Temp3Label]);

								//pH
								x=twidth*3/16;
								j+=45+i;
								Font.DrawCenterText(x,j,CustomLabels[PHLabel]);
								x+=twidth*5/16;
#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									Font.DrawCenterText(x,j,CustomLabels[SalinityLabel]);
									x+=twidth*5/16;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									Font.DrawCenterText(x,j,CustomLabels[ORPLabel]);
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
									Font.DrawCenterText(x,j,CustomLabels[PHExpLabel]);
									x+=twidth*5/16;
								}
#endif // PHEXPANSION
#ifdef HUMIDITYEXPANSION
								//Humidity
								if ((EM1&(1<<0))!=0)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									Font.DrawCenterText(x,j,CustomLabels[HumidityLabel]);
									x+=twidth*5/16;
								}
#endif // HUMIDITYEXPANSION								
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									for (int a=0; a<WATERLEVEL_CHANNELS; a++)
									{
										if (x>twidth*14/16)
										{
											x=twidth*3/16;
											j+=45+i;
										}
										Font.DrawCenterText(x,j,CustomLabels[WaterLevelLabel+a]);
										x+=twidth*5/16;
									}
								}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
							}
							else // Orientation is landscape
							{
								//Temperature, pH
								x=twidth*3/21;
								if (numexp==0) i=7;
								j=64+i;
								Font.SetColor(COLOR_GOLD,BKCOLOR,true);
								Font.DrawCenterText(x,j,CustomLabels[Temp3Label]);
								x+=twidth*5/21;
								Font.DrawCenterText(x,j,CustomLabels[Temp2Label]);
								x+=twidth*5/21;
								Font.DrawCenterText(x,j,CustomLabels[Temp1Label]);
								x+=twidth*5/21;
								Font.DrawCenterText(x,j,CustomLabels[PHLabel]);
								x+=twidth*5/21;

#ifdef SALINITYEXPANSION
								//Salinity
								if ((EM&(1<<3))!=0)
								{
									if (x>twidth*18/21)
									{
										x=twidth*3/21;
										j+=43+i;
									}
									Font.DrawCenterText(x,j,CustomLabels[SalinityLabel]);
									x+=twidth*5/21;
								}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
								//ORP
								if ((EM&(1<<4))!=0)
								{
									if (x>twidth*18/21)
									{
										x=twidth*3/21;
										j+=43+i;
									}
									Font.DrawCenterText(x,j,CustomLabels[ORPLabel]);
									x+=twidth*5/21;
								}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
								//pH Exp
								if ((EM&(1<<6))!=0)
								{
									if (x>twidth*18/21)
									{
										x=twidth*3/21;
										j+=43+i;
									}
									Font.DrawCenterText(x,j,CustomLabels[PHExpLabel]);
									x+=twidth*5/21;
								}
#endif // PHEXPANSION
#ifdef HUMIDITYEXPANSION
								//pH Exp
								if ((EM1&(1<<0))!=0)
								{
									if (x>twidth*18/21)
									{
										x=twidth*3/21;
										j+=43+i;
									}
									Font.DrawCenterText(x,j,CustomLabels[HumidityLabel]);
									x+=twidth*5/21;
								}
#endif // HUMIDITYEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
								//Water Level
								if ((EM&(1<<7))!=0)
								{
									for (int a=0;a<WATERLEVEL_CHANNELS;a++)
									{
										if (x>twidth*18/21)
										{
											x=twidth*3/21;
											j+=43+i;
										}
										Font.DrawCenterText(x,j,CustomLabels[WaterLevelLabel+a]);
										x+=twidth*5/21;
									}
								}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
							}
						}

						// Draw Parameter values
						if (i==12 || i==4) // Orientation is portrait
						{
							if (numexp>2) i=4;
							//Temperature
							x=twidth*3/16;
							j=31+i;
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							if (Params.Temp[T1_PROBE]!=LastParams.Temp[T1_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								LastParams.Temp[T1_PROBE]=Params.Temp[T1_PROBE];
							}
							x+=twidth*5/16;
							if (Params.Temp[T1_PROBE]!=LastParams.Temp[T2_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								LastParams.Temp[T2_PROBE]=Params.Temp[T2_PROBE];
							}
							x+=twidth*5/16;
							if (Params.Temp[T3_PROBE]!=LastParams.Temp[T3_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
								LastParams.Temp[T3_PROBE]=Params.Temp[T3_PROBE];
							}

							//pH
							x=twidth*3/16;
							j+=43+i;
							if (Params.PH!=LastParams.PH)
							{
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);
								LastParams.PH=Params.PH;
							}
							x+=twidth*5/16;
#ifdef SALINITYEXPANSION
							//Salinity
							if ((EM&(1<<3))!=0)
							{
								if (Params.Salinity!=LastParams.Salinity)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									LastParams.Salinity=Params.Salinity;
								}
								x+=twidth*5/16;
							}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
							//ORP
							if ((EM&(1<<4))!=0)
							{
								if (Params.ORP!=LastParams.ORP)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,1);
									LastParams.ORP=Params.ORP;
								}
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
								if (Params.PHExp!=LastParams.PHExp)
								{
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									LastParams.PHExp=Params.PHExp;
								}
								x+=twidth*5/16;
							}
#endif // PHEXPANSION
#ifdef HUMIDITYEXPANSION
							//Humidity
							if ((EM1&(1<<0))!=0)
							{
								if (x>twidth*14/16)
								{
									x=twidth*3/16;
									j+=45+i;
								}
								if (Humidity.GetLevel()!=Humidity.LastLevel)
								{
									LargeFont.DrawCenterNumber(x,j,Humidity.GetLevel(),10);
									Humidity.LastLevel=Humidity.GetLevel();
								}
								x+=twidth*5/16;
							}
#endif // HUMIDITYEXPANSION							
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
							//Water Level
							if ((EM&(1<<7))!=0)
							{
								for (int a=0;a<WATERLEVEL_CHANNELS;a++)
								{
									if (x>twidth*14/16)
									{
										x=twidth*3/16;
										j+=45+i;
									}
									if (WaterLevel.GetLevel(a)!=WaterLevel.LastLevel[a])
									{
										LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(a),0);
										WaterLevel.LastLevel[a]=WaterLevel.GetLevel(a);
									}
									x+=twidth*5/16;
								}
							}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
						}
						else // Orientation is landscape
						{
							x=twidth*3/21;
							if (numexp==0) i=7;
							//Temperature
							j=32+i;
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							if (Params.Temp[T1_PROBE]!=LastParams.Temp[T1_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T1_PROBE],10);
								LastParams.Temp[T1_PROBE]=Params.Temp[T1_PROBE];
							}
							x+=twidth*5/21;
							if (Params.Temp[T2_PROBE]!=LastParams.Temp[T2_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
								LastParams.Temp[T2_PROBE]=Params.Temp[T2_PROBE];
							}
							x+=twidth*5/21;
							if (Params.Temp[T3_PROBE]!=LastParams.Temp[T3_PROBE])
							{
								LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
								LastParams.Temp[T3_PROBE]=Params.Temp[T3_PROBE];
							}
							x+=twidth*5/21;
							if (Params.PH!=LastParams.PH)
							{
								LargeFont.DrawCenterNumber(x,j,Params.PH,100);
								LastParams.PH=Params.PH;
							}
							x+=twidth*5/21;

#ifdef SALINITYEXPANSION
							//Salinity
							if ((EM&(1<<3))!=0)
							{
								if (x>twidth*18/21)
								{
									x=twidth*3/21;
									j+=43+i;
								}
								if (Params.Salinity!=LastParams.Salinity)
								{
									LargeFont.DrawCenterNumber(x,j,Params.Salinity,10);
									LastParams.Salinity=Params.Salinity;
								}
								x+=twidth*5/21;
							}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
							//ORP
							if ((EM&(1<<4))!=0)
							{
								if (x>twidth*18/21)
								{
									x=twidth*3/21;
									j+=43+i;
								}
								if (Params.ORP!=LastParams.ORP)
								{
									LargeFont.DrawCenterNumber(x,j,Params.ORP,1);
									LastParams.ORP=Params.ORP;
								}
								x+=twidth*5/21;
							}
#endif // ORPEXPANSION
#ifdef PHEXPANSION
							//pH Exp
							if ((EM&(1<<6))!=0)
							{
								if (x>twidth*18/21)
								{
									x=twidth*3/21;
									j+=43+i;
								}
								if (Params.PHExp!=LastParams.PHExp)
								{
									LargeFont.DrawCenterNumber(x,j,Params.PHExp,100);
									LastParams.PHExp=Params.PHExp;
								}
								x+=twidth*5/21;
							}
#endif // PHEXPANSION
#ifdef HUMIDITYEXPANSION
							//Humidity
							if ((EM1&(1<<0))!=0)
							{
								if (x>twidth*18/21)
								{
									x=twidth*3/21;
									j+=43+i;
								}
								if (Humidity.GetLevel()!=Humidity.LastLevel)
								{
									LargeFont.DrawCenterNumber(x,j,Humidity.GetLevel(),10);
									Humidity.LastLevel=Humidity.GetLevel();
								}
								x+=twidth*5/21;
							}
#endif // HUMIDITYEXPANSION
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
							//Water Level
							if ((EM&(1<<7))!=0)
							{
								for (int a=0;a<5;a++)
								{
									if (x>twidth*18/21)
									{
										x=twidth*3/21;
										j+=43+i;
									}
									if (WaterLevel.GetLevel(a)!=WaterLevel.LastLevel[a])
									{
										LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(a),0);
										WaterLevel.LastLevel[a]=WaterLevel.GetLevel(a);
									}
									x+=twidth*5/21;
								}
							}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
						}

						if (i==4) i=12;
						if (i==7) i=0;

					}
					else if(DisplayedScreen==DIMMING_ATO)
					{
						int x=0;
						if (NeedsRedraw)
						{
							NeedsRedraw=false;

							TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
							PB[0].NeedsRedraw=true;
							PB[1].NeedsRedraw=true;
							if (Board==RAStar)
							{
								j=40;
								PB[2].NeedsRedraw=true;
								PB[3].NeedsRedraw=true;
							}
							else
							{
								j=40+i;
							}
							//ATO Ports
							if (Board==RAStar || bitRead(EM1,3))
								x=(twidth/4)-25;
							else
								x=(twidth/2)-25;
							Font.DrawText(COLOR_WHITE,COLOR_BLACK,x,j+3,CustomLabels[ATOHighLabel]);
							j+=20+(i*2);
							Font.DrawText(COLOR_WHITE,COLOR_BLACK,x,j+3,CustomLabels[ATOLowLabel]);
							if (Board==RAStar || bitRead(EM1,3))
							{
								j=40;
								x=(twidth*3/4)-25;
								Font.DrawText(COLOR_WHITE,COLOR_BLACK,x,j+3,CustomLabels[LeakLabel]);
							}
							if (Board==RAStar)
							{
								j+=20+(i*2);
								Font.DrawText(COLOR_WHITE,COLOR_BLACK,x,j+3,CustomLabels[AlarmLabel]);
							}
							j+=25+i;
							//Division
							TouchLCD.DrawLine(DIVISION,0,j,twidth,j);
						}
						if (Board==RAStar)
							j=40;
						else
							j=40+i;
						// ATO Buttons
						if (Board==RAStar || bitRead(EM1,3))
							x=(twidth/4)-50;
						else
							x=(twidth/2)-50;
							
						if (HighATO.IsActive())
							TouchLCD.DrawBMP(x,j,GREENBUTTON);
						else
							TouchLCD.DrawBMP(x,j,REDBUTTON);
						j+=20+(i*2);
						if (LowATO.IsActive())
							TouchLCD.DrawBMP(x,j,GREENBUTTON);
						else
							TouchLCD.DrawBMP(x,j,REDBUTTON);						
						if (Board==RAStar || bitRead(EM1,3))
						{
							j=40;
							x=(twidth*3/4)-50;
							if (IsLeakDetected())
								TouchLCD.DrawBMP(x,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(x,j,REDBUTTON);
						}
						if (Board==RAStar)
						{
							j+=20+(i*2);
							if (AlarmInput.IsActive())
								TouchLCD.DrawBMP(x,j,GREENBUTTON);
							else
								TouchLCD.DrawBMP(x,j,REDBUTTON);
						}
						
						j+=35+i;

						// Progress Bars
						PB[0].SetPosition(10,j);
						PB[0].SetColor(COLOR_ORANGE);
						PB[0].SetLabel(CustomLabels[DaylightLabel]);
#if defined(__SAM3X8E__)
						PB[0].SetCurrent(VariableControl.GetDaylightValue());
#else
						PB[0].SetCurrent(PWM.GetDaylightValue());
#endif
						PB[0].Show();
						j+=30+i;
						PB[1].SetPosition(10,j);
						PB[1].SetColor(COLOR_ROYALBLUE);
						PB[1].SetLabel(CustomLabels[ActinicLabel]);
#if defined(__SAM3X8E__)
						PB[1].SetCurrent(VariableControl.GetActinicValue());
#else
						PB[1].SetCurrent(PWM.GetActinicValue());
#endif
						PB[1].Show();
						j+=30+i;
						if (Board==RAStar)
						{
							PB[2].SetPosition(10,j);
							PB[2].SetColor(COLOR_ORANGE);
							PB[2].SetLabel(CustomLabels[Daylight2Label]);
#if defined(__SAM3X8E__)
							PB[2].SetCurrent(VariableControl.GetDaylight2Value());
#else
							PB[2].SetCurrent(PWM.GetDaylight2Value());
#endif
							PB[2].Show();
							j+=30+i;
							PB[3].SetPosition(10,j);
							PB[3].SetColor(COLOR_ROYALBLUE);
							PB[3].SetLabel(CustomLabels[Actinic2Label]);
#if defined(__SAM3X8E__)
							PB[3].SetCurrent(VariableControl.GetActinic2Value());
#else
							PB[3].SetCurrent(PWM.GetActinic2Value());
#endif
							PB[3].Show();
						}
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
							TempRelay=Relay.RelayDataE[DisplayedScreen-3];
							TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-3];
							TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-3];
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
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
							Font.SetColor(COLOR_WHITE,BKCOLOR,true);
							Font.DrawCenterText(k,j+3,CustomLabels[(a-1)+((DisplayedScreen-2)*8)]);
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)

							for (int a=0;a<6;a++)
								PB[a].NeedsRedraw=true;
						}

						// Progress Bars
						j=50+(i/2);
						Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
						for (int a=0;a<6;a++)
						{
							j+=22+i;
							PB[a].SetPosition(10,j);
							PB[a].SetColor(COLOR_GREEN);
							PB[a].SetLabel(CustomLabels[DimmingChannel0Label+a]);
#if defined(__SAM3X8E__)
							PB[a].SetCurrent(VariableControl.GetChannelValue(a));
#else
							PB[a].SetCurrent(PWM.GetChannelValue(a));
#endif
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							j=60;
							Font.SetColor(COLOR_GOLD,BKCOLOR,false);
							j+=40+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_MODE);
							j+=45+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_SPEED);
							j+=45+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_DURATION);
							
							j=27;
							j+=40+(i*2);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							char temp[25];
							char rf_mode[35];
#if defined (__SAM3X8E__)
							strcpy(temp,rf_items[RF.Mode]);
#else
							strcpy_P(temp,(char * )pgm_read_word(&(rf_items[RF.Mode])));
#endif // (__SAM3X8E__)
							strcpy(rf_mode,"   ");
							strcat(rf_mode,temp);
							strcat(rf_mode,"   ");
							LargeFont.DrawCenterText((twidth/2),j,rf_mode);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Speed,0);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,RF.Duration,0);
						}
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							for (int a=0;a<6;a++)
								PB[a].NeedsRedraw=true;
						}
						// Progress Bars
						j=50+(i/2);
						int rfcolor[] = {COLOR_ORANGE,COLOR_ROYALBLUE,COLOR_RED,COLOR_GREEN,COLOR_LIGHTBLUE,COLOR_MAGENTA};
						for (int a=0;a<6;a++)
						{
							j+=22+i;
							char tempname[16];
#if defined (__SAM3X8E__)
							strcpy(tempname, LABEL_RF[a]);
#else
							int ptr = pgm_read_word(&(LABEL_RF[a]));
							strcpy_P(tempname, (char *)ptr);
#endif // (__SAM3X8E__)
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							for (int a=0;a<3;a++)
								PB[a].NeedsRedraw=true;
						}

						// Progress Bars
						j=35;
						int aicolor[] = {COLOR_ORANGE,COLOR_LIGHTBLUE,COLOR_ROYALBLUE};
						for (int a=0;a<3;a++)
						{
							j+=40+(i*2);
							char tempname[16];
#if defined (__SAM3X8E__)
							strcpy(tempname, LABEL_AI[a]);
#else
							int ptr = pgm_read_word(&(LABEL_AI[a]));
							strcpy_P(tempname, (char *)ptr);
#endif // (__SAM3X8E__)
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
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
							j=50+(i/2);
							for (int a=0;a<6;a++)
							{
								j+=22+i;
								Font.DrawCenterText((twidth/2)-10,j+5,CustomLabels[IOChannel0Label+a]);
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
#ifdef DCPUMPCONTROL
					else if(DisplayedScreen==DCPUMP_SCREEN)
					{
						if (NeedsRedraw)
						{
							NeedsRedraw=false;
							TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
							//Gray Bar
							for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(DCLABELBAR,a*4),0,40+a,twidth,40+a);
							LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							j=60;
							Font.SetColor(COLOR_GOLD,BKCOLOR,false);
							j+=40+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_MODE);
							j+=45+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_SPEED);
							j+=45+(i*2);
							Font.DrawCenterTextP((twidth/2),j,LABEL_DURATION);
							
							j=27;
							j+=40+(i*2);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
							char temp[25];
							char rf_mode[35];
#if defined (__SAM3X8E__)
							strcpy(temp,rf_items[DCPump.Mode]);
#else
							strcpy_P(temp,(char * )pgm_read_word(&(rf_items[DCPump.Mode])));
#endif // (__SAM3X8E__)
							strcpy(rf_mode,"   ");
							strcat(rf_mode,temp);
							strcat(rf_mode,"   ");
							LargeFont.DrawCenterText((twidth/2),j,rf_mode);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,DCPump.Speed,0);
							j+=45+(i*2);
							LargeFont.DrawCenterNumber((twidth/2),j,DCPump.Duration,0);
						}
					}
#endif // DCPUMPCONTROL					
#ifdef CUSTOM_VARIABLES
					else if(DisplayedScreen==CVAR_SCREEN)
					{
						if (NeedsRedraw)
						{
							NeedsRedraw=false;
							TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
							//Gray Bar
							for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(CVARLABELBAR,a*4),0,40+a,twidth,40+a);
							LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
#if defined (__SAM3X8E__)
							LargeFont.DrawCenterText((twidth/2)+1,34,relay_items[DisplayedScreen-2]);
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterText((twidth/2),33,relay_items[DisplayedScreen-2]);
#else
							LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
							LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
							LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(relay_items[DisplayedScreen-2])));
#endif // (__SAM3X8E__)
							Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
							j=53+(i/2);
							for (int a=0;a<8;a++)
							{
								j+=16+(i/2);
								Font.DrawText((twidth/8),j+5,CustomLabels[CustomVar0Label+a]);
							}								
						}
						j=53+(i/2);
						for (int a=0;a<8;a++)
						{
							j+=16+(i/2);
							Font.DrawText((twidth*7/10),j+5,CustomVar[a]);
						}

					}
#endif //  CUSTOM_VARIABLES
					else if(DisplayedScreen==DIMMING_OVERRIDE)
					{
						if (NeedsRedraw)
						{
							NeedsRedraw=false;
							TouchLCD.Clear(COLOR_WHITE,0,28,twidth,theight);
							Font.SetColor(COLOR_BLACK,COLOR_WHITE,false);
							Font.DrawCenterTextP(twidth/2,theight*7/12,PWM_OVERRIDE_LABEL1);
							Font.DrawCenterTextP(twidth/2,theight*2/3,PWM_OVERRIDE_LABEL2);
							OkButton.SetPosition(twidth/4-40,theight*17/20);
							OkButton.Show();
							CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
							CancelButton.Show();
							Slider.SetPosition(0,theight/5);
							Slider.Show();
							//									Slider.Refresh();
						}
					}
				}
			}
			else // if touched
			{
				menutimeout=now();
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
						if(DisplayedScreen==DIMMING_ATO)
						{
							if (Board==RAStar)
								j=95+(i*3);
							else
								j=95+(i*4);
							if (TS.IsTouchedInside(0,j-5,twidth,j+25))
							{
								RecallScreen=DisplayedScreen;
								DisplayedScreen=DIMMING_OVERRIDE;
								NeedsRedraw=true;
								Slider.SetColor(COLOR_ORANGE);
#if defined(__SAM3X8E__)
								Slider.SetCurrent(VariableControl.GetDaylightValue());
#else
								Slider.SetCurrent(PWM.GetDaylightValue());
#endif
								Slider.SetOverrideID(OVERRIDE_DAYLIGHT);
								Slider.SetLabel(CustomLabels[DaylightLabel]);
							}
							j+=30+i;
							if (TS.IsTouchedInside(0,j-5,twidth,j+25))
							{
								RecallScreen=DisplayedScreen;
								NeedsRedraw=true;
								DisplayedScreen=DIMMING_OVERRIDE;
								Slider.SetColor(COLOR_ROYALBLUE);
#if defined(__SAM3X8E__)
								Slider.SetCurrent(VariableControl.GetActinicValue());
#else
								Slider.SetCurrent(PWM.GetActinicValue());
#endif
								Slider.SetOverrideID(OVERRIDE_ACTINIC);
								Slider.SetLabel(CustomLabels[ActinicLabel]);
							}
							j+=30+i;
							if (TS.IsTouchedInside(0,j-5,twidth,j+25))
							{
								RecallScreen=DisplayedScreen;
								DisplayedScreen=DIMMING_OVERRIDE;
								NeedsRedraw=true;
								Slider.SetColor(COLOR_ORANGE);
#if defined(__SAM3X8E__)
								Slider.SetCurrent(VariableControl.GetDaylight2Value());
#else
								Slider.SetCurrent(PWM.GetDaylight2Value());
#endif
								Slider.SetOverrideID(OVERRIDE_DAYLIGHT2);
								Slider.SetLabel(CustomLabels[Daylight2Label]);
							}
							j+=30+i;
							if (TS.IsTouchedInside(0,j-5,twidth,j+25))
							{
								RecallScreen=DisplayedScreen;
								NeedsRedraw=true;
								DisplayedScreen=DIMMING_OVERRIDE;
								Slider.SetColor(COLOR_ROYALBLUE);
#if defined(__SAM3X8E__)
								Slider.SetCurrent(VariableControl.GetActinic2Value());
#else
								Slider.SetCurrent(PWM.GetActinic2Value());
#endif
								Slider.SetOverrideID(OVERRIDE_ACTINIC2);
								Slider.SetLabel(CustomLabels[ActinicLabel]);
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
								TempRelay=Relay.RelayDataE[DisplayedScreen-3];
								TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-3];
								TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-3];
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
#ifdef RA_TOUCHDISPLAY
										SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-2)*10,0);
#endif // RA_TOUCHDISPLAY
										Relay.Override(a+(DisplayedScreen-2)*10,0);
									}
									else if (!bitRead(TempRelayOff,a-1))
									{
#ifdef RA_TOUCHDISPLAY
										SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-2)*10,1);
#endif // RA_TOUCHDISPLAY
										Relay.Override(a+(DisplayedScreen-2)*10,1);
									}
									else if (!bitRead(TempRelayOn,a-1) && bitRead(TempRelayOff,a-1))
									{
#ifdef RA_TOUCHDISPLAY
										SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-2)*10,!bitRead(TempRelay,a-1));
#endif // RA_TOUCHDISPLAY
										if (bitRead(TempRelay,a-1))
										{
											Relay.Override(a+(DisplayedScreen-2)*10,0);
										}
										else
										{
											Relay.Override(a+(DisplayedScreen-2)*10,1);
										}
									}
								}
								if (TS.IsTouchedInside(x1,k-h,x1+74,k+h))
								{
#ifdef RA_TOUCHDISPLAY
									SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-2)*10,2);
#endif // RA_TOUCHDISPLAY
									Relay.Override(a+(DisplayedScreen-2)*10,2);
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
#if defined(__SAM3X8E__)
									Slider.SetCurrent(VariableControl.GetChannelValue(a));
#else
									Slider.SetCurrent(PWM.GetChannelValue(a));
#endif
									Slider.SetOverrideID(OVERRIDE_CHANNEL0+a);
									Slider.SetLabel(CustomLabels[DimmingChannel0Label+a]);
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
									char tempname[16];
									RecallScreen=DisplayedScreen;
									NeedsRedraw=true;
									DisplayedScreen=DIMMING_OVERRIDE;
									Slider.SetColor(aicolor[a]);
									Slider.SetCurrent(AI.GetChannel(a));
									Slider.SetOverrideID(OVERRIDE_AI_WHITE+a);
#if defined (__SAM3X8E__)
									strcpy(tempname, LABEL_AI[a]);
#else
									int ptr = pgm_read_word(&(LABEL_AI[a]));
									strcpy_P(tempname, (char *)ptr);
#endif // (__SAM3X8E__)
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
									char tempname[16];
									RecallScreen=DisplayedScreen;
									NeedsRedraw=true;
									DisplayedScreen=DIMMING_OVERRIDE;
									Slider.SetColor(rfcolor[a]);
									Slider.SetCurrent(RF.GetChannel(a));
									Slider.SetOverrideID(OVERRIDE_RF_WHITE+a);
#if defined (__SAM3X8E__)
									strcpy(tempname, LABEL_RF[a]);
#else
									int ptr = pgm_read_word(&(LABEL_RF[a]));
									strcpy_P(tempname, (char *)ptr);
#endif // (__SAM3X8E__)
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
#ifdef RA_TOUCHDISPLAY
									SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,ovalue); 	// Send Override Request
#endif // RA_TOUCHDISPLAY
									if (oid<=OVERRIDE_CHANNEL5)
#if defined(__SAM3X8E__)
										ReefAngel.VariableControl.Override(oid,ovalue);
#else
										ReefAngel.PWM.Override(oid,ovalue);
#endif
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
#ifdef RA_TOUCHDISPLAY
									SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,255); 	// Send Cancel Override Request
#endif // RA_TOUCHDISPLAY
									if (oid<=OVERRIDE_CHANNEL5)
#if defined(__SAM3X8E__)
										ReefAngel.VariableControl.Override(oid,255);
#else
										ReefAngel.PWM.Override(oid,255);
#endif
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

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#ifdef RA_TOUCHDISPLAY
			SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#ifdef RA_TOUCHDISPLAY
			SendMaster(MESSAGE_BUTTON,1,1); 	// Simulate button press
#endif // RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined (__SAM3X8E__)
								Font.DrawCenterText(twidth/4,8+(border*3)+(ch*a),menu_button_items1[a*2]);
#else
								Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
#endif // (__SAM3X8E__)
						}
						else
						{
#if defined (__SAM3X8E__)
							Font.DrawCenterText(twidth/4,8+(border*3)+(ch*a),menu_button_items1[a*2]);
#else
							Font.DrawCenterTextP(twidth/4,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
#endif // (__SAM3X8E__)
						}
						TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,6+(ch*a),twidth-6,ch*(a+1),4,true);
						TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+5+border,6+border+(ch*a),twidth-6-border,(ch*(a+1))-border,4,false);
#if defined (__SAM3X8E__)
						Font.DrawCenterText(twidth/4,22+(border*4)+(ch*a),menu_button_items1[(a*2)+1]);
						Font.DrawCenterText(twidth/4*3,8+(border*3)+(ch*a),menu_button_items2[a*2]);
						Font.DrawCenterText(twidth/4*3,22+(border*4)+(ch*a),menu_button_items2[(a*2)+1]);
#else
						Font.DrawCenterTextP(twidth/4,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[(a*2)+1])));
						Font.DrawCenterTextP(twidth/4*3,8+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[a*2])));
						Font.DrawCenterTextP(twidth/4*3,22+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[(a*2)+1])));
#endif // (__SAM3X8E__)
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

			TouchEnabled=false;
			if (CancelButton.IsPressed())
				ShowTouchMenu();
			if (OkButton.IsPressed())
			{
				setTime(newnow);
				now();
#if defined(__SAM3X8E__)
				I2CRTC.set(now());
#else
				RTC.set(now());
#endif
				ShowTouchMenu();
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
					Font.DrawCenterTextP(twidth/2,theight/3,ORP_CALI4);
					Font.DrawCenterTextP(twidth/2,theight*5/12,ORP_CALI5);
					Font.DrawCenterTextP(twidth/2,theight/2,ORP_CALI6);
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
					Font.DrawCenterTextP(twidth/2,theight/3,ORP_CALI2);
					Font.DrawCenterNumber(twidth/2- 15,theight*5/12,470,0);
					Font.DrawCenterTextP(twidth/2+ 15,theight*5/12,ORP_CALI3);
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
					p+=ORP.Read();
					delay(50);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
			Font.DrawCenterNumber(twidth/2,theight*5/12,CalVal1,0);
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
					Font.DrawText(twidth/4- 27,theight/2,"0");
					Font.DrawTextP(twidth/4,theight/2,ORP_CALI3);
					Font.DrawText(twidth/4- 27,theight*7/12,470);
					Font.DrawTextP(twidth/4,theight*7/12,ORP_CALI3);
					Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
					Font.SetColor(COLOR_RED, COLOR_WHITE,false);
					Font.DrawText(twidth*3/4,theight/2,CalVal1);
					Font.SetColor(COLOR_RED, COLOR_WHITE,false);
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
					InternalMemory.ORPMin_write(CalVal1);
					ORPMin = CalVal1;
					InternalMemory.ORPMax_write(CalVal2);
					ORPMax = CalVal2;
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
#endif
		break;
	}
	case PHE_CALIBRATE_MENU:
	{
		unsigned long t=newnow-now();
		unsigned long p=0;

		if (!TS.IsTouched()) TouchEnabled=true;
		if (TouchEnabled && TS.IsTouched())
		{
			if (CancelButton.IsPressed())
				ShowTouchMenu();
		}
#ifdef PHEXPANSION
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
					p+=PH.Read();
					delay(50);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
					p+=PH.Read();
					delay(50);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
					InternalMemory.PHExpMin_write(CalVal1);
					PHExpMin = CalVal1;
					InternalMemory.PHExpMax_write(CalVal2);
					PHExpMax = CalVal2;
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
#endif
		break;
	}
	case WL_CALIBRATE_MENU:
	{
		unsigned long t=newnow-now();
		unsigned long p=0;

		if (!TS.IsTouched()) TouchEnabled=true;
		if (TouchEnabled && TS.IsTouched())
		{
			if (CancelButton.IsPressed())
				ShowTouchMenu();
		}
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
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
					p+=WaterLevel.Read();
					delay(50);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
					Font.DrawCenterTextP(twidth/2,theight/3,WL_CALI3);
					Font.DrawCenterTextP(twidth/2,theight*5/12,WL_CALI4);
					Font.DrawCenterTextP(twidth/2,theight/2,WL_CALI5);
					Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);
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
		case 5:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				for (int a=0;a<60;a++)
				{
					p+=WaterLevel.Read();
					delay(50);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
				}
				p/=60;
				CalVal2=p;
				CalStep++;
			}
			break;
		case 6:
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
		case 7:
			if (TouchEnabled && TS.IsTouched())
			{
				if (OkButton.IsPressed())
				{
					TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
					Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
					Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI12);
					Font.DrawTextP(twidth/4+15,theight/2,WL_CALI6);
					Font.DrawText(twidth/4-15,theight/2,"0");
					Font.DrawTextP(twidth/4+15,theight*7/12,WL_CALI6);
					Font.DrawText(twidth/4-15,theight*7/12,100);
					Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
					Font.SetColor(COLOR_RED, COLOR_WHITE,false);
					Font.DrawText(twidth*3/4,theight/2,CalVal1);
					Font.DrawText(twidth*3/4,theight*7/12,CalVal2);
					CalStep++;
				}
				TouchEnabled=false;
			}
			break;
		case 8:
			if (TouchEnabled && TS.IsTouched())
			{
				if (OkButton.IsPressed())
				{
					TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
					Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
					Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
					InternalMemory.WaterLevelMin_write(CalVal1);
					InternalMemory.WaterLevelMax_write(CalVal2);
					CalStep++;
				}
				TouchEnabled=false;
			}
			break;
		case 9:
			if (TouchEnabled && TS.IsTouched())
			{
				if (OkButton.IsPressed())
					ShowTouchMenu();
				TouchEnabled=false;
			}
			break;
		}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
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

#ifdef VersionMenu
void ReefAngelClass::DisplayVersion()
{
	LargeFont.DrawText(WARNING_TEXT,BKCOLOR,20,20,"Reef Angel");
	LargeFont.DrawText(WARNING_TEXT,BKCOLOR,20,20,"v"ReefAngel_Version);
}
#endif  // VersionMenu

void ReefAngelClass::ClearScreen(byte Color)
{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION
	TouchLCD.FullClear(BKCOLOR);
#else  // RA_TOUCH
	// clears the entire screen
	LCD.Clear(Color, 0, 0, 131, 131);
#endif  // RA_TOUCH
}

void ReefAngelClass::RefreshScreen()
{
}

void ReefAngelClass::CheckDrawGraph()
{
	NeedsRedraw=true;
}

void ReefAngelClass::CheckFeedingDrawing()
{
	ClearScreen(DefaultBGColor);
	NeedsRedraw=true;
}

void ReefAngelClass::CheckWaterChangeDrawing()
{
	ClearScreen(DefaultBGColor);
	NeedsRedraw=true;
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
		for (int a=0;a<10;a++) ReefAngel.TouchLCD.DrawPixel(COLOR_RED,x1+78,y1+(a*2));
		ReefAngel.Font.SetColor(textcolor,bkcolor,false);
		ReefAngel.Font.DrawText(x1+5,y1+5,str);
		ReefAngel.Font.DrawText(x1+85,y1+5,current);
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
	NeedsRedraw=false;
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY
		wdt_reset();
#elif defined(__SAM3X8E__)
		WDT_Restart( WDT );
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
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
