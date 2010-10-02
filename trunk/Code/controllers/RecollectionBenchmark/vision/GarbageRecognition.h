#ifndef vision_GarbageRecognition_h
#define vision_GarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <vision/Garbage.h>
#include <vision/IGarbageRecognition.h>
#include <WorldInfo.h>
#include <cv.h>

namespace vision {

class GarbageRecognition : virtual public IGarbageRecognition {

 public:

	GarbageRecognition(WorldInfo * wi);

    std::list<vision::Garbage*> getGarbageList();

 private:
    std::list<vision::Garbage*> garbageList(IplImage * src, IplImage * model);
	IplImage * model;

};

} /* End of namespace vision */

#endif // vision_GarbageRecognition_h
