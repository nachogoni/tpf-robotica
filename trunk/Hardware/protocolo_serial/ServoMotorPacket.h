// Class automatically generated by Dev-C++ New Class wizard

#ifndef SERVOMOTORPACKET_H
#define SERVOMOTORPACKET_H

#include "BoardPacket.h" // inheriting class's header file

#define CMD_SET_POSITION 0x40
#define CMD_SET_ALL_POSITIONS 0x41
#define CMD_GET_POSITION 0x42
#define CMD_GET_ALL_POSITIONS 0x43
#define CMD_SET_SPEED 0x44
#define CMD_SET_ALL_SPEEDS 0x45
#define CMD_GET_SPEED 0x46
#define CMD_GET_ALL_SPEEDS 0x47
#define CMD_FREE_SERVO 0x48
#define CMD_FREE_ALL_SERVOS 0x49

/**
 * No description
 */
class ServoMotorPacket : public BoardPacket
{
	public:
		// class constructor
		ServoMotorPacket(char groupid, char boardid);
		// class destructor
		~ServoMotorPacket();

		// Commands to be sent
		void setPosition(char servoId, char angle);
		void setPosition(char * angle, int qty);
		void getPosition(char servoId);
		void getPosition();
		void setSpeed(char servoId, char dps);
		void setSpeed(char * dps, int qty);
		void getSpeed(char servoId);
		void getSpeed();
		void freeServo(char servoId);
		void freeAllServos();
		
		// Get data from package
		char getPositionValue();
		char * getPositionValues();
		char getSpeedValue();
		char * getSpeedValues();
};

#endif // SERVOMOTORPACKET_H