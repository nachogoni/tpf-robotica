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


 private:
    std::list<Garbage*> garbageList(IplImage * src, IplImage * model);

};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
