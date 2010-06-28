#include "WebotsPCBattery.h"

namespace robotapi {
namespace webts {

	WebotsPCBattery::WebotsPCBattery(std::string & name, IWbDeviceTag tag) : WebotsDevice ( name, tag ){
	}

	void WebotsPCBattery::enable(int ms){
		return;
	}

	void WebotsPCBattery::disable(){
		return;
	}

	double WebotsPCBattery::getValue(){
		return 0.2;
	}

	bool WebotsPCBattery::isFull(){
		return true;
	}

	void WebotsPCBattery::setEmptyBias(double bias){
	}

	bool WebotsPCBattery::isEmpty(){
		return false;
	}

	void WebotsPCBattery::setFullBias(double bias){
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
