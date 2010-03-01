#ifndef behaviours_CollectGarbage_h
#define behaviours_CollectGarbage_h

#include "AbstractBehaviour.h"


namespace behaviours {

class CollectGarbage : public AbstractBehaviour {

	public:

		CollectGarbage(robotapi::IServo * lifter, robotapi::IServo * shovel);
		~CollectGarbage();

		void sense();

	    void action();

	private:
		robotapi::IServo * lifter;
		robotapi::IServo * shovel;
	};

} /* End of namespace behaviours */

#endif // behaviours_CollectGarbage_h
