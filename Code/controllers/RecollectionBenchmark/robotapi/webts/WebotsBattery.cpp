#include "WebotsBattery.h"

namespace robotapi {
namespace webts {

    WebotsBattery::WebotsBattery(webots::Robot & robot, std::string & name, IWbDeviceTag tag) : WebotsDevice ( name, tag ){
		this->robot = &robot;
		this->emptyBias = 0;
		this->fullBias = 0;
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
		return this->getValue() > this->fullBias;
	}

    void WebotsBattery::setEmptyBias(double bias){
		this->emptyBias = bias;
	}

    bool WebotsBattery::isEmpty(){
		return this->getValue() < this->emptyBias;
	}

    void WebotsBattery::setFullBias(double bias){
		this->fullBias = bias;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
