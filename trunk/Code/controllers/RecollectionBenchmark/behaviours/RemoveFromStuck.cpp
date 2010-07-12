#include "RemoveFromStuck.h"
#include "GarbageCleaner.h"
#include <sstream>

namespace behaviours {

	RemoveFromStuck::RemoveFromStuck(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Removing From A Stuck Situation"){
		this->wheels = wheels;
		this->robot = robot;
		this->steps = 0;
	}

	RemoveFromStuck::~RemoveFromStuck(){}

	void RemoveFromStuck::sense(){
		// pido angulo actual

		// calculo desv std de angulo

		// si es mayor a mi umbral, seteo steps en 0 y el desv std, contadores

		// si "estuvo mucho tiempo" con "el mismo angulo", activo el comportamiento
		if ( steps >= ACTIVATION_STEPS ){
			setStimulusPresent();
			//seteo steps en 0 y el desv std, contadores
		}
	}

	void RemoveFromStuck::action(){
		double cAngle = this->wheels->getOrientation();

		
	}


} /* End of namespace behaviours */
