


#include <iostream>
#include "xmlParser.h"
#include "recognition.h"

using namespace benchmark;

int main(int argc, char** argv)
{
	if (argc < 2) {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }
    
    std::list<Frame*> framesList=parseXmlObjects(argv[1]);
    printf("frames %d",framesList.size());
    
    int nextframe=1;
    
}
