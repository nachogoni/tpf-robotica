

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include "cObjectADT.h"
#include "frameADT.h"
#include "listADT.h"
#include "xmlParser.h"


listADT 
parseXmlObjects(char * filename)
{
    xmlDoc         *document;
    xmlNode        *root, *frames, *objects,*vals,*val,*object,*frame;
    char           *filename;
    listADT framesList=NewList();

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);
    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    frames = root->children;
    
    //get all frames
    for (frame = frames; frame; frame = frame->next) {
		Insert(framesList,parseFrame(frame),"");
    }
    fprintf(stdout, "...\n");
    return framesList;
}


frameADT 
parseFrame(xmlNode * frame)
{
	xmlNode *objects,*object;
	int nObjs=0;
	int frameNumber=atoi(xmlGetProp(frame,"number"));
	printf("frame number: %d\n",frameNumber);
	
	frameADT f=newFrame(frameNumber);
	
	objects=frame->children;
	
	for (object=objects,nObjs=0; object; object=object->next,nObjs++){
		cObjectADT obj=parseObject(object);
		addObject(f,obj);
	}

	return f;
}

cObjectADT
parseObject(xmlNode * object)
{
	xmlNode * vals,*val;
	int x,y,w,h;
	vals=object->children;
	
	//get vals
	x=atoi(vals->children->content);
	vals=vals->next;
	y=atoi(vals->children->content);
	vals=vals->next;
	w=atoi(vals->children->content);
	vals=vals->next;
	h=atoi(vals->children->content);
	
	cObjectADT aux=newCobject(x,y,w,h);
	print(aux);
	
	return aux;

}




