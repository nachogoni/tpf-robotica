#include <vision/IGarbageRecognition.h>

namespace vision {
	
	
 void IGarbageRecognition::setCamera(robotapi::ICamera &camera)
{
	cam = &camera;
	lastRequest = time(NULL);
	this->pooled = false;
}

bool IGarbageRecognition::thereIsGarbage()
{
	if ( ! this->pooled ){
		this->getGarbageList();
	}
    return !garbages.empty();
}

vision::Garbage * IGarbageRecognition::getClosestGarbage(std::list<vision::Garbage*> gs){
		if ( gs.empty() )
		    return NULL;

        std::list<vision::Garbage *>::iterator it;
		it=gs.begin();
		return *it;
}

double IGarbageRecognition::angleTo(vision::Garbage * g)
{
	if ( g == NULL )
	    return PI;
	int centerX = g->boundingBox()->getTopX() + g->boundingBox()->getWidth()/2;
	int centerY = g->boundingBox()->getTopY() + g->boundingBox()->getHeight()/2;
	
	int transformedX = centerX - this->wi->getCameraImageWidth()/2;
	int transformedY = this->wi->getCameraImageHeight() - centerY;
	
	double hfov = this->wi->getCameraFOVH();

	if ( transformedY == 0 ){
		return transformedX < 0 ? (-hfov/2) : (hfov/2);
	}
	
	// Negative if its in the left side of the screen, positive otherwise
	return atan((double)transformedX/(double)transformedY) * hfov / PI;
}

double IGarbageRecognition::distanceTo(vision::Garbage * g)
{
    if ( g == NULL )
	    return 10000;

	int centerY = g->boundingBox()->getTopY() + g->boundingBox()->getHeight()/2;

	int transformedY = this->wi->getCameraImageHeight() - centerY;

	double vAngleToGarbage = this->wi->getCameraFOVV()*transformedY/this->wi->getCameraImageHeight();
	double distanceToGarbage = this->getDistance(vAngleToGarbage);

/*
	printf("Minimum Distance : %g - Maximum Distance : %g\n",minDist,maxDist);
	printf("Garbage Y: %d - Camera Height : %d - Vertical FOV : %g\n",transformedY,this->wi->getCameraImageHeight(),this->wi->getCameraFOVV());
	printf("Vertical angle to: %g - Distance to : %g\n",vAngleToGarbage,distanceToGarbage);
*/
    return distanceToGarbage;
}

double IGarbageRecognition::getMaximumDistance(){
	return this->wi->getMaximumDistance();
}

double IGarbageRecognition::getMinimumDistance(){
	return this->wi->getMinimumDistance();
}

double IGarbageRecognition::getDistance(double angle){
	return this->wi->getDistance(angle);
}

void IGarbageRecognition::stepDone(){
	this->pooled = false;
}

IplImage * IGarbageRecognition::loadImage(std::string filename){
	cam->saveImage(filename, 85);
    return cvLoadImage(filename.c_str(),1);
}

IplImage * IGarbageRecognition::loadImage(void){
	IplImage * ret = cam->getImage().toIPL();
	return ret;
}





}
