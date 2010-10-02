#ifndef behaviours_FocusGarbage_h
#define behaviours_FocusGarbage_h

#include "AbstractBehaviour.h"
#include <vision/IGarbageRecognition.h>

namespace behaviours {

class FocusGarbage : public AbstractBehaviour {

	public:

		FocusGarbage(vision::IGarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		~FocusGarbage();

		void sense();

	    void action();

	private:
        vision::IGarbageRecognition * gr;
		robotapi::IDifferentialWheels * wheels;
		vision::Garbage * currentGarbage;
	};

} /* End of namespace behaviours */

#endif // behaviours_FocusGarbage_h
