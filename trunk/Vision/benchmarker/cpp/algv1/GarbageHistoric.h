#ifndef utils_GarbageHistoric_h
#define utils_GarbageHistoric_h

#include "MinimalBoundingRectangle.h"
#include <vector>
#include <list>
#include "Garbage.h"

namespace utils{

	class GarbageHistoric {
		
	 public:
		 ~GarbageHistoric();
		 GarbageHistoric();
		 GarbageHistoric(std::vector<int> currentPos);
		 GarbageHistoric(Garbage * garbage);
		 bool isSameObject(Garbage * garbage);
		 void updateHistoric(Garbage * garbage);

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
		
		
	};

}

#endif // utils_GarbageHistoric_h
