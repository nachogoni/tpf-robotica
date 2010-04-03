#include "RealCamera.h"
#include "RealImage.h"

namespace robotapi {
namespace real {

/**
* TODO IMPLEMENT!
**/

void RealCamera::enable(int ms){
}

void RealCamera::disable(){
}

IImage & RealCamera::getImage(){
	IImage * iim = new RealImage();
	return *iim;
}

int RealCamera::saveImage(std::string filename, int quality){
	return 0;
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
