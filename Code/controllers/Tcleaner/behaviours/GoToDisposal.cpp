// Class automatically generated by Dev-C++ New Class wizard

#include "GoToDisposal.h" // class's header file
#include "GarbageCleaner.h"
#include <math.h>

#define BASE_X -0.874047
#define TOLE 0.2
#define BASE_POSITION (BASE_X+TOLE)

#define AFTER_ALIGN_ORIENTATION -1.59682 //-1.59917

/*
4,686365307179586476925286766559
4.68252

-1.59917
4.6825
*/

namespace behaviours {

// class constructor
GoToDisposal::GoToDisposal(WorldInfo * wi, robotapi::IRobot * robot, robotapi::ITrashBin * tb, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss, robotapi::IServo * gate) : AbstractBehaviour("Go To Disposal")
{
	this->trashbin = tb;
	this->wi = wi;
	this->fss = &fss;
	this->wheels = wheels;

	this->trashbin->setFullBias(1);

	behaviours::AbstractBehaviour * ab = new behaviours::FindLine( wi, wheels, fss );
	disposalBehaviours[0] = ab;

	ab = new behaviours::PositionInLine( wi, wheels, fss );
	disposalBehaviours[1] = ab;

	ab = new behaviours::GoToBase( wheels, fss );
	disposalBehaviours[2] = ab;

	ab = new behaviours::UnloadGarbage( tb, gate, robot, wheels, fss );
	disposalBehaviours[3] = ab;

}

// class destructor
GoToDisposal::~GoToDisposal()
{
	// insert your code here
}

void GoToDisposal::sense(){

	if ( this->trashbin->isFull() )
		this->setStimulusPresent();

	/*
	for (int j = 0; j < FLOOR_SENSORS; j++){
		printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
	}
	*/
}

bool followingLine = false;

void GoToDisposal::action(){
	double xpos = this->wheels->getPosition()->getX();

	if ( xpos < BASE_POSITION && fabs( this->wheels->getOrientation() - 3*(PI/2)) < ORIENTATION_TOLE ){
		this->disposalBehaviours[3]->action();
		followingLine = false;
		return;
	}

	if ( ! followingLine ){
		if ( !this->inLine() ){
		    this->disposalBehaviours[0]->action();
	    	printf("Going to line\n");
		    return;
		}

		if ( this->inLine() && !this->inPosition() ){
		    this->disposalBehaviours[1]->action();
	    	printf("On Line, Positioning...\n");
		    return;
		}

	}
	followingLine = true;
//	if ( !this->inLine() && fabs( this->wheels->getOrientation() - PI/2 ) < ORIENTATION_TOLE ){

	if ( !this->inLine() ){
		if ( xpos < BASE_POSITION ){
			this->disposalBehaviours[3]->action();
		}
		followingLine = false;
	}else{
	    this->disposalBehaviours[2]->action();
	    printf("Following Line\n");
	}
    return;
}

bool GoToDisposal::inLine(){
	return (*this->fss).at(0)->getValue() < LINE_THRESHOLD ||
			(*this->fss).at(1)->getValue() < LINE_THRESHOLD ||
			(*this->fss).at(2)->getValue() < LINE_THRESHOLD;
}

bool GoToDisposal::inPosition(){
	double targetAngle = 0;
	if ( this->wheels->getPosition()->getY() < 0.1 )
	    targetAngle = PI;
	return fabs( this->wheels->getOrientation() - targetAngle ) < ORIENTATION_TOLE;
}

} /* End of namespace behaviours */
