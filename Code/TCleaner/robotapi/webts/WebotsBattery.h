#ifndef robotapi_webts_WebotsBattery_h
#define robotapi_webts_WebotsBattery_h

#include <robotapi/IBattery.h>
#include <robotapi/Webts/WebotsDevice.h>


namespace robotapi {
namespace webts {
//
class WebotsBattery : public robotapi::IBattery , robotapi::webts::WebotsDevice {

 public:

    void enable(int ms);

    void disable();

    double getValue();

    // Change parameter to Webots API Distance Sensor
    WebotsBattery(WebotsBattery * name);

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsBattery_h
