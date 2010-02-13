// Class automatically generated by Dev-C++ New Class wizard

#ifndef BASEPACKET_H
#define BASEPACKET_H

#include "packet.h" // inheriting class's header file
#include <string>

#define BP_INIT 0x01
#define BP_RESET 0x02
#define BP_PING 0x03
#define BP_ERROR 0x04

/**
 * No description
 */
class BasePacket : public Packet
{
	public:
		// class constructor
		BasePacket();
		// class destructor
		~BasePacket();
		void setInit();
		void setReset();
		void setPing();
		void setError();
		bool isError();
		char getErrorCode();
		std::string getErrorString();
};

#endif // BASEPACKET_H