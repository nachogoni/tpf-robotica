#include "WebotsCamera.h"
#include "WebotsImage.h"

namespace robotapi {
namespace webts {

    WebotsCamera::WebotsCamera( webots::Camera & cam ) : WebotsDevice ( cam ){
		this->mycam = &cam;
	}

    void WebotsCamera::enable(int ms){
		this->mycam->enable(ms);
	}

    void WebotsCamera::disable(){
		this->mycam->disable();
	}

	IImage & WebotsCamera::getImage(){
		WebotsImage * wi = new WebotsImage(this->mycam->getImage(),this->mycam->getWidth(),this->mycam->getHeight());
		return *wi;
	}

    int WebotsCamera::saveImage(std::string filename, int quality){
		return this->mycam->saveImage(filename,quality);
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
