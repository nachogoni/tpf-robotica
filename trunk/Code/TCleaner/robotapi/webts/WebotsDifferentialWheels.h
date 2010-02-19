#ifndef robotapi_webts_WebotsDifferentialWheels_h
#define robotapi_webts_WebotsDifferentialWheels_h

#include <robotapi/IDifferentialWheels.h>
#include <robotapi/Webts/WebotsDevice.h>
#include <webots/DifferentialWheels.hpp>

namespace robotapi {
namespace webts {

//class WebotsDifferentialWheels : public robotapi::IDifferentialWheels , robotapi::webts::WebotsDevice  {
class WebotsDifferentialWheels : public robotapi::IDifferentialWheels {

public:

    void setSpeed(double left, double right);

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

	bool isMotorOff();

    std::string getName();

    IWbDeviceTag & getTag();

    // Change parameter to Webots API Differential Wheels
    WebotsDifferentialWheels(webots::DifferentialWheels & wdw);

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsDifferentialWheels_h
