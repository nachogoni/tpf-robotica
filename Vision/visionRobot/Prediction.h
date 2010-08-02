#ifndef utils_Prediction_h
#define utils_Prediction_h

#include "GarbageHistoric.h"
#include <vector>
#include <list>
#include "Garbage.h"
#include "PredictionParameters.h"



namespace utils{

	class Prediction {
		
	 public:
		 //~ Prediction();
		 //~ ~Prediction();
	     std::list<Garbage*> getPrediction(std::list<Garbage*> garbageList);
	     void updateHistorical(std::list<Garbage*> garbageList);
	     GarbageHistoric * focusGarbage();
	     
	     
	    std::list<Garbage*> garbageList;
	    std::list<GarbageHistoric*> ghist;
	
	};

}

#endif // utils_Prediction_h
