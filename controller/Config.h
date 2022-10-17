#ifndef _R51_CTRL_CONFIG_H_
#define _R51_CTRL_CONFIG_H_

#define SERIAL_DEVICE Serial
#define SERIAL_BAUDRATE 115200
#define SERIAL_WAIT true

#define HMI_DEVICE Serial1
#define HMI_BAUDRATE 115200

#define J1939_ADDRESS 0x19
#define J1939_NAME 0x00000BB000FFFAC0
#define J1939_PROMISCUOUS true
#define J1939_CAN_MODE Canny::CAN20_250K
#define J1939_READ_BUFFER 128
#define J1939_WRITE_BUFFER 4

#define BRIDGE_ADDRESS 0x18

#define STEERING_KEYPAD_ID 0x00

#define ROTARY_ENCODER_ID 0x01
#define ROTARY_ENCODER_INTR false
#define ROTARY_ENCODER_INTR_PIN 8
#define ROTARY_ENCODER_ADDR0 0x36
#define ROTARY_ENCODER_ADDR1 0x37

#define BLINK_KEYPAD_ID 0x02
#define BLINK_KEYPAD_ADDR 0x24
#define BLINK_KEYPAD_KEYS 6

#define BLINK_KEYBOX_ID 0x01
#define BLINK_KEYBOX_ADDR 0x23

// Uncomment the following line to enable debug output.
#define DEBUG_ENABLE

#endif  // _R51_CTRL_CONFIG_H_
