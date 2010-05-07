#include "GoToGarbage.h" // class's header file
#include <list>

#define ANGLE_TOLE 0.1
#define MAX_SPD 100
#define MIN_SPD 10

namespace behaviours {

	// class constructor
	GoToGarbage::GoToGarbage(utils::GarbageRecognition * gr, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Go to Garbage")
	{
		this->gr = gr;
		this->wheels = wheels;
	}

	// class destructor
	GoToGarbage::~GoToGarbage()
	{
		// insert your code here
	}

	void GoToGarbage::sense(){
		bool garbagePresent = this->gr->thereIsGarbage();
		if ( ! garbagePresent )
			return;

        std::list<utils::Garbage*> gs = this->gr->getGarbageList();

		// Calculate nearest garbage and angle to it
        this->currentGarbage = this->gr->getClosestGarbage(gs);
        double angleToGarbage = this->gr->angleTo(currentGarbage);
        
        if ( fabs(angleToGarbage) < ANGLE_TOLE )
	        setStimulusPresent();
	}

    void GoToGarbage::action(){
		// Get distance to garbage ( it is supposed to be in front of the robot)
        double distanceToGarbage = this->gr->distanceTo(currentGarbage);

		double speed = this->calculateSpeed(distanceToGarbage);
		// Go forward till the garbage is about to leave the screen( with bias )
		this->wheels->setSpeed(speed,speed);
	}

	double GoToGarbage::calculateSpeed(double distanceToGarbage){
		double coeff = distanceToGarbage/(this->gr->getMaximumDistance() - this->gr->getMinimumDistance());
		return MIN_SPD * ( 1 - coeff ) + coeff * MAX_SPD;
	}

}
