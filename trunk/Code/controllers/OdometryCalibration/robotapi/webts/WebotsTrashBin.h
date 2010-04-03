#ifndef robotapi_webts_WebotsTrashBin_h
#define robotapi_webts_WebotsTrashBin_h

#include <robotapi/ITrashBin.h>
#include <robotapi/webts/WebotsDevice.h>
#include <webots/TouchSensor.hpp>

namespace robotapi {
namespace webts {

class WebotsTrashBin : public robotapi::ITrashBin, robotapi::webts::WebotsDevice  {

	public:

		int enable(int ms);

		int disable();

		int getValue();

		bool isFull();

		void setFullBias(double bias);

	    WebotsTrashBin( webots::TouchSensor & ts);

	private:
       	webots::TouchSensor * myts;

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsTrashBin_h
