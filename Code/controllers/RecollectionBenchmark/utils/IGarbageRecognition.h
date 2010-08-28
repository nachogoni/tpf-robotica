#ifndef utils_IGarbageRecognition_h
#define utils_IGarbageRecognition_h

#include <list>
#include <robotapi/ICamera.h>
#include <utils/Garbage.h>

namespace utils {

class IGarbageRecognition {

 public:

		virtual void setCamera(robotapi::ICamera &camera)  = 0;

		virtual bool thereIsGarbage()  = 0;

		virtual std::list<utils::Garbage*> getGarbageList()  = 0;

		virtual utils::Garbage * getClosestGarbage(std::list<utils::Garbage*> gs)  = 0;

		virtual double angleTo(utils::Garbage * g)  = 0;

		virtual double distanceTo(utils::Garbage * g)  = 0;

		virtual void stepDone()  = 0;

		virtual double getMaximumDistance()  = 0;

		virtual double getMinimumDistance()  = 0;

};

} /* End of namespace utils */

#endif // utils_IGarbageRecognition_h
