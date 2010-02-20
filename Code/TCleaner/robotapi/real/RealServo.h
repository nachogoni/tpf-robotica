#ifndef robotapi_real_RealServo_h
#define robotapi_real_RealServo_h

#include <robotapi/IServo.h>
#include <protocol/handlers/ServoBoardPacketHandler.h>

namespace robotapi {
namespace real {

class RealServo : virtual public robotapi::IServo {

	public:
		RealServo::RealServo(protocol::handlers::ServoBoardPacketHandler * sbph, int sensorNumber);

		void setAcceleration(double force);

		void setVelocity(double vel);

		void enablePosition(int ms);

		void disablePosition();

		void setForce(double force);

		void setMotorForce(double motor_force);

		void enableMotorForceFeedback(int ms);

		void disableMotorForceFeedback();

		double getMotorForceFeedback();

		double getPosition();

		void setPosition(double position);

	private:
		protocol::handlers::ServoBoardPacketHandler * boardHandler;
		int id;
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealServo_h
