#include "WebotsBattery.h"

namespace robotapi {
namespace webts {

    WebotsBattery::WebotsBattery(webots::Robot & robot, std::string & name, IWbDeviceTag tag) : WebotsDevice ( name, tag ){
		this->robot = &robot;
		this->bias = 0;
	}

    void WebotsBattery::enable(int ms){
		robot->batterySensorEnable(ms);
	}

    void WebotsBattery::disable(){
		robot->batterySensorDisable();
	}

    double WebotsBattery::getValue(){
		return robot->batterySensorGetValue();
	}

    bool WebotsBattery::isFull(){
		return false;
	}

    void WebotsBattery::setEmptyBias(double bias){
		this->bias = bias;
	}

    bool WebotsBattery::isEmpty(){
		return this->getValue() < this->bias;
	}

    void WebotsBattery::setFullBias(double bias){
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
