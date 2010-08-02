/*      polygon.cpp
 * 
 * 
 *      Takes care of all polygon analysis functions
 */
#include "polygon.h"

bool equalsCvPoint(CvPoint * a, CvPoint * b){
	if(a==NULL || b==NULL)
		return false;
	return (a->x==b->x && a->y==b->y);
}
bool equalsEdge(CvPoint * head, CvPoint * tail,CvPoint * head2, CvPoint * tail2){
	if(head==NULL || tail==NULL || head2==NULL || tail2==NULL)
		return false;
		
	return (equalsCvPoint(head,head2) && equalsCvPoint(tail,tail2) ) ||
		((equalsCvPoint(head,tail2) && equalsCvPoint(tail,head2)))  ;
}
bool isIncreasing(CvPoint * tail,CvPoint * head){
	if( (tail->x < head->x && tail->y < head->y) || (tail->x > head->x && tail->y > head->y))
		return true;
		
	return false;
}
CvPoint * getLongestEdge(CvSeq * points,CvPoint * excludedHead,CvPoint * excludedTail){
	CvPoint * tail,*head,*tailMax,*headMax;
	int i;
	double delta,maxDelta;
	
	tailMax=NULL;
	headMax=NULL;
	//initial
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	maxDelta=0;
		
	//traverse edges
	for(i=1; i< points->total;i++){
		head=CV_GET_SEQ_ELEM(CvPoint ,points,i);
		//ignore certain edges if told so
		if(!equalsEdge(head,tail,excludedHead,excludedTail))
		{			
			delta=edgeLength(*head,*tail);
			if(delta>maxDelta){
				tailMax=tail;
				headMax=head;
				maxDelta=delta;
			}
		}
		tail=head;
	}	
		
	//last edge
	head=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,points->total -1);
	if(!equalsEdge(head,tail,excludedHead,excludedTail))
	{
		delta=edgeLength(*head,*tail);
		if(delta>maxDelta){
			tailMax=tail;
			headMax=head;
			maxDelta=delta;
		}
	}
	
	//result
	CvPoint * resp;
	if( (resp=(CvPoint*) malloc(2*sizeof(CvPoint)))==NULL){
		return NULL;
	}
	
	if((tailMax==NULL || headMax ==NULL)){
		return NULL;
	}
	
	
	resp[0]=*tailMax;
	resp[1]=*headMax;

	return resp;
}
CvPoint * getLongestEdge2(CvSeq * points,CvPoint * excludedHead,CvPoint * excludedTail,int * shorterEdges){
	CvPoint * tail,*head,*tailMax,*headMax;
	int i,shorterEdgeCounter;
	double delta,maxDelta,largestEdgeLen;
	
	
	shorterEdgeCounter=0;
	
	if(excludedHead!=NULL && excludedTail!=NULL)
		largestEdgeLen=edgeLength(*excludedHead,*excludedTail);
	else 
		largestEdgeLen=-1;
	
	tailMax=NULL;
	headMax=NULL;
	//initial
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	maxDelta=0;
		
	//traverse edges
	for(i=1; i< points->total;i++){
		head=CV_GET_SEQ_ELEM(CvPoint ,points,i);
		//ignore certain edges if told so
		if(!equalsEdge(head,tail,excludedHead,excludedTail))
		{			
			delta=edgeLength(*head,*tail);
			if(delta>maxDelta){
				tailMax=tail;
				headMax=head;
				maxDelta=delta;
			}
			if(delta/largestEdgeLen <= 0.8)
				shorterEdgeCounter++;
		}
		tail=head;
	}	
		
	//last edge
	head=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,points->total -1);
	if(!equalsEdge(head,tail,excludedHead,excludedTail))
	{
		delta=edgeLength(*head,*tail);
		if(delta>maxDelta){
			tailMax=tail;
			headMax=head;
			maxDelta=delta;
		}
		if(delta/largestEdgeLen <= 0.8)
			shorterEdgeCounter++;
	}
	
	//result
	CvPoint * resp;
	if( (resp=(CvPoint*) malloc(2*sizeof(CvPoint)))==NULL){
		return NULL;
	}
	
	if((tailMax==NULL || headMax ==NULL)){
		return NULL;
	}
	
	
	resp[0]=*tailMax;
	resp[1]=*headMax;
	
	if(shorterEdges!=NULL)
		*shorterEdges=shorterEdgeCounter;

	return resp;
}

double edgeLength(CvPoint head,CvPoint tail){
	double xdelta=fabs(head.x - tail.x);
	double ydelta=fabs(head.y - tail.y);
	return sqrt( xdelta*xdelta +ydelta*ydelta);
}
bool minSeparationBetweenLongestEdges(CvPoint head,CvPoint tail,CvPoint head2,CvPoint tail2){
	double a;
	double b;
	double len=edgeLength(head2,tail2);
	
	a=edgeLength(tail,tail2);
	b=edgeLength(tail,head2);
	if(a<b){
		//tail con tail , head con head
		//the edge connecting longest edges should be at least 30% of 2nd largest edge
		if(a < 0.3 * len)
			return false;
	}else
		if(b<0.3*len)
			return false;
			
	return true;
		
}
