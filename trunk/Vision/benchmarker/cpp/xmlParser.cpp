

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

#include "Cobject.h"
#include "Frame.h"
#include "xmlParser.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace benchmark;
using namespace std;


std::list<Frame*>
parseXmlObjects(char * filename)
{
	
	
	
    rapidxml::xml_document<>         document;
    rapidxml::xml_node<>         	*root, *frames, *objects,*vals,*val,*object,*frame;
    std::list<Frame*> framesList;

    document.parse<0>(readFile(filename));
    root = document.first_node();
    fprintf(stdout, "Root is <%s> (%i)\n", root->name(), root->type());
    frames = root->first_node();
    
    
    //get all frames
    for (frame = frames; frame; frame = frame->next_sibling()) {
		framesList.push_back(parseFrame(frame));
    }
   
   
    return framesList;
    
    
}


Frame*
parseFrame(rapidxml::xml_node<> * frame)
{
	rapidxml::xml_node<> *objects,*object;
	int nObjs=0;
	rapidxml::xml_attribute<> *atr=frame->first_attribute();
	
	int frameNumber=atoi(atr->value());
	
	
	Frame* f=new Frame(frameNumber);
	
	objects=frame->first_node();
	
	for (object=objects,nObjs=0; object; object=object->next_sibling(),nObjs++){
		Cobject* obj=parseObject(object);
		obj->index=nObjs;
		f->addObject(obj);
	}

	return f;
}

Cobject*
parseObject(rapidxml::xml_node<> * object)
{
	rapidxml::xml_node<>    * vals,*val;
	int x,y,w,h;
	vals=object->first_node();
	
	//get vals
	x=atoi(vals->value());
	vals=vals->next_sibling();
	y=atoi(vals->value());
	vals=vals->next_sibling();
	w=atoi(vals->value());
	vals=vals->next_sibling();
	h=atoi(vals->value());
	
	Cobject* aux=new Cobject(x,y,w,h);
	
	//aux->print();
	
	return aux;

}

char * 
readFile(char * filename){
	
	std::string buf;
	std::string line;
	std::ifstream in(filename);
	
	while(std::getline(in,line))
	buf += line;
	
	//TODO: checks
	char * xml=(char*)malloc(buf.length()+1);

	strcpy(xml,buf.c_str());

	return xml;
}




