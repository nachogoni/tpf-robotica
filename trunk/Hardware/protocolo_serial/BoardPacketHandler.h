// Class automatically generated by Dev-C++ New Class wizard

#ifndef BOARDPACKETHANDLER_H
#define BOARDPACKETHANDLER_H

#include "Packet.h"

/**
 * No description
 */
class BoardPacketHandler
{
	public:
		// class constructor
		BoardPacketHandler();
		// class destructor
		virtual ~BoardPacketHandler() = 0;
		
		virtual void handlePacket(Packet * p) = 0;
};

#endif // BOARDPACKETHANDLER_H
