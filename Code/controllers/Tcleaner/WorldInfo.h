#ifndef WorldInfo_h
#define WorldInfo_h

#include <utils/MyLine.h>
#include <utils/MyWall.h>
#include <utils/MyPoint.h>
#include <stdio.h>
#include <list>
#define CONFIG_FILE "worldInfo.cfg"

class WorldInfo {

	public:

    	WorldInfo();

		utils::MyLine * getCurrentLine();
		utils::MyPoint * getInitialPosition();
		utils::MyAngle * getInitialOrientation();
		double getDistanceBetweenWheels();
		double getLeftWheelRadius();
		double getRightWheelRadius();
		double getEncoderResolution();

	private:
		std::list< utils::MyWall * > walls;
		std::list< utils::MyLine * > lines;
		void readWalls(FILE * f);
		void readLines(FILE * f);
};

#endif // WorldInfo_h
