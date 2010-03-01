#ifndef robotapi_webts_WebotsPCBattery_h
#define robotapi_webts_WebotsPCBattery_h

#include "robotapi/IBattery.h"
#include <robotapi/webts/WebotsDevice.h>

namespace robotapi {
namespace webts {

class WebotsPCBattery : public robotapi::IBattery , public robotapi::webts::WebotsDevice {

	public:
		WebotsPCBattery(std::string & name, IWbDeviceTag tag);

		void enable(int ms);

		void disable();

		double getValue();

		bool isFull();

		void setEmptyBias(double bias);

		bool isEmpty();

		void setFullBias(double bias);

	private:

	};

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsPCBattery_h
