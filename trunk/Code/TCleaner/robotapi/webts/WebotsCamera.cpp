#include "WebotsCamera.h"

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



} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
