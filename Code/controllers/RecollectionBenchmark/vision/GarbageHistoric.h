#ifndef vision_GarbageHistoric_h
#define vision_GarbageHistoric_h

#include "MinimalBoundingRectangle.h"
#include <vector>
#include <list>
#include "Garbage.h"

//maximum variation allowed between consecutive areas of a contour
const double AREA_DELTA_RATIO=0.25;
//maximum variation allowed between consecutive perimeters of a contour
const double AREA_PER_RATIO=0.25;
//when guessing establish the factor by which the delta of the movement 
//is multiplied
const double DELTA_MOVEMENT_FACTOR=0.3;

namespace vision{
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
		 bool compareContour(Garbage * garbage);
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

#endif // vision_GarbageHistoric_h
