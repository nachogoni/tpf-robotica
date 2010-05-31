// Class automatically generated by Dev-C++ New Class wizard

#ifndef DEFAULTBOARDPACKETHANDLER_H
#define DEFAULTBOARDPACKETHANDLER_H

#include <protocol/BoardPacketHandler.h> // inheriting class's header file
#include <protocol/Packet.h>
#include <protocol/PacketServer.h>
//#include <mutex>

namespace protocol {
namespace handlers {

/**
 * No description
 */
class DefaultBoardPacketHandler : public protocol::BoardPacketHandler
{
	public:
		// class constructor
		DefaultBoardPacketHandler(PacketServer * ps);
		// class destructor
		~DefaultBoardPacketHandler();
		
		void handlePacket(Packet * p);

	private:
		PacketServer * ps;
		
};

}
}


#endif // DEFAULTBOARDPACKETHANDLER_CPP