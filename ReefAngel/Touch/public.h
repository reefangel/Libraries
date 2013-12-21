// Touch

DS1307RTC RTC;
RA_TouchLCD TouchLCD;
RA_TFT TFT;
FontClass SmallFont;
FontClass Font;
FontClass LargeFont;
RA_TS TS;
RA_Tilt Tilt;
ButtonClass OkButton;
ButtonClass CancelButton;
ProgressBarClass PB[6];
SliderClass Slider;
typedef void (ReefAngelClass::* FuncPtr) (); // declare function pointers
FuncPtr MenuFunctionPtr;
FuncPtr menu_button_functions1[6];
FuncPtr menu_button_functions2[6];
FuncPtr menu_button_functions3[6];
FuncPtr menu_button_functions4[6];

boolean Splash;
byte LastOrientation;
boolean MilitaryTime;
signed char DisplayedScreen;
signed char RecallScreen;
bool NeedsRedraw;
bool TouchEnabled;
bool Sleeping;
byte orientation;
byte LongTouch;
bool SDFound;
time_t newnow;
int CalVal1, CalVal2;
byte CalStep;

void SetOrientation(byte o);
void CalibrateTouchScreen();
void SaveInitialSettings();
void ChangeDisplayedScreen(signed char index);
void MainScreen();
void ResetScreenSaver();
void SetupTouchDateTime();
void SetupTouchCalibratePH();
void SetupTouchCalibrateSal();
void SetupTouchCalibrateORP();
void SetupTouchCalibratePHExp();
void SetupTouchCalibrateWL();
void ApplySalinityCompensation();
void CheckMenuTimeout();
void ShowTouchMenu();
void ShowTouchInterface();	