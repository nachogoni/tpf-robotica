#include "Recharge.h"
#include <behaviours/GoToBaseGroup.h>
#include <math.h>
#include "GarbageCleaner.h"

#define TIME_STEP 32
#define BASE_SPD 20.0
#define R_ORIENTATION_TOLE 0.1

#define BACKWARD_STEPS 40
#define BACKWARD_SPD 100.0

#define FORWARD_SPD 100.0

#define BASE_X -0.847015
#define BASE_Z 0.102655
#define BASE_ANGLE (3.0*PI/2.0)

#define BASE_SPD 50.0
#define SPD_FACTOR 0.7
#define E_PUCK_DIAMETER 0.052


namespace behaviours {

	Recharge::Recharge(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Recharge"){
       	this->robotBattery = robotBatt;
		this->pcBattery = pcBatt;
		this->robot = robot;
		this->wheels = wheels;
		this->fss = &fss;
	}

	void Recharge::sense(){
	}

	void Recharge::action(){
		// This is a tricky one
		// The robot has to:
		// Go a little bit forward
		// then turn 180 degrees till the middle sensor is on the line
		// and wait till the battery is full

		this->positionSelf();
/*
		printf("HOLA4\n");
		system("pause");
*/

		this->lastRobotValue = this->robotBattery->getValue();
		this->lastPCValue = this->pcBattery->getValue();

		// Go back a little bit till the sensor is not on the line anymore
   		this->wheels->setSpeed(-BACKWARD_SPD,-BACKWARD_SPD);
		while( (*this->fss).at(1)->getValue() < LINE_THRESHOLD){
			printf("ACA\n");
			this->robot->step(TIME_STEP);
		}
		
		while ( !this->isCharging() ){
			printf("ACA2\n");
			this->robot->step(TIME_STEP);
		}

		this->wheels->setSpeed(0,0);
		while( !this->robotBattery->isFull() || !this->pcBattery->isFull() ){
			printf("ACA3\n");
			this->robot->step(TIME_STEP);
		}

		// Go forward till the sensor is on the line again
   		this->wheels->setSpeed(FORWARD_SPD,FORWARD_SPD);
		while( (*this->fss).at(1)->getValue() < LINE_THRESHOLD){
			printf("ACA4\n");
			this->robot->step(TIME_STEP);
		}
	}

	bool Recharge::isCharging(){
		double currentRobot = this->robotBattery->getValue();
		double currentPC = this->pcBattery->getValue();
		bool charging = this->lastRobotValue < currentRobot || this->lastPCValue < currentPC;
		this->lastRobotValue = currentRobot;
		this->lastPCValue = currentPC;
		return charging;
	}

	void Recharge::positionSelf(){
		double currentAngle = this->wheels->getOrientation();
		if ( ! ( currentAngle > 0 && currentAngle < PI ) )
			return;

   		this->wheels->setSpeed(FORWARD_SPD,FORWARD_SPD);
		this->goDistance(E_PUCK_DIAMETER/2.0);
		
		bool leftOnLine = (*this->fss).at(0)->getValue() < LINE_THRESHOLD;
		bool middleOnLine = (*this->fss).at(1)->getValue() < LINE_THRESHOLD;
		bool rightOnLine = (*this->fss).at(2)->getValue() < LINE_THRESHOLD;
		
		// Turn till the middle sensor is on the line
		this->wheels->setSpeed(BASE_SPD,-BASE_SPD);
		while( leftOnLine || !middleOnLine || rightOnLine ){
	        for (int j = 0; j < FLOOR_SENSORS; j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}
			this->robot->step(TIME_STEP);
			//currentAngle = this->wheels->getOrientation();
			leftOnLine = (*this->fss).at(0)->getValue() < LINE_THRESHOLD;
			middleOnLine = (*this->fss).at(1)->getValue() < LINE_THRESHOLD;
			rightOnLine = (*this->fss).at(2)->getValue() < LINE_THRESHOLD;
		}
		
		int steps = 0;
		// Continue turning till the middle sensor out of the line. Count Steps
		while( middleOnLine ){
	        for (int j = 0; j < FLOOR_SENSORS; j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}

			this->robot->step(TIME_STEP);
			//currentAngle = this->wheels->getOrientation();
			middleOnLine = (*this->fss).at(1)->getValue() < LINE_THRESHOLD;
			steps++;
		}
		
		// Now, turn steps/2 in the other direction, middle sensor should be in the middle of the line
		this->wheels->setSpeed(-BASE_SPD,BASE_SPD);
		for( int i = 0 ; i < (int)(ceil(steps/2.0)) ; i++ ){
            for (int j = 0; j < FLOOR_SENSORS; j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}

			this->robot->step(TIME_STEP);
		}

		this->alignWithLine();
	}

	void Recharge::alignWithLine(){
		this->followLine(false,E_PUCK_DIAMETER);
		this->followLine(true,E_PUCK_DIAMETER);
    }

	void Recharge::followLine(bool backwards, double distance){
		double distanceCovered = 0;
	
		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();
	
		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < distance ){
    		this->followLineSpd(backwards);
			this->robot->step(TIME_STEP);
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
	}

    void Recharge::followLineSpd(bool backwards){
		double lspd = BASE_SPD;
		double rspd = BASE_SPD;
		
		if ( backwards ){
			lspd = -1 * lspd;
			rspd = -1 * rspd;
		}

		// Left sensor is on line?
		if ( (*this->fss).at(0)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 - SPD_FACTOR );
			rspd = rspd * ( 1 + SPD_FACTOR );
		}
		// Right sensor is on line?
		if ( (*this->fss).at(2)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 + SPD_FACTOR );
			rspd = rspd * ( 1 - SPD_FACTOR );
		}
		this->wheels->setSpeed(lspd,rspd);
	}

	void Recharge::goDistance(double distance){
		double distanceCovered = 0;
	
		utils::MyPoint * lastPosition = this->wheels->getPosition();
		double initialX = lastPosition->getX();
		double initialY = lastPosition->getY();
	
		double currentX, currentY;
		utils::MyPoint * currentPosition;
		while ( distanceCovered < distance ){
			this->robot->step(TIME_STEP);
			currentPosition = this->wheels->getPosition();
			currentX = currentPosition->getX();
			currentY = currentPosition->getY();
			distanceCovered = sqrt( pow(currentX-initialX,2) + pow(currentY-initialY,2) );
		}
	}

} /* End of namespace behaviours */
