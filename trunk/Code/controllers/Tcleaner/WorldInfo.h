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
		double getDistanceBetweenWheelsFactor();
		double getLeftWheelFactor();
		double getRightWheelFactor();
		double getEncoderResolution();
		double getCameraFOV();
		double getCameraY();

	private:
		std::list< utils::MyWall * > walls;
		std::list< utils::MyLine * > lines;
		utils::MyPoint * ip;
		utils::MyAngle * ia;
		double distanceBetweenWheels;
		double distanceBetweenWheelsFactor;
		double leftWheelRadius;
		double leftWheelFactor;
		double rightWheelRadius;
		double rightWheelFactor;
		double encoderResolution;
		double cameraFOV;
		double cameraY;




		void readWalls(FILE * f);
		void readLines(FILE * f);
		void readDistanceBetweenWheels(FILE * f);
		void readDistanceBetweenWheelsFactor(FILE * f);
		void readWheelsRadius(FILE * f);
		void readWheelsFactors(FILE * f);
		void readInitialPosition(FILE * f);
		void readInitialAngle(FILE * f);
		void readEncoderResolution(FILE * f);
		void readCameraFOV(FILE * f);
		void readCameraY(FILE * f);

};

#endif // WorldInfo_h
