#include "Recharge.h"
#include <behaviours/GoToBaseGroup.h>
#include <math.h>
#include "GarbageCleaner.h"

#define TIME_STEP 32
#define BASE_SPD 20.0
#define R_ORIENTATION_TOLE 0.1

#define BACKWARD_STEPS 75
#define BACKWARD_SPD 100.0

#define FORWARD_STEPS 25
#define FORWARD_SPD 100.0

#define BASE_X -0.847015
#define BASE_Z 0.102655
#define BASE_ANGLE (3.0*PI/2.0)


namespace behaviours {

	Recharge::Recharge(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, WorldInfo * wi, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Recharge"){
       	this->robotBattery = robotBatt;
		this->pcBattery = pcBatt;
		this->wi = wi;
		this->robot = robot;
		this->wheels = wheels;
		this->fss = &fss;
	}

	void Recharge::sense(){
		// If low batt and front touch sensor is activated
	}

	void Recharge::action(){
		// This is a tricky one
		// The robot has to:
		// Go a little bit forward
		// then turn 180 degrees till the middle sensor is on the line
		// and wait till the battery is full
		double currentAngle = this->wheels->getOrientation();

		for( int i = 0 ; i < FORWARD_STEPS ; i ++ ){
      		this->wheels->setSpeed(FORWARD_SPD,FORWARD_SPD);
			this->robot->step(TIME_STEP);
		}

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
		for( int i = 0 ; i < steps/2 ; i++ ){
            for (int j = 0; j < FLOOR_SENSORS; j++){
				printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
			}

			this->robot->step(TIME_STEP);
		}

   		this->wheels->setSpeed(-BACKWARD_SPD,-BACKWARD_SPD);
		for( int i = 0 ; i < BACKWARD_STEPS ; i ++ ){

			this->robot->step(TIME_STEP);
		}
		
		this->wheels->setSpeed(0,0);
		while( !this->robotBattery->isFull() || !this->pcBattery->isFull() )
			this->robot->step(TIME_STEP);

//		ME ESTA ESTALLANDO... PORQUE????

		utils::MyPoint * p = new utils::MyPoint(BASE_X,BASE_Z);
		/*
		printf("ANTES DE SETEAR\n");
//		this->wheels->setPosition(p);
	//	this->wheels->setOrientation(BASE_ANGLE);
		printf("DESPUES DE SETEAR\n");
		*/
		this->robot->step(TIME_STEP);
	}

} /* End of namespace behaviours */
