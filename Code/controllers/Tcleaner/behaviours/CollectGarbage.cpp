#include "CollectGarbage.h"
#include <utils/MyPoint.h>
#include <list>

#define BOTTOM_SERVO_ANGLE 0.73
#define TIME_STEP 32
#define BASE_SPD 50.0

#define WAIT_STEPS 80
#define WAIT_STEPS_A_LAST 30
#define WAIT_STEPS_LAST 10

#define ANGLE_TOLE 0.1

// Experimental
#define DISTANCE_TOLE 0.0581187

#define SHOVEL_ANGLE -0.1
namespace behaviours {

	int getStepsToGarbage(double distanceInMeters,double speed);

	CollectGarbage::CollectGarbage(utils::GarbageRecognition * gr, robotapi::IRobot * robot, robotapi::ITrashBin * tb, robotapi::IDifferentialWheels * wheels, WorldInfo * wi,  robotapi::IServo * shovel) : AbstractBehaviour("Collect Garbage"){
		this->shovel = shovel;
		this->gr = gr;
		this->wi = wi;
		this->robot = robot;
		this->wheels = wheels;
		this->trashBin = tb;
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
        if ( fabs(angleToGarbage) < ANGLE_TOLE && distanceToGarbage < DISTANCE_TOLE )
	        setStimulusPresent();

	}

    void CollectGarbage::action(){
		this->wheels->setSpeed(0,0);
		this->robot->step(TIME_STEP);

		// Get distance to the garbage
        double distanceToGarbage = this->gr->distanceTo(currentGarbage);

		// Lift up the Shovel
		this->shovel->setPosition(1.57);
		for ( int i = 0 ; i < WAIT_STEPS_A_LAST ; i ++ ){
			this->robot->step(TIME_STEP);
		}

		this->wheels->setSpeed(BASE_SPD,BASE_SPD);

		/*
		int numberOfStepsToGarbage = getStepsToGarbage(0,BASE_SPD);
		for ( int i = 0 ; i < numberOfStepsToGarbage ; i ++ ){
			this->robot->step(TIME_STEP);
		}
		*/
		double distanceCovered = 0;

		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();

		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < distanceToGarbage ){
			this->robot->step(TIME_STEP);
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
		
		// Turn the shovel in order to put the garbage into the trash bin
		this->shovel->setPosition(SHOVEL_ANGLE);
		this->wheels->setSpeed(0,0);

		for ( int i = 0 ; i < WAIT_STEPS_A_LAST ; i ++ ){
			this->robot->step(TIME_STEP);
		}

		this->shovel->setPosition(0);
		for ( int i = 0 ; i < WAIT_STEPS_LAST ; i ++ ){
			this->robot->step(TIME_STEP);
		}

/*
		// Wait till the trashBin changes it's value
		int tbVal = this->trashBin->getValue();
		int oldVal = tbVal;
		while ( tbVal <= oldVal ){
			this->robot->step(TIME_STEP);
			tbVal = this->trashBin->getValue();
		}
		
		// Put the Shovel in its initial state
		this->shovel->setPosition(0);

		for ( int i = 0 ; i < WAIT_STEPS ; i ++ ){
			this->robot->step(TIME_STEP);
		}
*/
		// Make Garbage Recognition take a new image
		this->gr->stepDone();
	}

	int getStepsToGarbage(double distanceInMeters,double speed){
		// TODO
		return 100;
	}

} /* End of namespace behaviours */
