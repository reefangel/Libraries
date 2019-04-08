void ReefAngelClass::ChangeOrientation()
{
	byte tempo = orientation;
	SetOrientation(++tempo);
	ExitMenu();
}

void ReefAngelClass::SetOrientation(byte o)
{
	if (o>4) o=1;
	if (orientation!=o) NeedsRedraw=true;
	orientation=o;
	TouchLCD.SetOrientation(o);
	TS.SetOrientation(o);
	InternalMemory.Touch_Orientation_write(o);
}

void ReefAngelClass::CalibrateTouchScreen()
{
	boolean calibrated=false;
	long CalibrationPoints[4]={24,32,213,287};
	long TouchedPoints[4];

	SetOrientation(1);
	while (!calibrated)
	{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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

void ReefAngelClass::ChangeDisplayedScreen(signed char index)
{
	WDTReset();
	NeedsRedraw=true;
	if (millis()-lastDisplayChange>200)
	{
		lastDisplayChange=millis();
		DisplayedScreen+=index;
	}
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
					if (a<PWM_SCREEN)
					{
						if (bitRead(REM,a-EXP_BOX_1)) break;
					}
					else
					{
						if (a==PWM_SCREEN && bitRead(EM,0)) break;
						if (a==RF_SCREEN && bitRead(EM,1)) break;
						if (a==RF_SCREEN1 && bitRead(EM,1)) break;
						if (a==AI_SCREEN && bitRead(EM,2)) break;
						if (a==IO_SCREEN && bitRead(EM,5)) break;
						if (a==DCPUMP_SCREEN && bitRead(EM1,1)) break;
						if (a==CVAR_SCREEN && cvarcheck) break;
						if (a==STATUS_SCREEN) break;
						if (a==ALERT_SCREEN) break;
					}
					DisplayedScreen++;
				}
			}
			else
			{
				for (int a=DisplayedScreen;a>=EXP_BOX_1;a--)
				{
					if (a<PWM_SCREEN)
					{
						if (bitRead(REM,a-EXP_BOX_1)) break;
					}
					else
					{
						if (a==PWM_SCREEN && bitRead(EM,0)) break;
						if (a==RF_SCREEN && bitRead(EM,1)) break;
						if (a==RF_SCREEN1 && bitRead(EM,1)) break;
						if (a==AI_SCREEN && bitRead(EM,2)) break;
						if (a==IO_SCREEN && bitRead(EM,5)) break;
						if (a==DCPUMP_SCREEN && bitRead(EM1,1)) break;
						if (a==CVAR_SCREEN && cvarcheck) break;
						if (a==STATUS_SCREEN) break;
						if (a==ALERT_SCREEN) break;
					}
					DisplayedScreen--;
				}
			}
		if (DisplayedScreen>=MAX_SCREENS) DisplayedScreen=0;

	}
	ReDrawScreen();
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
		Font.DrawCenterTextP(twidth/6,y,LABEL_MONTH);
		Font.DrawCenterTextP(twidth/2,y,LABEL_DAY);
		Font.DrawCenterTextP(twidth*5/6,y,LABEL_YEAR);
		y=(theight*3/5)-35;
		Font.DrawCenterTextP(twidth/6,y,LABEL_HOUR);
		Font.DrawCenterTextP(twidth/2,y,LABEL_MINUTE);
		Font.DrawCenterTextP(twidth*5/6,y,LABEL_AMPM);
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
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
	Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);

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
	Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
	Font.DrawCenterNumber(twidth/2- 10,theight*5/12,35,0);
	Font.DrawCenterTextP(twidth/2+ 15,theight*5/12,SAL_CALI2);
	Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
	Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/2,MENU_BUTTON_NOT_ENABLED);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
	if (bitRead(ReefAngel.CEM,CloudSalinityBit)==1) ReefAngel.Network.CloudPublish("SALC:1");
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
	Font.DrawCenterTextP(twidth/2,theight/2,MENU_BUTTON_NOT_ENABLED);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
	if (bitRead(ReefAngel.CEM,CloudORPBit)==1) ReefAngel.Network.CloudPublish("ORPC:1");
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
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
	Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/2,MENU_BUTTON_NOT_ENABLED);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
	if (bitRead(ReefAngel.CEM,CloudPHExpBit)==1) ReefAngel.Network.CloudPublish("PHEC:1");
}

void ReefAngelClass::SetupTouchCalibrateWL(byte channel)
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	SetDisplayedMenu(WL_CALIBRATE_MENU+channel);
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
	Font.DrawCenterTextP(twidth/2,theight/3,WL_CALI7);
	Font.DrawText(channel);
	Font.DrawCenterTextP(twidth/2,theight*5/12,WL_CALI1);
	Font.DrawCenterTextP(twidth/2,theight/2,WL_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
#else
	Font.DrawCenterTextP(twidth/2,theight/2,MENU_BUTTON_NOT_ENABLED);
#endif
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
	char buffer[7];
	sprintf(buffer, "WL%dC:1", channel);
	if (bitRead(ReefAngel.CEM,CloudWLBit)==1 && channel==0) ReefAngel.Network.CloudPublish(buffer);
	if (bitRead(ReefAngel.CEM,CloudMultiWLBit)==1  && channel>0 && channel<5) ReefAngel.Network.CloudPublish(buffer);
}

void ReefAngelClass::SetupTouchCalibrateCustom(byte index)
{
	int twidth,theight;

	twidth=TouchLCD.GetWidth();
	theight=TouchLCD.GetHeight();
	SetDisplayedMenu(CUSTOM1_CALIBRATE_MENU+index);
	TouchLCD.FullClear(COLOR_WHITE);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 6, MENU_BUTTON_CEXP);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 5, MENU_BUTTON_CEXP);
	LargeFont.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	LargeFont.DrawCenterTextP(twidth/2+1, 36, MENU_BUTTON_CALIBRATION);
	LargeFont.SetColor(WARNING_TEXT, COLOR_WHITE,true);
	LargeFont.DrawCenterTextP(twidth/2, 35, MENU_BUTTON_CALIBRATION);

	Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
	Font.DrawCenterTextP(twidth/2,theight/3,CUSTOM_CALI3);
	Font.DrawText(index+1);
	Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI1);
	Font.DrawCenterTextP(twidth/2- 10,theight/2,CUSTOM_CALI1);
	Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI2);
	Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI15);
	Font.DrawCenterTextP(twidth/2,theight*3/4,PH_CALI3);

	CalVal1=0;
	CalVal2=0;
	CalStep=0;

	OkButton.SetPosition(twidth/4-40,theight*17/20);
	OkButton.Show();
	CancelButton.SetPosition(twidth*3/4-60,theight*17/20);
	CancelButton.Show();
	TouchEnabled=true;
	char buffer[9];
	sprintf(buffer, "CEXP%dC:1", index);
	ReefAngel.Network.CloudPublish(buffer);
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

void ReefAngelClass::ShowTouchMenu(byte index)
{
	menutimeout=now();
	SetDisplayedMenu(TOUCH_MENU);
	DisplayedScreen=index;
	NeedsRedraw=true;
	ReDrawScreen();
}

void ReefAngelClass::ShowTouchInterface()
{
	Refresh();
	// Needed to update Diming, ATO status, IO
	LowATO.IsActive();
	HighATO.IsActive();
	AlarmInput.IsActive();
	PWM.GetDaylightValue();
	PWM.GetActinicValue();
	PWM.GetDaylight2Value();
	PWM.GetActinic2Value();

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
			if (!TS.IsTouched())
				ReDrawScreen();
			CheckTouch();
		}
#endif //  CUSTOM_MAIN
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
			WDTReset();
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
		if ( TS.IsTouched() || ButtonPress > 0)
		{
			// screen was touched, so we stop the feeding mode
			bDone = true;
			TouchEnabled=false;
			ButtonPress=0;
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
		WDTReset();
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
		if ( TS.IsTouched() || ButtonPress > 0 )
		{
			// screen was touched, so we stop the feeding mode
			bDone = true;
			TouchEnabled=false;
			ButtonPress=0;
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
		CheckTouch();
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

		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
		}
		switch(CalStep)
		{
		case 0:
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI10);
				Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI11);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI3);
				CalStep++;
			}
			break;
		case 4:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
				Font.DrawCenterTextP(twidth/2- 15,theight*5/12,MENU_BUTTON_PH);
				Font.DrawCenterNumber(twidth/2+ 15,theight*5/12,10,1);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
				Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);
				CalStep++;
			}
			break;
		case 5:
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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
			break;
		case 9:
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
			break;
		case 10:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
			}
			break;
		}
		break;
	}
	case SAL_CALIBRATE_MENU:
	{
		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
				ReefAngel.Network.CloudPublish("SALC:0");
			}
		}

#ifdef SALINITYEXPANSION
		unsigned long t=newnow-now();
		unsigned long p=0;

		switch(CalStep)
		{
		case 0:
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
			break;
		case 1:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudSalinityBit)==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=Salinity.Read();
						delay(50);
	#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
						wdt_reset();
	#elif defined(__SAM3X8E__)
						WDT_Restart( WDT );
	#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					}
					p/=60;
					CalVal1=p;
				}
				else
				{
					CalVal1=CloudCalVal;
				}
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
				TouchEnabled=false;
			}
			break;
		case 4:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				InternalMemory.SalMax_write(CalVal1);
				SalMax = CalVal1;
				CalStep++;
				if (bitRead(ReefAngel.CEM,CloudSalinityBit)==1)
				{
					char buffer[16];
					sprintf(buffer, "SALC:2:%d", CalVal1);
					ReefAngel.Network.CloudPublish(buffer);
				}
			}
			break;
		case 5:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
			}
			break;
		}
#endif
		break;
	}
	case ORP_CALIBRATE_MENU:
	{
		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
				ReefAngel.Network.CloudPublish("ORPC:0");
			}
		}
#ifdef ORPEXPANSION
		unsigned long t=newnow-now();
		unsigned long p=0;

		switch(CalStep)
		{
		case 0:
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
			break;
		case 1:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudORPBit)==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=ORP.Read();
						delay(50);
	#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			wdt_reset();
	#elif defined(__SAM3X8E__)
			WDT_Restart( WDT );
	#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					}
					p/=60;
					CalVal1=p;
				}
				else
				{
					CalVal1=CloudCalVal;
				}
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
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,ORP_CALI4);
				Font.DrawCenterTextP(twidth/2,theight*5/12,ORP_CALI5);
				Font.DrawCenterTextP(twidth/2,theight/2,ORP_CALI6);
				CalStep++;
			}
			break;
		case 4:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
				Font.DrawCenterNumber(twidth/2- 15,theight*5/12,470,0);
				Font.DrawCenterTextP(twidth/2+ 15,theight*5/12,ORP_CALI3);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
				Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);
				CalStep++;
			}
			break;
		case 5:
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
			break;
		case 6:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudORPBit)==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=ORP.Read();
						delay(50);
	#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			wdt_reset();
	#elif defined(__SAM3X8E__)
			WDT_Restart( WDT );
	#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					}
					p/=60;
					CalVal2=p;
				}
				else
				{
					CalVal2=CloudCalVal;
				}
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
			break;
		case 9:
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
				if (bitRead(ReefAngel.CEM,CloudORPBit)==1)
				{
					char buffer[16];
					sprintf(buffer, "ORPC:2:%d", CalVal1);
					ReefAngel.Network.CloudPublish(buffer);
					sprintf(buffer, "ORPC:3:%d", CalVal2);
					ReefAngel.Network.CloudPublish(buffer);
				}
			}
			break;
		case 10:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
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

		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
				ReefAngel.Network.CloudPublish("PHEC:0");
			}
		}
#ifdef PHEXPANSION
		switch(CalStep)
		{
		case 0:
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
			break;
		case 1:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudPHExpBit)==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=PH.Read();
						delay(50);
	#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			wdt_reset();
	#elif defined(__SAM3X8E__)
			WDT_Restart( WDT );
	#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					}
					p/=60;
					CalVal1=p;
				}
				else
				{
					CalVal1=CloudCalVal;
				}
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
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI10);
				Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI11);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI3);
				CalStep++;
			}
			break;
		case 4:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
				Font.DrawCenterTextP(twidth/2- 15,theight*5/12,MENU_BUTTON_PH);
				Font.DrawCenterNumber(twidth/2+ 15,theight*5/12,10,1);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
				Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);
				CalStep++;
			}
			break;
		case 5:
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
			break;
		case 6:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudPHExpBit)==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=PH.Read();
						delay(50);
	#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			wdt_reset();
	#elif defined(__SAM3X8E__)
			WDT_Restart( WDT );
	#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					}
					p/=60;
					CalVal2=p;
				}
				else
				{
					CalVal2=CloudCalVal;
				}
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
			break;
		case 9:
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
				if (bitRead(ReefAngel.CEM,CloudPHExpBit)==1)
				{
					char buffer[16];
					sprintf(buffer, "PHEC:2:%d", CalVal1);
					ReefAngel.Network.CloudPublish(buffer);
					sprintf(buffer, "PHEC:3:%d", CalVal2);
					ReefAngel.Network.CloudPublish(buffer);
				}
			}
			break;
		case 10:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
			}
			break;
		}
#endif
		break;
	}
	case WL_CALIBRATE_MENU:
	case WL1_CALIBRATE_MENU:
	case WL2_CALIBRATE_MENU:
	case WL3_CALIBRATE_MENU:
	case WL4_CALIBRATE_MENU:
	{
		unsigned long t=newnow-now();
		unsigned long p=0;
		byte channel=DisplayedMenu-WL_CALIBRATE_MENU;

		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
				char buffer[7];
				sprintf(buffer, "WL%dC:0", channel);
				ReefAngel.Network.CloudPublish(buffer);
			}
		}
#if defined WATERLEVELEXPANSION || defined MULTIWATERLEVELEXPANSION
		switch(CalStep)
		{
		case 0:
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
			break;
		case 1:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudWLBit)==0 && channel==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=WaterLevel.Read();
						delay(50);
						WDTReset();
					}
					p/=60;
					CalVal1=p;
				}
				else if (bitRead(ReefAngel.CEM,CloudMultiWLBit)==0 && channel>0 && channel<5)
				{
					for (int a=0;a<60;a++)
					{
						p+=WaterLevel.Read(channel);
						delay(50);
						WDTReset();
					}
					p/=60;
					CalVal1=p;
				}
				else
				{
					CalVal1=CloudCalVal;
				}
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
			break;
		case 4:
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
			break;
		case 5:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				if (bitRead(ReefAngel.CEM,CloudWLBit)==0 && channel==0)
				{
					for (int a=0;a<60;a++)
					{
						p+=WaterLevel.Read();
						delay(50);
						WDTReset();
					}
					p/=60;
					CalVal2=p;
				}
				else if (bitRead(ReefAngel.CEM,CloudMultiWLBit)==0 && channel>0 && channel<5)
				{
					for (int a=0;a<60;a++)
					{
						p+=WaterLevel.Read(channel);
						delay(50);
						WDTReset();
					}
					p/=60;
					CalVal2=p;
				}
				else
				{
					CalVal2=CloudCalVal;
				}
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
			break;
		case 8:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				if (channel==0)
				{
					InternalMemory.WaterLevelMin_write(CalVal1);
					InternalMemory.WaterLevelMax_write(CalVal2);
				}
				else if (channel==1)
				{
					InternalMemory.WaterLevel1Min_write(CalVal1);
					InternalMemory.WaterLevel1Max_write(CalVal2);
				}
				else if (channel==2)
				{
					InternalMemory.WaterLevel2Min_write(CalVal1);
					InternalMemory.WaterLevel2Max_write(CalVal2);
				}
				else if (channel==3)
				{
					InternalMemory.WaterLevel3Min_write(CalVal1);
					InternalMemory.WaterLevel3Max_write(CalVal2);
				}
				else if (channel==4)
				{
					InternalMemory.WaterLevel4Min_write(CalVal1);
					InternalMemory.WaterLevel4Max_write(CalVal2);
				}
				CalStep++;
				if ((bitRead(ReefAngel.CEM,CloudWLBit)==1 && channel==0) || (bitRead(ReefAngel.CEM,CloudMultiWLBit)==1 && channel>0 && channel<5))
				{
					char buffer[16];
					sprintf(buffer, "WL%dC:2:%d", channel, CalVal1);
					ReefAngel.Network.CloudPublish(buffer);
					sprintf(buffer, "WL%dC:3:%d", channel, CalVal2);
					ReefAngel.Network.CloudPublish(buffer);
				}
			}
			break;
		case 9:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
			}
			break;
		}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
		break;
	}
	case CUSTOM1_CALIBRATE_MENU:
	case CUSTOM2_CALIBRATE_MENU:
	case CUSTOM3_CALIBRATE_MENU:
	case CUSTOM4_CALIBRATE_MENU:
	case CUSTOM5_CALIBRATE_MENU:
	case CUSTOM6_CALIBRATE_MENU:
	case CUSTOM7_CALIBRATE_MENU:
	case CUSTOM8_CALIBRATE_MENU:
	{
		unsigned long t=newnow-now();
		unsigned long p=0;
		byte index=DisplayedMenu-CUSTOM1_CALIBRATE_MENU;

		if (TS.IsTouched())
		{
			if (CancelButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_MENU_SCREEN);
				char buffer[9];
				sprintf(buffer, "CEXP%dC:0", index);
				ReefAngel.Network.CloudPublish(buffer);
			}
		}
		switch(CalStep)
		{
		case 0:
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
			break;
		case 1:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				CalVal1=CloudCalVal;
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
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI10);
				Font.DrawCenterTextP(twidth/2,theight*5/12,PH_CALI11);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI3);
				CalStep++;
			}
			break;
		case 4:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI1);
				Font.DrawCenterTextP(twidth/2- 15,theight*5/12,CUSTOM_CALI2);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI2);
				Font.DrawCenterTextP(twidth/2,theight*7/12,PH_CALI15);
				Font.DrawCenterTextP(twidth/2,theight*8/12,PH_CALI3);
				CalStep++;
			}
			break;
		case 5:
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
			break;
		case 6:
			Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
			Font.DrawCenterNumber(twidth/2,theight/2,t,0);
			if (t==0)
			{
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI6);
				CalVal2=CloudCalVal;
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
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/3,PH_CALI12);
				Font.DrawTextP(twidth/4,theight/2,CUSTOM_CALI4);
				Font.DrawTextP(twidth/4,theight*7/12,CUSTOM_CALI5);
				Font.DrawCenterTextP(twidth/2,theight*9/12,PH_CALI13);
				Font.SetColor(COLOR_RED, COLOR_WHITE,false);
				Font.DrawText(twidth*3/4,theight/2,CalVal1);
				Font.DrawText(twidth*3/4,theight*7/12,CalVal2);
				CalStep++;
			}
			break;
		case 9:
			if (OkButton.IsPressed())
			{
				TouchLCD.Clear(COLOR_WHITE,0,theight/3,twidth,theight-50);
				Font.SetColor(COLOR_BLACK, COLOR_WHITE,false);
				Font.DrawCenterTextP(twidth/2,theight/2,PH_CALI14);
				CalStep++;
				char buffer[16];
				sprintf(buffer, "CEXP%dC:2:%d", index, CalVal1);
				ReefAngel.Network.CloudPublish(buffer);
				sprintf(buffer, "CEXP%dC:3:%d", index, CalVal2);
				ReefAngel.Network.CloudPublish(buffer);
			}
			break;
		case 10:
			if (OkButton.IsPressed())
			{
				ShowTouchMenu(CALIBRATE_CUSTOM_MENU_SCREEN);
			}
			break;
		}
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

void ReefAngelClass::ReDrawScreen()
{
	int i,j;
	byte TempRelay,TempRelayOn,TempRelayOff;
	byte numexp=0;

	WDTReset();
	if ((EM&(1<<3))!=0) numexp++;
	if ((EM&(1<<4))!=0) numexp++;
	if ((EM&(1<<6))!=0) numexp++;
	if ((EM&(1<<7))!=0) numexp++;
	if (orientation%2==0) i=0; else i=12;

	if (!Sleeping)
	{
		int twidth=TouchLCD.GetWidth();
		int theight=TouchLCD.GetHeight();

		lastRedraw=millis();
		//Draw Top Bar
		if (NeedsRedraw)
		{
			//Bottom Bar
			TouchLCD.DrawRectangle(BOTTOMBAR_BC,0,theight-27,twidth,theight,true);
			for (int a=0;a<=5;a++)
			{
				TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,theight-28-a,twidth,theight-28-a);
			}
			//Arrows
			TouchLCD.DrawBMP(10,theight-25,ARROWLEFT);
			TouchLCD.DrawBMP(twidth-10-23,theight-25,ARROWRIGHT);
			//Menu Dividers and arrow
			TouchLCD.DrawBMP(twidth/2-50,theight-25,DIVIDER);
			TouchLCD.DrawBMP(twidth/2+50,theight-25,DIVIDER);
			TouchLCD.DrawBMP(twidth/2-3,theight-25,ARROWMENU);
			Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
			Font.DrawCenterTextP(twidth/2,theight-15,LABEL_MENU);
		}
		if (DisplayedMenu==DEFAULT_MENU || DisplayedMenu==MAIN_MENU)
		{
			Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
			if (Network.payload_ready)
			{
				Font.DrawTextP(38,9,DOWNLOADING);
				Font.DrawText((Network.downloadsize*100)/Network.lheader);
				Font.DrawText("%");
			}
			else
			{
				if (Network.downloading)
				{
					Font.DrawTextP(38,9,DOWNLOAD_FAILED);
					if (SD.exists("FIRMWARE.BIN")) {
						Serial.println(F("deleting firmware..."));
						SD.remove("FIRMWARE.BIN");
					}
				}
				else
				{
					TouchLCD.DrawDateTime(now(),38,9,MilitaryTime,Font);
				}
			}
			if (StatusFlags || AlertFlags)
				TouchLCD.DrawBMP(twidth-16,7,ALERTICON);
			else
				TouchLCD.Clear(TOPBAR_BC,twidth-16,7,twidth,23);
			if (DisplayedScreen==MAIN_SCREEN)
			{
				int x=0;
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					//Top Bar
					TouchLCD.DrawRectangle(TOPBAR_BC,0,0,twidth,27,true);
					for (int a=0;a<=5;a++)
					{
						TouchLCD.DrawLine(alphaBlend(COLOR_WHITE,(5-a)*15),0,28+a,twidth,28+a);
					}
					//Logo
					TouchLCD.DrawBMP(5,2,ICONLOGO);

					LastParams.Temp[T1_PROBE]=-1;
					LastParams.Temp[T2_PROBE]=-1;
					LastParams.Temp[T3_PROBE]=-1;
					LastParams.PH=0;
#ifdef EXTRA_TEMP_PROBES
					LastParams.Temp[T4_PROBE]=-1;
					LastParams.Temp[T5_PROBE]=-1;
					LastParams.Temp[T6_PROBE]=-1;
#endif
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
#ifdef PAREXPANSION
					PAR.LastLevel=-1;
#endif // PAREXPANSION

					TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
					if (i==12 || i==4) // Orientation is portrait
					{
						x=twidth*3/16;
						if (numexp>2) i=4;
						//Temperature
						j=65+i;
						Font.SetColor(COLOR_GOLD,BKCOLOR,true);
						Font.DrawCenterTextP(x,j,LABEL_TEMP1);
						x+=twidth*5/16;
						Font.DrawCenterTextP(x,j,LABEL_TEMP2);
						x+=twidth*5/16;
						Font.DrawCenterTextP(x,j,LABEL_TEMP3);
#ifdef EXTRA_TEMP_PROBES
						x=twidth*3/16;
						j+=45+i;
						Font.DrawCenterTextP(x,j,LABEL_TEMP4);
						x+=twidth*5/16;
						Font.DrawCenterTextP(x,j,LABEL_TEMP5);
						x+=twidth*5/16;
						Font.DrawCenterTextP(x,j,LABEL_TEMP6);
#endif
						//pH
						x=twidth*3/16;
						j+=45+i;
						Font.DrawCenterTextP(x,j,LABEL_PH);
						x+=twidth*5/16;
#ifdef SALINITYEXPANSION
						//Salinity
						if ((EM&(1<<3))!=0)
						{
							Font.DrawCenterTextP(x,j,LABEL_SALINITY);
							x+=twidth*5/16;
						}
#endif // SALINITYEXPANSION
#ifdef ORPEXPANSION
						//ORP
						if ((EM&(1<<4))!=0)
						{
							Font.DrawCenterTextP(x,j,LABEL_ORP);
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
							Font.DrawCenterTextP(x,j,LABEL_PHEXP);
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
							Font.DrawCenterTextP(x,j,LABEL_HUMIDITY);
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
								Font.DrawCenterTextP(x,j,(char * )pgm_read_word(&(LABEL_WL[a])));
								x+=twidth*5/16;
							}
						}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef PAREXPANSION
						//PAR
						if ((EM1&(1<<3))!=0)
						{
							if (x>twidth*14/16)
							{
								x=twidth*3/16;
								j+=45+i;
							}
							Font.DrawCenterTextP(x,j,LABEL_PAR);
							x+=twidth*5/16;
						}
#endif // PAREXPANSION
					}
					else // Orientation is landscape
					{
						//Temperature, pH
						x=twidth*3/21;
						if (numexp==0) i=7;
						j=64+i;
						Font.SetColor(COLOR_GOLD,BKCOLOR,true);
						Font.DrawCenterTextP(x,j,LABEL_TEMP1);
						x+=twidth*5/21;
						Font.DrawCenterTextP(x,j,LABEL_TEMP2);
						x+=twidth*5/21;
						Font.DrawCenterTextP(x,j,LABEL_TEMP3);
						x+=twidth*5/21;
#ifdef EXTRA_TEMP_PROBES
						Font.DrawCenterTextP(x,j,LABEL_TEMP4);
						x=twidth*3/21;
						j+=43+i;
						Font.DrawCenterTextP(x,j,LABEL_TEMP5);
						x+=twidth*5/21;
						Font.DrawCenterTextP(x,j,LABEL_TEMP6);
						x+=twidth*5/21;
#endif
						Font.DrawCenterTextP(x,j,LABEL_PH);
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
							Font.DrawCenterTextP(x,j,LABEL_SALINITY);
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
							Font.DrawCenterTextP(x,j,LABEL_ORP);
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
							Font.DrawCenterTextP(x,j,LABEL_PHEXP);
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
							Font.DrawCenterTextP(x,j,LABEL_HUMIDITY);
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
								Font.DrawCenterTextP(x,j,(char * )pgm_read_word(&(LABEL_WL[a])));
								x+=twidth*5/21;
							}
						}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef PAREXPANSION
						//PAR
						if ((EM1&(1<<3))!=0)
						{
							if (x>twidth*18/21)
							{
								x=twidth*3/21;
								j+=43+i;
							}
							Font.DrawCenterTextP(x,j,LABEL_PAR);
							x+=twidth*5/21;
						}
#endif // PAREXPANSION
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
					for (int a=1;a<=TEMP_PROBES;a++)
					{
						if (Params.Temp[a]!=LastParams.Temp[a])
						{
							LargeFont.DrawCenterNumber(x,j,Params.Temp[a],10);
							LastParams.Temp[a]=Params.Temp[a];
						}
						x+=twidth*5/16;
						if (x>twidth*14/16)
						{
							x=twidth*3/16;
							j+=45+i;
						}
					}
//					if (Params.Temp[T1_PROBE]!=LastParams.Temp[T2_PROBE])
//					{
//						LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
//						LastParams.Temp[T2_PROBE]=Params.Temp[T2_PROBE];
//					}
//					x+=twidth*5/16;
//					if (Params.Temp[T3_PROBE]!=LastParams.Temp[T3_PROBE])
//					{
//						LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
//						LastParams.Temp[T3_PROBE]=Params.Temp[T3_PROBE];
//					}
					//pH
//					x=twidth*3/16;
//					j+=43+i;
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
							LargeFont.DrawCenterNumber(x,j,Params.ORP,0);
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
							LargeFont.DrawCenterNumber(x,j,Humidity.GetLevel(),0);
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
#ifdef PAREXPANSION
					//PAR
					if ((EM1&(1<<3))!=0)
					{
						if (x>twidth*14/16)
						{
							x=twidth*3/16;
							j+=45+i;
						}
						if (PAR.GetLevel()!=PAR.LastLevel)
						{
							LargeFont.DrawCenterNumber(x,j,PAR.GetLevel(),10);
							PAR.LastLevel=PAR.GetLevel();
						}
						x+=twidth*5/16;
					}
#endif // PAREXPANSION
				}
				else // Orientation is landscape
				{
					x=twidth*3/21;
					if (numexp==0) i=7;
					//Temperature
					j=32+i;
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
					for (int a=1;a<=TEMP_PROBES;a++)
					{
						if (Params.Temp[a]!=LastParams.Temp[a])
						{
							LargeFont.DrawCenterNumber(x,j,Params.Temp[a],10);
							LastParams.Temp[a]=Params.Temp[a];
						}
						x+=twidth*5/21;
						if (x>twidth*18/21)
						{
							x=twidth*3/21;
							j+=43+i;
						}
					}
//					if (Params.Temp[T2_PROBE]!=LastParams.Temp[T2_PROBE])
//					{
//						LargeFont.DrawCenterNumber(x,j,Params.Temp[T2_PROBE],10);
//						LastParams.Temp[T2_PROBE]=Params.Temp[T2_PROBE];
//					}
//					x+=twidth*5/21;
//					if (Params.Temp[T3_PROBE]!=LastParams.Temp[T3_PROBE])
//					{
//						LargeFont.DrawCenterNumber(x,j,Params.Temp[T3_PROBE],10);
//						LastParams.Temp[T3_PROBE]=Params.Temp[T3_PROBE];
//					}
//					x+=twidth*5/21;
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
							LargeFont.DrawCenterNumber(x,j,Params.ORP,0);
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
							LargeFont.DrawCenterNumber(x,j,Humidity.GetLevel(),0);
							Humidity.LastLevel=Humidity.GetLevel();
						}
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
							if (WaterLevel.GetLevel(a)!=WaterLevel.LastLevel[a])
							{
								LargeFont.DrawCenterNumber(x,j,ReefAngel.WaterLevel.GetLevel(a),0);
								WaterLevel.LastLevel[a]=WaterLevel.GetLevel(a);
							}
							x+=twidth*5/21;
						}
					}
#endif // WATERLEVELEXPANSION || MULTIWATERLEVELEXPANSION
#ifdef PAREXPANSION
					//PAR
					if ((EM1&(1<<3))!=0)
					{
						if (x>twidth*18/21)
						{
							x=twidth*3/21;
							j+=43+i;
						}
						if (PAR.GetLevel()!=PAR.LastLevel)
						{
							LargeFont.DrawCenterNumber(x,j,PAR.GetLevel(),0);
							PAR.LastLevel=PAR.GetLevel();
						}
						x+=twidth*5/21;
					}
#endif // PAREXPANSION
				}

				if (i==4) i=12;
				if (i==7) i=0;

			}
			else if (DisplayedScreen==CUSTOM_EXP_SCREEN)
			{

				int x=0;
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
					if (i==12 || i==4) // Orientation is portrait
					{
						x=twidth*3/16;
						i=4;
						j=65+i;
						Font.SetColor(COLOR_GOLD,BKCOLOR,true);
						for (int a=0;a<CUSTOM_EXP_MODULES;a++)
						{
							CustomExpansionLastValue[a]=-1;
							Font.DrawCenterTextP(x,j,(char * )pgm_read_word(&(LABEL_CUSTOM_EXP[a])));
							x+=twidth*5/16;
							if (x>twidth*14/16)
							{
								x=twidth*3/16;
								j+=45+i;
							}
						}
					}
					else // Orientation is landscape
					{
						x=twidth*3/21;
						j=64+i;
						Font.SetColor(COLOR_GOLD,BKCOLOR,true);
						for (int a=0;a<CUSTOM_EXP_MODULES;a++)
						{
							CustomExpansionLastValue[a]=-1;
							Font.DrawCenterTextP(x,j,(char * )pgm_read_word(&(LABEL_CUSTOM_EXP[a])));
							x+=twidth*5/21;
							if (x>twidth*18/21)
							{
								x=twidth*3/21;
								j+=43+i;
							}
						}
					}
				}
				// Draw Parameter values
				if (i==12 || i==4) // Orientation is portrait
				{
					i=4;
					x=twidth*3/16;
					j=31+i;
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
					for (int a=0;a<CUSTOM_EXP_MODULES;a++)
					{
						if (CustomExpansionValue[a]!=CustomExpansionLastValue[a])
						{
							LargeFont.DrawCenterNumber(x,j,CustomExpansionValue[a],0);
							CustomExpansionLastValue[a]=CustomExpansionValue[a];
						}
						x+=twidth*5/16;
					}
				}
				else // Orientation is landscape
				{
					x=twidth*3/21;
					j=32+i;
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,false);
					for (int a=0;a<CUSTOM_EXP_MODULES;a++)
					{
						if (CustomExpansionValue[a]!=CustomExpansionLastValue[a])
						{
							LargeFont.DrawCenterNumber(x,j,CustomExpansionValue[a],0);
							CustomExpansionLastValue[a]=CustomExpansionValue[a];
						}
						x+=twidth*5/21;
						if (x>twidth*18/21)
						{
							x=twidth*3/21;
							j+=43+i;
						}
					}
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
					Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,x,j+3,LABEL_ATOHIGH);
					j+=20+(i*2);
					Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,x,j+3,LABEL_ATOLOW);
					if (Board==RAStar || bitRead(EM1,3))
					{
						j=40;
						x=(twidth*3/4)-25;
						Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,x,j+3,LABEL_LEAK);
					}
					if (Board==RAStar)
					{
						j+=20+(i*2);
						Font.DrawTextP(COLOR_WHITE,COLOR_BLACK,x,j+3,LABEL_ALARM);
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

				j+=30+i;

				// Progress Bars
				PB[0].SetPosition(10,j);
				PB[0].SetColor(COLOR_ORANGE);
				PB[0].SetLabelP(LABEL_DAYLIGHT);
#if defined(__SAM3X8E__)
				PB[0].SetCurrent(VariableControl.GetDaylightValue());
#else
				PB[0].SetCurrent(PWM.GetDaylightValue());
#endif
				PB[0].Show();
				j+=30+i;
				PB[1].SetPosition(10,j);
				PB[1].SetColor(COLOR_ROYALBLUE);
				PB[1].SetLabelP(LABEL_ACTINIC);
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
					PB[2].SetLabelP(LABEL_DAYLIGHT2);
#if defined(__SAM3X8E__)
					PB[2].SetCurrent(VariableControl.GetDaylight2Value());
#else
					PB[2].SetCurrent(PWM.GetDaylight2Value());
#endif
					PB[2].Show();
					j+=30+i;
					PB[3].SetPosition(10,j);
					PB[3].SetColor(COLOR_ROYALBLUE);
					PB[3].SetLabelP(LABEL_ACTINIC2);
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
					TempRelay=Relay.RelayDataE[DisplayedScreen-EXP_BOX_1];
					TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-EXP_BOX_1];
					TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-EXP_BOX_1];
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
#endif // (__SAM3X8E__)

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
						Font.DrawCenterTextP(k,j+3,(char * )pgm_read_word(&(LABEL_PORT[(a-1)+((DisplayedScreen-RELAY_BOX)*8)])));
					}
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
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
					PB[a].SetLabelP((char * )pgm_read_word(&(LABEL_PWME[a])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
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
					LargeFont.DrawCenterTextP((twidth/2),j,(char * )pgm_read_word(&(RF_MODE[RF.Mode])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
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
					PB[a].SetPosition(10,j);
					PB[a].SetColor(rfcolor[a]);
					PB[a].SetLabelP((char * )pgm_read_word(&(LABEL_RF[a])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
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
					PB[a].SetPosition(10,j);
					PB[a].SetColor(aicolor[a]);
					PB[a].SetLabelP((char * )pgm_read_word(&(LABEL_AI[a])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
#endif // (__SAM3X8E__)
					Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
					j=50+(i/2);
					for (int a=0;a<6;a++)
					{
						j+=22+i;
						Font.DrawCenterTextP((twidth/2)-10,j+5,(char * )pgm_read_word(&(LABEL_IO[a])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
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
					LargeFont.DrawCenterTextP((twidth/2),j,(char * )pgm_read_word(&(RF_MODE[DCPump.Mode])));
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
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
#endif // (__SAM3X8E__)
					Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
					j=53+(i/2);
					for (int a=0;a<8;a++)
					{
						j+=16+(i/2);
						Font.DrawTextP((twidth/8),j+5,(char * )pgm_read_word(&(LABEL_C[a])));
					}
				}
				Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
				j=53+(i/2);
				for (int a=0;a<8;a++)
				{
					j+=16+(i/2);
					Font.DrawText((twidth*7/10),j+5,CustomVar[a]);
				}

			}
#endif //  CUSTOM_VARIABLES
			else if(DisplayedScreen==STATUS_SCREEN)
			{
				if (NeedsRedraw)
				{
					int xstart=twidth/10;
					NeedsRedraw=false;
					TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
					//Gray Bar
					for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(STATUSLABELBAR,a*4),0,40+a,twidth,40+a);
					LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
#if defined (__SAM3X8E__)
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
#endif // (__SAM3X8E__)
					Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
					j=75;
					Font.DrawTextP(xstart,j,LABEL_LIBVER);
					Font.DrawText(ReefAngel_Version);
					j+=20;
					Font.DrawTextP(xstart,j,LABEL_IPADDRESS);
					const byte* ipAddr=EthernetDHCP.ipAddress();
					Font.DrawText(ip_to_str(ipAddr));
					j+=20;
					Font.DrawTextP(xstart,j,LABEL_CLOUD);
					if (Network.IsMQTTConnected())
						Font.DrawTextP(LABEL_CLOUD_CONNECTED);
					else
						Font.DrawTextP(LABEL_CLOUD_DISCONNECTED);
					j+=20;
					Font.DrawTextP(xstart,j,LABEL_SD);
					if (SDFound)
						Font.DrawTextP(LABEL_SD_INSERTED);
					else
						Font.DrawTextP(LABEL_SD_NOT_FOUND);
					j+=20;
					Font.DrawTextP(xstart,j,LABEL_LAST_BOOT);
					TouchLCD.DrawDateTime(RAStart,Font.GetX(),j,MilitaryTime,Font);
				}
			}
			else if(DisplayedScreen==ALERT_SCREEN)
			{
				if (NeedsRedraw)
				{
					NeedsRedraw=false;
					TouchLCD.Clear(COLOR_BLACK,0,34,twidth,theight-34);
					//Gray Bar
					for (int a=0;a<=25;a++) TouchLCD.DrawLine(alphaBlend(ALERTLABELBAR,a*4),0,40+a,twidth,40+a);
					LargeFont.SetColor(COLOR_GOLD,BKCOLOR,true);
#if defined (__SAM3X8E__)
					LargeFont.DrawCenterText((twidth/2)+1,34,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterText((twidth/2),33,LABEL_RELAY[DisplayedScreen-RELAY_BOX]);
#else
					LargeFont.DrawCenterTextP((twidth/2)+1,34,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
					LargeFont.SetColor(COLOR_WHITE,BKCOLOR,true);
					LargeFont.DrawCenterTextP((twidth/2),33,(char * )pgm_read_word(&(LABEL_RELAY[DisplayedScreen-RELAY_BOX])));
#endif // (__SAM3X8E__)
				}
				Font.SetColor(COLOR_WHITE,COLOR_BLACK,false);
				j=75;
				if ((StatusFlags & 1) == 1)
				{
					const unsigned char * const *iptr1=STATUSFLAGICONS;
					const unsigned char *arr2 = ( unsigned char* ) pgm_read_word( iptr1 );
					TouchLCD.DrawBMP(30,j,arr2);
					Font.DrawTextP(56,j+3,ALERT_LIGHTS_ON_LABEL);
					Font.DrawTextP(LABEL_EMPTY);
					j+=20;
				}
				byte tempalert=AlertFlags;
				for(byte i=0;i<8;i++,tempalert>>=1)
					if ((tempalert & 1) == 1)
					{
						const unsigned char * const *iptr=ALERTFLAGICONS;
						const unsigned char *arr1 = ( unsigned char* ) pgm_read_word( iptr + i );
						TouchLCD.DrawBMP(30,j,arr1);
						Font.DrawTextP(56,j+3,(char * )pgm_read_word(&(LABEL_ALERT[i])));
						Font.DrawTextP(LABEL_EMPTY);
						j+=20;
					}
				TouchLCD.Clear(BKCOLOR,30,j,twidth,theight-33);
			}
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
				}
			}
		}
		else if (DisplayedMenu==TOUCH_MENU)
		{
			if (NeedsRedraw)
			{
				NeedsRedraw=false;
				int ch;
				int border;
				if (orientation%2==0)
				{
					ch=42;
					border=0;
				}
				else
				{
					ch=57;
					border=2;
				}
				TouchLCD.Clear(COLOR_WHITE,0,0,twidth,theight-28); // Clear screen
				Font.SetColor(RGB565(0xBD, 0x13, 0x00),COLOR_WHITE,true);
				for (int a=0;a<5;a++)
				{
					TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),5,6+(ch*a),(twidth/2)-6,ch*(a+1),4,true);
					TouchLCD.DrawRoundRect(COLOR_SILVER,5+border,6+border+(ch*a),(twidth/2)-6-border,(ch*(a+1))-border,4,false);
					TouchLCD.DrawRoundRect(RGB565(0xD2, 0xE0, 0xAB),(twidth/2)+5,6+(ch*a),twidth-6,ch*(a+1),4,true);
					TouchLCD.DrawRoundRect(COLOR_SILVER,(twidth/2)+5+border,6+border+(ch*a),twidth-6-border,(ch*(a+1))-border,4,false);
					if (DisplayedScreen==MAIN_MENU_SCREEN)
					{
						if (a==2)
						{
							if (bitRead(StatusFlags,LightsOnFlag))
								Font.DrawCenterTextP(twidth/4,12+(border*3)+(ch*a),MENU_BUTTON_CANCEL);
							else
								Font.DrawCenterTextP(twidth/4,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
						}
						else
						{
							Font.DrawCenterTextP(twidth/4,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[a*2])));
						}
						Font.DrawCenterTextP(twidth/4,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items1[(a*2)+1])));
						Font.DrawCenterTextP(twidth/4*3,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[a*2])));
						Font.DrawCenterTextP(twidth/4*3,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items2[(a*2)+1])));
					}
					else if(DisplayedScreen==CALIBRATE_MENU_SCREEN)
					{
						Font.DrawCenterTextP(twidth/4,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items3[a*2])));
						Font.DrawCenterTextP(twidth/4,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items3[(a*2)+1])));
						Font.DrawCenterTextP(twidth/4*3,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items4[a*2])));
						Font.DrawCenterTextP(twidth/4*3,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items4[(a*2)+1])));
					}
					else if(DisplayedScreen==CALIBRATE_CUSTOM_MENU_SCREEN)
					{
						Font.DrawCenterTextP(twidth/4,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items5[a*2])));
						Font.DrawCenterTextP(twidth/4,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items5[(a*2)+1])));
						Font.DrawCenterTextP(twidth/4*3,12+(border*3)+(ch*a),(char * )pgm_read_word(&(menu_button_items6[a*2])));
						Font.DrawCenterTextP(twidth/4*3,26+(border*4)+(ch*a),(char * )pgm_read_word(&(menu_button_items6[(a*2)+1])));
					}
				}
			}
		}
	}
}

void ReefAngelClass::ProcessTouch()
{
	int i,j;
	byte TempRelay,TempRelayOn,TempRelayOff;
	byte numexp=0;

	WDTReset();
	if ((EM&(1<<3))!=0) numexp++;
	if ((EM&(1<<4))!=0) numexp++;
	if ((EM&(1<<6))!=0) numexp++;
	if ((EM&(1<<7))!=0) numexp++;

	menutimeout=now();
	// Check for Soft Reset of screen
	// If top left corner is long touched, we reinitialize the LCD.
//				if (TS.X<50 && TS.Y<50) // top left corner
//				{
//					if (LongTouch++==255) // 255 count is about 6 seconds
//					{
//						LongTouch=0;
//						NeedsRedraw=true;
//						TouchLCD.Init();
//					}
//				}
	if (Sleeping)
	{
		Sleeping=false;
		TouchEnabled=false;
		return;
	}
	if (TouchEnabled)
	{
		TouchEnabled=false;
		int twidth=TouchLCD.GetWidth();
		int theight=TouchLCD.GetHeight();

		if (DisplayedMenu==TOUCH_MENU)
		{
			if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
				ChangeDisplayedScreen(-1);
			if (TS.X>twidth-50 && TS.Y>theight-30)
				ChangeDisplayedScreen(1);
			int ch;
			int border;
			if (orientation%2==0)
			{
				ch=42;
				border=0;
			}
			else
			{
				ch=57;
				border=2;
			}
			for (int a=0;a<5;a++)
			{
				if (TS.IsTouchedInside(0,5+(ch*a),twidth/2,5+ch+(ch*a)))
				{
					if(DisplayedScreen==MAIN_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions1[a]; // Assing function pointers when buttons are touched
					else if(DisplayedScreen==CALIBRATE_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions3[a]; // Assing function pointers when buttons are touched
					else if(DisplayedScreen==CALIBRATE_CUSTOM_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions5[a]; // Assing function pointers when buttons are touched
				}
				if (TS.IsTouchedInside(twidth/2,5+(ch*a),twidth,5+ch+(ch*a)))
				{
					if(DisplayedScreen==MAIN_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions2[a]; // Assing function pointers when buttons are touched
					else if(DisplayedScreen==CALIBRATE_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions4[a]; // Assing function pointers when buttons are touched
					else if(DisplayedScreen==CALIBRATE_CUSTOM_MENU_SCREEN)
						MenuFunctionPtr=menu_button_functions6[a]; // Assing function pointers when buttons are touched
				}
			}
			if (MenuFunctionPtr!=&ReefAngelClass::Touch) // check to see if any menu choice was touched
			{
				(this->*MenuFunctionPtr)(); // call function
				NeedsRedraw=true;
				MenuFunctionPtr=&ReefAngelClass::Touch; // put pointer back to default
			}
		}
		if (DisplayedMenu==DEFAULT_MENU || DisplayedMenu==MAIN_MENU)
		{
			if (DisplayedScreen<MAX_SCREENS)
			{
				int j;
				if (TS.X<50 && TS.Y>theight-30 && TS.X>0)
					ChangeDisplayedScreen(-1);
				if (TS.X>twidth-50 && TS.Y>theight-30)
					ChangeDisplayedScreen(1);
				if (TS.X<(twidth/2+50) && TS.X>(twidth/2-50) && TS.Y>theight-30)
					ShowTouchMenu();

				if(DisplayedScreen==DIMMING_ATO)
				{
					if (Board==RAStar)
						j=90+(i*3);
					else
						j=95+(i*4);
					if (TS.IsTouchedInside(0,j,twidth,j+20))
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
						Slider.SetLabelP(LABEL_DAYLIGHT);
					}
					j+=30+i;
					if (TS.IsTouchedInside(0,j,twidth,j+20))
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
						Slider.SetLabelP(LABEL_ACTINIC);
					}
					j+=30+i;
					if (TS.IsTouchedInside(0,j,twidth,j+20))
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
						Slider.SetLabelP(LABEL_DAYLIGHT2);
					}
					j+=30+i;
					if (TS.IsTouchedInside(0,j,twidth,j+20))
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
						Slider.SetLabelP(LABEL_ACTINIC2);
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
						TempRelay=Relay.RelayDataE[DisplayedScreen-EXP_BOX_1];
						TempRelayOn=Relay.RelayMaskOnE[DisplayedScreen-EXP_BOX_1];
						TempRelayOff=Relay.RelayMaskOffE[DisplayedScreen-EXP_BOX_1];
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
								SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-RELAY_BOX)*10,0);
	#endif // RA_TOUCHDISPLAY
								Relay.Override(a+(DisplayedScreen-RELAY_BOX)*10,0);
							}
							else if (!bitRead(TempRelayOff,a-1))
							{
	#ifdef RA_TOUCHDISPLAY
								SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-RELAY_BOX)*10,1);
	#endif // RA_TOUCHDISPLAY
								Relay.Override(a+(DisplayedScreen-RELAY_BOX)*10,1);
							}
							else if (!bitRead(TempRelayOn,a-1) && bitRead(TempRelayOff,a-1))
							{
	#ifdef RA_TOUCHDISPLAY
								SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-RELAY_BOX)*10,!bitRead(TempRelay,a-1));
	#endif // RA_TOUCHDISPLAY
								if (bitRead(TempRelay,a-1))
								{
									Relay.Override(a+(DisplayedScreen-RELAY_BOX)*10,0);
								}
								else
								{
									Relay.Override(a+(DisplayedScreen-RELAY_BOX)*10,1);
								}
							}
						}
						if (TS.IsTouchedInside(x1,k-h,x1+74,k+h))
						{
	#ifdef RA_TOUCHDISPLAY
							SendMaster(MESSAGE_RELAY_OVERRIDE,a+(DisplayedScreen-RELAY_BOX)*10,2);
	#endif // RA_TOUCHDISPLAY
							Relay.Override(a+(DisplayedScreen-RELAY_BOX)*10,2);
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
							Slider.SetLabelP((char * )pgm_read_word(&(LABEL_PWME[a])));
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
							Slider.SetLabelP((char * )pgm_read_word(&(LABEL_AI[a])));
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
							Slider.SetLabelP((char * )pgm_read_word(&(LABEL_RF[a])));
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
					bool bDone=false;
					TS.GetTouch();
					TouchEnabled=true;
					byte oid=Slider.GetOverrideID();
					byte ovalue=Slider.GetCurrent();
					if (OkButton.IsPressed())
					{
						bDone=true;
#ifdef RA_TOUCHDISPLAY
						SendMaster(MESSAGE_CHANNEL_OVERRIDE,oid,ovalue); 	// Send Override Request
#endif // RA_TOUCHDISPLAY
						if (oid<=OVERRIDE_CHANNEL5 || oid==OVERRIDE_DAYLIGHT2 || oid==OVERRIDE_ACTINIC2)
#if defined(__SAM3X8E__)
							ReefAngel.VariableControl.Override(oid,ovalue);
#else
							ReefAngel.PWM.Override(oid,ovalue);
#endif
#ifdef AI_LED
						if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_ROYALBLUE)
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
						if (oid<=OVERRIDE_CHANNEL5 || oid==OVERRIDE_DAYLIGHT2 || oid==OVERRIDE_ACTINIC2)
#if defined(__SAM3X8E__)
							ReefAngel.VariableControl.Override(oid,255);
#else
							ReefAngel.PWM.Override(oid,255);
#endif
#ifdef AI_LED
						if (oid>=OVERRIDE_AI_WHITE && oid<=OVERRIDE_AI_ROYALBLUE)
							ReefAngel.AI.Override(oid-OVERRIDE_AI_WHITE,255);
#endif // AI_LED
#ifdef RFEXPANSION
						if (oid>=OVERRIDE_RF_WHITE && oid<=OVERRIDE_RF_INTENSITY)
							ReefAngel.RF.Override(oid-OVERRIDE_RF_WHITE,255);
#endif // RFEXPANSION
					}
					if (bDone)
					{
						DisplayedScreen=RecallScreen;
						NeedsRedraw=true;
					}
					Slider.Refresh();
				}
			}
		}
	}
}

void ReefAngelClass::CheckTouch()
{
	if (TS.IsTouched())
	{
		WDTReset();
		ProcessTouch();
		WDTReset();
		ResetScreenSaver();
	}
	else
	{
		TouchEnabled=true;
	}
}

#ifdef VersionMenu
void ReefAngelClass::DisplayVersion()
{
	LargeFont.DrawTextP(WARNING_TEXT,BKCOLOR,20,20,LABEL_REEFANGEL);
	LargeFont.DrawText(WARNING_TEXT,BKCOLOR,20,20,"v"ReefAngel_Version);
}
#endif  // VersionMenu

void ReefAngelClass::ClearScreen(byte Color)
{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION || defined RA_STAR
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

void ButtonClass::Create(int icolor, int itextcolor, const char  *istr)
{
	color=icolor;
	textcolor=itextcolor;
	str=istr;
}

void ButtonClass::Create(int icolor, int itextcolor, const char  *istr, const unsigned char *iPtr)
{
	Ptr=iPtr;
	Create(icolor,itextcolor,istr);
}

void ButtonClass::Show()
{
	visible=true;
	ReefAngel.Font.SetColor(COLOR_GRAY,COLOR_WHITE,true);
	ReefAngel.Font.DrawTextP(x1+26,y1+9,str); // This drawing is just to calculate the width of str
	x2=ReefAngel.Font.GetX()+25; // we add 25px in each side of str
	//	ReefAngel.TouchLCD.DrawRoundRect(RGB565(0xD0, 0xD0, 0xD0),x1+1,y1+1,x2+1,y1+41,10,false);
	ReefAngel.TouchLCD.DrawRoundRect(COLOR_GRAY,x1,y1,x2,y1+26,8,false);
	ReefAngel.TouchLCD.DrawRoundRect(color,x1+1,y1+1,x2-1,y1+25,8,true);
	if (Ptr!=0)
		ReefAngel.TouchLCD.DrawBMP(x1+5,y1+5,Ptr);
	ReefAngel.Font.SetColor(COLOR_GRAY88,COLOR_WHITE,true);
	ReefAngel.Font.DrawTextP(x1+26,y1+9,str);
	ReefAngel.Font.SetColor(textcolor,COLOR_WHITE,true);
	ReefAngel.Font.DrawTextP(x1+25,y1+8,str);
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
		ReefAngel.Font.DrawTextP(LABEL_PERCENTAGE);
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
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
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
