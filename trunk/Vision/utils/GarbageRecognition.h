#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <vector>

#include <List<Garbage>.h>
#include <robotapi/ICamera.h>


namespace utils {
class Garbage;
} /* End of namespace utils */

namespace utils {

class GarbageRecognition {

 public:

    static void setCamera(robotapi::ICamera &camera);

    static bool thereIsGarbage();

    static List<Garbage> getGarbageList();


 private:
    static robotapi::ICamera &cam;

 public:

    robotapi::ICamera *myICamera;

    /**
     * @element-type Garbage
     */
    std::vector< Garbage* > myGarbage;
};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
