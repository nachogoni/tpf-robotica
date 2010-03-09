#include "FocusGarbage.h"

namespace behaviours {

	FocusGarbage::FocusGarbage(robotapi::ICamera * camera, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Focus Garbage"){
		this->gr = new utils::GarbageRecognition();
		this->gr->setCamera(*camera);
		this->wheels = wheels;
	}

	FocusGarbage::~FocusGarbage(){}

	void FocusGarbage::sense(){
		//this->gr->thereIsGarbage();
	}

    void FocusGarbage::action(){}

} /* End of namespace behaviours */
