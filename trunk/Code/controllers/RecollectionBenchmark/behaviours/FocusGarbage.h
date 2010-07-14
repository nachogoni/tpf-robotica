#ifndef behaviours_FocusGarbage_h
#define behaviours_FocusGarbage_h

#include "AbstractBehaviour.h"
#include <utils/GarbageRecognition.h>

#define FOCUS_GARBAGE_BASE_SPD 100
#define FOCUS_GARBAGE_MIN_SPD 10

namespace behaviours {

class FocusGarbage : public AbstractBehaviour {

	public:

		FocusGarbage(utils::GarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		~FocusGarbage();

		void sense();

	    void action();

	private:
        utils::GarbageRecognition * gr;
		robotapi::IDifferentialWheels * wheels;
		utils::Garbage * currentGarbage;
	};

} /* End of namespace behaviours */

#endif // behaviours_FocusGarbage_h
