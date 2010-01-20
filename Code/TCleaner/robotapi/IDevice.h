#ifndef robotapi_IDevice_h
#define robotapi_IDevice_h

#include <vector>
#include <string>

#include "IWbDeviceTag.h"

namespace robotapi {
class IRobot;
} /* End of namespace robotapi */

namespace robotapi {

class IDevice {

 public:

    virtual std::string getName()  = 0;

    virtual IWbDeviceTag &getTag()  = 0;

    // virtual destructor for interface
    virtual ~IDevice() { }

    /**
     * @element-type IRobot
     */
    std::vector< IRobot* > myIRobot;
};

} /* End of namespace robotapi */

#endif // robotapi_IDevice_h
