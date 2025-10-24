#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#include "includes/divisiones.h"
#include "includes/vector.h"
#include "includes/comun.h"


/// actualizar monto divisiones

void actualizarMontoDivisiones(Vector *v)
{
    filtroIPC filtro;
    menu(&filtro, selecMontoDiv, selecRegionDiv, selecPeriodoDiv);

    char idxDesde[PERIODO_DIV_TAM] = {0};
    char idxHasta[PERIODO_DIV_TAM] = {0};

    ipc_div *div = buscarIndices(v, &filtro, idxDesde, idxHasta);
    if (!div) {
        printf("\nNo se encontraron ambos períodos para la región '%s'.\n", filtro.region);
        return;
    }

    resultIPC res = calcIPC(filtro.monto, idxDesde, idxHasta, filtro.region, filtro.fechaDesde, filtro.fechaHasta);
    printResultIPC(&res);
}

ipc_div *buscarIndices(Vector *v, filtroIPC *filtro, char *idxDesde, char *idxHasta)
{
    VectorIterador it;
    vectorIteradorCrear(&it, v);
    ipc_div *div = (ipc_div *)vectorIteradorPrimero(&it);
    int encontrados = 0;

    formatearFecha1(filtro->fechaDesde);
    formatearFecha1(filtro->fechaHasta);

    while (!vectorIteradorFin(&it) && encontrados < 2)
    {
        if (strcmp(div->desc, "Nivel general") == 0 &&
            strcmp(div->region, filtro->region) == 0)
        {
            if (strcmp(div->periodo, filtro->fechaDesde) == 0) {
                strcpy(idxDesde, div->indice_ipc);
                encontrados++;
            }
            else if (strcmp(div->periodo, filtro->fechaHasta) == 0) {
                strcpy(idxHasta, div->indice_ipc);
                encontrados++;
            }
        }
        div = (ipc_div *)vectorIteradorSiguiente(&it);
    }
    return (encontrados == 2) ? div : NULL;
}

resultIPC calcIPC(int monto, const char *desde, const char *hasta, const char *region, const char *fDesde, const char *fHasta)
{
    resultIPC res;
    res.montoOriginal = monto;

    float d = atof(desde);
    float h = atof(hasta);
    res.montoAjustado = monto * (h / d);
    res.variacion = (h / d - 1.0f) * 100.0f;

    strcpy(res.region, region);
    strcpy(res.desde, fDesde);
    strcpy(res.hasta, fHasta);

    return res;
}

void printResultIPC(const resultIPC *res)
{
    printf("\nRegión: %s\n", res->region);
    printf("Período: %s → %s\n", res->desde, res->hasta);
    printf("Monto inicial: %.2f\n", res->montoOriginal);
    printf("Monto ajustado: %.2f\n", res->montoAjustado);
    printf("Variación: %.2f%%\n\n", res->variacion);
}


/// calcular IPC promedio

void calcularIPCPromedio(Vector *v)
{
    Vector divClasificadas;
    vectorCrear(&divClasificadas, sizeof(divisiones));

    clasificarDivisiones(&divClasificadas, v, filtrarDivisiones);
    mostrarDIV(&divClasificadas); //muestra clasificadas por bienes y servicios
    calcularPromedios(&divClasificadas, mostrarIPC, "Nacional");

    vectorDestruir(&divClasificadas);
}

void clasificarDivisiones(Vector *dst, Vector *src, Clasificar filtrar)
{
    VectorIterador it;
    vectorIteradorCrear(&it, src);
    ipc_div *d = (ipc_div *)vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it))
    {
        divisiones div;
        if (filtrar(d, div.grupo))
        {
            strcpy(div.desc, d->desc);
            strcpy(div.indice_ipc, d->indice_ipc);
            strcpy(div.region, d->region);
            strcpy(div.fecha, d->periodo);
            vectorInsertar(dst, &div);
        }
        d = (ipc_div *)vectorIteradorSiguiente(&it);
    }
}

void calcularPromedios(Vector *vDivisiones, Fmt print, char *regionFiltro)
{
    Vector vAcumuladores, vIpc;
    vectorCrear(&vAcumuladores, sizeof(acumuladorRegion));
    vectorCrear(&vIpc, sizeof(indices));

    VectorIterador it;
    vectorIteradorCrear(&it, vDivisiones);
    divisiones *ptrDiv = (divisiones *)vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it))
    {
        int pos = buscarIndiceEnVector(&vAcumuladores, ptrDiv->fecha, ptrDiv->region);
        acumuladorRegion *ac = (acumuladorRegion *)vectorGet(&vAcumuladores, pos);
        float val = atof(ptrDiv->indice_ipc);

        if (strcmp(ptrDiv->grupo, "Bienes") == 0) {
            ac->bienes += val;
            ac->cantidadB++;
        }
        else if (strcmp(ptrDiv->grupo, "Servicios") == 0) {
            ac->servicios += val;
            ac->cantidadS++;
        }
        ptrDiv = (divisiones *)vectorIteradorSiguiente(&it);
    }

    // convertir acumuladores a IPC promedio
    vectorIteradorCrear(&it, &vAcumuladores);
    acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it))
    {
        indices ipc = {0};
        strcpy(ipc.fecha, ac->fecha);
        strcpy(ipc.region, ac->region);
        ipc.bienes = ac->cantidadB > 0 ? ac->bienes / ac->cantidadB : 0;
        ipc.servicios = ac->cantidadS > 0 ? ac->servicios / ac->cantidadS : 0;

        // solo insertar si coincide con la región deseada
        if (regionFiltro == NULL || strcmp(ipc.region, regionFiltro) == 0)
            vectorInsertar(&vIpc, &ipc);

        ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
    }

    printf("\n--- IPC Divisiones por Region ---\n\n");
    vectorMostrar(&vIpc, print);

    vectorDestruir(&vAcumuladores);
    vectorDestruir(&vIpc);
}

int buscarIndiceEnVector(Vector *v, char *fecha, char *region)
{
    VectorIterador it;
    vectorIteradorCrear(&it, v);
    acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);
    int pos = 0;

    while (!vectorIteradorFin(&it))
    {
        if (strcmp(ac->fecha, fecha) == 0 &&
            strcmp(ac->region, region) == 0)
        {
            return pos;
        }
        ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
        pos++;
    }

    // No se encontró: insertar nuevo acumulador
    acumuladorRegion nuevo = {0};
    strcpy(nuevo.fecha, fecha);
    strcpy(nuevo.region, region);
    vectorInsertar(v, &nuevo);
    return vectorCE(v) - 1;
}


/// otras funciones

void selecMontoDiv(void *f)
{
    int *monto = &((filtroIPC *)f)->monto;
    do {
		printf("\nSelecciona el monto (mayor a 0): ");
		scanf("%d", monto);
		if (*monto <= 0)
			printf("Monto invalido.\n");
	} while (*monto <= 0);
}

void selecRegionDiv(void *f)
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
            strcpy(region, "Nacional");
            break;
        case 2:
            strcpy(region, "GBA");
            break;
        case 3:
            strcpy(region, "Pampeana");
            break;
        case 4:
            strcpy(region, "Cuyo");
            break;
        case 5:
            strcpy(region, "Noroeste");
            break;
        case 6:
            strcpy(region, "Noreste");
            break;
        case 7:
            strcpy(region, "Patagonia");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
        }
    } while (opcion < 1 || opcion > 7);
}

void selecPeriodoDiv(void *f)
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

bool filtrarDivisiones(const void *elem, char *grupo)
{
    const char *code = ((const ipc_div *)elem)->code;

    if (strncmp(code, "01", 2) == 0 || strncmp(code, "02", 2) == 0 ||
        strncmp(code, "03", 2) == 0 || strncmp(code, "12", 2) == 0 ||
        strncmp(code, "05", 2) == 0)
    {
        strcpy(grupo, "Bienes");
        return true;
    }
    if (strncmp(code, "04", 2) == 0 || strncmp(code, "06", 2) == 0 ||
        strncmp(code, "07", 2) == 0 || strncmp(code, "08", 2) == 0 ||
        strncmp(code, "09", 2) == 0 || strncmp(code, "10", 2) == 0 ||
        strncmp(code, "11", 2) == 0)
    {
        strcpy(grupo, "Servicios");
        return true;
    }
    return false;
}

bool formatearDivisiones(char *c, void *elem)
{
    ipc_div *s = (ipc_div *)elem;
    char *act;
    char *p = c;
    while (*p)
    {
        if (*p == '"')
            memmove(p, p + 1, strlen(p));
        else
            p++;
    }

    act = strrchr(c, '\n');
    if (act)
        *act = '\0';

    /// periodo
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->periodo, act + 1, sizeof(s->periodo) - 1);
    s->periodo[sizeof(s->periodo) - 1] = '\0';
    if(!decodificarFecha(s->periodo))
        return false;
    if(!formatearFecha1(s->periodo))
        return false;
    *act = '\0';

    /// region
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->region, act + 1, sizeof(s->region) - 1);
    s->region[sizeof(s->region) - 1] = '\0';
    *act = '\0';

    /// v_a_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->v_a_ipc, act + 1, sizeof(s->v_a_ipc) - 1);
    s->v_a_ipc[sizeof(s->v_a_ipc) - 1] = '\0';
    *act = '\0';

    /// v_m_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->v_m_ipc, act + 1, sizeof(s->v_m_ipc) - 1);
    s->v_m_ipc[sizeof(s->v_m_ipc) - 1] = '\0';
    *act = '\0';

    /// indice_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->indice_ipc, act + 1, sizeof(s->indice_ipc) - 1);
    s->indice_ipc[sizeof(s->indice_ipc) - 1] = '\0';
    // reemplazar ',' por '.'
    for (char *q = s->indice_ipc; *q; q++)
        if (*q == ',') *q = '.';
    *act = '\0';

    /// clasificador
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->clasificador, act + 1, sizeof(s->clasificador) - 1);
    s->clasificador[sizeof(s->clasificador) - 1] = '\0';
    *act = '\0';

    /// desc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->desc, act + 1, sizeof(s->desc) - 1);
    s->desc[sizeof(s->desc) - 1] = '\0';
    // normalizar primer caracter
    if (s->desc[0])
        s->desc[0] = toupper((unsigned char)s->desc[0]);
    for (char *q = s->desc + 1; *q; q++)
        *q = tolower((unsigned char)*q);
    *act = '\0';

    /// code
    strncpy(s->code, c, sizeof(s->code) - 1);
    s->code[sizeof(s->code) - 1] = '\0';

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

void mostrarIPC(const void *elem)
{
	indices *i = (indices *)elem;
	printf("%15s;%10s;%10.4f;%10.4f\n", i->fecha, i->region, i->bienes, i->servicios);
}

void mostrarDIV(const Vector *v)
{
    divisiones *ptr = (divisiones *)v->vec;
    divisiones *fin = ptr + v->cantElem;
    for (; ptr < fin; ptr++)
    {
        printf("%15s;%56s;%12s;%12s;%12s\n",
               ptr->fecha, ptr->desc, ptr->indice_ipc, ptr->region, ptr->grupo);
    }
}


//regiones filtro

int regNacional(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Nacional") == 0;
}

int regGBA(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "GBA") == 0;
}

int regPampeana(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Pampeana") == 0;
}

int regCuyo(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Cuyo") == 0;
}

int regNoroeste(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Noroeste") == 0;
}

int regNoreste(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Noreste") == 0;
}

int regPatagonia(const void *elem)
{
    indices *i = (indices *)elem;
    return strcmp(i->region, "Patagonia") == 0;
}

