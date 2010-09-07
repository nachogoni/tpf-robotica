#ifndef robotapi_IDevice_h
#define robotapi_IDevice_h

#include <string>

#include "IWbDeviceTag.h"

namespace robotapi {

class IDevice {

 public:

    virtual std::string getName() = 0;

    virtual IWbDeviceTag &getTag() = 0;
    
    virtual void refresh() = 0;

    // virtual destructor for interface
    virtual ~IDevice() { }

};

} /* End of namespace robotapi */

#endif // robotapi_IDevice_h
