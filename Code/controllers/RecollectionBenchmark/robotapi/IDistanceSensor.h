#ifndef robotapi_IDistanceSensor_h
#define robotapi_IDistanceSensor_h

#include "IDevice.h"

namespace robotapi {

class IDistanceSensor : virtual public IDevice {

 public:

    virtual void enable(int ms)  = 0;

    virtual void disable()  = 0;

    virtual int getValue()  = 0;

    virtual double getDistance()  = 0;

    // virtual destructor for interface
    virtual ~IDistanceSensor() { }
};

} /* End of namespace robotapi */

#endif // robotapi_IDistanceSensor_h
