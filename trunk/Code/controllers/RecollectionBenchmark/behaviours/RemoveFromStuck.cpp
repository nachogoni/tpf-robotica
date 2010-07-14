#include "RemoveFromStuck.h"
#include "GarbageCleaner.h"
#include <sstream>
#include <float.h>

namespace behaviours {

	RemoveFromStuck::RemoveFromStuck(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Removing From A Stuck Situation"){
		this->wheels = wheels;
		this->robot = robot;
		this->resetCounters();
	}

	RemoveFromStuck::~RemoveFromStuck(){}

	void RemoveFromStuck::sense(){

		//double cAngle = this->wheels->getOrientation();
		utils::MyPoint * pos = this->wheels->getPosition();

		this->lastXMin = this->getMin(this->lastXMin,pos->getX());
		this->lastZMin = this->getMin(this->lastZMin,pos->getY());
		this->lastXMax = this->getMax(this->lastXMax,pos->getX());
		this->lastZMax = this->getMax(this->lastZMax,pos->getY());
/*
		this->angles[this->steps] = cAngle;
		this->xs[this->steps] = pos->getX();
		this->zs[this->steps] = pos->getY();
		this->steps++;
		this->sumA += cAngle;
		this->sumX += pos->getX();
		this->sumZ += pos->getY();
		// calculo desv std de angulo
		double desvstdA = this->desvStd(this->angles, this->sumA);
		double desvstdX = this->desvStd(this->xs, this->sumX);
		double desvstdZ = this->desvStd(this->zs, this->sumZ);
*/
/*
		printf("Steps : %d - devstdA : %d - devstdX : %d - devstdZ : %d\n",this->steps, desvstdA > REMOVE_DESVSTD_ANGLE_THRESHOLD? 1 : 0,
			 	desvstdX > REMOVE_DESVSTD_DIST_THRESHOLD? 1 : 0, desvstdZ > REMOVE_DESVSTD_DIST_THRESHOLD? 1 : 0);
*/
		// si es mayor a mi umbral, seteo steps en 0 y la suma
/*
		if ( desvstdA > REMOVE_DESVSTD_ANGLE_THRESHOLD ||
			 desvstdX > REMOVE_DESVSTD_DIST_THRESHOLD ||
			 desvstdZ > REMOVE_DESVSTD_DIST_THRESHOLD){
*/
		if ( fabs(this->lastXMax - this->lastXMin) > REMOVE_DESVSTD_DIST_THRESHOLD ||
			 fabs(this->lastZMax - this->lastZMin) > REMOVE_DESVSTD_DIST_THRESHOLD ){
			this->resetCounters();
		}

		// si "estuvo mucho tiempo" con "el mismo angulo" y "en el mismo lugar", activo el comportamiento
		if ( this->steps >= ACTIVATION_STEPS ){
			setStimulusPresent();
			this->resetCounters();
			//system("pause");
		}
	}

	void RemoveFromStuck::resetCounters(){
		this->lastXMax = DBL_MIN;
		this->lastXMin = DBL_MAX;
		this->lastZMax = DBL_MIN;
		this->lastZMin = DBL_MAX;
		this->steps = 0;
	}

	double RemoveFromStuck::getMin(double oldValue, double newValue){
		if ( oldValue > newValue )
			return newValue;
		return oldValue;
	}

	double RemoveFromStuck::getMax(double oldValue, double newValue){
		if ( oldValue < newValue )
			return newValue;
		return oldValue;
	}

	double RemoveFromStuck::desvStd(double values [] , double sum){
		double mean = sum / this->steps;
		int i;
		double acum = 0;
		for ( i = 0 ; i < this->steps ; i++ )
			acum += ( (values[i] - mean) * (values[i] - mean));
		
		if ( this->steps == 1 )
			acum = acum / this->steps;
		else
			acum = acum / (this->steps - 1);
		return acum;
	}
 
	void RemoveFromStuck::action(){
		double cAngle = this->wheels->getOrientation();
		utils::MyAngle * ma = new utils::MyAngle(cAngle+REMOVE_ANGLE_TO_ADD);
		double targetAngle = ma->getNormalizedValue();
		delete ma;

		this->wheels->setSpeed(REMOVE_SPD,-REMOVE_SPD);
		while ( fabs(targetAngle - cAngle) > REMOVE_ANGLE_TOLE ){
			this->robot->step(REMOVE_STEP);
			cAngle = this->wheels->getOrientation();
		}
	}


} /* End of namespace behaviours */
