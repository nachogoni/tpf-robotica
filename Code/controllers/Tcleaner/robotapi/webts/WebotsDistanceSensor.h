#ifndef robotapi_webts_WebotsDistanceSensor_h
#define robotapi_webts_WebotsDistanceSensor_h

#include <robotapi/IDistanceSensor.h>
#include <robotapi/webts/WebotsDevice.h>
#include <webots/DistanceSensor.hpp>

namespace robotapi {
namespace webts {

class WebotsDistanceSensor : public robotapi::IDistanceSensor, robotapi::webts::WebotsDevice  {

	public:

	    void enable(int ms);

	    void disable();

	    int getValue();

	    double getDistance();

	    // Change parameter to Webots API Distance Sensor
	    WebotsDistanceSensor( webots::DistanceSensor & ds);

	private:
		webots::DistanceSensor * myds;

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsDistanceSensor_h
