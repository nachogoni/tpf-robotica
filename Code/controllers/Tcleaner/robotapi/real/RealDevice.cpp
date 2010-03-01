#include <robotapi/real/RealDevice.h>
#include <robotapi/real/RealDeviceTag.h>

namespace robotapi {
namespace real {

RealDevice::RealDevice(std::string * name){
	this->dname = name;
	this->dtag = new RealDeviceTag();
}

RealDevice::~RealDevice(){
}

std::string RealDevice::getName(){
	return *this->dname;
}

IWbDeviceTag & RealDevice::getTag(){
	return *this->dtag;
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
