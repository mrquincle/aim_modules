#ifndef RFXCOM_H
#define RFXCOM_H

#include <bitset>
#include <cstdint>

#define DEVICE_TRX_315_310_MHZ                   0x50
#define DEVICE_TRX_315_315_MHZ                   0x51
#define DEVICE_RFX_REC_433                       0x52
#define DEVICE_RFX_TRX_433                       0x53

#define CMD_OFF                                  0
#define CMD_ON                                   1

/**
 * Usage:
 *   std::bitset<8> msg5;
 *   msg5[MODE_X10] = 1;
 *   msg5.clear(); // clears every bit
 *   unsigned long value = msg5.to_ulong(); // return value as long integer
 */

// bits of msg5
#define MODE_X10                                 0
#define MODE_ARC                                 1
#define MODE_AC                                  2
#define MODE_HOMEEASY_EU                         3
#define MODE_MEIANTECH                           4
#define MODE_OREGON_SCIENTIFIC                   5
#define MODE_ATI                                 6
#define MODE_VISONIC                             7

// bits of msg4
#define MODE_MERTIK                              0
#define MODE_AD                                  1
#define MODE_HIDEKI                              2
#define MODE_LA_CROSSE                           3
#define MODE_FS20                                4
#define MODE_PROGUARD                            5
#define MODE_BLINDST0                            6
#define MODE_BLINDST1                            7

// bits in msg3
#define MODE_AE                                  0
#define MODE_RUBICSON                            1
#define MODE_FINEOFFSET                          2
#define MODE_LIGHTING4                           3
#define MODE_RSL                                 4
#define MODE_BYRON_SX                            5
#define MODE_RFU_PROTOCOL6                       6
#define MODE_UNDECODED_MESSAGES                  7

#define MSG_CONTROL                              0x00

#define MSG_CONTROL_MODE                         0x00

#define MSG_CONTROL_CMD_RESET                    0x00
#define MSG_CONTROL_CMD_GET_STATUS               0x02
#define MSG_CONTROL_CMD_SET_MODE                 0x03
#define MSG_CONTROL_CMD_ALL_RECV_MODES           0x04
#define MSG_CONTROL_CMD_REPORT_UNDECODED         0x05
#define MSG_CONTROL_CMD_SAVE_RECV_MODES          0x06


#define MSG_INTERFACE                            0x01
#define MSG_RECV_TRANS                           0x02
#define MSG_UNDECODED_RF                         0x03
#define MSG_LIGHTING1                            0x10

#define MSG_LIGHTING1_X10_LIGHTING               0x00
#define MSG_LIGHTING1_ARC                        0x01
#define MSG_LIGHTING1_ELRO_AB400D                0x02
#define MSG_LIGHTING1_WAVEMAN                    0x03
#define MSG_LIGHTING1_CHACON_EMW200              0x04
#define MSG_LIGHTING1_IMPULS                     0x05
#define MSG_LIGHTING1_RISING_SUN                 0x06


// ARC is used by different brands with units with code wheels A-P and 1-16
// so this means KlikAanKlikUit, Nexa, Chacon, HomeEasy, Proove, DomiaLite, InterTechno, etc.

#define MSG_LIGHTING1_X10                        0x00
#define MSG_LIGHTING1_ARC                        0x01
#define MSG_LIGHTING1_ELRO                       0x02
#define MSG_LIGHTING1_WAVEMAN                    0x03
#define MSG_LIGHTING1_EMW200                     0x04
#define MSG_LIGHTING1_IMPULS                     0x05
#define MSG_LIGHTING1_RISING_SUN                 0x06

#define MSG_LIGHTING2                            0x11

#define MSG_LIGHTING2_AC                         0x00
#define MSG_LIGHTING2_HOMEEASY_EU                0x01
#define MSG_LIGHTING2_ANSLUT                     0x02

#define MSG_LIGHTING3                            0x12
#define MSG_LIGHTING4                            0x13
#define MSG_LIGHTING5                            0x14

#define MSG_LIGHTING5_LIGHTWAVE_RF               0x00
#define MSG_LIGHTING5_SIEMENS                    0x00
#define MSG_LIGHTING5_EMW100_GAO                 0x01
#define MSG_LIGHTING5_EVERFLOURISH               0x01
#define MSG_LIGHTING5_BBSB_NEW_TYPES             0x02
#define MSG_LIGHTING5_MDREMOTE_LED_DIMMER        0x03
#define MSG_LIGHTING5_CONRAD_RSL2                0x04
#define MSG_LIGHTING5_LIVOLO                     0x05
#define MSG_LIGHTING5_RGB_TRC02                  0x06


#define MSG_CURTAIN1                             0x18

#define MAX_MSG_LENGTH                           14

union RFXmsg {
	struct {
		uint8_t message_length;
		uint8_t message_type;            // n/a
		uint8_t sub_type;
		uint8_t seqn;
	} standard;	
	struct {
		uint8_t message_length;
		uint8_t message_type;            // MSG_CONTROL
		uint8_t sub_type;
		uint8_t seqn;
		uint8_t cmd;
		uint8_t payload[9];
	} control;
	struct {
		uint8_t message_length;
		uint8_t message_type;            // MSG_LIGHTING1
		uint8_t sub_type;
		uint8_t seqn;
		uint8_t housecode;
		uint8_t unitcode;
		uint8_t cmd;
		uint8_t filler: 4;
		uint8_t rssi: 4;
	} lighting1;
	struct {
		uint8_t message_length;
		uint8_t message_type;            // MSG_LIGHTING2
		uint8_t sub_type;
		uint8_t seqn;
		uint8_t id1: 2;
		uint8_t filler1 : 6;
		uint8_t id[3];
		uint8_t unitcode;
		uint8_t cmd;
		uint8_t level;
		uint8_t filler2: 4;
		uint8_t rssi: 4;
	} lighting2;
	struct {
		uint8_t message_length;
		uint8_t message_type;            // MSG_LIGHTING5
		uint8_t sub_type;
		uint8_t seqn;
		uint8_t id[3];
		uint8_t unitcode;
		uint8_t cmd;
		uint8_t level;
		uint8_t filler2: 4;
		uint8_t rssi: 4;
	} lighting5;
	uint8_t raw[MAX_MSG_LENGTH];
};

#endif // _RFXCOM_H
