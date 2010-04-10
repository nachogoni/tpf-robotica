#ifndef robotapi_webts_WebotsDifferentialWheels_h
#define robotapi_webts_WebotsDifferentialWheels_h

#include <robotapi/DifferentialWheelsWOdometry.h>
#include <robotapi/Webts/WebotsDevice.h>
#include <webots/DifferentialWheels.hpp>
#include <WorldInfo.h>

namespace robotapi {
namespace webts {

class WebotsDifferentialWheels : public robotapi::DifferentialWheelsWOdometry {

public:

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

    std::string getName();

    IWbDeviceTag & getTag();

    // Change parameter to Webots API Differential Wheels
    WebotsDifferentialWheels(WorldInfo * wi, webots::DifferentialWheels & wdw);

	private:
		double leftSpeed;
		double rightSpeed;
		webots::DifferentialWheels * dw;

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsDifferentialWheels_h
