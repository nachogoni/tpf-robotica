#ifndef robotapi_ITrashBin_h
#define robotapi_ITrashBin_h

#include "IDevice.h"

namespace robotapi {

class ITrashBin : virtual public robotapi::IDevice {

 public:

    virtual int enable(int ms)  = 0;

    virtual int disable()  = 0;

    virtual int getValue()  = 0;

    virtual bool isFull()  = 0;

    virtual int setFullBias(double bias)  = 0;

public:
    // virtual destructor for interface 
    virtual ~ITrashBin() { }
};

} /* End of namespace robotapi */

#endif // robotapi_ITrashBin_h
