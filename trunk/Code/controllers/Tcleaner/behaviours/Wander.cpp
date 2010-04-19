#include "Wander.h"

namespace behaviours {

	Wander::Wander(robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Wander"){
		this->wheels = wheels;
	}

	Wander::~Wander(){}

    void Wander::sense(){}

    void Wander::action(){
		this->wheels->setSpeed(WANDER_SPD,WANDER_SPD);
	}

} /* End of namespace behaviours */
