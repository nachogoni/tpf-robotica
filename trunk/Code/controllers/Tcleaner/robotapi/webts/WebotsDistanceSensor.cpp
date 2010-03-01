#include "WebotsDistanceSensor.h"

namespace robotapi {
namespace webts {

    void WebotsDistanceSensor::enable(int ms){
		this->myds->enable(ms);
		return;
	}

    void WebotsDistanceSensor::disable(){
		this->myds->disable();
		return;
	}

    int WebotsDistanceSensor::getValue(){
		return (int)this->myds->getValue();
	}

    double WebotsDistanceSensor::getDistance(){
		// TODO Calculate distance according to lookup table & getValue()
		return 5.4;
	}

    // Change parameter to Webots API Distance Sensor
    WebotsDistanceSensor::WebotsDistanceSensor( webots::DistanceSensor & ds) : WebotsDevice ( ds ){
		this->myds = &ds;
	}



} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
