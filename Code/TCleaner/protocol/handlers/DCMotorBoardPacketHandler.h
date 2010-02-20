// Class automatically generated by Dev-C++ New Class wizard

#ifndef DCMOTORBOARDPACKETHANDLER_H
#define DCMOTORBOARDPACKETHANDLER_H

#include <protocol/BoardPacketHandler.h> // inheriting class's header file
#include <protocol/Packet.h>
#include <protocol/PacketServer.h>

namespace protocol {
namespace handlers {

/**
 * No description
 */
class DCMotorBoardPacketHandler : public protocol::BoardPacketHandler
{
	public:
		// class constructor
		DCMotorBoardPacketHandler(PacketServer * ps, char groupid, char boardid);
		// class destructor
		~DCMotorBoardPacketHandler();
		
		void handlePacket(Packet * p);
		
		void setSpeed(double value);

		void enableEncoder();

		double getEncoder();

		void moveWheel(double value);

		void moveWheel(double counts, double speed);

		double getMotorConsumption();

		double getAverageConsumption();
	private:
		char groupid;
		char boardid;
		PacketServer * ps;
		
		double encoderValue;
		double consumptionValue;
};

}
}


#endif // DCMOTORBOARDPACKETHANDLER_CPP
