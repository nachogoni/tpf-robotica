

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include "cObjectADT.h"
#include "frameADT.h"
#include "listADT.h"

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

cObjectADT parseObject(xmlNode * object);

frameADT parseFrame(xmlNode * frame);


#endif /*XMLPARSER_H_*/

