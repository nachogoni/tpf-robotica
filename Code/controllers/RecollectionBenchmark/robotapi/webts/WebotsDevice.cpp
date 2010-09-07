#include "WebotsDevice.h"

namespace robotapi {
namespace webts {

    std::string dname;
   	IWbDeviceTag dtag;

	void WebotsDevice::init(std::string & name, IWbDeviceTag tag){
        dname = name;
        dtag = tag;
    }

    WebotsDevice::WebotsDevice( webots::Device & dev){
        std::string aux = dev.getName();
        IWbDeviceTag * iwdt = new WebotsDeviceTag(3);
   		init(aux, *iwdt);

//		mydev = &dev;
//		wbt = ;
	}

    WebotsDevice::WebotsDevice(std::string & name, IWbDeviceTag tag){
		init(name, tag);
    }

    std::string WebotsDevice::getName(){
		return dname;
	}

    IWbDeviceTag & WebotsDevice::getTag(){
		return dtag;
	}
	
	void WebotsDevice::refresh(){
		return ;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
