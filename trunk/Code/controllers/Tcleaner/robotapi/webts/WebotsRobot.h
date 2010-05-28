#ifndef robotapi_webts_WebotsRobot_h
#define robotapi_webts_WebotsRobot_h

#include <robotapi/IRobot.h>
#include <webots/DifferentialWheels.hpp>
#include <WorldInfo.h>
#include <list>
#include <utils/ArenaGrid.h>

namespace robotapi {
namespace webts {

class WebotsRobot : virtual public robotapi::IRobot {

public:

	WebotsRobot(WorldInfo * wi, webots::DifferentialWheels & dw );

    std::string getName();

    double getTime();

    int getMode();

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

	void saveChanges(std::list<utils::ArenaGridSlot *> ags);

	std::list<utils::ArenaGridSlot *> getSlotsSeen(utils::ArenaGridSlot * currentSlot);

    // destructor for interface
    ~WebotsRobot() { }
    
	private:
        WorldInfo * wi;
		IDifferentialWheels * df;
		IBattery * robotBattery;
		IBattery * pcBattery;
		ITrashBin * tb;
		utils::ArenaGrid * ag;

    };
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsRobot_h
