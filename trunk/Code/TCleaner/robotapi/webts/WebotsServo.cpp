#include "WebotsServo.h"

namespace robotapi {
namespace webts {

	webots::Servo * myservo;

    WebotsServo::WebotsServo( webots::Servo & servo ) : WebotsDevice ( servo ){
		myservo = &servo;
	}

    void WebotsServo::setAcceleration(double force){
        myservo->setAcceleration(force);
		return;
	}

    void WebotsServo::setVelocity(double vel){
        myservo->setVelocity(vel);
		return;
	}

    void WebotsServo::enablePosition(int ms){
        myservo->enablePosition(ms);
		return;
	}

    void WebotsServo::disablePosition(){
        myservo->disablePosition();
		return;
	}

    void WebotsServo::setForce(double force){
        myservo->setForce(force);
		return;
	}

    void WebotsServo::setMotorForce(double motor_force){
        myservo->setMotorForce(motor_force);
		return;
	}

    void WebotsServo::enableMotorForceFeedback(int ms){
        myservo->enableMotorForceFeedback(ms);
		return;
	}

    void WebotsServo::disableMotorForceFeedback(){
        myservo->disableMotorForceFeedback();
		return;
	}

    double WebotsServo::getMotorForceFeedback(){
        return myservo->getMotorForceFeedback();
	}

    double WebotsServo::getPosition(){
		return myservo->getPosition();
	}

    void WebotsServo::setPosition(double position){
        myservo->setPosition(position);
		return;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
