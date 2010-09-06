#ifndef robotapi_real_RealBattery_h
#define robotapi_real_RealBattery_h

#include "robotapi/IBattery.h"
#include <protocol/handlers/BatteryBoardPacketHandler.h>
#include <robotapi/real/RealDevice.h>

namespace robotapi {
namespace real {

class RealBattery : public robotapi::IBattery , public robotapi::real::RealDevice {

	public:
		RealBattery(protocol::handlers::BatteryBoardPacketHandler * bbph,std::string name, int sensorNumber);

		void enable(int ms);

		void disable();

		double getValue();

		bool isFull();

		void setEmptyBias(double bias);

		bool isEmpty();

		void setFullBias(double bias);

	private:
		protocol::handlers::BatteryBoardPacketHandler * bbph;
		int id;
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealBattery_h
