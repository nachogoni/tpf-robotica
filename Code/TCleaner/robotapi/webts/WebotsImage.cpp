#include "WebotsImage.h"

namespace robotapi {
namespace webts {

	int WebotsImage::getWidth(){
		return 1;
	}

	int WebotsImage::getHeight(){
		return 2;
	}

    int WebotsImage::getDepth(){
		return 32;
	}

	WebotsImage::WebotsImage(unsigned char * img){
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
