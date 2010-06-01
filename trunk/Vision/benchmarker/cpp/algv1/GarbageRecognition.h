#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <list>
#include "Garbage.h"
#include "Prediction.h"
#include "Windowing.h"
#include <cv.h>



namespace utils {

class GarbageRecognition {

 public:

	GarbageRecognition();
	GarbageRecognition(int a);
	~GarbageRecognition();
    std::list<utils::Garbage*> getGarbageList(IplImage * img);
    void enablePrediction();
    void disablePrediction();
    void enableWindowing();
    void disableWindowing();
    Prediction* prediction;
    int frameNumber;
    bool focused;
    Windowing* window;



 private:
	bool doWindowing;
	bool doPrediction;
    std::list<Garbage*> garbageList(IplImage * src, IplImage * model);
    //std::list<utils::Garbage*> garbages;

};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
