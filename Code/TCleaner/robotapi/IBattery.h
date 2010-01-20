#ifndef robotapi_IBattery_h
#define robotapi_IBattery_h

#include "IDevice.h"

namespace robotapi {

class IBattery : virtual public IDevice {

 public:

    virtual void enable(int ms)  = 0;

    virtual void disable() = 0;

    virtual double getValue() = 0;

    // virtual destructor for interface
    virtual ~IBattery() { }
    
};

} /* End of namespace robotapi */

#endif // robotapi_IBattery_h
