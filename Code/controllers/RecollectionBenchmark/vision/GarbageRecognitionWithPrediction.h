#ifndef vision_GarbageRecognitionWithPrediction_h
#define vision_GarbageRecognitionWithPrediction_h

#include <list>
#include "Garbage.h"
#include "Prediction.h"
#include "Windowing.h"
#include <cv.h>
#include "IGarbageRecognition.h"



namespace vision {

class GarbageRecognitionWithPrediction : virtual public IGarbageRecognition {

 public:

	GarbageRecognitionWithPrediction();
	GarbageRecognitionWithPrediction(WorldInfo * wi);
	~GarbageRecognitionWithPrediction();
	
    std::list<vision::Garbage*> getGarbageList();
    
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
    
};

} /* End of namespace vision */

#endif // vision_GarbageRecognition_h
