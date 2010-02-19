#ifndef robotapi_webts_WebotsBattery_h
#define robotapi_webts_WebotsBattery_h

#include <robotapi/IBattery.h>
#include <robotapi/Webts/WebotsDevice.h>


namespace robotapi {
namespace webts {
//
class WebotsBattery : public robotapi::IBattery, public robotapi::webts::WebotsDevice {

 public:

    void enable(int ms);

    void disable();

    double getValue();

    bool isFull();

    void setEmptyBias(double bias);

    bool isEmpty();

    void setFullBias(double bias);

    WebotsBattery(std::string & name, IWbDeviceTag tag);

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsBattery_h
