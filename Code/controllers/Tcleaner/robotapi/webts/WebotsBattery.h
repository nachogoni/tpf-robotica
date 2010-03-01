#ifndef robotapi_webts_WebotsBattery_h
#define robotapi_webts_WebotsBattery_h

#include <robotapi/IBattery.h>
#include <robotapi/webts/WebotsDevice.h>
#include <webots/Robot.hpp>

namespace robotapi {
namespace webts {
//
class WebotsBattery : public robotapi::IBattery, robotapi::webts::WebotsDevice {

	public:

	    void enable(int ms);

	    void disable();

	    double getValue();

	    bool isFull();

	    void setEmptyBias(double bias);

	    bool isEmpty();

	    void setFullBias(double bias);

	    WebotsBattery(webots::Robot & robot, std::string & name, IWbDeviceTag tag);

	private:
		webots::Robot * robot;
    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsBattery_h
