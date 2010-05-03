#include "FocusGarbage.h"
#include <math.h>
#include <list>
#include <utils/MyAngle.h>
#include <stdio.h>

#define BASE_SPD 100
#define MIN_SPD 1

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

		printf("PRESENTE? SI\n");
        std::list<utils::Garbage*> gs = this->gr->getGarbageList();

		// Calculate nearest garbage and angle to it
        this->currentGarbage = this->gr->getClosestGarbage(gs);
        setStimulusPresent();

	}

    void FocusGarbage::action(){
		// Get angle to garbage
        double angleToGarbage = this->gr->angleTo(currentGarbage);
		double module = fabs(angleToGarbage);

		double leftSpeed = BASE_SPD * ( module / (PI/2) ) + MIN_SPD;
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
	}

} /* End of namespace behaviours */
