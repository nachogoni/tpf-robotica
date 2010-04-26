#include <stdio.h>
#include <vector>

#ifndef COBJECTADT_H_
#define COBJECTADT_H_


namespace benchmark{
	
class Cobject
{
		public:
			Cobject(int index,int x,int y,int w,int h);
			Cobject(int x,int y,int w,int h);
			bool contains(Cobject * other);
			bool contains(int x,int y);
			bool overlap(Cobject* other);
			bool isSimilar(Cobject * other);
			std::vector<int> centroid();
			~Cobject();
			void print();
			
			int x,y,w,h;
			int index;
	
};

}

#endif //COBJECTADT_H_
