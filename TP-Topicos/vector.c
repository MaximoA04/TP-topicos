#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "includes/vector.h"
#include "includes/string.h"

typedef void (*ordenamiento)(Vector *v, Cmp cmp);

static void intercambiar(void *a, void *b, size_t tamElem);
static bool redimensionarVector(Vector *v, float factor);
ordenamiento metodos[] = { NULL, ordenarBurbujeo, ordenarInsercion,
			   ordenarSeleccion, ordenarSeleccion2 };

/*** Primitivas ***/
//Creacion
int vectorCrear(Vector *v, size_t tam)
{
	v->cantElem = 0;
	v->vec = malloc(TAM_INIT * tam);
	if (v->vec == NULL) {
		v->cap = 0;
		return ERR_MEM;
	}
	v->cap = TAM_INIT;
	v->tamElem = tam;
	return OK;
}
int vectorCrearDeArchivoBIN(Vector *v, size_t tam, FILE *f)
{
	vectorCrear(v, tam);
	void *elem = malloc(tam); //Solucion al VLA
	if (!elem)
		return false;
	while (fread(elem, tam, 1, f) == 1) {
		vectorInsertar(v, elem);
	}
	free(elem);
	return true;
}

//Insercion
int vectorInsertar(Vector *v, void *elem)
{
	if (v->cantElem == v->cap)
		if (!redimensionarVector(v, FACT_INC))
			return SIN_MEM;

	void *posIns = v->vec + v->tamElem * v->cantElem;
	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp)
{
	size_t ce = v->cantElem;
	int cod;
	if (ce == v->cap) {
		cod = redimensionarVector(v, FACT_INC);
		if (cod != OK)
			return SIN_MEM;
	}

	void *posIns = v->vec;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	while (posIns <= ult && cmp(posIns, elem) > 0)
		posIns += v->tamElem;

	memmove(posIns + v->tamElem, posIns, (ult - posIns) + v->tamElem);
	/*Otra forma:
	for (void *j = ult; j >= posIns; j -= v->tamElem)
		memcpy(j + v->tamElem, j, v->tamElem);
	*/
	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
int vectorInsertarAlInicio(Vector *v, void *elem)
{
	int cod;
	if (v->cantElem == v->cap) {
		cod = redimensionarVector(v, FACT_INC);
		if (cod != OK)
			return SIN_MEM;
	}
	//void *ult = v->vec + (v->cantElem) * v->tamElem;

	//Otra forma:
	memmove(v->vec, v->vec + 1, v->tamElem * v->cantElem);

	/*for (; ult > v->vec; ult -= v->tamElem) {
		memcpy(ult, ult - v->tamElem, v->tamElem);
	}*/

	memcpy(v->vec, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
int vectorInsertarDeArchivoBIN(Vector *v, FILE *f)
{
	int cod = OK;
	void *elem = malloc(v->tamElem);
	if (!elem)
		return ERR_MEM;
	while (fread(elem, v->tamElem, 1, f) == 1) {
		cod = vectorInsertar(v, elem);
		if (cod != OK) {
			free(elem);
			return cod;
		}
	}
	free(elem);
	return cod;
}
//La cabecera de, por ejemplo un csv, NO SE GUARDARA
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert formatear, int count)
{
    if (count <= 0)
        count = -1;

    bool ok, cod = OK;
    char linea[150];
    int numLinea = 0;
    int errores = 0;
    // Saltear cabecera
    if (!fgets(linea, sizeof(linea), f))
        return ERR;

    while ((count != 0) && fgets(linea, sizeof(linea), f)) {
        numLinea++;

        void *elemFmt = malloc(v->tamElem);
        if (!elemFmt)
            return ERR_MEM;
        ok = formatear(linea, elemFmt);
        if (ok) {
            cod = vectorInsertar(v, elemFmt);
            if (cod != OK) {
                free(elemFmt);
                return cod;
            }
        }
        else{
            printf("Línea %d inválida:  %s\n", numLinea, linea);
            errores++;
        }
        free(elemFmt);
        if (count > 0)
            count--;
    }
    if (errores > 0) {
        printf("\nSe detectaron %d líneas inválidas en el archivo. Ultima invalida: %s\n", errores, linea);
        return ERR;
    }
    return cod;
}
//Funcion para guardar datos del vector, la cabecera del archivo, si es que lo desea, debera introducirse antes
int vectorGuardarAArchivoTXT(Vector *v, FILE *f, FmtWrite formatear)
{
	int cod = OK;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i = v->vec;
	char *elem = malloc(sizeof(v->tamElem));
	for (; i < ult; i += v->tamElem) {
		formatear(f, i);
	}
	free(elem);
	return cod;
}
int vectorGuardarAArchivoBIN(Vector *v, FILE *f)
{
	int cod = OK;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i = v->vec;
	for (; i < ult; i += v->tamElem) {
		fwrite(i, v->tamElem, 1, f);
	}
	return cod;
}

//Busqueda
void *vectorBuscar(Vector *v, int elem)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i = v->vec;
	bool encontrado = false;
	while (i <= ult && !encontrado) {
		if (*(int *)i == elem) {
			encontrado = true;
		}
		i += v->tamElem;
	}
	return i;
}
void *vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp)
{
	void *li = v->vec;
	void *ls = v->vec + (v->cantElem - 1);
	void *m;
	bool encontrado = false;
	int pos;

	while (li <= ls && !encontrado) {
		m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
		if (cmp(elem, m) == 0) {
			encontrado = true;
			memcpy(elem, m, v->tamElem);
			pos = (m - v->vec) * v->tamElem;
		}
		if (cmp(elem, m) > 0) {
			li = m + 1;
		} else {
			ls = m - 1;
		}
	}
	return encontrado ? (v->vec + pos) : NULL;
}
void *vectorGet(const Vector *v, size_t pos)
{
	if (pos < 0 || pos >= v->cantElem)
		return NULL;
	return v->vec + pos * v->tamElem;
}

//Eliminacion
bool vectorEliminarDePosicion(Vector *v, size_t pos)
{
    if (pos >= v->cantElem)
        return false;

    if (pos < v->cantElem - 1) { // mover solo si hay elementos después
        memmove((char*)v->vec + pos * v->tamElem,
                (char*)v->vec + (pos + 1) * v->tamElem,
                (v->cantElem - pos - 1) * v->tamElem);
    }
    v->cantElem--;

    if (((float)v->cantElem / v->cap) <= FACT_OCUP && v->cap > 1)
        redimensionarVector(v, FACT_DEC);

    return true;
}
bool vectorOrdEliminar(Vector *v, void *elem, Cmp cmp)
{
    char* encontrado = (char*)vectorOrdBuscar(v, elem, cmp);
    if (!encontrado)
        return false;

    size_t pos = (encontrado - (char*)v->vec) / v->tamElem;
    return vectorEliminarDePosicion(v, pos);
}
bool vectorEliminar(Vector *v, void *elem, Cmp cmp)
{
	void *ult = v->vec + v->cantElem - 1;
	void *i = v->vec;
	bool eliminado = false;
	while (i < ult && !eliminado) {
		if (cmp(i, elem) == 0) {
			for (void *j = i + 1; j <= ult; j++) {
				memcpy(j - 1, j, v->tamElem);
				ult--;
			}
			eliminado = true;
		}
		i++;
	}
	return eliminado;
}
bool vectorEliminarPorFiltro(Vector *v, CmpFil cmp)
{
    char *vec = (char *)v->vec;
    size_t nuevoIndice = 0;
    for (size_t i = 0; i < v->cantElem; i++)
    {
        char *elem = vec + i * v->tamElem;
        if (cmp(elem) != 0) {
            if (nuevoIndice != i)
                memcpy(vec + nuevoIndice * v->tamElem, elem, v->tamElem);
            nuevoIndice++;
        }
    }
    v->cantElem = nuevoIndice;

    if (((float)v->cantElem / v->cap) <= FACT_OCUP)
        redimensionarVector(v, FACT_DEC);

    return true;
}

//Ordenamiento
int vectorOrdenar(Vector *vector, int metodo, Cmp cmp)
{
	metodos[metodo](vector, cmp);
	return OK;
}

//varios
void vectorVaciar(Vector *v)
{
	v->cantElem = 0;
	v->cap = TAM_INIT;
	v->vec = realloc(v->vec, TAM_INIT * v->tamElem);
}
void vectorDestruir(Vector *v)
{
	free(v->vec);
	v->vec = NULL;
	v->cantElem = 0;
	v->tamElem = 0;
	v->cap = 0;
}
int vectorCE(Vector *v)
{
	return v->cantElem;
}
void vectorMostrar(const Vector *v, Fmt fmt)
{
	for (void *i = v->vec; i < v->vec + v->cantElem * v->tamElem;
	     i += v->tamElem) {
		fmt(i);
	}
}
static bool redimensionarVector(Vector *v, float factor)
{
    size_t nuevaCap = (size_t)(v->cap * factor);

    if (nuevaCap <= v->cap) {
        nuevaCap = v->cap + 1;
    }

    void *nuevoVec = realloc(v->vec, nuevaCap * v->tamElem);
    if (!nuevoVec) {
        return false;
    }

    printf("Redimension de %zu a %zu\n", v->cap, nuevaCap);
    v->cap = nuevaCap;
    v->vec = nuevoVec;
    return true;
}
static void intercambiar(void *a, void *b, size_t tamElem)
{
	void *aux = malloc(tamElem);
	if (!aux)
		return;
	memcpy(aux, a, tamElem);
	memcpy(a, b, tamElem);
	memcpy(b, aux, tamElem);
	free(aux);
}

// Vector iterador
void vectorIteradorCrear(VectorIterador *it, const Vector *v)
{
	it->act = NULL;
	it->ult = NULL;
	it->finIter = true;
	it->vector = (Vector *)v;
}
void *vectorIteradorPrimero(VectorIterador *it)
{
    Vector *v = it->vector;
    if (v->cantElem == 0) {
        it->act = NULL;
        it->ult = NULL;
        it->finIter = true;
        return NULL;
    }
    it->act = v->vec;
    it->ult = (char *)v->vec + (v->cantElem - 1) * v->tamElem;
    it->finIter = false;

    return it->act;
}
void *vectorIteradorSiguiente(VectorIterador *it)
{
    Vector *v = it->vector;
    if (it->finIter || it->act == NULL)
        return NULL;

    char *siguiente = (char *)it->act + v->tamElem;
    if (siguiente > (char *)it->ult) {
        it->finIter = true;
        return NULL;
    }
    it->act = siguiente;
    return it->act;
}
void *vectorIteradorDesplazamiento(VectorIterador *it, size_t cantidad)
{
    Vector *v = it->vector;
    if (it->finIter || it->act == NULL)
        return NULL;

    char *cur = (char *)it->act + v->tamElem * cantidad;
    if (cur > (char *)it->ult) {
        it->finIter = true;
        return NULL;
    }
    it->act = cur;
    return it->act;
}
bool vectorIteradorFin(VectorIterador *it)
{
	return it->finIter;
}
void *vectorIteradorActual(VectorIterador *it)
{
	return it->act;
}


/*** No Primitivas ***/
void ordenarBurbujeo(Vector *v, Cmp cmp)
{
    char *vec = (char *)v->vec;
    size_t tam = v->tamElem;
    size_t n = v->cantElem;

    for (size_t i = 0; i < n - 1; i++)
    {
        for (size_t j = 0; j < n - 1 - i; j++)
        {
            char *p1 = vec + j * tam;
            char *p2 = vec + (j + 1) * tam;
            if (cmp(p1, p2) > 0)
                intercambiar(p1, p2, tam);
        }
    }
}
void ordenarSeleccion(Vector *v, Cmp cmp)
{
    char *vec = (char *)v->vec;
    size_t tam = v->tamElem;
    size_t n = v->cantElem;

    for (size_t i = 0; i < n - 1; i++)
    {
        char *elemMax = vec + i * tam;
        for (size_t j = i + 1; j < n; j++)
        {
            char *cur = vec + j * tam;
            if (cmp(cur, elemMax) > 0)
                elemMax = cur;
        }
        if (elemMax != vec + i * tam)
            intercambiar(vec + i * tam, elemMax, tam);
    }
}
void ordenarSeleccion2(Vector *v, Cmp cmp)
{
    char *vec = (char *)v->vec;
    size_t tam = v->tamElem;
    size_t n = v->cantElem;

    for (size_t i = 0, j = n - 1; i < j; i++, j--)
    {
        char *min = vec + i * tam;
        char *max = vec + i * tam;

        for (size_t k = i; k <= j; k++)
        {
            char *cur = vec + k * tam;
            if (cmp(cur, min) < 0) min = cur;
            if (cmp(cur, max) > 0) max = cur;
        }

        if (min != vec + i * tam)
            intercambiar(vec + i * tam, min, tam);
        if (max == vec + i * tam)
            max = min;
        if (max != vec + j * tam)
            intercambiar(vec + j * tam, max, tam);
    }
}
void ordenarInsercion(Vector *v, Cmp cmpFunc)
{
    if (v->cantElem < 2)
        return;
    char *vec = (char *)v->vec;
    size_t tam = v->tamElem;
    void *elemAIns = malloc(tam);
    if (!elemAIns)
        return;

    for (size_t i = 1; i < v->cantElem; i++)
    {
        memcpy(elemAIns, vec + i * tam, tam);
        ssize_t j = i - 1;
        while (j >= 0 && cmpFunc(vec + j * tam, elemAIns) > 0)
        {
            memcpy(vec + (j + 1) * tam, vec + j * tam, tam);
            j--;
        }
        memcpy(vec + (j + 1) * tam, elemAIns, tam);
    }
    free(elemAIns);
}


