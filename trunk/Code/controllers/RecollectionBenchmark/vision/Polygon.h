#ifndef Polygon_h_
#define Polygon_h_

#include <stdio.h>
#include <vector>
#include <cv.h>

//percentage of the 2nd largest edge is the minimum distance that
//must exist between largest edges of a glass
const double SEPARATION_PERCENT = 0.3;

/*
 * Test equality of two given points
 * */
bool equalsCvPoint(CvPoint * a, CvPoint * b);
/*
 * Test equality of two given edges (head-tail) and (head2-tail2)
 * */
bool equalsEdge(CvPoint * head, CvPoint * tail,CvPoint * head2, CvPoint * tail2);
/*
 * Test if an edge is increasing or decreasing
 * */
bool isIncreasing(CvPoint * tail,CvPoint * head);
/*
 * Gets the longest edge out of a polygon 
 * 
 * */
CvPoint * getLongestEdge(CvSeq * points,CvPoint * excludedHead=NULL,CvPoint * excludedTail=NULL);
CvPoint * getLongestEdge2(CvSeq * points,CvPoint * excludedHead=NULL,CvPoint * excludedTail=NULL,int * shorterEdges=NULL);
/*
 * Gets the length in pixels of a given edge
 * */
double edgeLength(CvPoint head,CvPoint tail);
/*
 * Checks if two edges(head-tail),(head2,tail2) are separated from each other at least 30% of their length
 */
bool minSeparationBetweenLongestEdges(CvPoint head,CvPoint tail,CvPoint head2,CvPoint tail2);
#endif


