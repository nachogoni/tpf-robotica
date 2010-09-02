#ifndef utils_GarbageRecognition_h
#define utils_GarbageRecognition_h

#include <list>
#include "Garbage.h"
#include "Prediction.h"
#include "Windowing.h"
#include <cv.h>
#include "IGarbageRecognition.h"



namespace utils {

class GarbageRecognition : virtual public IGarbageRecognition {

 public:

	GarbageRecognition();
	GarbageRecognition(WorldInfo * wi);
	~GarbageRecognition();
	
    std::list<utils::Garbage*> getGarbageList();
    
    void enablePrediction();
    
    void disablePrediction();
    
    void enableWindowing();
    
    void disableWindowing();
    
public:    
    
    Prediction* prediction;
    
    int frameNumber;
    
    bool focused;
    
    Windowing* window;


private:
	bool doWindowing;
	
	bool doPrediction;
    
    std::list<Garbage*> garbageList(IplImage * src, IplImage * model);
    
    IplImage * oldImage;
    
    //std::list<utils::Garbage*> garbages;

};

} /* End of namespace utils */

#endif // utils_GarbageRecognition_h
