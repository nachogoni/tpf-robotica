#ifndef behaviours_FocusGarbage_h
#define behaviours_FocusGarbage_h

#include "AbstractBehaviour.h"
#include <utils/GarbageRecognition.h>



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
