#ifndef robotapi_real_RealDistanceSensor_h
#define robotapi_real_RealDistanceSensor_h

#include <robotapi/IDistanceSensor.h>
#include <protocol/handlers/DistanceSensorBoardPacketHandler.h>


namespace robotapi {
namespace real {

class RealDistanceSensor : virtual public robotapi::IDistanceSensor {

	public:
		RealDistanceSensor::RealDistanceSensor(protocol::handlers::DistanceSensorBoardPacketHandler * dsbph,
												int sensorNumber);

		void enable(int ms);

		void disable();

		int getValue();

		double getDistance();

	private:
		protocol::handlers::DistanceSensorBoardPacketHandler * boardHandler;
		int id;
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealDistanceSensor_h
