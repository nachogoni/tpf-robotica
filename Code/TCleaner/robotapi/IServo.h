#ifndef robotapi_IServo_h
#define robotapi_IServo_h

#include "IDevice.h"


namespace robotapi {

class IServo : virtual public IDevice {

 public:

    virtual void setAcceleration(double force)  = 0;

    virtual void setVelocity(double vel)  = 0;

    virtual void enablePosition(int ms)  = 0;

    virtual void disablePosition()  = 0;

    virtual void setForce(double force)  = 0;

    virtual void setMotorForce(double motor_force)  = 0;

    virtual void enableMotorForceFeedback(int ms)  = 0;

    virtual void disableMotorForceFeedback()  = 0;

    virtual double getMotorForceFeedback()  = 0;

    virtual double getPosition()  = 0;

    virtual void setPosition(double position)  = 0;

public:
    // virtual destructor for interface 
    virtual ~IServo() { }
};

} /* End of namespace robotapi */

#endif // robotapi_IServo_h
