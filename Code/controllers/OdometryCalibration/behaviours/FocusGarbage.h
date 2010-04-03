#ifndef behaviours_FocusGarbage_h
#define behaviours_FocusGarbage_h

#include "AbstractBehaviour.h"
#include <utils/GarbageRecognition.h>

namespace behaviours {

class FocusGarbage : public AbstractBehaviour {

	public:

		FocusGarbage( robotapi::ICamera * camera, robotapi::IDifferentialWheels * wheels );
		~FocusGarbage();

		void sense();

	    void action();

	private:
        utils::GarbageRecognition * gr;
		robotapi::IDifferentialWheels * wheels;
	};

} /* End of namespace behaviours */

#endif // behaviours_FocusGarbage_h
