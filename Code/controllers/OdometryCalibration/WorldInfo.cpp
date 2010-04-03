#include <WorldInfo.h>

WorldInfo::WorldInfo()
{

	FILE * pFile;

	pFile = fopen (CONFIG_FILE,"r");
/*
	if ( pFile == NULL )
		throw new exception();
*/
	this->readInitialPosition(pFile);
	this->readInitialAngle(pFile);
	this->readDistanceBetweenWheels(pFile);
	this->readWheelsRadius(pFile);
	this->readEncoderResolution(pFile);
	this->readWalls(pFile);
	this->readLines(pFile);
	this->readCameraY(pFile);
	this->readCameraFOV(pFile);
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

void WorldInfo::readDistanceBetweenWheels(FILE * f){
	fscanf (f, "%lf\n", &this->distanceBetweenWheels);
	printf("%g\n",this->distanceBetweenWheels);
}

void WorldInfo::readWheelsRadius(FILE * f){
	fscanf (f, "%lf:%lf\n", &this->leftWheelRadius,&this->rightWheelRadius);
	printf("%g:%g\n",this->leftWheelRadius,this->rightWheelRadius);
}

void WorldInfo::readInitialPosition(FILE * f){
	double x,y;
	fscanf (f, "%lf:%lf\n", &x, &y);
	this->ip = new utils::MyPoint(x,y);
	printf("%g:%g\n",x,y);
}

void WorldInfo::readInitialAngle(FILE * f){
	double angle;
	fscanf (f, "%lf\n", &angle);
	this->ia = new utils::MyAngle(angle);
	printf("%g\n",angle);
}

void WorldInfo::readEncoderResolution(FILE * f){
	fscanf (f, "%lf\n", &this->encoderResolution);
	printf("%g\n",this->encoderResolution);
}

void WorldInfo::readCameraFOV(FILE * f){
	fscanf (f, "%lf\n", &this->cameraFOV);
	printf("%g\n",this->cameraFOV);
}

void WorldInfo::readCameraY(FILE * f){
	fscanf (f, "%lf\n", &this->cameraY);
	printf("%g\n",this->cameraY);
}

utils::MyLine * WorldInfo::getCurrentLine(){
	// TODO Make calculations acording to the actual position
	return new utils::MyLine(1,1,1,1,1);
}

utils::MyPoint * WorldInfo::getInitialPosition(){
	return this->ip;
}

utils::MyAngle * WorldInfo::getInitialOrientation(){
	return this->ia;
}

double WorldInfo::getDistanceBetweenWheels(){
	return this->distanceBetweenWheels;
}

double WorldInfo::getLeftWheelRadius(){
	return this->leftWheelRadius;
}

double WorldInfo::getRightWheelRadius(){
	return this->rightWheelRadius;
}

double WorldInfo::getEncoderResolution(){
	return this->encoderResolution;
}

double WorldInfo::getCameraFOV(){
	return this->cameraFOV;
}

double WorldInfo::getCameraY(){
	return this->cameraY;
}

