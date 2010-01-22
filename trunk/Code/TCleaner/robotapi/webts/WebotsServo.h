#ifndef robotapi_webts_WebotsServo_h
#define robotapi_webts_WebotsServo_h

#include <robotapi/IServo.h>
#include <robotapi/webts/WebotsDevice.h>
#include <webots/Servo.hpp>


namespace robotapi {
namespace webts {

class WebotsServo : public robotapi::IServo, robotapi::webts::WebotsDevice {

 public:

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

    // Change parameter to Webots API Distance Sensor
    WebotsServo( webots::Servo & servo);

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsServo_h
