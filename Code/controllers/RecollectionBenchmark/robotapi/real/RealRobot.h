#ifndef robotapi_real_RealRobot_h
#define robotapi_real_RealRobot_h

#include <robotapi/IRobot.h>
#include <protocol/handlers/BatteryBoardPacketHandler.h>
#include <protocol/handlers/DCMotorBoardPacketHandler.h>
#include <protocol/handlers/DistanceSensorBoardPacketHandler.h>
#include <protocol/handlers/ServoBoardPacketHandler.h>
#include <protocol/handlers/TrashBinBoardPacketHandler.h>
#include <protocol/PacketServer.h>
#include <WorldInfo.h>

#include <string>
#include <map>

namespace robotapi {
namespace real {

class RealRobot : virtual public robotapi::IRobot {

	public:
		RealRobot(WorldInfo * wi);

		std::string getName();

		double getTime();

		int getMode();

    	void setGC(IGarbageCleaner * gc);

		bool getSynchronization();

		double getBasicTimeStep();

		ICamera & getCamera(std::string name);

		IDistanceSensor & getDistanceSensor(std::string name);

		IServo & getServo(std::string name);

		IDevice & getDevice(std::string name);

		IDifferentialWheels & getDifferentialWheels(std::string name);

		IBattery & getBattery(std::string name);

		ITrashBin & getTrashBin(std::string name);

		void step(int ms);

	private:
        void initServos(protocol::PacketServer * ps);
		void initServo(protocol::PacketServer * ps, protocol::handlers::ServoBoardPacketHandler * sbph, std::string name, int id);
        
        void initWheels(protocol::PacketServer * ps);
        void initBatteries(protocol::PacketServer * ps);
        void initDistanceSensors(protocol::PacketServer * ps);
        void initDistanceSensor(protocol::PacketServer * ps, protocol::handlers::DistanceSensorBoardPacketHandler * dsbph, std::string name, int id);
        
        void initCameras();
        void initTrashBins(protocol::PacketServer * ps);

		std::map<std::string, IServo *> servos;
		std::map<std::string, IDifferentialWheels *> wheels;
		std::map<std::string, IBattery *> batteries;
		std::map<std::string, IDistanceSensor *> distanceSensors;
		std::map<std::string, ICamera *> cameras;
		std::map<std::string, ITrashBin *> trashBins;
		WorldInfo * wi;
		IGarbageCleaner * gc;
		IDifferentialWheels * df;
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealRobot_h
