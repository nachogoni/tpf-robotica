#ifndef robotapi_IDifferentialWheels_h
#define robotapi_IDifferentialWheels_h

#include "IDevice.h"
#include <utils/MyPoint.h>

namespace robotapi {

class IDifferentialWheels : virtual public IDevice {

 public:

    virtual void setSpeed(double left, double right)  = 0;

    virtual double getLeftSpeed()  = 0;

    virtual double getRightSpeed()  = 0;

    virtual void enableEncoders(int ms)  = 0;

    virtual void disableEncoders()  = 0;

    virtual double getLeftEncoder()  = 0;

    virtual double getRightEncoder()  = 0;

    virtual void moveWheels(double left, double right)  = 0;

    virtual void moveLeftWheel(double left)  = 0;

    virtual void moveRightWheel(double right)  = 0;

    virtual void moveLeftWheel(double left, double speed)  = 0;

    virtual void moveRightWheel(double right, double speed)  = 0;

    virtual double getLeftMotorConsumption()  = 0;

    virtual double getRightMotorConsumption()  = 0;
    
    virtual bool isAlarmPresent()  = 0;
    
    virtual bool isAlarmPresent(bool left)  = 0;

    virtual bool motorIsOff()  = 0;

    virtual bool motorIsOff(bool left)  = 0;

	virtual void computeOdometry()  = 0;

	virtual double getOrientation()  = 0;

	virtual utils::MyPoint * getPosition()  = 0;

	virtual void setOrientation(double angle)  = 0;

	virtual void setPosition(double x, double z, bool s)  = 0;

    // virtual destructor for interface
    virtual ~IDifferentialWheels() { }
};

} /* End of namespace robotapi */

#endif // robotapi_IDifferentialWheels_h
