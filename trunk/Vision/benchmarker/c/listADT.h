/**
*   @file listADT.h
*   Source EDA
*   Authors EDA.
*	Modification: now every node holds a string besides its list element.
*/
#include "frameADT.h"

#ifndef LISTADT_H_
#define LISTADT_H_

	typedef frameADT listElementT;
	
	typedef struct listCDT * listADT;
	
	listADT NewList( void );
	
	void Insert ( listADT list, listElementT element, char * m);
	
	int Delete( listADT list, listElementT element);
	
	int ListIsEmpty( listADT list );
	
	int BelongsToList( listADT list, listElementT elemento);
	
	void SetBegin( listADT list);
	
	int GetDato ( listADT list, listElementT * element, char * m);
	
	void FreeList( listADT list );

#endif /*LISTADT_H_*/
