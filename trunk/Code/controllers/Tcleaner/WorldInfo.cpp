#include <WorldInfo.h>

WorldInfo::WorldInfo()
{

	FILE * pFile;

	pFile = fopen (CONFIG_FILE,"r");
/*
	if ( pFile == NULL )
		throw new exception();
*/
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
		fscanf (f, "%lf:%lf:%lf:%lf:%d\r\n", &x, &y, &l, &w, &h);
		utils::MyWall * wall = new utils::MyWall(l,w,x,y,(h?true:false));
		this->walls.push_back(wall);
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
		fscanf (f, "%lf:%lf:%lf:%lf:%lf\n", &x, &y, &l, &w, &o);
		utils::MyLine * line = new utils::MyLine(l,x,y,o,w);
		this->lines.push_back(line);
		printf("%f:%f:%f:%f:%f\n",x,y,l,w,o);
		i++;
	}
}

utils::MyPoint * WorldInfo::getInitialPosition(){
	//TODO Get it from config file
	return new utils::MyPoint(1,2);
}

utils::MyAngle * WorldInfo::getInitialOrientation(){
	//TODO Get it from config file
	return new utils::MyAngle(2);
}

utils::MyLine * WorldInfo::getCurrentLine(){
	// TODO Make calculations acording to the actual position
	return new utils::MyLine(1,1,1,1,1);
}

double WorldInfo::getDistanceBetweenWheels(){
   	//TODO Get it from config file
   	return 0.052;
}

double WorldInfo::getLeftWheelRadius(){
	//TODO Get it from config file
	return 0.0205;
}

double WorldInfo::getRightWheelRadius(){
	//TODO Get it from config file
	return 0.0205;
}

double WorldInfo::getEncoderResolution(){
	//TODO Get it from config file
	return 159.23;
}
