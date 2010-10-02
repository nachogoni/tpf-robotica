#ifndef vision_IGarbageRecognition_h
#define vision_IGarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <vision/Garbage.h>
#include <WorldInfo.h>

namespace vision {

class IGarbageRecognition {

 public:

		void setCamera(robotapi::ICamera &camera) ;

		bool thereIsGarbage()  ;

		virtual std::list<vision::Garbage*> getGarbageList()  = 0;

		vision::Garbage * getClosestGarbage(std::list<vision::Garbage*> gs);

		double angleTo(vision::Garbage * g) ;

		double distanceTo(vision::Garbage * g) ;

		void stepDone() ;

		double getMaximumDistance()  ;

		double getMinimumDistance()  ;
		
		double getDistance(double angle);
		
		IplImage * loadImage(std::string filename);
		
		IplImage * loadImage(void);




		WorldInfo * wi;
		bool pooled;
		robotapi::ICamera * cam;
		std::list<vision::Garbage*> garbages;
		time_t lastRequest;


};

} /* End of namespace vision */

#endif // vision_IGarbageRecognition_h
