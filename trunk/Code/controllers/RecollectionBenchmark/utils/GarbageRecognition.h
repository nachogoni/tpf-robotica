#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <utils/Garbage.h>
#include <utils/IGarbageRecognition.h>
#include <WorldInfo.h>
#include <cv.h>

namespace utils {

class GarbageRecognition : virtual public IGarbageRecognition {

 public:

	GarbageRecognition(WorldInfo * wi);

    void setCamera(robotapi::ICamera &camera);

    bool thereIsGarbage();

    std::list<utils::Garbage*> getGarbageList();

	utils::Garbage * getClosestGarbage(std::list<utils::Garbage*> gs);

	double angleTo(utils::Garbage * g);

	double distanceTo(utils::Garbage * g);

	void stepDone();

	double getMaximumDistance();
	
	double getMinimumDistance();

 private:
    std::list<Garbage*> garbageList(IplImage * src, IplImage * model);
	WorldInfo * wi;
	bool pooled;
	IplImage * model;
	
    double getDistance(double angle);
    IplImage * loadImage(std::string filename);
    IplImage * loadImage();
};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
