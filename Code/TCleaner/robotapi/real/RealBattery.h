#ifndef robotapi_real_RealBattery_h
#define robotapi_real_RealBattery_h

#include "robotapi/IBattery.h"
#include <protocol/handlers/BatteryBoardPacketHandler.h>


namespace robotapi {
namespace real {

class RealBattery : virtual public robotapi::IBattery {

	public:
		RealBattery::RealBattery(protocol::handlers::BatteryBoardPacketHandler * bbph);

		void enable(int ms);

		void disable();

		double getValue();

		bool isFull();

		void setEmptyBias(double bias);

		bool isEmpty();

		void setFullBias(double bias);

	private:
		protocol::handlers::BatteryBoardPacketHandler * bbph;

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealBattery_h
