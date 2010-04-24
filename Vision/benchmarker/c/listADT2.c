#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "listADT2.h"

#define Precondicion(c) assert(c)

struct listCDT
{
	struct nodoCDT * nodos;
	struct nodoCDT * actual;
};

typedef struct nodoCDT
{
	listElementT dato;
	char msg[200];
	struct nodoCDT * tail;
} nodoCDT;

listADT 
NewList( void )
{
	listADT auxi;
	
	
	if ( (auxi = malloc(sizeof(struct listCDT))) == NULL)
		return NULL;
		
	auxi->nodos=NULL;
	auxi->actual=NULL;
	return auxi;
}

int
ListIsEmpty( listADT list )
{
	if(list!=NULL)
		return list->nodos == NULL;
	else
		return 1;
}

static nodoCDT*
ListTail( nodoCDT * listInterna )
{
	return listInterna->tail;
}

void
SetBegin( listADT list)
{
	if(list==NULL)
		return;

	list->actual = list->nodos;
	return;
}

int
GetDato ( listADT list, listElementT * element, char * m)
{

	if(list==NULL)
		return 0;

	// Si el actual de la lista es NULL, entonces termino el iterador
	if (list->actual == NULL)
		return 0;

	*element = list->actual->dato;
	strcpy( m, list->actual->msg );

	list->actual = ListTail( list->actual );
	
	return 1;
}

void
Insert ( listADT list, listElementT element, char * m)
{
	nodoCDT * ant, *rec, *auxi;
	
	if( list ==NULL)
		return;

	// Guardo en ant y rec el comienzo de la lista
	ant = rec = list->nodos;
	
	// Recorro la lista hasta encontrar la posición donde debo insertar
	// que es una antes de que la fn de cmp me de <0
	while ( rec != NULL && (rec->dato < element ) )
	{
		ant = rec;
		rec = ListTail( rec );
	}
		
	if ( rec != NULL && (rec->dato == element))
		return;

	// Creo el nuevo elemento de la lista
	if ((auxi = malloc(sizeof(nodoCDT))) == NULL)
		return;
		
	// La cola del nuevo nodo quedo en rec
	auxi->tail = rec;

	// Utilizo la funcion de copia para cargar el dato del nuevo elemento
	strcpy( auxi->msg, m );
	auxi->dato = element;

	if (ant == rec)
		list->nodos = auxi;
	else
		ant->tail = auxi;
		
	return;
}

int
Delete( listADT list, listElementT element)
{
	nodoCDT *ant, *rec;
	
	if( list ==NULL)
		return 0;
		
	rec = ant = list->nodos;
	
	// Me muevo x la lista comparando nodo a nodo
	while( rec != NULL && ( rec->dato < element) )
	{
		ant = rec;
		rec = ListTail(rec);
	}
	
	// Si rec es null o la funcion de comparacion me dice que el siguiente 
	// es mayor, el elemento no se encontro
	if (rec == NULL || (rec->dato !=  element) )
		return 0;

	if ( ant == rec ) 
		list->nodos = ant->tail;
	else
		ant->tail = rec->tail;
		
	if (rec == list->actual)
		list->actual = (rec->tail);
	
	free(rec);
	return 1;
}

int
BelongsToList( listADT list, listElementT elemento)
{
	nodoCDT * actual;
	listElementT dato;
	char msg[200];
	int aux;
	int rta;

	if( list ==NULL)
		return 0;
	
	// Guardamos el actual del iterador para restaurarlo al final
	actual = list->actual;
		
	// Seteamos el iterador al comienzo
	SetBegin(list);
	
	// Recorrenmos mientras haya datos y el elemento no sea mayor al que busco
	while((aux=GetDato(list, &dato,msg)) && (dato > elemento));

	// Restauramos el valor del iterador
	list->actual = actual;
	rta = (dato == elemento);
		
	return rta;
}

void
FreeList( listADT list)
{
	nodoCDT * auxi;
	
	while ( ! ListIsEmpty ( list) )
	{
		auxi = list->nodos;
		list->nodos = auxi->tail;
		free( auxi );
	}
	free(list);
}
