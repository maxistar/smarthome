#define MODBUS_SIZE 17

#define MODBUS_R_REGISTERS 0
#define MODBUS_RW_REGISTERS 1
// 1 - 1 word - registers to read
// 1.0 light switch shower room
#define MODBUS_R_LIGHT_BUTTON 0
// 1.1 door sensor shower room
#define MODBUS_R_SHOWER_DOOR_SENSOR 1
// 1.2 shower water sensor
#define MODBUS_R_LEAK_SENSOR 2
// 1.3 door sensor kitchen room
#define MODBUS_R_KITCHEN_DOOR_SENSOR 3
// 1.4 PIR sensor shower room
#define MODBUS_R_SHOWER_PIR_SENSOR 4

// 2 - 1 word - registers to write/read
// 2.0 light shower room
#define MODBUS_RW_SHOWER_LIGHT 0
// 2.1 fan shower room
#define MODBUS_RW_SHOWER_FAN 1
// 2.2 warm floor heater
#define MODBUS_RW_SHOWER_FLOOR_HEATER 2


// 3 - 5 words
// 3.1 temperature in shower room
#define MODBUS_SHOWER_ROOM_TEMPERATURE 2
// 3.2 humidity
#define MODBUS_SHOWER_ROOM_HUMIDITY 3
// 3.3 temperature of warm floor in shower room
#define MODBUS_SHOWER_ROOM_FLOOR_TEMPERATURE 4
// 3.4 count of modbus errors
#define MODBUS_ERRORS_COUNT 5

// 4 - 6 words hold registers
// 4.1 - light preset num
#define MODBUS_LIGHT_PRESET_NUM 6
// 4.2 - maximum pwd value W1
#define MODBUS_LIGHT_W1_VALUE 7
// 4.3 - maximum pwd value W2
#define MODBUS_LIGHT_W2_VALUE 8
// 4.4 - maximum pwd value R
#define MODBUS_LIGHT_R_VALUE 9
// 4.5 - maximum pwd value G
#define MODBUS_LIGHT_G_VALUE 10
// 4.6 - maximum pwd value B
#define MODBUS_LIGHT_B_VALUE 11

#define MODBUS_SHOWER_FLOOR_INTERVAL 12

#define MODBUS_SHOWER_FLOOR_SCALE 13
