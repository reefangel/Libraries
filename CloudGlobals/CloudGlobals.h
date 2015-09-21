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
#define AI_CHANNELS     				3
#define RF_CHANNELS						6

#define I2CRF				0X10
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
#define I2CHumidity			0x5c

static byte RelayDataE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayMaskOnE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayMaskOffE[MAX_RELAY_EXPANSION_MODULES];
static byte RelayFallBackE[MAX_RELAY_EXPANSION_MODULES];
static byte ExpansionChannel[PWM_EXPANSION_CHANNELS];
static byte ExpansionChannelOverride[PWM_EXPANSION_CHANNELS];
static byte lastcrc;

#define	MQTTPort	1883

static EthernetServer NetServer(2000);
static byte NetMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE0 };
static byte MQTTServer[] = { 69, 198, 171, 165 }; // forum.reefangel.com
static EthernetClient ethClient;
static boolean FoundIP=false;
static unsigned long MQTTReconnectmillis=millis();
static unsigned long MQTTSendmillis=millis();