#include "DifferentialWheelsWOdometry.h"
#include <math.h>

namespace robotapi {

DifferentialWheelsWOdometry::DifferentialWheelsWOdometry(double distanceBetweenWheels, double wheelRadius,
							double encoderResolution, double initialX,
							double initialY, double initialTheta){
 		this->distanceBetweenWheels = distanceBetweenWheels;
		this->wheelRadius = wheelRadius;
		this->encoderResolution = encoderResolution;
		this->lastPosition = new utils::MyPoint(initialX,initialY);
		this->lastAngle = new utils::MyAngle(initialTheta);
		this->lastLeftEncoder = this->getLeftEncoder();
		this->lastRightEncoder = this->getRightEncoder();
}

double DifferentialWheelsWOdometry::getLeftEncoder(){return 0.0;}
double DifferentialWheelsWOdometry::getRightEncoder(){return 0.0;}

void DifferentialWheelsWOdometry::computeOdometry() {
	double l = this->getLeftEncoder();
	double r = this->getRightEncoder();
	double dl = (l - this->lastLeftEncoder ) * this->wheelRadius / this->encoderResolution ; // distance covered by left wheel in meter
	double dr = (r - this->lastRightEncoder ) * this->wheelRadius / this->encoderResolution ; // distance covered by right wheel in meter

	this->computePosition(dl, dr);

	this->lastLeftEncoder = l;
	this->lastRightEncoder = r;

	//printf("estimated distance covered by left wheel: %g m.\n",dl);
	//printf("estimated distance covered by right wheel: %g m.\n",dr);
	//printf("estimated change of orientation: %g rad.\n",da);
}

void DifferentialWheelsWOdometry::computePosition(double ldist, double rdist){
	double lc = 0;
	lc = (ldist+rdist)/2.0;

	utils::MyAngle * dtita = new utils::MyAngle((rdist-ldist)/this->distanceBetweenWheels); // delta orientation
	this->lastAngle->add(dtita);
	delete dtita;

	this->lastPosition->add(lc * cos(this->lastAngle->getNormalizedValue()), lc * sin(this->lastAngle->getNormalizedValue()));
	return;
}

double DifferentialWheelsWOdometry::getOrientation(){
	return this->lastAngle->getNormalizedValue();
}

utils::MyPoint * DifferentialWheelsWOdometry::getPosition(){
	return this->lastPosition;
}

} /* End of namespace robotapi */
