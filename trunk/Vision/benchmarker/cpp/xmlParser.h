

#include <stdio.h>
#include <string.h>
#include "Cobject.h"
#include "Frame.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"


#ifndef XMLPARSER_H_
#define XMLPARSER_H_


char * readFile(char * filename);

std::list<benchmark::Frame*>  parseXmlObjects(char * filename);

benchmark::Cobject* parseObject(rapidxml::xml_node<> * object);

benchmark::Frame* parseFrame(rapidxml::xml_node<> *frame);


#endif /*XMLPARSER_H_*/

