#ifndef WorldInfo_h
#define WorldInfo_h

#include <utils/OdometryInfo.h>
#include <utils/Line.h>

class WorldInfo {

 public:

    WorldInfo();

	utils::OdometryInfo * getOdometryInfo();

	utils::Line * getCurrentLine();
};

#endif // WorldInfo_h
