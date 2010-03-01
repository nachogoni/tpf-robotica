#include "CollectGarbage.h"

namespace behaviours {

	CollectGarbage::CollectGarbage(robotapi::IServo * lifter, robotapi::IServo * shovel) : AbstractBehaviour("Collect Garbage"){
		this->lifter = lifter;
		this->shovel = shovel;
	}
	
	CollectGarbage::~CollectGarbage(){}
	
	void CollectGarbage::sense(){
	}

    void CollectGarbage::action(){
	}

	/*
TODO FOR COLLECTING BEHAVIOUR
		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);
		servoFront->setPosition(1.57);

		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

		servoBottom->setPosition(1.57/2);
		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);
		servoFront->setPosition(-1.57);

		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

		servoFront->setPosition(0);
		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

		servoBottom->setPosition(0);
		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

*/
} /* End of namespace behaviours */
