#include <stdio.h>

#ifndef COBJECTADT_H_
#define COBJECTADT_H_
typedef struct cObjectCDT* cObjectADT;

//functions
cObjectADT newCobject(int x,int y,int w,int h);
void print(cObjectADT obj);
#endif //COBJECTADT_H_
