// Evolution

DS1307RTC I2CRTC;
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
RA_ATOALARMClass AlarmInput;

String CustomLabels[128];
ParamsStruct LastParams;
typedef void (ReefAngelClass::* FuncPtr) (); // declare function pointers
FuncPtr MenuFunctionPtr;
FuncPtr menu_button_functions1[6];
FuncPtr menu_button_functions2[6];
FuncPtr menu_button_functions3[6];
FuncPtr menu_button_functions4[6];

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

byte RANetSeq, RANetCRC;
byte RANetData[RANET_SIZE];
byte RANetStatus[RANET_SIZE];
unsigned long RANetlastmillis;

void SetOrientation(byte o);
void CalibrateTouchScreen();
void InitCustomLabels();
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

void BuzzerOn(byte style=0);
void BuzzerOff();
