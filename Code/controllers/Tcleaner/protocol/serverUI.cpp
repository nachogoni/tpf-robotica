#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <protocol/packets/BoardPacket.h>
#include <protocol/packets/ServoMotorPacket.h>
#include <protocol/packets/DCMotorPacket.h>
#include <protocol/Packet.h>
#include "serverUI.h"

#define SERIAL_PORT "/dev/ttyUSB0"

#define MAX(a,b) (a>b?a:b)
#define PIPE_IN 0
#define PIPE_OUT 1

typedef struct {
    const char * group;
    const char * cmd;
    void (*f)(char * data);
    const char * cmd_help;
} cmd_type;

bool quit = false, groupBC = false, fullBC = false;
int fd;
int dest_group = 0, dest_card = 0, from_group = 0, from_card = 0;
protocol::packets::DCMotorPacket * packetToSend = NULL;
int pipes[2];

bool init();

void sendAPacket(protocol::Packet * p);

// Common commands
void cmd_init(char * data);
void cmd_reset(char * data);
void cmd_ping(char * data);
void cmd_error(char * data);
void cmd_help(char * data);
void cmd_quit(char * data);
void cmd_from(char * data);
void cmd_dest(char * data);
void cmd_groupBC(char * data);
void cmd_fullBC(char * data);

// Command list
cmd_type commands[] = {
    // Common commands
    {"common", "dest", cmd_dest, "Set group and card id for destination. Params: \%d \%d for group and card (0 to 15)"},
    {"common", "from", cmd_from, "Set group and card id for origin. Params: \%d \%d for group and card (0 to 15)"},
    {"common", "groupBC", cmd_groupBC, "Change group broadcast state"},
    {"common", "fullBC", cmd_fullBC, "Change full broadcast state"},
    // Commands for MainController (mc)
    {"mc", "init", cmd_init, "Send init command"},
    {"mc", "reset", cmd_reset, "Send reset command"},
    {"mc", "ping", cmd_ping, "Send ping command"},
    {"mc", "error", cmd_error, "Send error command. Params: \%d for error"},
    // Commands for MotorDC (dc)                                                                      
    {"dc", "dcSetDirection", cmd_dcSetDirection, "Set motor turn, Params: \%d for Clockwise:0 or Unclockwise:1"},
    {"dc", "dcSetSpeed", cmd_dcSetSpeed, "Set motor speed, Params: \%d \%d for Clockwise:0 or Unclockwise:1 and counts per second"},
    {"dc", "dcSetEncoder", cmd_dcSetEncoder, ""},
    {"dc", "dcGetEncoder", cmd_dcGetEncoder, ""},
    {"dc", "dcResetEncoder", cmd_dcResetEncoder, ""},
    {"dc", "dcSetEncoderToStop", cmd_dcSetEncoderToStop, ""},
    {"dc", "dcGetEncoderToStop", cmd_dcGetEncoderToStop, ""},
    {"dc", "dcDontStop", cmd_dcDontStop, ""},        
    {"dc", "dcConsumption", cmd_dcConsumption, ""},     
    {"dc", "dcStressAlarm", cmd_dcStressAlarm, ""},     
    {"dc", "dcShutDownAlarm", cmd_dcShutDownAlarm, ""},   
    {"dc", "dcGetSpeed", cmd_dcGetSpeed, "Get motor speed in counts per second"},
    // Commands for ServoMotor (sm)
    {"sm", "smSetPos", cmd_smSetPos, "Set servo position"},
    {"sm", "smSetAllPos", cmd_smSetAllPos, ""},
    {"sm", "smGetPos", cmd_smGetPos, ""},   
    {"sm", "smGetAllPos", cmd_smGetAllPos, ""},
    {"sm", "smSetSpeed", cmd_smSetSpeed, ""}, 
    {"sm", "smSetAllSpeed", cmd_smSetAllSpeed, ""},
    {"sm", "smGetSpeed", cmd_smGetSpeed, ""},   
    {"sm", "smGetAllSpeed", cmd_smGetAllSpeed, ""},
    {"sm", "smFree", cmd_smFree, ""},       
    {"sm", "smFreeAll", cmd_smFreeAll, ""},    
    // Commands for DistanceSensor (ds)
    {"ds", "dsEnable", cmd_dsEnable, ""},
    {"ds", "dsDisable", cmd_dsDisable, ""},
    {"ds", "dsSetAll", cmd_dsSetAll, ""}, 
    {"ds", "dsGetValue", cmd_dsGetValue, ""},
    {"ds", "dsGetAllValue", cmd_dsGetAllValue, ""},
    {"ds", "dsGetOneValue", cmd_dsGetOneValue, ""},
    {"ds", "dsGetOneValueAll", cmd_dsGetOneValueAll, ""},
    // Commands for FloorSensor (fs)      
    {"fs", "fsEnable", cmd_fsEnable, ""},
    {"fs", "fsDisable", cmd_fsDisable, ""},
    {"fs", "fsSetAll", cmd_fsSetAll, ""},
    {"fs", "fsGetValue", cmd_fsGetValue, ""},
    {"fs", "fsGetAllValue", cmd_fsGetAllValue, ""},
    {"fs", "fsGetOneValue", cmd_fsGetOneValue, ""},
    {"fs", "fsGetOneValueAll", cmd_fsGetOneValueAll, ""},
    // Commands for UltraSonicSensor (us)
    {"us", "usEnable", cmd_usEnable, ""},
    {"us", "usDisable", cmd_usDisable, ""},
    {"us", "usSetAll", cmd_usSetAll, ""},
    {"us", "usGetValue", cmd_usGetValue, ""},
    {"us", "usGetAllValue", cmd_usGetAllValue, ""},
    {"us", "usGetOneValue", cmd_usGetOneValue, ""},
    {"us", "usGetOneValueAll", cmd_usGetOneValueAll, ""},
    // Commands for BatteryController (bc)
    {"bc", "bcEnable", cmd_bcEnable, ""},
    {"bc", "bcDisable", cmd_bcDisable, ""},
    {"bc", "bcGetValue", cmd_bcGetValue, ""},
    {"bc", "bcFullAlarm", cmd_bcFullAlarm, ""},
    {"bc", "bcSetEmptyValue", cmd_bcSetEmptyValue, ""},
    {"bc", "bcEmptyAlarm", cmd_bcEmptyAlarm, ""},
    {"bc", "bcSetFullValue", cmd_bcSetFullValue, ""},
    // Commands for TrashBin (tb)
    {"tb", "tbGetValue", cmd_tbGetValue, ""},
    {"tb", "tbFullAlarm", cmd_tbFullAlarm, ""},
    {"tb", "tbSetFullValue", cmd_tbSetFullValue, ""},

    {"common", "help", cmd_help, "This help. Params: \%s one of: all, common, mc, dc, sm, ds, fs, us, bc, tb"},
    {"common", "quit", cmd_quit, "Quit to system"},
    {"common", NULL, cmd_quit}
};

char getFrom()
{
    return (from_group & 0x0F) * 16 + (from_card & 0x0F);
}

char getDest()
{
    int group = dest_group & 0x0F;
    int card = dest_card & 0x0F;
    
    if (fullBC == true)
    {
        group = 0x0F;
        card = 0x0F;
    } else if (groupBC == true)
    {
        card = 0x0F;
    } 
    
    return group * 16 + card;
}

bool init()
{
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd < 0)
    {
        perror("/dev/ttyUSB0");
        fprintf(stderr, "cannot connect to the serial port %s\n", SERIAL_PORT);
        return false;
    }
    
    struct termios tc; // 115200 baud, 8n1, no flow control
    
    tc.c_iflag = IGNBRK;
    tc.c_oflag = 0;
    tc.c_cflag = CS8 | CREAD | CLOCAL | CSTOPB;
    tc.c_lflag = 0;
 
    cfsetispeed(&tc, B115200);
    cfsetospeed(&tc, B115200);

    tcsetattr(fd, TCSANOW, &tc);
    
    return true;
}

void command(char * cmd, int lenght)
{
    int i = 0;
    bool found = false;
    char * data = NULL;
    
    // Get parameters
    if ((data = strstr(cmd, " ")) != NULL)
    {
        data[0] = '\0';
        data++;
    }
    
    if (strlen(cmd) < 1)
        return;
    
    while (found == false && commands[i].cmd != NULL)
    {
        if (strcmp(commands[i].cmd, cmd) == 0)
        {
            // Command found!
            commands[i].f(data);
            found = true;
        }
        i++;
    }
    
    if (found == false)
        printf("Command not found\n");
    
    return;
}

int main( int argc, const char **argv)
{
    int maxfd = 0, select_resp = 0;
    char stdin_buffer[256] = {0}, serial_buffer[256] = {0};
    int stdin_idx = 0;
    int c = 0;
    struct timeval timeout;
    fd_set readfd, writefd;
    fd_set readfd_b, writefd_b;
    
    if ( pipe(pipes) == -1 ){
        printf("PIPE ERROR");
        return 1;
    }

    packetToSend = new protocol::packets::DCMotorPacket(0x01,0x00);
//TODO:    packetToSend = new protocol::packets::(0x02,0x01);
    packetToSend->setOriginGroup(0);
    packetToSend->setOriginId(0);
    
    if (init() != true)
    {
        close(pipes[0]);
        close(pipes[1]);
        return -1;
    }

    // Set file descriptors
    FD_SET(fd,&readfd);
    FD_SET(pipes[PIPE_IN],&readfd);
    FD_SET(0,&readfd);

    maxfd = MAX(fd,0) + 1;

    readfd_b = readfd;
    writefd_b = writefd;
    
    cmd_help((char *)"common");

    while ( quit != true )
    {

        if ((select_resp = select(maxfd, &readfd, &writefd, NULL, NULL)) == 0)
        {
            // Select timed out!
            // TODO: retransmitir la lista de mensajes sin responder...
            printf("TIME OUT!\n");
        }

        if (errno == EINTR)
        {
            // A signal was delivered befor time_out 
            errno = 0;
            continue;
        }

        // SERIAL PORT
        if ( FD_ISSET(fd, &readfd) )
        {
            // Have things in buffer! :P
            read(fd, serial_buffer, 1);
            // TODO: hacer lo mismo que en los pics para interpretar el comando
            printf("caracter : %X", serial_buffer[0]);
            fflush(stdout);
        }

        // PIPE
        if ( FD_ISSET(pipes[PIPE_IN], &readfd) )
        {
            int c;
            int pipe_buf[256];
            // Have things in buffer! :P
            read(pipes[PIPE_IN], &c, 1);
            write(fd,&c,1);
            read(pipes[PIPE_IN], pipe_buf, c);
            printf("escribi : %d bytes en el serial\n",write(fd,pipe_buf,c)+1);
        }

        // STDIN
        if ( FD_ISSET(0, &readfd) )
        {
            // Have things in buffer! :P
            c = read(0, stdin_buffer + stdin_idx, 1);
            
            if (stdin_buffer[stdin_idx] == '\n')
            {
                stdin_buffer[stdin_idx] = '\0';
                command(stdin_buffer, stdin_idx);
                stdin_idx = 0;
            } else {
                stdin_idx++;
            }
        }

        // Restore structures from backup
        readfd = readfd_b;
        writefd = writefd_b;
    }
    
    close(fd);
    close(pipes[0]);
    close(pipes[1]);
    
    return 0;
}


void sendAPacket(protocol::Packet * p){
    unsigned char i;
    p->calculateCRC();
    char * packet = p->getPacket();
    p->print();
    printf("escribi : %d bytes en el pipe\n",write(pipes[PIPE_OUT],packet,p->getActualLength()));
    //this->waitingForResponse.push_back(p);
}

// Commands for MainController (mc)
void cmd_help(char * data)
{
    int i = 0;
    
    printf("\nPossible commands:\n\n");

    if (data == NULL)
        data = (char *)"common";
    
    while (commands[i].cmd != NULL)
    {
        if (strcmp(commands[i].group, "all") == 0 || strcmp(commands[i].group, data) == 0)
            printf("%-25s%s\n", commands[i].cmd, commands[i].cmd_help);
        i++;
    }
    
    return;
}

void cmd_quit(char * data)
{
    quit = true;
    return;
}

// COMMON_INIT 0X01
void cmd_init(char * data)
{
    packetToSend->clear();
    packetToSend->setInit();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// COMMON_RESET    0X02
void cmd_reset(char * data)
{
    packetToSend->clear();
    packetToSend->setReset();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// COMMON_PING 0X03
void cmd_ping(char * data)
{
    packetToSend->clear();
    packetToSend->setPing();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// COMMON_ERROR    0X04
void cmd_error(char * data)
{
    packetToSend->clear();
    packetToSend->setPing();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    // TODO: armar paquete de error y mandarlo
    return;
}

void cmd_dest(char * data)
{
    if (data == NULL || sscanf(data, "%d %d", &dest_group, &dest_card) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    packetToSend = new protocol::packets::DCMotorPacket(dest_group,dest_card);
    printf("GroupID: %d CardID: %d -> (%X)\n", dest_group, dest_card, getDest());
    
    return;
}

void cmd_from(char * data)
{
    if (data == NULL || sscanf(data, "%d %d", &from_group, &from_card) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    packetToSend->setOriginGroup(from_group);
    packetToSend->setOriginId(from_card);
    printf("GroupID: %d CardID: %d -> (%X)\n", from_group, from_card, getFrom());
    
    return;
}

void cmd_groupBC(char * data)
{
    groupBC = !groupBC;
    printf("Group broadcast state: %s\n", (groupBC?"Activated":"Deactivated"));
    return;
}

void cmd_fullBC(char * data)
{
    fullBC = !fullBC;
    printf("Full broadcast state: %s\n", (fullBC?"Activated":"Deactivated"));
    return;
}

// Commands for MotorDC (dc)

// DC_MOTOR_SET_DIRECTION      0X40
void cmd_dcSetDirection(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_SET_DC_SPEED       0X41
void cmd_dcSetSpeed(char * data)
{
    int turn;
    int speed;
    if (data == NULL || sscanf(data, "%d %d", &turn, &speed) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    packetToSend = new protocol::packets::DCMotorPacket(0x01,0x00);
//TODO:    packetToSend = new protocol::packets::(0x02,0x01);
    packetToSend->setOriginGroup(0);
    packetToSend->setOriginId(0);
    bool clockwise = (turn == 0);

    packetToSend->setDCSpeed(clockwise,speed);
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// DC_MOTOR_SET_ENCODER        0X42
void cmd_dcSetEncoder(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_GET_ENCODER        0X43
void cmd_dcGetEncoder(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_RESET_ENCODER      0X44
void cmd_dcResetEncoder(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_SET_ENCODER_TO_STOP    0X45
void cmd_dcSetEncoderToStop(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_GET_ENCODER_TO_STOP    0X46
void cmd_dcGetEncoderToStop(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_DONT_STOP      0X47
void cmd_dcDontStop(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_MOTOR_CONSUMPTION  0X48
void cmd_dcConsumption(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_MOTOR_STRESS_ALARM 0X49
void cmd_dcStressAlarm(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_MOTOR_SHUT_DOWN_ALARM  0X4A
void cmd_dcShutDownAlarm(char * data)
{
    // TODO
    return;
}

// DC_MOTOR_GET_DC_SPEED       0X4B
void cmd_dcGetSpeed(char * data)
{
    
    //TODO
    
    int turn;
    int speed;
    if (data == NULL || sscanf(data, "%d %d", &turn, &speed) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    packetToSend = new protocol::packets::DCMotorPacket(0x01,0x00);
//TODO:    packetToSend = new protocol::packets::(0x02,0x01);
    packetToSend->setOriginGroup(0);
    packetToSend->setOriginId(0);
    bool clockwise = (turn == 0);

    packetToSend->setDCSpeed(clockwise,speed);
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// Commands for ServoMotor (sm)

// SERVO_MOTOR_SET_POSITION    0X40
void cmd_smSetPos(char * data)
{
    int servoid;
    int servopos;
    if (data == NULL || sscanf(data, "%d %d", &servoid, &servopos) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    packetToSend->clear();
    //packetToSend->setPosition(servoid,servopos);
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

// SERVO_MOTOR_SET_ALL_POSITIONS   0X41
void cmd_smSetAllPos(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_GET_POSITION    0X42
void cmd_smGetPos(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_GET_ALL_POSITIONS   0X43
void cmd_smGetAllPos(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_SET_SERVO_SPEED 0X44
void cmd_smSetSpeed(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_SET_ALL_SPEEDS  0X45
void cmd_smSetAllSpeed(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_GET_SERVO_SPEED 0X46
void cmd_smGetSpeed(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_GET_ALL_SPEEDS  0X47
void cmd_smGetAllSpeed(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_FREE_SERVO      0X48
void cmd_smFree(char * data)
{
    // TODO
    return;
}

// SERVO_MOTOR_FREE_ALL_SERVOS 0X49
void cmd_smFreeAll(char * data)
{
    // TODO
    return;
}

// Commands for DistanceSensor (ds)

// DISTANCE_SENSOR_ENABLE_DISTANCE_SENSOR      0X40
void cmd_dsEnable(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_DISABLE_DISTANCE_SENSOR     0X41
void cmd_dsDisable(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_SET_ALL_DISTANCE_SENSORS    0X42
void cmd_dsSetAll(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_GET_VALUE           0X43
void cmd_dsGetValue(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_GET_ALL_VALUES          0X44
void cmd_dsGetAllValue(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_GET_ONE_VALUE           0X45
void cmd_dsGetOneValue(char * data)
{
    // TODO
    return;
}

// DISTANCE_SENSOR_GET_ONE_VALUE_FOR_ALL       0X46
void cmd_dsGetOneValueAll(char * data)
{
    // TODO
    return;
}

// Commands for FloorSensor (fs)

// FLOOR_SENSOR_ENABLE_FLOOR_SENSOR    0X40
void cmd_fsEnable(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_DISABLE_FLOOR_SENSOR   0X41
void cmd_fsDisable(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_SET_ALL_FLOOR_SENSORS  0X42
void cmd_fsSetAll(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_GET_VALUE          0X43
void cmd_fsGetValue(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_GET_ALL_VALUES     0X44
void cmd_fsGetAllValue(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_GET_ONE_VALUE      0X45
void cmd_fsGetOneValue(char * data)
{
    // TODO
    return;
}

// FLOOR_SENSOR_GET_ONE_VALUE_FOR_ALL  0X46
void cmd_fsGetOneValueAll(char * data)
{
    // TODO
    return;
}

// Commands for UltraSonicSensor (us)

// ULTRASONIC_SENSOR_ENABLE_ULTRASONIC_SENSOR  0X40
void cmd_usEnable(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_DISABLE_ULTRASONIC_SENSOR 0X41
void cmd_usDisable(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_SET_ALL_ULTRASONIC_SENSOR 0X42
void cmd_usSetAll(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_GET_VALUE         0X43
void cmd_usGetValue(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_GET_ALL_VALUES        0X44
void cmd_usGetAllValue(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_GET_ONE_VALUE         0X45
void cmd_usGetOneValue(char * data)
{
    // TODO
    return;
}

// ULTRASONIC_SENSOR_GET_ONE_VALUE_FOR_ALL     0X46
void cmd_usGetOneValueAll(char * data)
{
    // TODO
    return;
}

// Commands for BatteryController (bc)

// BATTERY_CONTROLLER_ENABLE           0X40
void cmd_bcEnable(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_DISABLE          0X41
void cmd_bcDisable(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_GET_BATTERY_VALUE        0X42
void cmd_bcGetValue(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_BATTERY_FULL_ALARM       0X43
void cmd_bcFullAlarm(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_SET_BATTERY_EMPTY_VALUE  0X44
void cmd_bcSetEmptyValue(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_BATTERY_EMPTY_ALARM      0X45
void cmd_bcEmptyAlarm(char * data)
{
    // TODO
    return;
}

// BATTERY_CONTROLLER_SET_FULL_BATTERY_VALUE   0X46
void cmd_bcSetFullValue(char * data)
{
    // TODO
    return;
}

// Commands for TrashBin (tb)

// TRASH_BIN_GET_TRASH_BIN_VALUE   0X40
void cmd_tbGetValue(char * data)
{
    // TODO
    return;
}

// TRASH_BIN_BIN_FULL_ALARM    0X41
void cmd_tbFullAlarm(char * data)
{
    // TODO
    return;
}

// TRASH_BIN_SET_FULL_BIN_VALUE    0X42
void cmd_tbSetFullValue(char * data)
{
    // TODO
    return;
}
