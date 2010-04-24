#include <stdio.h>

#ifndef COBJECTADT_H_
#define COBJECTADT_H_


namespace benchmark{
	
class Cobject
{
		public:
			Cobject(int x,int y,int w,int h);
			~Cobject();
			void print();
			
			int x,y,w,h;
	
};

}

#endif //COBJECTADT_H_
