#include "Wander.h"

namespace behaviours {

	Wander::Wander(robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Wander"){
		this->wheels = wheels;
	}

	Wander::~Wander(){}

    void Wander::sense(){}

    void Wander::action(){
//		this->wheels->setSpeed(100,100);
	}

} /* End of namespace behaviours */
