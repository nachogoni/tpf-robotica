#include "WebotsCamera.h"
#include "WebotsImage.h"

namespace robotapi {
namespace webts {

    WebotsCamera::WebotsCamera( webots::Camera & cam ) : WebotsDevice ( cam ){
		this->mycam = &cam;
		this->wi = new robotapi::webts::WebotsImage(this->mycam->getWidth(),this->mycam->getHeight());
	}

    void WebotsCamera::enable(int ms){
		this->mycam->enable(ms);
	}

    void WebotsCamera::disable(){
		this->mycam->disable();
	}

	IImage & WebotsCamera::getImage(){
		this->wi->setImage(this->mycam->getImage());
		return *wi;
	}

    int WebotsCamera::saveImage(std::string filename, int quality){
		return this->mycam->saveImage(filename,quality);
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
