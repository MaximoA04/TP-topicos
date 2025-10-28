#ifndef APERTURAS_H
#define APERTURAS_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"
#include "vector.h"
#include "comun.h"
#include <ctype.h>


#define CODE_AP_TAM 20
#define DESC_AP_TAM 63
#define CLASIFICADOR_AP_TAM 40
#define PERIODO_AP_TAM 20
#define INDICE_IPC_AP_TAM 20
#define V_M_IPC_AP_TAM 20
#define V_A_IPC_AP_TAM 20
#define REGION_AP_TAM 12

typedef struct {
	char code[CODE_AP_TAM];
	char desc[DESC_AP_TAM];
	char clasificador[CLASIFICADOR_AP_TAM];
	char periodo[PERIODO_AP_TAM];
	char indice_ipc[INDICE_IPC_AP_TAM];
	char v_m_ipc[V_M_IPC_AP_TAM];
	char v_a_ipc[V_A_IPC_AP_TAM];
	char region[REGION_AP_TAM];
}
ipc_ap;

typedef struct {
	char periodo[PERIODO_AP_TAM];
	float indice;
	float variacion;
	float montoAjustado;
}
aperturas;

/// Aperturas
void calcularAlquileres(Vector *v, Vector *vA);
bool clasificarAperturasIPC(Vector *v, const filtroIPC *f, Vector *vA);
void guardarTablaBinario(Vector *v, const char *nombreArchivo);
bool leerTablaBinario(Vector *v, const char *nombreArchivo);


///otras funciones
void selecPeriodoAp(void *f);
bool filtrarAlquiler(const ipc_ap *s, const filtroIPC *f);
bool formatearAperturas(char *c, void *elem);
void mostrarIpcAp(const void *elem);
void mostrarAperturas(const void *elem);
void mostrarTotales(const Vector *vA, float montoInicial);

#endif
