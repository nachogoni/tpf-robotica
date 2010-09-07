#ifndef robotapi_real_RealDifferentialWheels_h
#define robotapi_real_RealDifferentialWheels_h

#include <robotapi/IDifferentialWheels.h>
#include <robotapi/DifferentialWheelsWOdometry.h>
#include <protocol/handlers/DCMotorBoardPacketHandler.h>
#include <robotapi/real/RealDevice.h>
#include <WorldInfo.h>

namespace robotapi {
namespace real {

class RealDifferentialWheels : public robotapi::DifferentialWheelsWOdometry , public robotapi::real::RealDevice {

	public:
		RealDifferentialWheels(WorldInfo * wi, protocol::handlers::DCMotorBoardPacketHandler * dcmbphl,
								protocol::handlers::DCMotorBoardPacketHandler * dcmbphr,
								std::string name);

		~RealDifferentialWheels();

    	void setSpeed(double left, double right);

		double getLeftSpeed();

		double getRightSpeed();

    	void enableEncoders(int ms);

    	void disableEncoders();

    	double getLeftEncoder();

    	double getRightEncoder();

    	void moveWheels(double left, double right);

    	void moveLeftWheel(double left);

    	void moveRightWheel(double right);

    	void moveLeftWheel(double left, double speed);

    	void moveRightWheel(double right, double speed);

    	double getLeftMotorConsumption();

    	double getRightMotorConsumption();

    	bool isAlarmPresent();

		bool isAlarmPresent(bool left);

		bool motorIsOff();

		bool motorIsOff(bool left);
		
		void refresh();

	private:
		protocol::handlers::DCMotorBoardPacketHandler * leftBoard;
		protocol::handlers::DCMotorBoardPacketHandler * rightBoard;

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealDifferentialWheels_h
