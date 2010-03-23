#include "WorldInfo.h"

WorldInfo::WorldInfo()
{
}

utils::OdometryInfo * WorldInfo::getOdometryInfo(){
	return new utils::OdometryInfo();
}

utils::Line * WorldInfo::getCurrentLine(){
	return new utils::Line();
}
