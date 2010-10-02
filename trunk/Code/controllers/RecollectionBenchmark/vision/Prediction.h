#ifndef vision_Prediction_h
#define vision_Prediction_h

#include "GarbageHistoric.h"
#include <vector>
#include <list>
#include "Garbage.h"
#include "PredictionParameters.h"



namespace vision{

	class Prediction {
		
	 public:
	     std::list<Garbage*> getPrediction(std::list<Garbage*> garbageList);
	     void updateHistorical(std::list<Garbage*> garbageList);
	     GarbageHistoric * focusGarbage();
	     
	     
	    std::list<Garbage*> garbageList;
	    std::list<GarbageHistoric*> ghist;
	
	};

}

#endif // vision_Prediction_h
