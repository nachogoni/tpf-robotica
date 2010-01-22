#include "WebotsDistanceSensor.h"

namespace robotapi {
namespace webts {

	webots::DistanceSensor * myds;

    void WebotsDistanceSensor::enable(int ms){
		myds->enable(ms);
		return;
	}

    void WebotsDistanceSensor::disable(){
		myds->disable();
		return;
	}

    int WebotsDistanceSensor::getValue(){
		return (int)myds->getValue();
	}

    double WebotsDistanceSensor::getDistance(){
		// TODO Calculate distance according to lookup table & getValue()
		return 5.4;
	}

    // Change parameter to Webots API Distance Sensor
    WebotsDistanceSensor::WebotsDistanceSensor( webots::DistanceSensor & ds) : WebotsDevice ( ds ){
		myds = &ds;
	}



} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
