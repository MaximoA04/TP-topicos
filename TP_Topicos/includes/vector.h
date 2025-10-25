#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

//Factores de redimension
#define FACT_INC 2
#define FACT_DEC 0.5
#define FACT_OCUP 0.25

//Tipos de ordenamiento
#define BURBUJEO 1
#define INSERCION 2
#define SELECCION 3
#define SELECCION2 4

typedef struct {
	void *vec;
	size_t cap;
	size_t cantElem;
	size_t tamElem;
}
Vector;

typedef struct {
	void *act;
	void *ult;
	bool finIter;
	Vector *vector;
}
VectorIterador;

typedef int (*Cmp)(const void *, const void *);
typedef int (*CmpFil)(const void *);
typedef void (*Fmt)(const void *);
typedef bool (*FmtInsert)(char *, void *);
typedef void (*FmtRead)(char *, void *);
typedef void (*FmtWrite)(FILE *, void *);

/// Primitivas

// Creacion
int vectorCrear(Vector *v, size_t tam);
int vectorCrearDeArchivoBIN(Vector *v, size_t tam, FILE *f);

// Insercion
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp);
int vectorInsertar(Vector *v, void *elem);
int vectorInsertarAlInicio(Vector *v, void *elem);
int vectorInsertarDeArchivoBIN(Vector *v, FILE *f);
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert insertar, int count);
int vectorGuardarAArchivoBIN(Vector *v, FILE *f);

// Busqueda
void *vectorBuscar(Vector *v, int elem);
void *vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp);
void *vectorGet(const Vector *v, size_t pos);

// Eliminacion
bool vectorOrdEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminarDePosicion(Vector *v, size_t pos);
bool vectorEliminarPorFiltro(Vector *v, CmpFil cmp);

// Ordenamiento
int vectorOrdenar(Vector *V, int metodo, Cmp cmp);

// varios
void vectorVaciar(Vector *V);
void vectorMostrar(const Vector *v, Fmt fmt);
void vectorDestruir(Vector *v);
int vectorCE(Vector *v);

// Vector Iterador
void vectorIteradorCrear(VectorIterador *it, const Vector *v);
void *vectorIteradorPrimero(VectorIterador *it);
void *vectorIteradorSiguiente(VectorIterador *it);
void *vectorIteradorDesplazamiento(VectorIterador *it, size_t cantidad);
bool vectorIteradorFin(VectorIterador *it);
void *vectorIteradorActual(VectorIterador *it);

/// No Primitivas

void ordBurbujeo(Vector *v, Cmp cmp);
void ordSeleccion(Vector *v, Cmp cmp);
void ordInsercion(Vector *v, Cmp cmpFunc);
void intercambiar(void *a, void *b, size_t tamElem);
bool redimensionarVector(Vector *v, float factor);


#endif
