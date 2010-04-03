#include "WebotsServo.h"

namespace robotapi {
namespace webts {

    WebotsServo::WebotsServo( webots::Servo & servo ) : WebotsDevice ( servo ){
		this->myservo = &servo;
	}

    void WebotsServo::setAcceleration(double force){
        this->myservo->setAcceleration(force);
		return;
	}

    void WebotsServo::setVelocity(double vel){
        this->myservo->setVelocity(vel);
		return;
	}

    void WebotsServo::enablePosition(int ms){
        this->myservo->enablePosition(ms);
		return;
	}

    void WebotsServo::disablePosition(){
        this->myservo->disablePosition();
		return;
	}

    void WebotsServo::setForce(double force){
        this->myservo->setForce(force);
		return;
	}

    void WebotsServo::setMotorForce(double motor_force){
        this->myservo->setMotorForce(motor_force);
		return;
	}

    void WebotsServo::enableMotorForceFeedback(int ms){
        this->myservo->enableMotorForceFeedback(ms);
		return;
	}

    void WebotsServo::disableMotorForceFeedback(){
        this->myservo->disableMotorForceFeedback();
		return;
	}

    double WebotsServo::getMotorForceFeedback(){
        return this->myservo->getMotorForceFeedback();
	}

    double WebotsServo::getPosition(){
		return this->myservo->getPosition();
	}

    void WebotsServo::setPosition(double position){
        this->myservo->setPosition(position);
		return;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
