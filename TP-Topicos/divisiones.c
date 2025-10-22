#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "includes/comun.h"
#include "includes/string.h"
#include "includes/divisiones.h"
#include "includes/vector.h"

static void seleccionarPeriodoDivisiones(void *f)
{
    char *fechaDesde = ((filtroIPC *)f)->fechaDesde;
    char *fechaHasta = ((filtroIPC *)f)->fechaHasta;
    bool fechaValida = false;
    do {
        printf("Ingrese una fecha desde (AAAAMM): ");
        scanf("%s", fechaDesde);

        if (strlen(fechaDesde) != 6)
            printf("Fecha inválida. Intente de nuevo.\n");
        else
        {
            char anio[5] = {0};
            char mes[3] = {0};
            strncpy(anio, fechaDesde, 4);
            strncpy(mes, fechaDesde + 4, 2);
            int anioInt = atoi(anio);
            int mesInt = atoi(mes);
            if (anioInt >= 1900 && mesInt >= 1 && mesInt <= 12)
                fechaValida = true;
            else
                printf("Fecha inválida. Intente de nuevo.\n");
        }
    } while (!fechaValida);

    fechaValida = false;
    do {
        printf("Ingrese una fecha hasta (AAAAMM): ");
        scanf("%s", fechaHasta);
        if (strlen(fechaHasta) != 6)
            printf("Fecha inválida. Intente de nuevo.\n");
        else {
            char anio[5] = {0};
            char mes[3] = {0};
            strncpy(anio, fechaHasta, 4);
            strncpy(mes, fechaHasta + 4, 2);
            int anioInt = atoi(anio);
            int mesInt = atoi(mes);
            if (anioInt >= 1900 && mesInt >= 1 && mesInt <= 12)
                fechaValida = true;
            else
                printf("Fecha invalida. Intente de nuevo.\n");
        }
    }
    while (!fechaValida);
}
static void seleccionarRegionDivisiones(void *f)
{
	char *region = ((filtroIPC *)f)->region;
	printf("Seleccione la region: \n");
	printf("1. Nacional\n");
	printf("2. GBA\n");
	printf("3. Pampeana\n");
	printf("4. Cuyo\n");
	printf("5. Noroeste\n");
	printf("6. Noreste\n");
	printf("7. Patagonia\n");
	int opcion = 0;
	do {
		printf("Ingrese una opcion (1-7): ");
		scanf("%d", &opcion);
		switch (opcion) {
		case 1:
			cpyString(region, "Nacional");
			break;
		case 2:
			cpyString(region, "GBA");
			break;
		case 3:
			cpyString(region, "Pampeana");
			break;
		case 4:
			cpyString(region, "Cuyo");
			break;
		case 5:
			cpyString(region, "Noroeste");
			break;
		case 6:
			cpyString(region, "Noreste");
			break;
		case 7:
			cpyString(region, "Patagonia");
			break;
		default:
			printf("Opcion invalida. Intente de nuevo.\n");
			break;
		}
	} while (opcion < 1 || opcion > 7);
}
static void seleccionarMontoDivisiones(void *f)
{
    int *monto = &((filtroIPC *)f)->monto;
    do {
		printf("Selecciona el monto (mayor a 0): ");
		scanf("%d", monto);
		if (*monto <= 0)
			printf("Monto invalido.\n");
	} while (*monto <= 0);
}
static bool filtrarDivisiones(const void *elem, char *grupo)
{
	const char *codeB[BIENES] = { "01", "02", "03", "12", "05" };
	const char *codeS[SERVICIOS] = { "04", "06", "07", "08",
					 "09", "10", "11" };
	if (includeString(((serie_ipc_divisiones *)elem)->code, codeB,
			  BIENES)) {
		cpyString(grupo, "Bienes");
		return true;
	} else if (includeString(((serie_ipc_divisiones *)elem)->code, codeS,
				 SERVICIOS)) {
		cpyString(grupo, "Servicios");
		return true;
	}
	return false;
}
static int regNacional(const void *elem)
{
	indices *i = (indices *)elem;
	return cmpString(i->region, "Nacional") == 0;
}
static void printIPC(const void *elem)
{
	indices *i = (indices *)elem;
	printf("%15s;%10s;%10.2f;%10.2f\n", i->fecha, i->region, i->bienes, i->servicios);
}
static void printNacional(const void *elem)
{
	if (cmpString(((indices *)elem)->region, "Nacional") == 0)
		printIPC(elem);
}
bool formatearDivisiones(char *c, void *elem)
{
    removerCharEnString(c, '"');
    serie_ipc_divisiones *s = (serie_ipc_divisiones *)elem;
    char *act;

    // quita el salto de línea si existe
    act = buscarCharEnStringEnReversa(c, '\n');
    if (act)
        *act = '\0';

    /// periodo
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->periodo, act + 1, sizeof(s->periodo) - 1);
    s->periodo[sizeof(s->periodo) - 1] = '\0';
    //formato AAAAMM
    if(!decodificarFecha(s->periodo))
      return false;
    //formato MES-AAAA
    if(!formatearFecha(s->periodo))
      return false;

    *act = '\0';
    /// region
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->region, act + 1, sizeof(s->region) - 1);
    s->region[sizeof(s->region) - 1] = '\0';

    *act = '\0';
    /// v_a_ipc
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->v_a_ipc, act + 1, sizeof(s->v_a_ipc) - 1);
    s->v_a_ipc[sizeof(s->v_a_ipc) - 1] = '\0';

    *act = '\0';
    /// v_m_ipc
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->v_m_ipc, act + 1, sizeof(s->v_m_ipc) - 1);
    s->v_m_ipc[sizeof(s->v_m_ipc) - 1] = '\0';

    *act = '\0';
    /// indice_ipc
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->indice_ipc, act + 1, sizeof(s->indice_ipc) - 1);
    s->indice_ipc[sizeof(s->indice_ipc) - 1] = '\0';
    reemplazarCharEnString(s->indice_ipc, ',', '.');

    *act = '\0';
    /// clasificador
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->clasificador, act + 1, sizeof(s->clasificador) - 1);
    s->clasificador[sizeof(s->clasificador) - 1] = '\0';

    *act = '\0';
    /// desc
    act = buscarCharEnStringEnReversa(c, ';');
    if (!act)
        return false;
    strncpy(s->desc, act + 1, sizeof(s->desc) - 1);
    s->desc[sizeof(s->desc) - 1] = '\0';
    normalizarPrimerChar(s->desc);

    *act = '\0';
    /// code
    strncpy(s->code, c, sizeof(s->code) - 1);
    s->code[sizeof(s->code) - 1] = '\0';

    return true;
}
bool formatearFecha(char *c)
{
    char meses[12][11] = {
        "Enero", "Febrero", "Marzo", "Abril",
        "Mayo", "Junio", "Julio", "Agosto",
        "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    if (!c || strlen(c) < 6)  // mínimo AAAAMM
        return false;

    char mesReg[3] = {0};
    strncpy(mesReg, c + 4, 2);
    int mes = atoi(mesReg);
    if (mes < 1 || mes > 12)
        return false;
    mes -= 1;

    char anio[5] = {0};
    strncpy(anio, c, 4);

    size_t lenMes = strlen(meses[mes]);
    memcpy(c, meses[mes], lenMes);
    c[lenMes] = '-';
    strncpy(c + lenMes + 1, anio, 4);
    c[lenMes + 1 + 4] = '\0';

    return true;
}
bool decodificarFecha(char *fecha)
{
    if (!fecha || strlen(fecha) < 6)
        return false;

    const char *cod = "4687195032"; // mapa de decodificación, asumiendo q para 5 es un 9

    for (int i = 0; i < 6; i++) {
        char c = fecha[i];
        if (c < '0' || c > '9')
            return false;
        fecha[i] = cod[c - '0'];
    }
    return true;
}
void printDIV(const Vector *v)
{
    divisiones *ptr = (divisiones *)v->vec;
    divisiones *fin = ptr + v->cantElem;
    for (; ptr < fin; ptr++)
    {
        printf("%15s;%56s;%12s;%12s;%12s\n",
               ptr->fecha, ptr->desc, ptr->indice_ipc, ptr->region, ptr->grupo);
    }
}

/***** actualizarMontoDivisiones ******/
void actualizarMontoDivisiones(Vector *v)
{
	filtroIPC filtro;
	menu(&filtro, seleccionarMontoDivisiones, seleccionarRegionDivisiones,
	     seleccionarPeriodoDivisiones);
	generarHerramienta(filtro, v);
}
void generarHerramienta(filtroIPC filtro, Vector *v)
{
    serie_ipc_divisiones *s;
    VectorIterador it;
    vectorIteradorCrear(&it, v);
    s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);

    int encontrados = 0;
    char sDesde[PERIODO_DIV_TAM], sHasta[PERIODO_DIV_TAM];

    while (!vectorIteradorFin(&it) && encontrados != 2)
    {
        if (cmpString(s->desc, "Nivel general") == 0 &&
                cmpString(s->region, filtro.region) == 0)
        {
            //formato MES-AAAA
            formatearFecha(filtro.fechaDesde);
            formatearFecha(filtro.fechaHasta);
            if (cmpString(s->periodo, filtro.fechaDesde) == 0) {
                printf("Encontrado desde: %s\n", s->periodo);
                cpyString(sDesde, s->indice_ipc);
                encontrados++;
            }

            else if (cmpString(s->periodo, filtro.fechaHasta) == 0) {
                printf("Encontrado hasta: %s\n", s->periodo);
                cpyString(sHasta, s->indice_ipc);
                encontrados++;
            }
        }
        s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
    }
    if (encontrados == 2)
        calcularIPC(sDesde, sHasta, filtro.monto);
    else
        printf("\nNo se encontraron ambos períodos para la región '%s'.\n", filtro.region);
}
void calcularIPC(char *sDesde, char *sHasta, int monto)
{
    float pDesde = atof(sDesde);
    float pHasta = atof(sHasta);
    float montoAjustado = monto * (pHasta / pDesde);
    float variacion = (pHasta / pDesde - 1) * 100;
    printf("\nMonto inicial: %d\n", monto);
    printf("Monto ajustado: %.2f\n", montoAjustado);
    printf("Variacion: %.2f%%\n\n", variacion);
}

/***** calcularIPCPromedio ******/
void calcularIPCPromedio(Vector *v)
{
	Vector Divisiones;
	vectorCrear(&Divisiones, sizeof(divisiones));
	clasificarDivisiones(&Divisiones, v, filtrarDivisiones);
	//mostrar diviciones
    //printDIV(&Divisiones);
	calcularPromedios(&Divisiones, printIPC);
	vectorEliminarPorFiltro(&Divisiones, regNacional);
	vectorDestruir(&Divisiones);
}
void clasificarDivisiones(Vector *D, Vector *v, Clasificar c)
{
    VectorIterador it;
    vectorIteradorCrear(&it, v);
    serie_ipc_divisiones *s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it))
    {
        divisiones div;
        if (c(s, div.grupo))
        {
            cpyString(div.desc, s->desc);
            cpyString(div.indice_ipc, s->indice_ipc);
            cpyString(div.region, s->region);
            cpyString(div.fecha, s->periodo);
            vectorInsertar(D, &div);
        }
        s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
    }
}
void calcularPromedios(Vector *D, Fmt print)
{
	Vector vAc, vIpc;
	vectorCrear(&vAc, sizeof(acumuladorRegion));
	vectorCrear(&vIpc, sizeof(indices));
	VectorIterador it;
	vectorIteradorCrear(&it, D);
	divisiones *d = (divisiones *)vectorIteradorPrimero(&it);

	while (!vectorIteradorFin(&it))
    {
		int pos = buscarIndiceEnVector(&vAc, d->fecha, d->region);
		acumuladorRegion *ac = (acumuladorRegion *)vectorGet(&vAc, pos);
		float val = atof(d->indice_ipc);

		if (cmpString(d->grupo, "Bienes") == 0) {
			ac->bienes += val;
			ac->cantidadB++;
		} else if (cmpString(d->grupo, "Servicios") == 0) {
			ac->servicios += val;
			ac->cantidadS++;
		}

		d = (divisiones *)vectorIteradorSiguiente(&it);
	}
	vectorIteradorCrear(&it, &vAc);
	acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);

	while (!vectorIteradorFin(&it))
    {
		indices ipc = { 0 };
		cpyString(ipc.fecha, ac->fecha);
		cpyString(ipc.region, ac->region);
		ipc.bienes = ac->cantidadB > 0 ? ac->bienes / ac->cantidadB : 0;
		ipc.servicios = ac->cantidadS > 0 ? ac->servicios / ac->cantidadS : 0;

		vectorInsertar(&vIpc, &ipc);
		ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
	}
	vectorMostrar(&vIpc, printNacional);

	vectorDestruir(&vAc);
	vectorDestruir(&vIpc);
}
int buscarIndiceEnVector(Vector *v, char *fecha, char *region)
{
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);
	int pos = 0;
	while (!vectorIteradorFin(&it)) {
		if (cmpString(ac->fecha, fecha) == 0 &&
		    cmpString(ac->region, region) == 0) {
			return pos;
		}
		ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
		pos++;
	}
	acumuladorRegion nuevo = { 0 };
	cpyString(nuevo.fecha, fecha);
	cpyString(nuevo.region, region);
	vectorInsertar(v, &nuevo);
	return vectorCE(v) - 1;
}
