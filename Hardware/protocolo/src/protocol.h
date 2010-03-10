#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

/* DEFINE CARD CONTROLLERS ID */
#define MAIN_CONTROLLER		0X00
#define MOTOR_DC		0X10
#define SERVO_MOTOR		0X20
#define DISTANCE_SENSOR		0X30
#define FLOOR_SENSOR		0X40
#define ULTRASONIC_SENSOR	0X50
#define BATTERY_CONTROLLER	0X60
#define TRASH_BIN		0X70

/* DEFINE COMMON COMMANDS */
#define COMMON_INIT	0X01
#define COMMON_RESET	0X02
#define COMMON_PING	0X03
#define COMMON_ERROR	0X04

/* DEFINE MAIN_CONTROLLER COMMANDS */

/* DEFINE MOTOR_DC COMMANDS */
#define DC_MOTOR_SET_DIRECTION		0X40
#define DC_MOTOR_SET_DC_SPEED		0X41
#define DC_MOTOR_SET_ENCODER		0X42
#define DC_MOTOR_GET_ENCODER		0X43
#define DC_MOTOR_RESET_ENCODER		0X44
#define DC_MOTOR_SET_ENCODER_TO_STOP	0X45
#define DC_MOTOR_GET_ENCODER_TO_STOP	0X46
#define DC_MOTOR_DONT_STOP		0X47
#define DC_MOTOR_MOTOR_CONSUMPTION	0X48
#define DC_MOTOR_MOTOR_STRESS_ALARM	0X49
#define DC_MOTOR_MOTOR_SHUT_DOWN_ALARM	0X4A
#define DC_MOTOR_GET_DC_SPEED		0X4B

/* DEFINE SERVO_MOTOR COMMANDS */
#define SERVO_MOTOR_SET_POSITION	0X40
#define SERVO_MOTOR_SET_ALL_POSITIONS	0X41
#define SERVO_MOTOR_GET_POSITION	0X42
#define SERVO_MOTOR_GET_ALL_POSITIONS	0X43
#define SERVO_MOTOR_SET_SERVO_SPEED	0X44
#define SERVO_MOTOR_SET_ALL_SPEEDS	0X45
#define SERVO_MOTOR_GET_SERVO_SPEED	0X46
#define SERVO_MOTOR_GET_ALL_SPEEDS	0X47
#define SERVO_MOTOR_FREE_SERVO		0X48
#define SERVO_MOTOR_FREE_ALL_SERVOS	0X49

/* DEFINE DISTANCE_SENSOR COMMANDS */
#define DISTANCE_SENSOR_ENABLE_DISTANCE_SENSOR		0X40
#define DISTANCE_SENSOR_DISABLE_DISTANCE_SENSOR		0X41
#define DISTANCE_SENSOR_SET_ALL_DISTANCE_SENSORS	0X42
#define DISTANCE_SENSOR_GET_VALUE			0X43
#define DISTANCE_SENSOR_GET_ALL_VALUES			0X44
#define DISTANCE_SENSOR_GET_ONE_VALUE			0X45
#define DISTANCE_SENSOR_GET_ONE_VALUE_FOR_ALL		0X46

/* DEFINE FLOOR_SENSOR COMMANDS */
#define FLOOR_SENSOR_ENABLE_FLOOR_SENSOR	0X40
#define FLOOR_SENSOR_DISABLE_FLOOR_SENSOR	0X41
#define FLOOR_SENSOR_SET_ALL_FLOOR_SENSORS	0X42
#define FLOOR_SENSOR_GET_VALUE			0X43
#define FLOOR_SENSOR_GET_ALL_VALUES		0X44
#define FLOOR_SENSOR_GET_ONE_VALUE		0X45
#define FLOOR_SENSOR_GET_ONE_VALUE_FOR_ALL	0X46

/* DEFINE ULTRASONIC_SENSOR COMMANDS */
#define ULTRASONIC_SENSOR_ENABLE_ULTRASONIC_SENSOR	0X40
#define ULTRASONIC_SENSOR_DISABLE_ULTRASONIC_SENSOR	0X41
#define ULTRASONIC_SENSOR_SET_ALL_ULTRASONIC_SENSOR	0X42
#define ULTRASONIC_SENSOR_GET_VALUE			0X43
#define ULTRASONIC_SENSOR_GET_ALL_VALUES		0X44
#define ULTRASONIC_SENSOR_GET_ONE_VALUE			0X45
#define ULTRASONIC_SENSOR_GET_ONE_VALUE_FOR_ALL		0X46

/* DEFINE BATTERY_CONTROLLER COMMANDS */
#define BATTERY_CONTROLLER_ENABLE			0X40
#define BATTERY_CONTROLLER_DISABLE			0X41
#define BATTERY_CONTROLLER_GET_BATTERY_VALUE		0X42
#define BATTERY_CONTROLLER_BATTERY_FULL_ALARM		0X43
#define BATTERY_CONTROLLER_SET_BATTERY_EMPTY_VALUE	0X44
#define BATTERY_CONTROLLER_BATTERY_EMPTY_ALARM		0X45
#define BATTERY_CONTROLLER_SET_FULL_BATTERY_VALUE	0X46

/* DEFINE TRASH_BIN COMMANDS */
#define TRASH_BIN_GET_TRASH_BIN_VALUE	0X40
#define TRASH_BIN_BIN_FULL_ALARM	0X41
#define TRASH_BIN_SET_FULL_BIN_VALUE	0X42

/* STANDAR PACKAGE LENGTH */
#define MIN_LENGTH		0x04

/* Buffer del puerto serial */
#define MAX_BUFFER_SIZE	45
#define DATA_SIZE		20

/* COMMAND TYPE */
struct command_t {
	int len;
	int to;
	int from;
	int cmd;
	char data[DATA_SIZE + 1];
	int crc;
};

/* CARD IDs */
#define THIS_CARD		(CARD_GROUP + CARD_ID)
#define THIS_GROUP		(CARD_GROUP)

// Si la placa es la ultima en la familia -> comentar la siguiente linea
#define RESEND_GROUP_BROADCAST

/* Inicializa puertos y variables */
void init();

/* Examina y ejecula el comando */
void doCommand(struct command_t * cmd);

/* Envia los datos por el pto serial */
void send(struct command_t * cmd);

/* Inicializa puertos y variables */
void initProtocol();

/* Analiza el buffer, ejecuta los 
comandos y envia las respuestas */
void runProtocol(struct command_t * cmd);

#endif