#ifndef utils_IGarbageRecognition_h
#define utils_IGarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <utils/Garbage.h>
#include <WorldInfo.h>

namespace utils {

class IGarbageRecognition {

 public:

		void setCamera(robotapi::ICamera &camera) ;

		bool thereIsGarbage()  ;

		virtual std::list<utils::Garbage*> getGarbageList()  = 0;

		utils::Garbage * getClosestGarbage(std::list<utils::Garbage*> gs);

		double angleTo(utils::Garbage * g) ;

		double distanceTo(utils::Garbage * g) ;

		void stepDone() ;

		double getMaximumDistance()  ;

		double getMinimumDistance()  ;
		
		double getDistance(double angle);
		
		IplImage * loadImage(std::string filename);
		
		IplImage * loadImage(void);




		WorldInfo * wi;
		bool pooled;
		robotapi::ICamera * cam;
		std::list<utils::Garbage*> garbages;
		time_t lastRequest;


};

} /* End of namespace utils */

#endif // utils_IGarbageRecognition_h
