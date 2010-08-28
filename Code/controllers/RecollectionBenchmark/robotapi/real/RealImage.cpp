#include "RealImage.h"

namespace robotapi {
namespace real {
	
RealImage::RealImage(IplImage * img){
	this->img=img;
	this->size=cvGetSize(this->img);
}

int RealImage::getWidth(){
	return size.width;
}

int RealImage::getHeight(){
	return size.height;
}

int RealImage::getDepth(){
	return 0;
}

IplImage * RealImage::toIPL(){
	return this->img;
}
} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
