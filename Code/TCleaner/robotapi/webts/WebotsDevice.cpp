#include "WebotsDevice.h"

namespace robotapi {
namespace webts {
/*
    WebotsDevice::WebotsDevice(std::string name, IWbDeviceTag tag){
        dname = name;
        dtag = tag;
    }

    std::string WebotsDevice::getName(){
        return dname;
    }

    IWbDeviceTag & WebotsDevice::getTag(){
        return dtag;
    }
*/

	webots::Device * mydev;
	WebotsDeviceTag * wbt;
	
    WebotsDevice::WebotsDevice( webots::Device & dev){
		mydev = &dev;
		wbt = new WebotsDeviceTag(3);
	}

    std::string WebotsDevice::getName(){
		return mydev->getName();
	}

    IWbDeviceTag & WebotsDevice::getTag(){
		return *wbt;
	}


} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
