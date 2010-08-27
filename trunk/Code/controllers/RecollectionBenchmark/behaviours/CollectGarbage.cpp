#include "CollectGarbage.h"
#include <utils/MyPoint.h>
#include <list>

namespace behaviours {

	int getStepsToGarbage(double distanceInMeters,double speed);

	CollectGarbage::CollectGarbage(utils::GarbageRecognition * gr, robotapi::IRobot * robot, robotapi::ITrashBin * tb, robotapi::IDifferentialWheels * wheels, WorldInfo * wi,  robotapi::IServo * shovel, robotapi::IServo * container) : AbstractBehaviour("Collect Garbage"){
		this->shovel = shovel;
		this->gr = gr;
		this->wi = wi;
		this->robot = robot;
		this->wheels = wheels;
		this->trashBin = tb;
		this->cont = container;
	}
	
	CollectGarbage::~CollectGarbage(){}
	
	void CollectGarbage::sense(){
		bool garbagePresent = this->gr->thereIsGarbage();
		if ( ! garbagePresent )
			return;

        std::list<utils::Garbage*> gs = this->gr->getGarbageList();

		// Calculate nearest garbage and angle to it
        this->currentGarbage = this->gr->getClosestGarbage(gs);
		double angleToGarbage = this->gr->angleTo(currentGarbage);
        double distanceToGarbage = this->gr->distanceTo(currentGarbage);

		// If there is garbage and the distance to the garbage is less than the
		// bias, then the behaviour is present
        if ( fabs(angleToGarbage) < behaviours::BehavioursParameters::getParameter(COLLECT_ANGLE_TOLE) && distanceToGarbage < behaviours::BehavioursParameters::getParameter(DISTANCE_TOLE) )
	        setStimulusPresent();

	}

    void CollectGarbage::action(){
		this->wheels->setSpeed(0,0);
		this->robot->step(behaviours::BehavioursParameters::getParameter(COLLECT_TIME_STEP));

		printf("COLLECTING\n");
		// Get distance to the garbage
        double distanceToGarbage = this->gr->distanceTo(currentGarbage);

		// Lift up the Shovel
		this->shovel->setPosition(1.57);
		this->cont->setPosition(0);

		for ( int i = 0 ; i < behaviours::BehavioursParameters::getParameter(COLLECT_WAIT_STEPS_A_LAST) ; i ++ ){
			this->robot->step(behaviours::BehavioursParameters::getParameter(COLLECT_TIME_STEP));
		}

		this->wheels->setSpeed(behaviours::BehavioursParameters::getParameter(COLLECT_BASE_SPD),behaviours::BehavioursParameters::getParameter(COLLECT_BASE_SPD));

		/*
		int numberOfStepsToGarbage = getStepsToGarbage(0,behaviours::BehavioursParameters::getParameter(BASE_SPD));
		for ( int i = 0 ; i < numberOfStepsToGarbage ; i ++ ){
			this->robot->step(behaviours::BehavioursParameters::getParameter(TIME_STEP));
		}
		*/
		double distanceCovered = 0;

		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();

		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < (distanceToGarbage + 0.001) ){
			this->robot->step(behaviours::BehavioursParameters::getParameter(COLLECT_TIME_STEP));
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
		
		// Turn the shovel in order to put the garbage into the trash bin
		this->shovel->setPosition(behaviours::BehavioursParameters::getParameter(SHOVEL_ANGLE));
		this->wheels->setSpeed(0,0);

		for ( int i = 0 ; i < behaviours::BehavioursParameters::getParameter(COLLECT_WAIT_STEPS) ; i ++ ){
			this->robot->step(behaviours::BehavioursParameters::getParameter(COLLECT_TIME_STEP));
		}

		this->shovel->setPosition(0);
		for ( int i = 0 ; i < behaviours::BehavioursParameters::getParameter(COLLECT_WAIT_STEPS) ; i ++ ){
			this->robot->step(behaviours::BehavioursParameters::getParameter(COLLECT_TIME_STEP));
		}

		// Make Garbage Recognition take a new image
		this->gr->stepDone();
	}

} /* End of namespace behaviours */

