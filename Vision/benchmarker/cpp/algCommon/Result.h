
#include <stdio.h>
#include <cv.h>
#include <list>
#include "Frame.h"
#include "Cobject.h"

using namespace std;

namespace benchmark { 
	

class Result{
	
	public:
		Result();
		Result(Frame * frameXml);
		~Result();
		int falsePositives;
		int falseNegatives;
		int nObjects;
		void addMiss(Cobject * obj);
		void addFound(int index);
		int detectedObjects();
		bool hasTestFrame();
		
	private:
		list<int> foundList;
		list<Cobject*> missList;
		Frame *frameXml;

};
}
