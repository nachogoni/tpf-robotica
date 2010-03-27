#include "WorldInfo.h"

WorldInfo::WorldInfo()
{
	FILE * pFile;

	pFile = fopen (CONFIG_FILE,"r");
	this->readWalls(pFile);
	this->readLines(pFile);
	fclose(pFile);
}

void WorldInfo::readWalls(FILE * f){
   	int h, cant, i = 0;
	double x,y,l,w;
	//	-0.16:0.66:0.55:0.02:1
	fscanf (f, "%d\r\n", &cant);
	while ( i < cant ){
		fscanf (f, "%g:%g:%g:%g:%d\r\n", &x, &y, &l, &w, &h);
		printf("%g:%g:%g:%g:%d\n",x,y,l,w,h);
		i++;
	}
}

void WorldInfo::readLines(FILE * f){
	int cant, i = 0;
	double x,y,l,w,o;
	//-0.01:-0.2625:0.015000000000000013:0.075:4.71238898038469
	fscanf (f, "%d\r\n", &cant);
	while ( i < cant ){
		fscanf (f, "%g:%g:%g:%g:%g\r\n", &x, &y, &l, &w, &o);
		printf("%g:%g:%g:%g:%g\n",x,y,l,w,o);
		i++;
	}
}

utils::OdometryInfo * WorldInfo::getOdometryInfo(){
	return new utils::OdometryInfo();
}

utils::MyLine * WorldInfo::getCurrentLine(){
	return new utils::MyLine(1,1,1,1,1);
}
