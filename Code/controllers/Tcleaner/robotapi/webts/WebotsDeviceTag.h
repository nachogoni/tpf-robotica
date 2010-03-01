#ifndef robotapi_webts_WebotsDeviceTag_h
#define robotapi_webts_WebotsDeviceTag_h

#include <robotapi/IWbDeviceTag.h>
#include <webots/Device.hpp>

namespace robotapi {
namespace webts {

class WebotsDeviceTag : virtual public robotapi::IWbDeviceTag {

public:
    int dtag;

    WebotsDeviceTag(int tag);
    
    WebotsDeviceTag(WbDeviceTag tag);

};

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsDeviceTag_h
