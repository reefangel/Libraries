// Port bits
#define Port8Bit   1<<7
#define Port7Bit   1<<6
#define Port6Bit   1<<5
#define Port5Bit   1<<4
#define Port4Bit   1<<3
#define Port3Bit   1<<2
#define Port2Bit   1<<1
#define Port1Bit   1<<0

#define MAX_RELAY_EXPANSION_MODULES     8
#define PWM_EXPANSION_CHANNELS     		6
#define IO_EXPANSION_CHANNELS     		6
#define WL_CHANNELS						5
#define CUSTOM_EXP_MODULES				8
#define TEMP_PROBES						2

#define CUSTOM_NONE	0
#define CUSTOM_SALINITY	1
#define CUSTOM_ORP	2
#define CUSTOM_PHEXP	3
#define CUSTOM_WL	4
#define CUSTOM_MULTI_WL1	5
#define CUSTOM_MULTI_WL2	6
#define CUSTOM_MULTI_WL3	7
#define CUSTOM_MULTI_WL4	8

typedef struct  {
  int Temp[TEMP_PROBES+1];
  int Salinity;
  int ORP;
  int PHExp;
  int PAR;
  byte WL[WL_CHANNELS];
  byte Leak;
  byte IO;
} ParamsStruct;

typedef struct  {
  int CalMin;
  int CalMax;
  int CalLow;
  int CalHigh;
} CustomCalStruct;

#define I2CIO_PCF8574       0x27
#define I2CExpModule        0x38 // 0x38-3f
#define I2CPWM_PCA9685		0x40
#define I2CLeak				0X48
#define I2CMultiWaterLevel	0X49
#define I2CPAR				0X4a
#define I2CORP				0X4c
#define I2CSalinity			0X4d
#define I2CPH				0X4e
#define I2CWaterLevel		0X4f
#define I2CEEPROM1          0x50

#define VarsStart                 200
#define Mem_I_SalMax			  VarsStart+47
#define Mem_I_ORPMin			  VarsStart+80
#define Mem_I_ORPMax			  VarsStart+82
#define Mem_I_PHExpMin			  VarsStart+121
#define Mem_I_PHExpMax			  VarsStart+123
#define Mem_I_WaterLevelMin		  VarsStart+125
#define Mem_I_WaterLevelMax		  VarsStart+127
#define Mem_I_WaterLevel1Min	  VarsStart+140
#define Mem_I_WaterLevel1Max	  VarsStart+142
#define Mem_I_WaterLevel2Min	  VarsStart+144
#define Mem_I_WaterLevel2Max	  VarsStart+146
#define Mem_I_WaterLevel3Min	  VarsStart+148
#define Mem_I_WaterLevel3Max	  VarsStart+150
#define Mem_I_WaterLevel4Min	  VarsStart+152
#define Mem_I_WaterLevel4Max	  VarsStart+154
#define Mem_I_CustomExp1Min		  VarsStart+179
#define Mem_I_CustomExp1Max		  VarsStart+181
#define Mem_I_CustomExp2Min		  VarsStart+183
#define Mem_I_CustomExp2Max		  VarsStart+185
#define Mem_I_CustomExp3Min		  VarsStart+187
#define Mem_I_CustomExp3Max		  VarsStart+189
#define Mem_I_CustomExp4Min		  VarsStart+191
#define Mem_I_CustomExp4Max		  VarsStart+193
#define Mem_I_CustomExp5Min		  VarsStart+195
#define Mem_I_CustomExp5Max		  VarsStart+197
#define Mem_I_CustomExp6Min		  VarsStart+199
#define Mem_I_CustomExp6Max		  VarsStart+201
#define Mem_I_CustomExp7Min		  VarsStart+203
#define Mem_I_CustomExp7Max		  VarsStart+205
#define Mem_I_CustomExp8Min		  VarsStart+207
#define Mem_I_CustomExp8Max		  VarsStart+209


static char username[16];
static char password[16];
static char clientid[22];
static char pub_buffer[20];
static char sub_buffer[23];
static char buffer[15];
static byte RelayDataE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayMaskOnE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayMaskOffE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayFallBackE[MAX_RELAY_EXPANSION_MODULES];
static byte ExpansionChannel[PWM_EXPANSION_CHANNELS];
static byte ExpansionChannelOverride[PWM_EXPANSION_CHANNELS];
static byte lastcrc;
static ParamsStruct Params;
static ParamsStruct OldParams;
static unsigned long Paramsmillis=millis();
static int CalVal1;
static int ORPMin, ORPMax;
static boolean ORPCal;
static int SalMax;
static boolean SalCal;
static int PHExpMin, PHExpMax;
static boolean PHExpCal;
static int WLMin[WL_CHANNELS], WLMax[WL_CHANNELS];
static boolean WLCal[WL_CHANNELS];
static byte CustomExpansion[CUSTOM_EXP_MODULES];
static char pub_salinity[16];
static char pub_orp[16];
static char pub_phexp[16];
static char pub_wl[16];
static char pub_multiwl[16];
static char pub_custom_wl[16];
static char pub_custom_multiwl[16];
static boolean SalinityFound;
static boolean ORPFound;
static boolean PHExpFound;
static boolean WLFound;
static boolean MultiWLFound;


#if defined(ARDUINO_ARCH_SAMD)
WiFiServer server(2000);
WiFiClient client;
WiFiClient portalclient;
WiFiClient mqttclient;
#else
static EthernetServer NetServer(2000);
static byte NetMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE0 };
static EthernetClient ethClient;
#endif // ARDUINO_ARCH_SAMD

static boolean FoundIP=false;
static unsigned long MQTTReconnectmillis=millis();
static unsigned long MQTTSendmillis=millis();