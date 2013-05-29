// Menu Headings
const prog_char Menu_0_label[] PROGMEM = "Main:";
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
const prog_char Menu_1_label[] PROGMEM = "Setup:";
#ifndef RemoveAllLights
const prog_char Menu_2_label[] PROGMEM = "Lights:";
#endif  // RemoveAllLights
const prog_char Menu_3_label[] PROGMEM = "Temp:";
#if defined SetupExtras || defined ATOSetup
const prog_char Menu_4_label[] PROGMEM = "Timeouts:";
#endif  // defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
PROGMEM const char * const menulabel_items[] = {
	Menu_0_label,
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
	Menu_1_label,
#ifndef RemoveAllLights
	Menu_2_label,
#endif  // RemoveAllLights
	Menu_3_label,
#if defined SetupExtras || defined ATOSetup
	Menu_4_label,
#endif  // defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
};

// Return menu entries
const prog_char Return_0_label[] PROGMEM = "<- Prev Menu";
const prog_char Return_1_label[] PROGMEM = "Exit";
//const prog_char Return_2_label[] PROGMEM = "Press to exit...";
PROGMEM const char * const return_items[] = { Return_0_label, Return_1_label/*, Return_2_label*/ };

// NOTE for nested menus
// NOTE Menu labels can be a max of 20 characters
// Associate a menu name to a numeric value
// the total number must match the max number of menus

enum Menus {
    MainMenu,
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
    SetupMenu,
#ifndef RemoveAllLights
    LightsMenu,
#endif  // RemoveAllLights
    TempsMenu,
#if defined SetupExtras || defined ATOSetup
    TimeoutsMenu,
#endif  // if defined SetupExtras || defined ATOSetup
//	PreviousMenu,
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
//	ExitMenu,
};

#ifdef CUSTOM_MENU
enum CustomMenuItem {
	CustomMenu_1,
#if CUSTOM_MENU_ENTRIES >= 2
	CustomMenu_2,
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
	CustomMenu_3,
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
	CustomMenu_4,
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
	CustomMenu_5,
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
	CustomMenu_6,
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
	CustomMenu_7,
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
	CustomMenu_8,
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
	CustomMenu_9,
#endif  // CUSTOM_MENU_ENTRIES >= 9
};
#else  // CUSTOM_MENU
#ifdef SIMPLE_MENU
// This is the simplified menu
// Main Menu
const prog_char mainmenu_0_label[] PROGMEM = "Feeding";
const prog_char mainmenu_1_label[] PROGMEM = "Water Change";
const prog_char mainmenu_2_label[] PROGMEM = "ATO Clear";
const prog_char mainmenu_3_label[] PROGMEM = "Overheat Clear";
const prog_char mainmenu_4_label[] PROGMEM = "PH Calibration";
#ifdef SALINITYEXPANSION
const prog_char mainmenu_5_label[] PROGMEM = "Sal Calibration";
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
const prog_char mainmenu_6_label[] PROGMEM = "ORP Calibration";
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
const prog_char mainmenu_7_label[] PROGMEM = "PH Exp Calibration";
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
const prog_char mainmenu_8_label[] PROGMEM = "Water Calibration";
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
const prog_char mainmenu_9_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
#ifdef VersionMenu
const prog_char mainmenu_10_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char * const mainmenu_items[] = {
                    mainmenu_0_label,
                    mainmenu_1_label,
                    mainmenu_2_label,
                    mainmenu_3_label,
                    mainmenu_4_label,
#ifdef SALINITYEXPANSION
					mainmenu_5_label,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
                    mainmenu_6_label,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
					mainmenu_7_label,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
					mainmenu_8_label,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
                    mainmenu_9_label,
#endif  // DateTimeSetup
#ifdef VersionMenu
                    mainmenu_10_label
#endif  // VersionMenu
                    };
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
    MainMenu_ATOClear,
    MainMenu_OverheatClear,
    MainMenu_PHCalibration,
#ifdef SALINITYEXPANSION
	MainMenu_SalinityCalibration,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
    MainMenu_ORPCalibration,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	MainMenu_PHExpCalibration,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	MainMenu_WaterCalibration,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
    MainMenu_DateTime,
#endif  // DateTimeSetup
#ifdef VersionMenu
    MainMenu_Version
#endif  // VersionMenu
};

#else  // SIMPLE_MENU
// This is the standard menu

// Main Menu
const prog_char mainmenu_0_label[] PROGMEM = "Feeding";
const prog_char mainmenu_1_label[] PROGMEM = "Water Change";
#ifndef RemoveAllLights
const prog_char mainmenu_2_label[] PROGMEM = "Lights ->";
#endif  // RemoveAllLights
const prog_char mainmenu_3_label[] PROGMEM = "Temps ->";
#if defined SetupExtras || defined ATOSetup
const prog_char mainmenu_4_label[] PROGMEM = "Timeouts ->";
#endif  // if defined SetupExtras || defined ATOSetup
const prog_char mainmenu_5_label[] PROGMEM = "Setup ->";
#ifdef VersionMenu
const prog_char mainmenu_6_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char * const mainmenu_items[] = {
                    mainmenu_0_label,
                    mainmenu_1_label,
#ifndef RemoveAllLights
                    mainmenu_2_label,
#endif  // RemoveAllLights
                    mainmenu_3_label,
#if defined SetupExtras || defined ATOSetup
                    mainmenu_4_label,
#endif  // if defined SetupExtras || defined ATOSetup
                    mainmenu_5_label,
#ifdef VersionMenu
                    mainmenu_6_label
#endif  // VersionMenu
                    };
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
#ifndef RemoveAllLights
    MainMenu_Lights,
#endif  // RemoveAllLights
    MainMenu_Temps,
#if defined SetupExtras || defined ATOSetup
    MainMenu_Timeouts,
#endif  // if defined SetupExtras || defined ATOSetup
    MainMenu_Setup,
#ifdef VersionMenu
    MainMenu_Version
#endif  // VersionMenu
};


// Setup MenuWavemakerSetup
#ifdef WavemakerSetup
const prog_char setupmenu_0_label[] PROGMEM = "Wavemaker";
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
const prog_char setupmenu_1_label[] PROGMEM = "Single Dose";
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
const prog_char setupmenu_2_label[] PROGMEM = "Multi Dose";
#endif  // DosingPumpIntervalSetup
const prog_char setupmenu_3_label[] PROGMEM = "Calibrate pH";
#ifdef SALINITYEXPANSION
const prog_char setupmenu_4_label[] PROGMEM = "Calibrate Sal";
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
const prog_char setupmenu_5_label[] PROGMEM = "Calibrate ORP";
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
const prog_char setupmenu_6_label[] PROGMEM = "Calibrate PH Exp";
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
const prog_char setupmenu_7_label[] PROGMEM = "Calibrate Water";
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
const prog_char setupmenu_8_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
PROGMEM const char * const setupmenu_items[] = {
#ifdef WavemakerSetup
                    setupmenu_0_label,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
                    setupmenu_1_label,
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
					setupmenu_2_label,
#endif  // DosingPumpIntervalSetup
                    setupmenu_3_label,
#ifdef SALINITYEXPANSION
					setupmenu_4_label,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
                    setupmenu_5_label,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
					setupmenu_6_label,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
					setupmenu_7_label,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
                    setupmenu_8_label
#endif  // DateTimeSetup
                    };
enum SetupMenuItem {
#ifdef WavemakerSetup
    SetupMenu_Wavemaker,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
    SetupMenu_DosingPump,
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
	SetupMenu_DosingPumpInterval,
#endif  // DosingPumpIntervalSetup
    SetupMenu_CalibratePH,
#ifdef SALINITYEXPANSION
	SetupMenu_CalibrateSalinity,
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
    SetupMenu_CalibrateORP,
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
	SetupMenu_PHExpCalibration,
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
	SetupMenu_WaterCalibration,
#endif  // WATERLEVELEXPANSION
#ifdef DateTimeSetup
    SetupMenu_DateTime
#endif  // DateTimeSetup
};

#ifndef RemoveAllLights
// Lights Menu
const prog_char lightsmenu_0_label[] PROGMEM = "Lights On";
const prog_char lightsmenu_1_label[] PROGMEM = "Lights Off";
#ifdef MetalHalideSetup
const prog_char lightsmenu_2_label[] PROGMEM = "Metal Halides";
const prog_char lightsmenu_3_label[] PROGMEM = "MH On Delay";
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
const prog_char lightsmenu_4_label[] PROGMEM = "Standard Lights";
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
const prog_char lightsmenu_5_label[] PROGMEM = "LED PWM";
#endif  // DisplayLEDPWM
PROGMEM const char * const lightsmenu_items[] = {
                            lightsmenu_0_label, lightsmenu_1_label,
#ifdef MetalHalideSetup
                            lightsmenu_2_label,
                            lightsmenu_3_label,
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
                            lightsmenu_4_label,
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
                            lightsmenu_5_label
#endif  // DisplayLEDPWM
                            };
enum LightsMenuItem {
    LightsMenu_On,
    LightsMenu_Off,
#ifdef MetalHalideSetup
    LightsMenu_MetalHalides,
    LightsMenu_MetalHalideDelay,
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
    LightsMenu_StandardLights,
#endif  // StandardLightSetup
#ifdef DisplayLEDPWM
    LightsMenu_LEDPWM
#endif  // DisplayLEDPWM
};
#endif  // RemoveAllLights

// Temps Menu
const prog_char tempsmenu_0_label[] PROGMEM = "Heater";
const prog_char tempsmenu_1_label[] PROGMEM = "Chiller";
#ifdef OverheatSetup
const prog_char tempsmenu_2_label[] PROGMEM = "Overheat Set";
#endif  // OverheatSetup
const prog_char tempsmenu_3_label[] PROGMEM = "Overheat Clear";
PROGMEM const char * const tempsmenu_items[] = {
                        tempsmenu_0_label,
                        tempsmenu_1_label,
#ifdef OverheatSetup
                        tempsmenu_2_label,
#endif  // OverheatSetup
                        tempsmenu_3_label};
enum TempsMenuItem {
    TempsMenu_Heater,
    TempsMenu_Chiller,
#ifdef OverheatSetup
    TempsMenu_Overheat,
#endif  // OverheatSetup
    TempsMenu_OverheatClr
};

// Timeouts Menu
#if defined SetupExtras || defined ATOSetup
#ifdef ATOSetup
const prog_char timeoutsmenu_0_label[] PROGMEM = "ATO Set";
#ifdef SingleATOSetup
const prog_char timeoutsmenu_1_label[] PROGMEM = "ATO Interval";
#endif  // SingleATOSetup
const prog_char timeoutsmenu_2_label[] PROGMEM = "ATO Clear";
#endif  // ATOSetup
#ifdef SetupExtras
const prog_char timeoutsmenu_3_label[] PROGMEM = "Feeding";
const prog_char timeoutsmenu_4_label[] PROGMEM = "LCD";
#endif  // SetupExtras
PROGMEM const char * const timeoutsmenu_items[] = {
#ifdef ATOSetup
                        timeoutsmenu_0_label,
#ifdef SingleATOSetup
                        timeoutsmenu_1_label,
#endif  // SingleATOSetup
                        timeoutsmenu_2_label,
#endif  // ATOSetup
#ifdef SetupExtras
                        timeoutsmenu_3_label,
                        timeoutsmenu_4_label
#endif  // SetupExtras
                        };
enum TimeoutsMenuItem {
#ifdef ATOSetup
    TimeoutsMenu_ATOSet,
#ifdef SingleATOSetup
    TimeoutsMenu_ATOHrInterval,
#endif  // SingleATOSetup
    TimeoutsMenu_ATOClear,
#endif  // ATOSetup
#ifdef SetupExtras
    TimeoutsMenu_Feeding,
    TimeoutsMenu_LCD
#endif  // SetupExtras
};
#endif // if defined SetupExtras || defined ATOSetup

#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU

void ReefAngelClass::ShowInterface()
{
    Refresh();
    // are we displaying the menu or not??
    if ( showmenu )
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        DisplayMenuHeading();
        DisplayMenu();
    }
    else
    {
        // not displaying the menu, so we're gonna show the appropriate screen
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        switch ( DisplayedMenu )
        {
        	case TOUCH_MENU:
        	case DATE_TIME_MENU:
			case PH_CALIBRATE_MENU:
			case SAL_CALIBRATE_MENU:
			case ORP_CALIBRATE_MENU:
			case PHE_CALIBRATE_MENU:
			case WL_CALIBRATE_MENU:
			case DEFAULT_MENU:  // This is the home screen
			{
				// process screensaver timeout
				if ( Timer[LCD_TIMER].IsTriggered() )
				{
					// Screensaver timeout expired
					LCD.BacklightOff();
				}

				if ( Joystick.IsButtonPressed() )
				{
					// turn the backlight on
					LCD.BacklightOn();

					// TODO check Timer[LCD_TIMER] code
					if ( Timer[LCD_TIMER].Trigger == 0 )
					{
						Timer[LCD_TIMER].Start();
						return;
					}
					PrepMenuScreen();
					// get out of this function and display the menu
					return;
				}

				if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
				{
					// Turn backlight on
					LCD.BacklightOn();
					Timer[LCD_TIMER].Start();
				}

#ifdef CUSTOM_MAIN
				DrawCustomMain();
#else
				// display everything on the home screen except the graph
				// the graph is drawn/updated when we exit the main menu & when the parameters are saved
#ifdef DATETIME24
				LCD.DrawDateTimeISO8601(6, 112);
#else
				LCD.DrawDate(6, 112);
#endif // DATETIME24
#if defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params, PWM.GetDaylightValue(), PWM.GetActinicValue());
#else  // defined DisplayLEDPWM && ! defined RemoveAllLights
				LCD.DrawMonitor(15, 60, Params);
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
#if defined WDT || defined WDT_FORCE
				wdt_reset();
#endif  // defined WDT || defined WDT_FORCE

				byte TempRelay = Relay.RelayData;
				TempRelay &= Relay.RelayMaskOff;
				TempRelay |= Relay.RelayMaskOn;
				LCD.DrawOutletBox(12, 93, TempRelay);
#endif  // CUSTOM_MAIN
				// Process any checks/tests/events that can happen while displaying the home screen
				// This can be the timers for wavemakers or any overheat temperatures

				// process timers
				// If bus is locked, it will trigger wdt when drawing graph
				if ( Timer[STORE_PARAMS_TIMER].IsTriggered() && !BusLocked) // Only access eeprom if bus is not locked
				{
					int CurTemp;

					// Values are stored in the I2CEEPROM1
					taddr++;
					if ( taddr >= 120 ) taddr = 0;
					Timer[STORE_PARAMS_TIMER].Start();
					CurTemp = map(Params.Temp[T1_PROBE], T1LOW, T1HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.Temp[T2_PROBE], T2LOW, T2HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+120, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.Temp[T3_PROBE], T3LOW, T3HIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+240, CurTemp);
//					LCD.PutPixel(DefaultBGColor,1,1);
					CurTemp = map(Params.PH, PHLOW, PHHIGH, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					//LCD.PutPixel(DefaultBGColor,1,1);
					Memory.Write(taddr+360, CurTemp);
					LCD.PutPixel(DefaultBGColor,1,1);
					if ((taddr%10)==0) InternalMemory.T1Pointer_write(taddr);
#if defined WDT || defined WDT_FORCE
					wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
#ifdef CUSTOM_MAIN
					DrawCustomGraph();
#else
					LCD.DrawGraph(5, 5);
#endif  // CUSTOM_MAIN
				}
				break;
			}  // DEFAULT_MENU
			case FEEDING_MODE:
			{
				int t;
				bool bDone = false;
				t = Timer[FEEDING_TIMER].Trigger - now();
				if ( (t >= 0) && ! Timer[FEEDING_TIMER].IsTriggered() )
				{
					LCD.Clear(DefaultBGColor,60+(intlength(t)*5),100,100,108);
					LCD.DrawText(DefaultFGColor,DefaultBGColor,60,100,t);
					delay(200);  // to keep from redraw flicker on timer
				}
				else
				{
					bDone = true;
				}

				LastStart = now();  // Set the time normal mode is started
				if ( Joystick.IsButtonPressed() )
				{
					// joystick button pressed, so we stop the feeding mode
					bDone = true;
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
					ExitMenu();
				}
//				Relay.Write();
				break;
			}
			case WATERCHANGE_MODE:
			{
				LastStart = now();  // Set the time normal mode is started
				if ( Joystick.IsButtonPressed() )
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
					ExitMenu();
				}
//				Relay.Write();
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
				if ( Joystick.IsButtonPressed() )
				{
					// button is pressed, so we gotta exit out, show the menu & redraw it too
					redrawmenu = true;
					showmenu = true;
					Timer[FEEDING_TIMER].ForceTrigger();
					Timer[LCD_TIMER].Start();
				}
				break;
			}
        }  // switch DisplayedMenu
    }  // if showmenu
#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
}

void ReefAngelClass::PrepMenuScreen()
{
	// Clears the screen to draw the menu
	// Displays main menu, select first item, save existing menu
	ClearScreen(DefaultBGColor);
	SelectedMenuItem = DEFAULT_MENU_ITEM;
	PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(MAIN_MENU);
	showmenu = true;
	redrawmenu = true;
	menutimeout = now();
}

void ReefAngelClass::DisplayMenu()
{
    // redrawmenu should only get set from within this function when we move the joystick or press the button
    byte qty = menuqtysptr[DisplayedMenu];
    int ptr = menusptr[DisplayedMenu];

#if defined WDT || defined WDT_FORCE
	wdt_reset();
#endif  // defined WDT || defined WDT_FORCE

    if ( Joystick.IsUp() )
    {
        // process UP press
        if ( (--SelectedMenuItem) > qty )
        {
            // we're moving up and we hit the top of the list
            // gotta wrap down to the bottom of the list
            // qty - 1 gives us the last item in our list
            //SelectedMenuItem = qty - 1;
            // This allows us to add in our last item
            SelectedMenuItem = qty;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    if ( Joystick.IsDown() )
    {
        // process DOWN press
        // > allows for selection of last item, >= skips it
        if ( (++SelectedMenuItem) > qty )
        {
            // we've hit the bottom of the list
            // wrap around to the top of the list
            SelectedMenuItem = DEFAULT_MENU_ITEM;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    // TODO Have ability to customize menu timeout delay
    if ( (now() - menutimeout) > MENU_TIMEOUT )
    {
        // menu timeout returns to the main screen
        // skip all the other menu checks
        SelectedMenuItem = EXCEED_TIMEOUT_MENU;
		SetDisplayedMenu(EXCEED_TIMEOUT_MENU);
        ButtonPress++;
    }

    if ( Joystick.IsButtonPressed() )
    {
        // button gets pressed, so we need to handle the button press
        ProcessButtonPress();
        redrawmenu = true;
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        // Don't finish processing the rest of the menu
        return;
    }

    // don't redraw the menu if we don't have to
    if ( ! redrawmenu )
        return;

    byte i;
    byte bcolor, fcolor;
    char buffer[22];
    for ( i = 0; i <= qty; i++ )
    {
        bcolor = DefaultBGColor;
        fcolor = DefaultFGColor;
        if ( i < qty )
        {
            strcpy_P(buffer, (char *)ptr++);
        }
        else
        {
            // the last item in the list is either Exit or Prev Menu
            int ptr2 = pgm_read_word(&(return_items[0]));
            if ( DisplayedMenu == MainMenu )
            {
                ptr2 += strlen(Return_0_label) + 1;
            }
            strcpy_P(buffer, (char *)ptr2);
        }
        ptr += strlen(buffer);

        // change the background color on the selected menu entry
        if ( i == SelectedMenuItem )
        {
            bcolor = SelectionBGColor;
            fcolor = SelectionFGColor;
        }
        LCD.Clear(bcolor, MENU_START_COL-3,
                         (i*MENU_START_ROW)+MENU_HEADING_SIZE-1,
                          MENU_END_COL,
                         (i*MENU_START_ROW)+(MENU_HEADING_SIZE+MENU_ITEM_HEIGHT-1));
        LCD.DrawText(fcolor, bcolor, MENU_START_COL, (i*MENU_START_ROW)+MENU_HEADING_SIZE, buffer);
    }  // for i
    // once drawn, no need to redraw yet
    redrawmenu = false;
}

void ReefAngelClass::DisplayMenuHeading()
{
    // NOTE do we redraw the menu heading or not?  use same logic as with the menu
    if ( ! redrawmenu )
        return;

    char buffer[10];
    int ptr = pgm_read_word(&(menulabel_items[0]));

    switch ( DisplayedMenu )
    {
        default:
//            {
//                //strcpy(buffer, "Menu:");
//                sprintf(buffer, "Menu (%d):", MenuNum);
//            }
//            break;
        case MainMenu:
            {
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
        case SetupMenu:
            {
                ptr += strlen(Menu_0_label) + 1;
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#ifndef RemoveAllLights
        case LightsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // RemoveAllLights
        case TempsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + 2;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
            {
                ptr += strlen(Menu_0_label) + strlen(Menu_1_label) + strlen(Menu_3_label) + 3;
#ifndef RemoveAllLights
				ptr += strlen(Menu_2_label) + 1;
#endif  // RemoveAllLights
//                strcpy_P(buffer, (char*)ptr);
            }
            break;
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
    }  // switch MenuNum
    strcpy_P(buffer, (char*)ptr);

    // clear the line that has the menu heading on it
    LCD.Clear(DefaultBGColor, MENU_START_COL, MENU_START_ROW, MAX_X, MAX_Y);
    // Display the menu heading
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, buffer);
}

void ReefAngelClass::DisplayMenuEntry(char *text)
{
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, text);
    /*
    char buffer[18];
	int ptr = pgm_read_word(&(return_items[0]));
	ptr += strlen(Return_0_label) + strlen(Return_1_label) + 2;
	strcpy_P(buffer, (char *)ptr);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, buffer);
    */
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "Press to exit...");
}

#ifdef CUSTOM_MENU
void ReefAngelClass::InitMenu(int ptr, byte qty)
{
    // loads all the menus
    menusptr[MainMenu] = ptr;
    menuqtysptr[MainMenu] = qty;
    // initialize menus
    PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(DEFAULT_MENU);
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}
#else
void ReefAngelClass::InitMenus()
{
    // loads all the menus
    menusptr[MainMenu] = pgm_read_word(&(mainmenu_items[0]));
    menuqtysptr[MainMenu] = SIZE(mainmenu_items);

#ifndef SIMPLE_MENU
    menusptr[SetupMenu] = pgm_read_word(&(setupmenu_items[0]));
    menuqtysptr[SetupMenu] = SIZE(setupmenu_items);
#ifndef RemoveAllLights
    menusptr[LightsMenu] = pgm_read_word(&(lightsmenu_items[0]));
    menuqtysptr[LightsMenu] = SIZE(lightsmenu_items);
#endif  // RemoveAllLights
    menusptr[TempsMenu] = pgm_read_word(&(tempsmenu_items[0]));
    menuqtysptr[TempsMenu] = SIZE(tempsmenu_items);
#if defined SetupExtras || defined ATOSetup
    menusptr[TimeoutsMenu] = pgm_read_word(&(timeoutsmenu_items[0]));
    menuqtysptr[TimeoutsMenu] = SIZE(timeoutsmenu_items);
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // SIMPLE_MENU

    // initialize menus
    PreviousMenu = DEFAULT_MENU;
	SetDisplayedMenu(DEFAULT_MENU);
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}
#endif  // CUSTOM_MENU

void ReefAngelClass::ProcessButtonPress()
{
    bool bResetMenuTimeout = true;
    switch ( DisplayedMenu )
    {
        default:  // DEFAULT_MENU == 255
//        {
//            // Default Screen
            break;
//        }
        case MainMenu:
        {
#ifdef CUSTOM_MENU
			ProcessButtonPressCustom();
            break;
        }
#else
            ProcessButtonPressMain();
            break;
        }

#ifndef SIMPLE_MENU
        case SetupMenu:
        {
            ProcessButtonPressSetup();
            break;
        }
#ifndef RemoveAllLights
        case LightsMenu:
        {
            ProcessButtonPressLights();
            break;
        }
#endif  // RemoveAllLights
        case TempsMenu:
        {
            ProcessButtonPressTemps();
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
        {
            ProcessButtonPressTimeouts();
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU

        case EXCEED_TIMEOUT_MENU:
        {
            // we bypass all the other menus when the timeout has exceeded
            // we need to mimic the default action for the main menu
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            showmenu = false;
            bResetMenuTimeout = false;
            // we are exiting the menu, so draw the graph
			ExitMenu();
            break;
        }
    }
    // if a button was pressed, we have to reset the timeout after processing the press
    if ( bResetMenuTimeout )
    {
        menutimeout = now();
    }
}

#ifdef CUSTOM_MENU
void ReefAngelClass::ProcessButtonPressCustom()
{
	showmenu = false;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case CustomMenu_1:
        {
        	MenuEntry1();
            break;
        }
#if CUSTOM_MENU_ENTRIES >= 2
        case CustomMenu_2:
        {
        	MenuEntry2();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
        case CustomMenu_3:
        {
        	MenuEntry3();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
        case CustomMenu_4:
        {
        	MenuEntry4();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
        case CustomMenu_5:
        {
        	MenuEntry5();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
        case CustomMenu_6:
        {
        	MenuEntry6();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
        case CustomMenu_7:
        {
        	MenuEntry7();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
        case CustomMenu_8:
        {
        	MenuEntry8();
            break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
        case CustomMenu_9:
        {
        	MenuEntry9();
        	break;
        }
#endif  // CUSTOM_MENU_ENTRIES >= 9
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
			ExitMenu();
            break;
        }
    }
}

#else  // CUSTOM_MENU
void ReefAngelClass::ProcessButtonPressMain()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case MainMenu_FeedingMode:
        {
        	// run feeding mode
        	FeedingModeStart();
        	showmenu = false;
            break;
        }
        case MainMenu_WaterChangeMode:
        {
        	WaterChangeModeStart();
        	showmenu = false;
            break;
        }


#ifdef SIMPLE_MENU
		// Simplified menu
		case MainMenu_ATOClear:
		{
			ATOClear();
			DisplayMenuEntry("Clear ATO Timeout");
			showmenu = false;
			break;
		}
		case MainMenu_OverheatClear:
		{
			OverheatClear();
			DisplayMenuEntry("Clear Overheat");
			showmenu = false;
			break;
		}
		case MainMenu_PHCalibration:
		{
#if defined SETUP_CALIBRATEPH_CHOICE
			SetupCalibrateChoicePH();
#else
			SetupCalibratePH();
#endif
			break;
		}
#ifdef SALINITYEXPANSION
		case MainMenu_SalinityCalibration:
		{
			SetupCalibrateSalinity();
			break;
		}
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
        case MainMenu_ORPCalibration:
        {
            SetupCalibrateORP();
            break;
        }
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
		case MainMenu_PHExpCalibration:
		{
			SetupCalibratePHExp();
			break;
		}
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
		case MainMenu_WaterCalibration:
		{
			SetupCalibrateWaterLevel();
			break;
		}
#endif  // WATERLEVELEXPANSION
#if defined DateTimeSetup
		case MainMenu_DateTime:
		{
#ifdef DATETIME24
            SetupDateTime24();
#else
			SetupDateTime();
#endif //DATETIME24
			break;
		}
#endif  // DateTimeSetup

#else  // SIMPLE_MENU

		// Standard menus
#ifndef RemoveAllLights
        case MainMenu_Lights:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(LightsMenu);
            break;
        }
#endif  // RemoveAllLights
        case MainMenu_Temps:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(TempsMenu);
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case MainMenu_Timeouts:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(TimeoutsMenu);
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
        case MainMenu_Setup:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
        	SetDisplayedMenu(SetupMenu);
            break;
        }
#endif  // SIMPLE_MENU


#ifdef VersionMenu
        case MainMenu_Version:
        {
            DisplayVersion();
            // turn off the menu so we can wait for the button press to exit
            showmenu = false;
            break;
        }
#endif  // VersionMenu
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // disable the menu, display main screen
            showmenu = false;
            // When we exit the main menu, we will redraw the graph
			ExitMenu();
            break;
        }
    }
}

#ifndef SIMPLE_MENU
void ReefAngelClass::ProcessButtonPressSetup()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef WavemakerSetup
        case SetupMenu_Wavemaker:
        {
            int v = InternalMemory.WM1Timer_read();
            int y = InternalMemory.WM2Timer_read();
            if ( SetupOption(v, y, 0, 21600, 5, "s", "", "Setup Wavemakers", "WM1:", "WM2:") )
            {
                InternalMemory.WM1Timer_write(v);
                InternalMemory.WM2Timer_write(y);
//                Relay.On(WM1Port);
//                Relay.On(WM2Port);
//                Relay.Write();
            }
            break;
        }
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
        case SetupMenu_DosingPump:
        {
            SetupDosingPump();
            break;
        }
#endif  // DosingPumpSetup
#ifdef DosingPumpIntervalSetup
		case SetupMenu_DosingPumpInterval:
		{
            int v = InternalMemory.DP1RepeatInterval_read();
            int y = InternalMemory.DP2RepeatInterval_read();
            if ( SetupOption(v, y, 1, 1440, 4, "m", "", "Repeat Interval", "DP1:", "DP2:") )
            {
            	InternalMemory.DP1RepeatInterval_write(v);
            	InternalMemory.DP2RepeatInterval_write(y);
            }
            v = InternalMemory.DP1Timer_read();
            y = InternalMemory.DP2Timer_read();
            if ( SetupOption(v, y, 1, 255, 3, "s", "", "Run Time", "DP1:", "DP2:") )
            {
            	InternalMemory.DP1Timer_write(v);
            	InternalMemory.DP2Timer_write(y);
            }
			break;
		}
#endif  // DosingPumpIntervalSetup
        case SetupMenu_CalibratePH:
        {
#if defined SETUP_CALIBRATEPH_CHOICE
            SetupCalibrateChoicePH();
#else
            SetupCalibratePH();
#endif
            break;
        }
#ifdef SALINITYEXPANSION
		case SetupMenu_CalibrateSalinity:
		{
			SetupCalibrateSalinity();
			break;
		}
#endif  // SALINITYEXPANSION
#ifdef ORPEXPANSION
        case SetupMenu_CalibrateORP:
        {
            SetupCalibrateORP();
            break;
        }
#endif  // ORPEXPANSION
#ifdef PHEXPANSION
		case SetupMenu_PHExpCalibration:
		{
			SetupCalibratePHExp();
			break;
		}
#endif  // PHEXPANSION
#ifdef WATERLEVELEXPANSION
		case SetupMenu_WaterCalibration:
		{
			SetupCalibrateWaterLevel();
			break;
		}
#endif  // WATERLEVELEXPANSION
#if defined DateTimeSetup
        case SetupMenu_DateTime:
        {
#ifdef DATETIME24
            SetupDateTime24();
#else
			SetupDateTime();
#endif
            break;
        }
#endif  // DateTimeSetup
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}

#ifndef RemoveAllLights
void ReefAngelClass::ProcessButtonPressLights()
{
    showmenu = true;  // set to true when displaying setup screens
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case LightsMenu_On:
        {
        	LightsOn();
            DisplayMenuEntry("Lights On");
        	showmenu = false;
            break;
        }
        case LightsMenu_Off:
        {
        	LightsOff();
            DisplayMenuEntry("Restore Lights");
            showmenu = false;
            break;
        }
#ifdef MetalHalideSetup
        case LightsMenu_MetalHalides:
        {
            SetupLightsOptionDisplay(true);
            break;
        }
        case LightsMenu_MetalHalideDelay:
        {
            int v = InternalMemory.MHDelay_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 255, 3, "m", "", "Setup MH Delay", "", "") )
            {
                InternalMemory.MHDelay_write((uint8_t)v);
            }
            break;
        }
#endif  // MetalHalideSetup
#ifdef StandardLightSetup
        case LightsMenu_StandardLights:
        {
            SetupLightsOptionDisplay(false);
            break;
        }
#endif  // StandardLightSetup
#if defined DisplayLEDPWM && !defined REEFANGEL_MINI
        case LightsMenu_LEDPWM:
        {
            int v = InternalMemory.LEDPWMActinic_read();
            int y = InternalMemory.LEDPWMDaylight_read();
            if ( SetupOption(v, y, 0, 100, 3, "%", "", "Setup LED", "AP:", "DP:") )
            {
                InternalMemory.LEDPWMActinic_write((uint8_t)v);
                InternalMemory.LEDPWMDaylight_write((uint8_t)y);
                // Restore PWM values
                PWM.SetActinic((uint8_t)v);
                PWM.SetDaylight((uint8_t)y);
            }
            break;
        }
#endif  // defined DisplayLEDPWM && !defined REEFANGEL_MINI
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}
#endif  // RemoveAllLights

void ReefAngelClass::ProcessButtonPressTemps()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case TempsMenu_Heater:
        {
            int v = InternalMemory.HeaterTempOn_read();
            int y = InternalMemory.HeaterTempOff_read();
            int min, max;
            char *s = "0";
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_LOW_TEMP;
            	max = DEGREE_C_HIGH_TEMP;
            	s = "C";
            }
            else
            {
            	min = DEGREE_F_LOW_TEMP;
            	max = DEGREE_F_HIGH_TEMP;
            	s = "F";
            }
            if ( SetupOption(v, y, min, max, 3, s, ".", "Setup Heater", "On @", "Off @") )
            {
                InternalMemory.HeaterTempOn_write(v);
                InternalMemory.HeaterTempOff_write(y);
            }
            break;
        }
        case TempsMenu_Chiller:
        {
            int v = InternalMemory.ChillerTempOn_read();
            int y = InternalMemory.ChillerTempOff_read();
            int min, max;
            char *s = "0";
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_LOW_TEMP;
            	max = DEGREE_C_HIGH_TEMP;
            	s = "C";
            }
            else
            {
            	min = DEGREE_F_LOW_TEMP;
            	max = DEGREE_F_HIGH_TEMP;
            	s = "F";
            }
            if ( SetupOption(v, y, min, max, 3, s, ".", "Setup Chiller", "On @", "Off @") )
            {
                InternalMemory.ChillerTempOn_write(v);
                InternalMemory.ChillerTempOff_write(y);
            }
            break;
        }
#ifdef OverheatSetup
        case TempsMenu_Overheat:
        {
            int v = InternalMemory.OverheatTemp_read();
            int y = -1;
            int min, max;
            char *s = "0";
            byte d;
            if ( TempSensor.unit )
            {
            	min = DEGREE_C_OVERHEAT_LOW_TEMP;
            	max = DEGREE_C_OVERHEAT_HIGH_TEMP;
            	s = "C";
            	d = 3;
            }
            else
            {
            	min = DEGREE_F_OVERHEAT_LOW_TEMP;
            	max = DEGREE_F_OVERHEAT_HIGH_TEMP;
            	s = "F";
            	d = 4;
            }
            if ( SetupOption(v, y, min, max, d, s, ".", "Setup Overheat", "", "") )
            {
                InternalMemory.OverheatTemp_write(v);
            }
            break;
        }
#endif  // OverheatSetup
        case TempsMenu_OverheatClr:
        {
        	OverheatClear();
            DisplayMenuEntry("Clear Overheat");
            showmenu = false;
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}

#if defined SetupExtras || defined ATOSetup
void ReefAngelClass::ProcessButtonPressTimeouts()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef ATOSetup
        case TimeoutsMenu_ATOSet:
        {
#ifdef SingleATOSetup
            int v = InternalMemory.ATOTimeout_read();
            int y = InternalMemory.ATOHighTimeout_read();
            if ( SetupOption(v, y, 0, 255, 3, "s", "", "ATO Timeout", "Low:", "High:") )
            {
                InternalMemory.ATOTimeout_write((uint8_t)v);
                InternalMemory.ATOHighTimeout_write((uint8_t)y);
            }
#else  // SingleATOSetup
            int v = InternalMemory.ATOTimeout_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 255, 3, "s", "", "ATO Timeout", "", "") )
            {
                InternalMemory.ATOTimeout_write((uint8_t)v);
            }
#endif  // SingleATOSetup
            break;
        }
#ifdef SingleATOSetup
        case TimeoutsMenu_ATOHrInterval:
        {
            int v = InternalMemory.ATOHourInterval_read();
            int y = InternalMemory.ATOHighHourInterval_read();
            if ( SetupOption(v, y, 0, 24, 2, "h", "", "ATO Interval", "Low:", "High:") )
            {
                InternalMemory.ATOHourInterval_write((uint8_t)v);
                InternalMemory.ATOHighHourInterval_write((uint8_t)y);
            }
            break;
        }
#endif  // SingleATOSetup
        case TimeoutsMenu_ATOClear:
        {
        	ATOClear();
            DisplayMenuEntry("Clear ATO Timeout");
            showmenu = false;
            break;
        }
#endif  // ATOSetup
#ifdef SetupExtras
        case TimeoutsMenu_Feeding:
        {
            int v = InternalMemory.FeedingTimer_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 3600, 4, "s", "", "Feeding Timer", "", "") )
            {
                InternalMemory.FeedingTimer_write(v);
                // update the feeding timer value
                Timer[FEEDING_TIMER].SetInterval(v);
            }
            break;
        }
        case TimeoutsMenu_LCD:
        {
            int v = InternalMemory.LCDTimer_read();
            int y = -1;
            if ( SetupOption(v, y, 0, 3600, 4, "s", "", "Screen Timeout", "", "") )
            {
                InternalMemory.LCDTimer_write(v);
                // update the timer value
                Timer[LCD_TIMER].SetInterval(v);
                Timer[LCD_TIMER].Start();
            }
            break;
        }
#endif  // SetupExtras
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
        	SetDisplayedMenu(PreviousMenu);
            break;
        }
    }
}
#endif  // if defined SetupExtras || defined ATOSetup

// Setup Menu Screens
bool ReefAngelClass::SetupOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                       char* unit, char* subunit, char* title,
                       char* prefix1, char* prefix2)
{
    // return true to save value stored in out in memory
    enum choices {
        OPT1,
        OPT2,
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    bool bSingle = (y < 0) ? true : false;
    byte offset = 50;
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, title);
    // prefix for each option
    if ( ! bSingle )
    {
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, prefix1);
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, prefix2);
    }
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                    {
                        if ( ! bSingle )
                            LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        LCD.DrawOption(v, 1, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OPT2:
                    {
                        LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        LCD.DrawOption(y, 1, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OK:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            if ( ! bSingle )
                                LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(true);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case CANCEL:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            if ( ! bSingle )
                                LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(false);
                            LCD.DrawCancel(true);
                        }
                        break;
                    }
            }
            bRedraw = false;
            bDrawButtons = false;
        } // if bRedraw
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v++;
                if ( v > rangemax || v < rangemin)
                {
                    v = rangemin;
                }
            }
            else if ( sel == OPT2 )
            {
                y++;
                if ( y > rangemax || y < rangemin )
                {
                    y = rangemin;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v--;
                if ( v < rangemin || v > rangemax )
                {
                    v = rangemax;
                }
            }
            else if ( sel == OPT2 )
            {
                y--;
                if ( y < rangemin || y > rangemax )
                {
                    y = rangemax;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( bSingle && sel == OPT2 ) sel++; // advance again if single
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( bSingle && sel == OPT2 ) sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    // return true saves the value, false ignores the value
    return bSave;
}

void ReefAngelClass::SetupLightsOptionDisplay(bool bMetalHalide)
{
    enum choices {
        OPT1, // On Hour
        OPT2, // On Minute
        OPT3, // Off Hour
        OPT4, // Off Minute
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2;
    char msg[20];
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
#ifdef MetalHalideSetup
	strcpy(msg, "Metal Halide Setup");
	h1 = InternalMemory.MHOnHour_read();
	m1 = InternalMemory.MHOnMinute_read();
	h2 = InternalMemory.MHOffHour_read();
	m2 = InternalMemory.MHOffMinute_read();
#endif // MetalHalideSetup
#ifdef StandardLightSetup
	strcpy(msg, "Std Lights Setup");
	h1 = InternalMemory.StdLightsOnHour_read();
	m1 = InternalMemory.StdLightsOnMinute_read();
	h2 = InternalMemory.StdLightsOffHour_read();
	m2 = InternalMemory.StdLightsOffMinute_read();
#endif // StandardLightSetup
    ClearScreen(DefaultBGColor);
    // header / title
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, msg);
    // prefixes, draw in ':' between options
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*4, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "Off:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
        	byte WhichOption=0;
        	boolean OKStatus=false;
        	boolean CancelStatus=false;
            switch ( sel )
            {
                case OPT1:
                {
                	WhichOption=1;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT2:
                {
                	WhichOption=2;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT3:
                {
                	WhichOption=3;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OPT4:
                {
                	WhichOption=4;
                	OKStatus=false;
                	CancelStatus=false;
                    break;
                }
                case OK:
                {
                	WhichOption=0;
                	OKStatus=true;
                	CancelStatus=false;

                    break;
                }
                case CANCEL:
                {
                	WhichOption=0;
                	OKStatus=false;
                	CancelStatus=true;
                    break;
                }
            }
            LCD.DrawOption(h1, WhichOption==1, MENU_START_COL+offset_hr, MENU_START_ROW*4, "", "");
            LCD.DrawOption(m1, WhichOption==2, MENU_START_COL+offset_min, MENU_START_ROW*4, "", "");
            LCD.DrawOption(h2, WhichOption==3, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
            LCD.DrawOption(m2, WhichOption==4, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
            if ( bDrawButtons )
            {
                LCD.DrawOK(OKStatus);
                LCD.DrawCancel(CancelStatus);
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT4 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT4 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        if ( bMetalHalide )
        {
#ifdef MetalHalideSetup
            InternalMemory.MHOnHour_write(h1);
            InternalMemory.MHOnMinute_write(m1);
            InternalMemory.MHOffHour_write(h2);
            InternalMemory.MHOffMinute_write(m2);
#endif MetalHalideSetup
        }
        else
        {
#ifdef StandardLightSetup
            InternalMemory.StdLightsOnHour_write(h1);
            InternalMemory.StdLightsOnMinute_write(m1);
            InternalMemory.StdLightsOffHour_write(h2);
            InternalMemory.StdLightsOffMinute_write(m2);
#endif // StandardLightSetup
        }
    }
}
#endif  // SIMPLE_MENU
#endif  // CUSTOM_MENU

void ReefAngelClass::SetupCalibratePH()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {7,10};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL + 18, MENU_START_ROW*5, (int)iCal[b]);
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<30;a++)
			{
				iO[b] += analogRead(PHPin);
			}
			iO[b]/=30;
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
        // save PHMin & PHMax to memory
        InternalMemory.PHMin_write(iO[0]);
        PHMin = iO[0];
        InternalMemory.PHMax_write(iO[1]);
		PHMax = iO[1];
	}
}

#ifdef SETUP_CALIBRATEPH_CHOICE
void ReefAngelClass::SetupCalibrateChoicePH()
{
	enum choices {
        TARGETPH,
        CANCEL,
        OK
    };
    byte sel = CANCEL;
	
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    unsigned int iStart[2] = {7,10};
    unsigned int iTarget[2] = {0,0};
    unsigned int iValue[2] = {0,0};
    char msg[15];
	unsigned int maxPh = 10;
	unsigned int minPh = 4;
    byte offset = 65;
		
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
		bRedraw = true;
    	bDrawButtons = true;
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate pH");		
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, "pH");
		
		strcpy(msg, b==0 ? "First value\0" : "Second value\0");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, msg);
		
		iTarget[b] = iStart[b];
		if(b==1 && iTarget[0]==iTarget[b])
		{
			iTarget[b]--;
		}
	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iValue[b]=0;
			for (int a=0;a<30;a++)
			{
				iValue[b] += analogRead(PHPin);
			}
			iValue[b]/=30;
			LCD.DrawCalibrate(iValue[b], MENU_START_COL + offset, MENU_START_ROW*6);
			
			
			if ( bRedraw )
			{
				switch ( sel )
				{
					case TARGETPH:
					{
						LCD.DrawOption(iTarget[b], 1, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
						if ( bDrawButtons )
						{
							LCD.DrawOK(false);
							LCD.DrawCancel(false);
						}
						break;
					}
					case OK:
					{
						if ( bDrawButtons )
						{
							LCD.DrawOption(iTarget[b], 0, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
							LCD.DrawOK(true);
							LCD.DrawCancel(false);
						}
						break;
					}
					case CANCEL:
					{
						if ( bDrawButtons )
						{
							LCD.DrawOption(iTarget[b], 0, MENU_START_COL + 18, MENU_START_ROW*6, "", "", 2);
							LCD.DrawOK(false);
							LCD.DrawCancel(true);
						}
						break;
					}
				}
				bRedraw = false;
				bDrawButtons = false;
			}
			
			if ( Joystick.IsUp() )
			{
				if (sel == TARGETPH)
				{
					iTarget[b]++;
					if(b==1 && iTarget[0]==iTarget[b])
					{
						if((iTarget[b] + 1) <= maxPh)
						{
							iTarget[b]++;
						} else {
							iTarget[b]--;
						}
					}
					
					if ( iTarget[b] > maxPh )
					{
						iTarget[b] = maxPh;
					}
					else 
					{
						bRedraw = true;
					}
				}
			}
			if ( Joystick.IsDown() )
			{
				if (sel == TARGETPH)
				{
					iTarget[b]--;
					if(b==1 && iTarget[0]==iTarget[b])
					{
						if((iTarget[b] - 1) >= minPh)
						{
							iTarget[b]--;
						} else {
							iTarget[b]++;
						}
					}
					
					if ( iTarget[b] < minPh )
					{
						iTarget[b] = minPh;
					}
					else 
					{
						bRedraw = true;
					}
				}
			}
			
			if ( Joystick.IsLeft() )
			{
				bRedraw = true;
				bDrawButtons = true;
				sel--;
				if ( sel > OK )
				{
					sel = OK;
				}
			}
			
			if ( Joystick.IsRight() )
			{
				bRedraw = true;
				bDrawButtons = true;
				sel++;
				if ( sel > OK )
				{
					sel = TARGETPH;
				}
			}
			
			if ( Joystick.IsButtonPressed() )
			{
				if ( sel == OK || sel == TARGETPH)
				{
					bDone = true;
					bSave = true;
				}
				else if ( sel == CANCEL )
				{
					bDone = true;
				}
			}
		} while ( ! bDone );
    }
	
    ClearScreen(DefaultBGColor);
	
	if ( bSave )
	{
		PHMin = map(7.0, iTarget[0], iTarget[1], iValue[0], iValue[1]);
		PHMax = map(10.0, iTarget[0], iTarget[1], iValue[0], iValue[1]);
		
        // save PHMin & PHMax to memory
        InternalMemory.PHMin_write(PHMin);
        InternalMemory.PHMax_write(PHMax);
	}
}
#endif // SETUP_CALIBRATEPH_CHOICE

#ifdef SALINITYEXPANSION
void ReefAngelClass::SetupCalibrateSalinity()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iS = 0;
    byte offset = 65;
    // draw labels
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate");
    	LargeFont.DrawText(30, 50, "Salinity");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, "35 PPT");
#else //  REEFTOUCH
        ClearScreen(DefaultBGColor);
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate Salinity");
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "35 PPT");
#endif //  REEFTOUCH     
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
		iS=0;
	    for (int a=0;a<15;a++)
	    {
	    	iS += Salinity.Read();
	    }
	    iS/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iS);
	    	Font.DrawText("   ");
#else //  REEFTOUCH				    
        LCD.DrawCalibrate(iS, MENU_START_COL + offset, MENU_START_ROW*5);
        if (  bDrawButtons )
        {
            if ( bOKSel )
            {
                LCD.DrawOK(true);
                LCD.DrawCancel(false);
            }
            else
            {
                LCD.DrawOK(false);
                LCD.DrawCancel(true);
            }
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
        {
            // toggle the selection
            bOKSel = !bOKSel;
            bDrawButtons = true;
        }
        if ( Joystick.IsButtonPressed() )
        {
            bDone = true;
            if ( bOKSel )
            {
                bSave = true;
            }
        }
#endif //  REEFTOUCH     
    } while ( ! bDone );

    if ( bSave )
    {
        // save SalMax to memory
        InternalMemory.SalMax_write(iS);
        InternalMemory.SalTempComp_write(Params.Temp[TempProbe]);
		SalMax = iS;
    }
}

#endif  // SALINITYEXPANSION

#ifdef ORPEXPANSION
void ReefAngelClass::SetupCalibrateORP()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {0,470};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate ORP");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, iCal[b]);
    	Font.DrawText(" mV  ");
#else //  REEFTOUCH
        LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate ORP");
		char text[10];
		itoa(iCal[b],text,10);
		strcat(text , " mV  ");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, text);
#endif //  REEFTOUCH  
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += ORP.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH     
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save ORPMin & ORPMax to memory
		InternalMemory.ORPMin_write(iO[0]);
		ORPMin = iO[0];
		InternalMemory.ORPMax_write(iO[1]);
		ORPMax = iO[1];
	}
}
#endif  // ORPEXPANSION

#ifdef PHEXPANSION
void ReefAngelClass::SetupCalibratePHExp()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {7,10};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate PH");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, "pH ");
    	Font.DrawText(iCal[b]);
#else //  REEFTOUCH
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate PH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "pH");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL + 18, MENU_START_ROW*5, (int)iCal[b]);
#endif //  REEFTOUCH      	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += PH.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
	    	LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH 			
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save PHExpMin & PHExpMax to memory
		InternalMemory.PHExpMin_write(iO[0]);
		PHExpMin = iO[0];
		InternalMemory.PHExpMax_write(iO[1]);
		PHExpMax = iO[1];
	}
}
#endif  // PHEXPANSION

#ifdef WATERLEVELEXPANSION
void ReefAngelClass::SetupCalibrateWaterLevel()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    unsigned int iO[2] = {0,0};
    unsigned int iCal[2] = {0,100};
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    for (int b=0;b<2;b++)
    {
    	if (b==1 && !bSave) break;
    	bOKSel=false;
    	bSave=false;
    	bDone = false;
    	bDrawButtons = true;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
    	LargeFont.SetColor(WARNING_TEXT, BKCOLOR,false);
    	LargeFont.DrawText(30, 30, "Calibrate PH");
    	LargeFont.DrawText(30, 50, "Water Level");
    	Font.SetColor(WARNING_TEXT, BKCOLOR,false);
    	Font.DrawText(30, 60, iCal[b]);
    	Font.DrawText(" %  ");
#else //  REEFTOUCH
    	LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate Water Level");
		char text[10];
		itoa(iCal[b],text,10);
		strcat(text , " %  ");
		LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, text);
#endif //  REEFTOUCH    	
		do
		{
#if defined WDT || defined WDT_FORCE
			wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
			iO[b]=0;
			for (int a=0;a<15;a++)
			{
				iO[b] += WaterLevel.Read();
			}
			iO[b]/=15;
#if defined REEFTOUCH || defined REEFTOUCHDISPLAY		
	    	Font.DrawText(COLOR_RED, BKCOLOR,130, 60, iO[b]);
	    	Font.DrawText("   ");
#else //  REEFTOUCH		
			LCD.DrawCalibrate(iO[b], MENU_START_COL + offset, MENU_START_ROW*5);
			if (  bDrawButtons )
			{
				if ( bOKSel )
				{
					LCD.DrawOK(true);
					LCD.DrawCancel(false);
				}
				else
				{
					LCD.DrawOK(false);
					LCD.DrawCancel(true);
				}
				bDrawButtons = false;
			}
			if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
			{
				// toggle the selection
				bOKSel = !bOKSel;
				bDrawButtons = true;
			}
			if ( Joystick.IsButtonPressed() )
			{
				bDone = true;
				if ( bOKSel )
				{
					bSave = true;
				}
			}
#endif //  REEFTOUCH    			
		} while ( ! bDone );
    }
    ClearScreen(DefaultBGColor);
	if ( bSave )
	{
		// save WaterLevelMin & WaterLevelMax to memory
		InternalMemory.WaterLevelMin_write(iO[0]);
		InternalMemory.WaterLevelMax_write(iO[1]);
	}
}
#endif  // WATERLEVELEXPANSION

#if defined DateTimeSetup && !defined DATETIME24
void ReefAngelClass::SetupDateTime()
{
    enum choices {
        MONTH,
        DAY,
        YEAR,
        HOUR,
        MINUTE,
        OK,
        CANCEL
    };
    byte sel = MONTH;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte Year, Month, Day, Hour, Minute;
    byte MonthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    byte DateRow = 45, TimeRow = 75;

    Year = year() - 2000;
    Month = month();
    Day = day();
    Hour = hour();
    Minute = minute();

#if defined REEFTOUCH || defined REEFTOUCHDISPLAY
#else //  REEFTOUCH
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Set Date & Time");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, DateRow,"Date:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, TimeRow,"Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, TimeRow, ":");

    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    LCD.DrawOption(Month, 1, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case DAY:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 1, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case YEAR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 1, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case HOUR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 1, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case MINUTE:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 1, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }

            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month++;
                    if ( Month > 12 )
                    {
                        Month = 1;
                    }
                    break;
                }
                case DAY:
                {
                    Day++;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day > MonthDays[Month] )
                        {
                            Day = 1;
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day > 29 )
                            {
                                Day = 1;
                            }
                        }
                        else
                        {
                            if ( Day > MonthDays[Month] )
                            {
                                Day = 1;
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year++;
                    if ( Year > 99 )
                    {
                        Year = 0;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour++;
                    if ( Hour > 23 )
                    {
                        Hour = 0;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute++;
                    if ( Minute > 59 )
                    {
                        Minute = 0;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsDown() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month--;
                    if ( Month < 1 || Month > 12 )
                    {
                        Month = 12;
                    }
                    break;
                }
                case DAY:
                {
                    Day--;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day < 1 || Day > MonthDays[Month] )
                        {
                            Day = MonthDays[Month];
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = 29;
                            }
                        }
                        else
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = MonthDays[Month];
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year--;
                    if ( Year > 99 )
                    {
                        Year = 99;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour--;
                    if ( Hour > 23 )
                    {
                        Hour = 23;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute--;
                    if ( Minute > 59 )
                    {
                        Minute = 59;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = MONTH;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
#endif //  REEFTOUCH

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }

}
#endif  // DateTimeSetup


#if defined DateTimeSetup && defined DATETIME24
void ReefAngelClass::SetupDateTime24()
{
    enum choices {
        YEAR,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
        OK,
        CANCEL
    };
    byte sel = YEAR;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte Year, Month, Day, Hour, Minute;
    byte MonthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	uint8_t iTimeformat;
    byte DateRow = 35, TimeRow = 60, FormatRow = 85;

    Year = year() - 2000;
    Month = month();
    Day = day();
    Hour = hour();
    Minute = minute();

    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Set Date & Time");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, DateRow,"Date:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, TimeRow,"Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 57, DateRow, "20");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, DateRow, "-");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 102, DateRow, "-");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, TimeRow, ":");

    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
				case YEAR:
				{
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 1, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
					if ( bDrawButtons )
					{
						LCD.DrawOK(false);
						LCD.DrawCancel(false);
					}
					break;
				}
                case MONTH:
                {
                    LCD.DrawOption(Month, 1, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case DAY:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 1, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case HOUR:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 1, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case MINUTE:
                {
                    LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 1, 89, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }

                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 109, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 89, TimeRow, "", "", 2);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }

            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month++;
                    if ( Month > 12 )
                    {
                        Month = 1;
                    }
                    break;
                }
                case DAY:
                {
                    Day++;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day > MonthDays[Month] )
                        {
                            Day = 1;
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day > 29 )
                            {
                                Day = 1;
                            }
                        }
                        else
                        {
                            if ( Day > MonthDays[Month] )
                            {
                                Day = 1;
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year++;
                    if ( Year > 99 )
                    {
                        Year = 0;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour++;
                    if ( Hour > 23 )
                    {
                        Hour = 0;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute++;
                    if ( Minute > 59 )
                    {
                        Minute = 0;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsDown() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month--;
                    if ( Month < 1 || Month > 12 )
                    {
                        Month = 12;
                    }
                    break;
                }
                case DAY:
                {
                    Day--;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day < 1 || Day > MonthDays[Month] )
                        {
                            Day = MonthDays[Month];
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = 29;
                            }
                        }
                        else
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = MonthDays[Month];
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year--;
                    if ( Year > 99 )
                    {
                        Year = 99;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour--;
                    if ( Hour > 23 )
                    {
                        Hour = 23;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute--;
                    if ( Minute > 59 )
                    {
                        Minute = 59;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = YEAR;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }
}

#endif  // DATETIME24

#if !defined SIMPLE_MENU && !defined CUSTOM_MENU
#ifdef DosingPumpSetup
void ReefAngelClass::SetupDosingPump()
{
    enum choices {
        OPT1, // DP1 On Hour
        OPT2, // DP1 On Minute
        OPT3, // DP1 Run Time
        OPT4, // DP2 On Hour
        OPT5, // DP2 On Minute
        OPT6, // DP2 Run Time
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2, r1, r2;
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
    h1 = InternalMemory.DP1OnHour_read();
    m1 = InternalMemory.DP1OnMinute_read();
    r1 = InternalMemory.DP1Timer_read();
    h2 = InternalMemory.DP2OnHour_read();
    m2 = InternalMemory.DP2OnMinute_read();
    r2 = InternalMemory.DP2Timer_read();
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "DP Setup");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*3, "DP1 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*3, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "Run Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "DP2 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*9, "Run Time:");
    do
    {
#if defined WDT || defined WDT_FORCE
		wdt_reset();
#endif  // defined WDT || defined WDT_FORCE
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 1, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT2:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m1, 1, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT3:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 1, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT4:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 1, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT5:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m2, 1, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT6:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 1, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                r1++;
            }
            else if ( sel == OPT4 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT5 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
            else if ( sel == OPT6 )
            {
                r2++;
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                r1--;
            }
            else if ( sel == OPT4 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT5 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
            else if ( sel == OPT6 )
            {
                r2--;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        InternalMemory.DP1OnHour_write(h1);
        InternalMemory.DP1OnMinute_write(m1);
        InternalMemory.DP1Timer_write(r1);
        InternalMemory.DP2OnHour_write(h2);
        InternalMemory.DP2OnMinute_write(m2);
        InternalMemory.DP2Timer_write(r2);
    }
}
#endif  // DosingPumpSetup
#endif  // !defined SIMPLE_MENU && !defined CUSTOM_MENU
