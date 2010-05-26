#ifndef utils_GarbageHistoric_h
#define utils_GarbageHistoric_h

#include "MinimalBoundingRectangle.h"
#include <vector>
#include <list>
#include "Garbage.h"

namespace utils{
	enum gHistState { NOT_SHOW, SHOW, DEAD };

	class GarbageHistoric {
		
	 public:
		 ~GarbageHistoric();
		 GarbageHistoric();
		 GarbageHistoric(std::vector<int> currentPos);
		 GarbageHistoric(Garbage * garbage);
		 bool isSameObject(Garbage * garbage);
		 void updateHistoric();
		 void updateHistoricWithGarbage(Garbage * garbage);
		 Garbage * guessPosition();
		 void printPrediction();


	    //garbage found
	    Garbage * garbage;
	    //position 
	    std::vector<int> currentPos; 
		//moving pattern
		std::vector<int> deltaPos;
		//how many frames ago did the last appeareance
		int lastAppeareance;
		//how many frames had it lived in total
		int age; 
		//how many appeareances it had during life frames
		int appeareances;
		//automata state
		gHistState state;
		//number of frames that can live without an appeareance
		int maxNumberOfFramesNoAppear;
		//is used for focus
		bool focus;
		
		
	};

}

#endif // utils_GarbageHistoric_h
