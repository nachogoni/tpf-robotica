#ifndef WorldInfo_h
#define WorldInfo_h

#include <utils/OdometryInfo.h>
#include <utils/MyLine.h>
#include <stdio.h>
#define CONFIG_FILE "worldInfo.cfg"

class WorldInfo {

	public:

    	WorldInfo();

		utils::OdometryInfo * getOdometryInfo();

		utils::MyLine * getCurrentLine();

	private:
		void readWalls(FILE * f);
		void readLines(FILE * f);
};

#endif // WorldInfo_h
