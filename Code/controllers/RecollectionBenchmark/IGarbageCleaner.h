#ifndef IGarbageCleaner_h
#define IGarbageCleaner_h

#include <string>

class IGarbageCleaner {

 public:

	virtual void stepWasDone()  = 0;
    virtual ~IGarbageCleaner() { }

};

#endif // IGarbageCleaner_h
