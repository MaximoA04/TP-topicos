#ifndef COMUN_H
#define COMUN_H

#include <stdlib.h>
#include "string.h"
#include "vector.h"

// Tamaños
#define TAM_INIT 10
#define TAM 100

// Retornos
#define ERR -1
#define TODO_OK 0
#define ERR_ESPACIO 1
#define ERR_REP 2
#define SIN_MEM 3
#define ERR_MEM 4
#define ERR_ORD 5

typedef struct {
	int monto;
	char region[10];
	char fechaDesde[18];
	char fechaHasta[18];
}
filtroIPC;

typedef void (*Filtro)(void *filtro);

void menu(void *f, Filtro monto, Filtro region, Filtro periodo);
bool formatearFecha1(char *c); //AAAAMM -> MES-AAAA



#endif
