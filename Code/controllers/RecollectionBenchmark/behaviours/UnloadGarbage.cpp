#include "UnloadGarbage.h"
#include "GoToBaseGroup.h"
#include "GarbageCleaner.h"
#include <math.h>

namespace behaviours {

	UnloadGarbage::UnloadGarbage(robotapi::ITrashBin * tb, robotapi::IServo * gate, robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Unload Garbage"){
		this->trashbin = tb;
		this->gate = gate;
		this->robot = robot;
		this->wheels = wheels;
		this->fss = &fss;
		this->timesEntered = 0;
	}

	UnloadGarbage::~UnloadGarbage(){}
		
    void UnloadGarbage::sense(){
	}

   	void UnloadGarbage::action(){
		// This is a tricky one
		// The robot has to:
		// Go a little bit forward
		// then turn 180 degrees till the middle sensor is on the line
		// open the trashbin's gate
		// go forward a little bit
		// and close the trashbin's gate
	
		this->positionSelf();
		this->timesEntered = this->timesEntered + 1;
    	printf("Unloading Garbage - Times Entered: %d\n", this->timesEntered);

		// Open Gate!
		this->wheels->setSpeed(0,0);
		this->gate->setPosition(-1.57);

		for( int i = 0; i < GET_PARAM(UNLOAD_WAIT_STEPS) ; i++ )
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));

		// Go forward a distance of GET_PARAM(UNLOAD_E_PUCK_DIAMETER)
		this->wheels->setSpeed(GET_PARAM(UNLOAD_BASE_SPD),GET_PARAM(UNLOAD_BASE_SPD));

		goDistance(GET_PARAM(UNLOAD_E_PUCK_DIAMETER));

		// Close Gate!
		this->gate->setPosition(0);
		for( int i = 0; i < GET_PARAM(UNLOAD_WAIT_STEPS) ; i++ )
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));

		this->trashbin->disable();
		this->trashbin->enable(GET_PARAM(UNLOAD_TIME_STEP));

		this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
	}

	void UnloadGarbage::positionSelf(){
		double currentAngle = this->wheels->getOrientation();
		if ( ! ( currentAngle > 0 && currentAngle < PI ) )
			return;

   		this->wheels->setSpeed(GET_PARAM(UNLOAD_FORWARD_SPD),GET_PARAM(UNLOAD_FORWARD_SPD));
		this->goDistance(GET_PARAM(UNLOAD_E_PUCK_DIAMETER)/2.0);
		
		bool leftOnLine = this->inLine((*this->fss).at(0)->getValue());
		bool middleOnLine = this->inLine((*this->fss).at(1)->getValue());
		bool rightOnLine = this->inLine((*this->fss).at(2)->getValue());
		
		// Turn till the middle sensor is on the line
		this->wheels->setSpeed(GET_PARAM(UNLOAD_BASE_SPD),-GET_PARAM(UNLOAD_BASE_SPD));
		while( leftOnLine || !middleOnLine || rightOnLine ){
	        for (int j = 0; j < GET_PARAM(FLOOR_SENSORS); j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
			//currentAngle = this->wheels->getOrientation();
			leftOnLine = this->inLine((*this->fss).at(0)->getValue());
			middleOnLine = this->inLine((*this->fss).at(1)->getValue());
			rightOnLine = this->inLine((*this->fss).at(2)->getValue());
		}
		
		int steps = 0;
		// Continue turning till the middle sensor out of the line. Count Steps
		while( middleOnLine ){
	        for (int j = 0; j < GET_PARAM(FLOOR_SENSORS); j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}

			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
			//currentAngle = this->wheels->getOrientation();
			middleOnLine = this->inLine((*this->fss).at(1)->getValue());
			steps++;
		}
		
		// Now, turn steps/2 in the other direction, middle sensor should be in the middle of the line
		this->wheels->setSpeed(-GET_PARAM(UNLOAD_BASE_SPD),GET_PARAM(UNLOAD_BASE_SPD));
		for( int i = 0 ; i < (int)(ceil(steps/2.0)) ; i++ ){
			/*
            for (int j = 0; j < GET_PARAM(FLOOR_SENSORS); j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}
			*/
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
		}

		this->alignWithLine();
	}

	void UnloadGarbage::alignWithLine(){
		this->followLine(false,GET_PARAM(UNLOAD_E_PUCK_DIAMETER));
		this->followLine(true,GET_PARAM(UNLOAD_E_PUCK_DIAMETER));
    }

	void UnloadGarbage::followLine(bool backwards, double distance){
		double distanceCovered = 0;
	
		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();
	
		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < distance ){
    		this->followLineSpd(backwards);
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
	}

    void UnloadGarbage::followLineSpd(bool backwards){
		double lspd = GET_PARAM(UNLOAD_BASE_SPD);
		double rspd = GET_PARAM(UNLOAD_BASE_SPD);
		
		if ( backwards ){
			lspd = -1 * lspd;
			rspd = -1 * rspd;
		}

		// Left sensor is on line?
		if ( this->inLine((*this->fss).at(0)->getValue()) ){
			lspd = lspd * ( 1 - GET_PARAM(UNLOAD_SPD_FACTOR) );
			rspd = rspd * ( 1 + GET_PARAM(UNLOAD_SPD_FACTOR) );
		}
		// Right sensor is on line?
		if ( this->inLine((*this->fss).at(2)->getValue()) ){
			lspd = lspd * ( 1 + GET_PARAM(UNLOAD_SPD_FACTOR) );
			rspd = rspd * ( 1 - GET_PARAM(UNLOAD_SPD_FACTOR) );
		}
		this->wheels->setSpeed(lspd,rspd);
	}

	void UnloadGarbage::goDistance(double distance){
		double distanceCovered = 0;
	
		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();
	
		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < distance ){
			this->robot->step(GET_PARAM(UNLOAD_TIME_STEP));
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
	}

} /* End of namespace behaviours */
