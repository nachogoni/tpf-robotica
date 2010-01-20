#ifndef robotapi_webts_WebotsDevice_h
#define robotapi_webts_WebotsDevice_h

#include <robotapi/IDevice.h>
#include <robotapi/webts/WebotsDeviceTag.h>
#include <webots/Device.hpp>

namespace robotapi {
namespace webts {
//
class WebotsDevice : virtual public robotapi::IDevice {

    public:
        std::string dname;
        IWbDeviceTag dtag;

        WebotsDevice(webots::Device & dev);

        WebotsDevice(std::string name, IWbDeviceTag tag);

        std::string getName();

        IWbDeviceTag &getTag();

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsDevice_h