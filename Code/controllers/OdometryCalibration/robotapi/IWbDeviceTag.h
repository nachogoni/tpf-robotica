#ifndef robotapi_IWbDeviceTag_h
#define robotapi_IWbDeviceTag_h

namespace robotapi {
class IDevice;
} /* End of namespace robotapi */

namespace robotapi {

class IWbDeviceTag {

public:
    // virtual destructor for interface 
    virtual ~IWbDeviceTag() { }

 public:

    IDevice *myIDevice;
};

} /* End of namespace robotapi */

#endif // robotapi_IWbDeviceTag_h
