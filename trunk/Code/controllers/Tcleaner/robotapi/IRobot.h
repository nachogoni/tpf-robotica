#ifndef robotapi_IRobot_h
#define robotapi_IRobot_h

#include "IBattery.h"
#include "ICamera.h"
#include "IDevice.h"
#include "IDifferentialWheels.h"
#include "IDistanceSensor.h"
#include "IServo.h"
#include "ITrashBin.h"
#include <string>
//#include <GarbageCleaner.h>

class GarbageCleaner;

namespace robotapi {

class IRobot {

 public:

    virtual std::string getName()  = 0;

    virtual double getTime()  = 0;

    virtual int getMode()  = 0;

    virtual void setGC(GarbageCleaner * gc)  = 0;

    virtual bool getSynchronization()  = 0;

    virtual double getBasicTimeStep()  = 0;

    virtual ICamera & getCamera(std::string name)  = 0;

    virtual IDistanceSensor & getDistanceSensor(std::string name)  = 0;

    virtual IServo & getServo(std::string name)  = 0;

    virtual IDevice & getDevice(std::string name)  = 0;

    virtual IDifferentialWheels & getDifferentialWheels(std::string name)  = 0;

    virtual IBattery & getBattery(std::string name)  = 0;

    virtual ITrashBin & getTrashBin(std::string name)  = 0;

    virtual void step(int ms)  = 0;

    // virtual destructor for interface
    virtual ~IRobot() { }

};

} /* End of namespace robotapi */

#endif // robotapi_IRobot_h
