#ifndef robotapi_real_RealDevice_h
#define robotapi_real_RealDevice_h

#include <robotapi/IDevice.h>

namespace robotapi {
namespace real {

class RealDevice : virtual public robotapi::IDevice {

	public:
		RealDevice(std::string * name);
		
		~RealDevice();

		std::string getName();

		IWbDeviceTag & getTag();
		
		void refresh();
	private:
		std::string * dname;
		IWbDeviceTag * dtag;

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealDevice_h
