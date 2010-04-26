
#include<stdio>
#include<opencv/cv>

namespace benchmark { 
	

class Result{
	
	public:
		FrameResult();
		~FrameResult();
		int falsePositives;
		int falseNegatives;
	

}
}
