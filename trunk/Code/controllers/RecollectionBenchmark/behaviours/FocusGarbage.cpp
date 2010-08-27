#include "FocusGarbage.h"
#include <math.h>
#include <list>
#include <utils/MyAngle.h>
#include <stdio.h>

namespace behaviours {

	FocusGarbage::FocusGarbage(utils::GarbageRecognition * gr, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Focus Garbage"){
		this->gr = gr;
		this->wheels = wheels;
	}

	FocusGarbage::~FocusGarbage(){}

	void FocusGarbage::sense(){
		bool garbagePresent = this->gr->thereIsGarbage();
		if ( ! garbagePresent )
			return;

        std::list<utils::Garbage*> gs = this->gr->getGarbageList();

		// Calculate nearest garbage and angle to it
        this->currentGarbage = this->gr->getClosestGarbage(gs);
        setStimulusPresent();

	}

    void FocusGarbage::action(){
		// Get angle to garbage
        double angleToGarbage = this->gr->angleTo(currentGarbage);
//        printf("Angle To Garbage: %g\n",angleToGarbage);
		double module = fabs(angleToGarbage);

		double leftSpeed = behaviours::BehavioursParameters::getParameter(FOCUS_GARBAGE_BASE_SPD) * ( module / (PI/2) ) + behaviours::BehavioursParameters::getParameter(FOCUS_GARBAGE_MIN_SPD);
		double rightSpeed = leftSpeed;

		// Turn as long as the angle to the garbage is greater than zero
		if ( angleToGarbage < 0 ){
            // Turn left
			leftSpeed *= -1;
		}else{
			// Turn right
			rightSpeed *= -1;
		}
   		this->wheels->setSpeed(leftSpeed,rightSpeed);
		this->currentGarbage = NULL;
	}

} /* End of namespace behaviours */
