#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

//Tamaños
#define TAM_INIT 10
#define TAM 100

//Factores de redimension
#define FACT_INC 2
#define FACT_DEC 0.5
#define FACT_OCUP 0.25 // Siempre menos a FACT_OCUP

//Retornos
#define OK 1
#define ERR_ESPACIO 2
#define ERR_REP 3
#define ERR 4
#define SIN_MEM 5
#define ERR_MEM 6
#define ERR_ORD 7

//Tipos de ordenamiento
#define BURBUJEO 1
#define INSERCION 2
#define SELECCION 3
#define SELECCION2 4

typedef struct {
	void *vec;
	size_t cantElem;
	size_t cap;
	size_t tamElem;
} Vector;

typedef struct {
	void *act;
	void *ult;
	Vector *vector;
	bool finIter;
} VectorIterador;

typedef int (*Cmp)(const void *, const void *); //Puntero a funcion
typedef int (*CmpFil)(const void *); //Puntero a funcion para filtros
typedef void (*Fmt)(const void *);
typedef bool (*FmtInsert)(char *, void *);
typedef void (*FmtRead)(char *, void *);
typedef void (*FmtWrite)(FILE *, void *);

//memmove: usar cuando entre destino y origen tocan una misma direccion de memoria, usa un buffer
//memcpy: usar cuando entre destino y origen no tocan una misma direccion de memoria, no usa un buffer
//La diferencia esta en la especificacion y no en la funcionalidad, memmove es mas SEGURO en estos casos

/*** Primitivas ***/
// Creacion
int vectorCrear(Vector *v, size_t tam);
int vectorCrearDeArchivoBIN(Vector *v, size_t tam, FILE *f);

//Insercion
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp);
int vectorInsertar(Vector *v, void *elem);
int vectorInsertarAlInicio(Vector *v, void *elem);
int vectorInsertarDeArchivoBIN(Vector *v, FILE *f);
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert insertar, int count); //Probable csv
int vectorGuardarAArchivoTXT(Vector *v, FILE *f, FmtWrite formatear);
int vectorGuardarAArchivoBIN(Vector *v, FILE *f);

//Busqueda
void *vectorBuscar(Vector *v, int elem);
void *vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp);
void *vectorGet(const Vector *v, size_t pos);

//Eliminacion
bool vectorOrdEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminarDePosicion(Vector *v, size_t pos);
bool vectorEliminarPorFiltro(Vector *v, CmpFil cmp);

//Ordenamiento
int vectorOrdenar(Vector *V, int metodo, Cmp cmp);

//varios
void vectorVaciar(Vector *V);
void vectorMostrar(const Vector *v, Fmt fmt);
void vectorDestruir(Vector *v);
int vectorCE(Vector *v);

//Vector Iterador
void vectorIteradorCrear(VectorIterador *it, const Vector *v);
void *vectorIteradorPrimero(VectorIterador *it);
void *vectorIteradorSiguiente(VectorIterador *it);
void *vectorIteradorDesplazamiento(VectorIterador *it, size_t cantidad);
bool vectorIteradorFin(VectorIterador *it);
void *vectorIteradorActual(VectorIterador *it);

/*** No Primitivas ***/
void ordenarBurbujeo(Vector *v, Cmp cmp);
void ordenarSeleccion(Vector *v, Cmp cmp);
void ordenarSeleccion2(Vector *v, Cmp cmp);
void ordenarInsercion(Vector *v, Cmp cmpFunc);



#endif // !VECTOR_H
