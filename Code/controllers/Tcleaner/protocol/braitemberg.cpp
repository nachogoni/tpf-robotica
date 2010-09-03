#include <stdio.h>
#include <time.h>
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
#include <protocol/packets/DistanceSensorPacket.h>
#include <protocol/handlers/DCMotorBoardPacketHandler.h> // class's header file
#include <protocol/handlers/BatteryBoardPacketHandler.h>
#include <protocol/handlers/DistanceSensorBoardPacketHandler.h>
#include <protocol/handlers/ServoBoardPacketHandler.h>
#include <protocol/handlers/TrashBinBoardPacketHandler.h>
#include <protocol/Packet.h>
#include <protocol/PacketServer.h>
#include "braitemberg.h"

#define MAX(a,b) (a>b?a:b)

//define group and board ids
#define DCMOTOR_GID 1
#define DISTANCESENSOR_GID 3

#define DCMOTOR_BOARD_ID0 0
#define DCMOTOR_BOARD_ID1 1
#define DISTANCESENSOR_BOARD_ID0 0
#define DISTANCESENSOR_BOARD_ID1 1
#define DISTANCESENSOR_BOARD_ID2 2

#define PIPE_IN 0
#define PIPE_OUT 1

typedef struct {
    const char * group;
    const char * cmd;
    void (*f)(char * data);
    const char * cmd_help;
    const char * cmd_help_param;
} cmd_type;

bool quit = false, groupBC = false, fullBC = false, debug = false, alarmSignal = false;
int fd = 5;
int pipes[2];
int lastCMD = -1;

int dest_group = 0x01, dest_card = 0x01;

// Packet Server
protocol::PacketServer * ps;

// Packet boards
protocol::packets::BoardPacket * boardPacket = NULL;
protocol::packets::DCMotorPacket * dcPacket = NULL;
protocol::packets::DistanceSensorPacket * dsPacket = NULL;

protocol::packets::DCMotorPacket * dcPacket0 = NULL;
protocol::packets::DCMotorPacket * dcPacket1 = NULL;

protocol::packets::DistanceSensorPacket * dsPacket0 = NULL;
protocol::packets::DistanceSensorPacket * dsPacket1 = NULL;
protocol::packets::DistanceSensorPacket * dsPacket2 = NULL;

// Handlers for the boards
protocol::handlers::DCMotorBoardPacketHandler * dcHandler = NULL;
protocol::handlers::DistanceSensorBoardPacketHandler * dsHandler = NULL;

protocol::handlers::DCMotorBoardPacketHandler * dcMotorHandler0 = NULL;
protocol::handlers::DCMotorBoardPacketHandler * dcMotorHandler1 = NULL;
protocol::handlers::DistanceSensorBoardPacketHandler * distanceSensorHandler0 = NULL;
protocol::handlers::DistanceSensorBoardPacketHandler * distanceSensorHandler1 = NULL;
protocol::handlers::DistanceSensorBoardPacketHandler * distanceSensorHandler2 = NULL;

// Common commands
void cmd_help(char * data);
void cmd_quit(char * data);
void cmd_getDest(char * data);
void cmd_setDest(char * data);
void cmd_groupBC(char * data);
void cmd_fullBC(char * data);

bool init();
char getDest();
void sendAPacket(protocol::Packet * p);

// Implementation

void alarmHandler(int s) {
	alarm(1);
	alarmSignal = true;
	return;
}

void delay_centecimos(int cents) {

	struct timeval uLast, uNow;
	int elapsed, count = 0;
    gettimeofday(&uLast, NULL);
    while (count < cents) {
		gettimeofday(&uNow, NULL);
		elapsed = (uNow.tv_sec * 1000000 + uNow.tv_usec) - (uLast.tv_sec * 1000000 + uLast.tv_usec);
		if (elapsed < 100000) continue;
		count++;
		gettimeofday(&uLast, NULL);
	}
	return;
}

int main( int argc, const char **argv)
{
    int maxfd = 1, select_resp = 0;
    char stdin_buffer[256] = {0}, serial_buffer[256] = {0};
    int stdin_idx = 0;
    int c = 0;
    struct timeval timeout;
    fd_set readfd, writefd;
    fd_set readfd_b, writefd_b;		
    
	ps = new protocol::PacketServer();
	registerHandlers(ps);
	ps->start();

    createPacketBoards();

	int i,j;
	double left, right;

	double sensors_value[8];
	int sensors[6];
	// Pesos de pre definidos para la rueda izquierda (-s# para la derecha)
	double w0 = -0.9, w2 = -0.2, w5 = 0.35, w7 = 0.6;
	// Pesos de prueba para la rueda izquierda (-s# para la derecha)
	double w1 = -0.85, w3 = 0.6, w4 = 0.5, w6 = 0.8;
	// Definicion de los pesos para braitenberg
	double bc[8][2] = { {w0,-w0}, {w1,-w1}, {w2,-w2}, {w3,w3}, 
						{w4,w4}, {w5,-w5}, {w6,-w6}, {w7,-w7}};

	/* main loop */
	for (;;) {
		
		/* get sensors values */
		//Derecha
		distanceSensorHandler2->getValue(sensors, 6);
		for (i = 0; i < 4; i++) {
			sensors_value[i] = sensors[i];
		}
		//Izquierda
		distanceSensorHandler0->getValue(sensors, 6);
		for (i = 0; i < 4; i++) {
			sensors_value[7-i] = sensors[i];
		}
		
		left = 40.0;
		right = 40.0;
		for (j = 0; j < 8; j++) {
			left += bc[j][0] * sensors_value[j];
			right += bc[j][1] * sensors_value[j];
			printf("Valor (%d): %g\n",j,sensors_value[j]);
		}
		
		// Para que vaya mas rapido...
		//left *=20;
		//right *=20;
		
		/* set speed values */
		dcMotorHandler0->setSpeed(right);
		dcMotorHandler1->setSpeed(left);

		printf("Left: %g\tRight: %g\n", left, right);

		delay_centecimos(1);

	}
    
    return 0;
}

void createPacketBoards() {
	
    dcPacket0 = new protocol::packets::DCMotorPacket(DCMOTOR_GID, DCMOTOR_BOARD_ID0);
    dcPacket1 = new protocol::packets::DCMotorPacket(DCMOTOR_GID, DCMOTOR_BOARD_ID1);

    dsPacket0 = new protocol::packets::DistanceSensorPacket(DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID0);
    dsPacket1 = new protocol::packets::DistanceSensorPacket(DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID1);
    dsPacket2 = new protocol::packets::DistanceSensorPacket(DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID2);
	
	dcPacket = dcPacket0;
	dsPacket = dsPacket0;
	
    boardPacket = dcPacket0;

	return;
}

void
registerHandlers(protocol::PacketServer * ps){

	// Create handlers
    dcMotorHandler0 = new protocol::handlers::DCMotorBoardPacketHandler(ps,DCMOTOR_GID,DCMOTOR_BOARD_ID0);
    dcMotorHandler1 = new protocol::handlers::DCMotorBoardPacketHandler(ps,DCMOTOR_GID,DCMOTOR_BOARD_ID1);
	distanceSensorHandler0 = new protocol::handlers::DistanceSensorBoardPacketHandler(ps,DISTANCESENSOR_GID,DISTANCESENSOR_BOARD_ID0);
	distanceSensorHandler1 = new protocol::handlers::DistanceSensorBoardPacketHandler(ps,DISTANCESENSOR_GID,DISTANCESENSOR_BOARD_ID1);
	distanceSensorHandler2 = new protocol::handlers::DistanceSensorBoardPacketHandler(ps,DISTANCESENSOR_GID,DISTANCESENSOR_BOARD_ID2);

    dcHandler = dcMotorHandler0;
	dsHandler = distanceSensorHandler0;
	    
	// Register handlers
    ps->registerHandler(dcMotorHandler0, DCMOTOR_GID, DCMOTOR_BOARD_ID0);
    ps->registerHandler(dcMotorHandler1, DCMOTOR_GID, DCMOTOR_BOARD_ID1);
    ps->registerHandler(distanceSensorHandler0, DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID0);
    ps->registerHandler(distanceSensorHandler1, DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID1);
    ps->registerHandler(distanceSensorHandler2, DISTANCESENSOR_GID, DISTANCESENSOR_BOARD_ID2);

}
