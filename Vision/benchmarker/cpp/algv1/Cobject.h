#include <stdio.h>
#include <vector>

#ifndef COBJECTADT_H_
#define COBJECTADT_H_


namespace benchmark{
	
class Cobject
{
	
	
		public:
			Cobject(int index,int x,int y,int w,int h,std::vector<int> centroid);
			Cobject(int index,int x,int y,int w,int h);
			Cobject(int x,int y,int w,int h);
			bool contains(Cobject * other);
			bool contains(int x,int y);
			bool overlap(Cobject* other);
			bool isSimilar(Cobject * other);
			std::vector<int> getCentroid();
			~Cobject();
			void print();
			
			
			//OJO CON ESTO!!!!
			int x,y,w,h;
			int index;
			std::vector<int> centroid;
			
			//benchmark purposes
			bool predicted;
			bool focused;
			bool visualized;
			void setPredicted(bool val);
			void setVisualized(bool val);
			void setFocused(bool val);
			bool isPredicted();
			bool isVisualized();
			bool isFocused();
			bool tagged;
			
};

}

#endif //COBJECTADT_H_
