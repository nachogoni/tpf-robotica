#include "WebotsCamera.h"
#include "WebotsImage.h"

namespace robotapi {
namespace webts {

	webots::Camera * mycam;

    WebotsCamera::WebotsCamera( webots::Camera & cam ) : WebotsDevice ( cam ){
		mycam = &cam;
	}

    void WebotsCamera::enable(int ms){
		mycam->enable(ms);
	}

    void WebotsCamera::disable(){
		mycam->disable();
	}

	IImage & WebotsCamera::getImage(){
		WebotsImage * wi = new WebotsImage(NULL);
		return *wi;
	}

    int WebotsCamera::saveImage(std::string filename, int quality){
		return 4;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
