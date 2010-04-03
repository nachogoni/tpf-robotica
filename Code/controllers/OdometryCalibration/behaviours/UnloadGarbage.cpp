#include "UnloadGarbage.h"

namespace behaviours {

		UnloadGarbage::UnloadGarbage(robotapi::ITrashBin * tb, robotapi::IServo * gate) : AbstractBehaviour("Unload Garbage"){
			this->trashbin = tb;
			this->gate = gate;
		}

		UnloadGarbage::~UnloadGarbage(){}
		
	    void UnloadGarbage::sense(){
		}

    	void UnloadGarbage::action(){
		}

/*
TODO FOR UNLOADING BEHAVIOUR

		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);
		servoRear->setPosition(-1.57);

		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);

		servoRear->setPosition(0);
		for( int i = 0; i < 80 ; i++ )
			this->myIRobot.step(TIME_STEP);
*/
} /* End of namespace behaviours */
