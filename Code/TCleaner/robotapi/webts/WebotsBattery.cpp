#include "WebotsBattery.h"

namespace robotapi {
namespace webts {

    WebotsBattery::WebotsBattery(std::string & name, IWbDeviceTag tag) : WebotsDevice ( name, tag ){
	}

    void WebotsBattery::enable(int ms){
	}

    void WebotsBattery::disable(){
	}

    double WebotsBattery::getValue(){
		return 2.0;
	}

    bool WebotsBattery::isFull(){
		return false;
	}

    void WebotsBattery::setEmptyBias(double bias){
	}

    bool WebotsBattery::isEmpty(){
		return false;
	}

    void WebotsBattery::setFullBias(double bias){
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
