#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <vector>
#include <robotapi/ICamera.h>


namespace utils {
class Garbage;
} /* End of namespace utils */

namespace utils {

class GarbageRecognition {

 public:

    static void setCamera(robotapi::ICamera &camera);

    static bool thereIsGarbage();

    static std::vector<Garbage*> getGarbageList();


 private:
    static robotapi::ICamera &cam;

 public:

    static robotapi::ICamera *myICamera;

};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
