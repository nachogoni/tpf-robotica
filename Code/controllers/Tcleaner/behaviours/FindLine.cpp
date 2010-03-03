#include "FindLine.h"

namespace behaviours {

	FindLine::FindLine(robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Find Line"){
		this->robotBatt = robotBatt;
		this->pcBatt = pcBatt;
		this->wheels = wheels;
	}

	void FindLine::sense(){}

    void FindLine::action(){}

} /* End of namespace behaviours */
