#include "GoToBase.h"

namespace behaviours {

	GoToBase::GoToBase(robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Go To Base"){
		this->robotBatt = robotBatt;
		this->pcBatt = pcBatt;
		this->wheels = wheels;
	}

	void GoToBase::sense(){}

    void GoToBase::action(){}

} /* End of namespace behaviours */
