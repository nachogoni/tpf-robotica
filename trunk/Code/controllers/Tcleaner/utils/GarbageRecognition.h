#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <utils/Garbage.h>
#include <cv.h>

namespace utils {

class GarbageRecognition {

 public:

    void setCamera(robotapi::ICamera &camera);

    bool thereIsGarbage();

    std::list<utils::Garbage*> getGarbageList();

	utils::Garbage * getClosestGarbage(std::list<utils::Garbage*> gs);

	double angleTo(utils::Garbage * g);

	double GarbageRecognition::distanceTo(utils::Garbage * g);

 private:
    std::list<Garbage*> garbageList(IplImage * src, IplImage * model);
	int imgWidth;
	int imgHeight;
};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
