#ifndef COMUN_H
#define COMUN_H

#include <stdlib.h>
#include "string.h"
#include "vector.h"

//archivos
#define DIVISIONES "./otros/serie_ipc_divisiones.csv"
#define APERTURAS "./otros/serie_ipc_aperturas.csv"
#define ALQUILERES "./otros/alquileres.dat"

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
#define ERR_ARCH 6

typedef struct {
	float monto;
	char region[10];
	char fechaDesde[18];
	char fechaHasta[18];
}
filtroIPC;

typedef void (*Filtro)(void *filtro);

void solicitarDatos(void *f, Filtro monto, Filtro region, Filtro periodo);
void selecMonto(void *f);
void selecRegion(void *f);
bool formatearFecha1(char *c); //AAAAMM -> MES-AAAA
bool formatearFecha2(char *c); //AAAAMM -> AAAA-MM-DD


#endif
