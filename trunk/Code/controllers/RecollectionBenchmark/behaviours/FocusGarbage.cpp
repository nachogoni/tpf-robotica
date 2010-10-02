#include "FocusGarbage.h"
#include <math.h>
#include <list>
#include <utils/MyAngle.h>
#include <stdio.h>

namespace behaviours {

	FocusGarbage::FocusGarbage(vision::IGarbageRecognition * gr, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Focus Garbage"){
		this->gr = gr;
		this->wheels = wheels;
		printf("Focus garbage");
	}

	FocusGarbage::~FocusGarbage(){}

	void FocusGarbage::sense(){
		bool garbagePresent = this->gr->thereIsGarbage();
		if ( ! garbagePresent )
			return;

        std::list<vision::Garbage*> gs = this->gr->getGarbageList();

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
		printf("Angle To garbage: %g\n",angleToGarbage);
		printf("Module: %g\n",module);
		printf("speeds: %g - %g\n",leftSpeed,rightSpeed);
		
   		this->wheels->setSpeed(leftSpeed,rightSpeed);
		this->currentGarbage = NULL;
	}

} /* End of namespace behaviours */
