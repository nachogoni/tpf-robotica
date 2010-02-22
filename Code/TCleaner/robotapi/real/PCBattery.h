#ifndef robotapi_real_PCBattery_h
#define robotapi_real_PCBattery_h

#include "robotapi/IBattery.h"
#include <robotapi/real/RealDevice.h>

namespace robotapi {
namespace real {

class PCBattery : public robotapi::IBattery , public robotapi::real::RealDevice {

	public:
		PCBattery(std::string name);

		void enable(int ms);

		void disable();

		double getValue();

		bool isFull();

		void setEmptyBias(double bias);

		bool isEmpty();

		void setFullBias(double bias);

	private:

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_PCBattery_h
