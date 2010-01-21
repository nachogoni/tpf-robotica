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

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
