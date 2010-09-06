#include "RealCamera.h"
#include "RealImage.h"
#include <stdio.h>


namespace robotapi {
namespace real {

RealCamera::RealCamera(int cameraID, std::string name) : RealDevice(&name){
	if((capture=cvCaptureFromCAM(cameraID))==0){
		printf("Error en el id de la camara:%d\n",cameraID);
	}
	
}

void RealCamera::enable(int ms){
}

void RealCamera::disable(){
}

IImage & RealCamera::getImage(){
	if(iim==NULL)
		iim = new RealImage(cvQueryFrame(capture));
	else
		cvQueryFrame(capture);
		
	return *iim;
}

int RealCamera::saveImage(std::string filename, int quality){
	return 0;
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
