#ifndef robotapi_webts_WebotsRobot_h
#define robotapi_webts_WebotsRobot_h

#include <robotapi/IRobot.h>



namespace robotapi {
namespace webts {

class WebotsRobot : public robotapi::IRobot {

    std::string getName();

    double getTime();

    int getMode();

    bool getSynchronization();

    double getBasicTimeStep();

    ICamera & getCamera(std::string name);

    IDistanceSensor & getDistanceSensor(std::string name);

    IServo & getServo(std::string name);

    IDevice & getDevice(std::string name);

    IDifferentialWheels &getDifferentialWheels(std::string name);

    IBattery &getBattery(std::string name);

    void step(int ms);

    // destructor for interface
    ~WebotsRobot() { }
    
    
    
    };
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsRobot_h
