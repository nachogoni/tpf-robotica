#ifndef robotapi_webts_WebotsTrashBin_h
#define robotapi_webts_WebotsTrashBin_h

#include <robotapi/ITrashBin.h>
#include <robotapi/webts/WebotsDevice.h>

namespace robotapi {
namespace webts {

class WebotsTrashBin : public robotapi::ITrashBin, robotapi::webts::WebotsDevice  {

	public:

		int enable(int ms);

		int disable();

		int getValue();

		bool isFull();

		void setFullBias(double bias);

	    WebotsTrashBin(std::string & name, IWbDeviceTag tag);
    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsTrashBin_h
