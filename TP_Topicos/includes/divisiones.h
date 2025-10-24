#ifndef DIVISIONES_H
#define DIVISIONES_H

#include "string.h"
#include "comun.h"
#include "vector.h"

#define BIENES 5
#define SERVICIOS 7
#define REGIONES 7

#define CODE_DIV_TAM 21
#define DESC_DIV_TAM 55
#define CLASIFICADOR_DIV_TAM 35
#define INDICE_IPC_DIV_TAM 17
#define V_M_IPC_DIV_TAM 17
#define V_A_IPC_DIV_TAM 17
#define REGION_DIV_TAM 10
#define PERIODO_DIV_TAM 18
#define GRUPO_DIV_TAM 10

typedef struct {
	char code[CODE_DIV_TAM];
	char desc[DESC_DIV_TAM];
	char clasificador[CLASIFICADOR_DIV_TAM];
	char indice_ipc[INDICE_IPC_DIV_TAM];
	char v_m_ipc[V_M_IPC_DIV_TAM];
	char v_a_ipc[V_A_IPC_DIV_TAM];
	char region[REGION_DIV_TAM];
	char periodo[PERIODO_DIV_TAM];
}
ipc_div;

typedef struct {
	char fecha[PERIODO_DIV_TAM]; //AAAA-MM-DD
	char desc[DESC_DIV_TAM];
	char indice_ipc[INDICE_IPC_DIV_TAM];
	char region[REGION_DIV_TAM];
	char grupo[GRUPO_DIV_TAM];
}
divisiones;

typedef struct {
	char fecha[PERIODO_DIV_TAM];
	char region[REGION_DIV_TAM];
	float bienes;
	float servicios;
	int cantidadB;
	int cantidadS;
}
acumuladorRegion;

typedef struct {
    float montoOriginal;
    float montoAjustado;
    float variacion;
    char region[REGION_DIV_TAM];
    char desde[PERIODO_DIV_TAM];
    char hasta[PERIODO_DIV_TAM];
}
resultIPC;

typedef struct {
	char fecha[PERIODO_DIV_TAM];
	char region[REGION_DIV_TAM];
	float bienes;
	float servicios;
}
indices;


typedef bool (*Clasificar)(const void *elem, char *grupo);

/// actualizar monto divisiones
void actualizarMontoDivisiones(Vector *v);
ipc_div *buscarIndices(Vector *v, filtroIPC *filtro, char *idxDesde, char *idxHasta);
resultIPC calcIPC(int monto, const char *desde, const char *hasta, const char *region, const char *fDesde, const char *fHasta);
void printResultIPC(const resultIPC *res);


/// calcular IPC prom
void calcularIPCPromedio(Vector *v);
void clasificarDivisiones(Vector *dst, Vector *src, Clasificar filtrar);
void calcularPromedios(Vector *divisiones, Fmt print, char *regionFiltro);
int buscarIndiceEnVector(Vector *v, char *fecha, char *region);

/// otras funciones
void selecMontoDiv(void *f);
void selecRegionDiv(void *f);
void selecPeriodoDiv(void *f);
bool filtrarDivisiones(const void *elem, char *grupo);
bool formatearDivisiones(char *c, void *elem);
bool decodificarFecha(char *fecha);
void mostrarDIV(const Vector *v);
void mostrarIPC(const void *elem);
void mostrarNacional(const void *elem);

//regiones filtro
int regNacional(const void *elem);
int regGBA(const void *elem);
int regPampeana(const void *elem);
int regCuyo(const void *elem);
int regNoroeste(const void *elem);
int regNoreste(const void *elem);
int regPatagonia(const void *elem);



#endif // !DIVISIONES_H
