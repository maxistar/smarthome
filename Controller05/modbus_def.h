#define MODBUS_SIZE 10

#define MODBUS_R_REGISTERS 0
#define MODBUS_RW_REGISTERS 1
// 1 - 1 word - registers to read
// 1.0 light switch shower room
#define MODBUS_R_LIGHT_BUTTON 0

// 2 - 1 word - registers to write/read
// 2.0 light
#define MODBUS_RW_LIGHT 0

// 3 - 1 word
// 3.1 count of modbus errors
#define MODBUS_ERRORS_COUNT 0

// 4 - 7 words hold registers
// 4.1 - light preset num
#define MODBUS_LIGHT_PRESET_NUM 3
// 4.2 - maximum pwd value W1
#define MODBUS_LIGHT_W1_VALUE 4
// 4.3 - maximum pwd value W2
#define MODBUS_LIGHT_W2_VALUE 5
// 4.4 - maximum pwd value R
#define MODBUS_LIGHT_W3_VALUE 6
// 4.5 - maximum pwd value G
#define MODBUS_LIGHT_W4_VALUE 7
// 4.6 - maximum pwd value B
#define MODBUS_LIGHT_W5_VALUE 8
// 4.7 - maximum pwd value B
#define MODBUS_LIGHT_W6_VALUE 9
