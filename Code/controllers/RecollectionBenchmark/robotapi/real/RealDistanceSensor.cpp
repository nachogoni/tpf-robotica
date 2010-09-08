#include "RealDistanceSensor.h"

namespace robotapi {
namespace real {

RealDistanceSensor::RealDistanceSensor(protocol::handlers::DistanceSensorBoardPacketHandler * dsbph,
												int sensorNumber,std::string name)
							: RealDevice(&name){
	this->boardHandler = dsbph;
	this->id = sensorNumber;
}

void RealDistanceSensor::enable(int ms){
	this->boardHandler->enable(this->id);
}

void RealDistanceSensor::disable(){
	this->boardHandler->disable(this->id);
}

int RealDistanceSensor::getValue(){
	return this->boardHandler->getValue(this->id);
}

double RealDistanceSensor::getDistance(){
	// TODO Maybe getValue and a lookup table?
	/*
	int value = this->getValue();
	double d = makeConversionUsingLookupTable(value);
	return d;
	*/
	return 0.0;
}

void RealDistanceSensor::refresh(){
	this->boardHandler->getValue(this->id,true);
}


} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
