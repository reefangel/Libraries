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

typedef struct  {
  int Salinity;
  int ORP;
  int PHExp;
  int PAR;
  byte WL[WL_CHANNELS];
  byte Leak;
  byte IO;
} ParamsStruct;

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

#define	MQTTPort	1883

static EthernetServer NetServer(2000);
static byte NetMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE0 };
static byte MQTTServer[] = { 69, 198, 171, 165 }; // forum.reefangel.com
static EthernetClient ethClient;
static boolean FoundIP=false;
static unsigned long MQTTReconnectmillis=millis();
static unsigned long MQTTSendmillis=millis();