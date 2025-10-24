#include <ctype.h>

#include "includes/aperturas.h"
#include "includes/vector.h"
#include "includes/comun.h"

///Aperturas

void clasificarAperturas(Vector *v, filtroIPC *f, Vector *vA)
{
    if (!v || !f || !vA)
        return;

    VectorIterador it;
    vectorIteradorCrear(&it, v);

    serie_ipc_aperturas *s = (serie_ipc_aperturas *)vectorIteradorPrimero(&it);
    aperturas a;
    float indiceDesde = 0.0f;
    char *fechaDesde = formatearFechaAperturas(f->fechaDesde);

    while (!vectorIteradorFin(&it) && s)
    {
        if (strcmp(s->region, f->region) == 0 &&
            strcmp(s->code, "04.1.1") == 0)
        {
            strcpy(a.periodo, s->periodo);
            a.indice = atof(s->indice_ipc);

            if (indiceDesde == 0.0f && strcmp(s->periodo, fechaDesde) >= 0)
                indiceDesde = a.indice;

            if (indiceDesde != 0.0f)
            {
                if (strcmp(s->periodo, fechaDesde) == 0)
                {
                    a.variacion = 0.0f;
                    a.montoAjustado = f->monto;
                }
                else if (strcmp(s->periodo, fechaDesde) > 0)
                {
                    float factor = a.indice / indiceDesde;
                    a.montoAjustado = f->monto * factor;
                    a.variacion = (factor - 1.0f) * 100.0f;
                }
                vectorInsertar(vA, &a);
            }
        }
        s = (serie_ipc_aperturas *)vectorIteradorSiguiente(&it);
    }
}

void calcularPromedioAlquiler(Vector *v, Vector *vA)
{
    if (!v || !vA)
        return;

    filtroIPC f;
    menu(&f, selecMontoAp, selecRegionAp, selecPeriodoAp);

    clasificarAperturas(v, &f, vA);

    if (vA->cantElem > 0)
        vectorMostrar(vA, printAperturas);
    else
        printf("\nNo hay aperturas que cumplan los filtros.\n");
}

char *formatearFecha2(char *c)
{
    char anio[5];
    strncpy(anio, c, 4);
    anio[4] = '\0';

    snprintf(c, 11, "%04d-%02d-01", atoi(anio), atoi(c + 4));
    return c;
}


///otras funciones

void selecMontoAp(void *f)
{
	int *monto = &((filtroIPC *)f)->monto;
	do {
        printf("Seleccione el monto a actualizar del alquiler: ");
		scanf("%d", monto);
		if (*monto <= 0)
			printf("Monto invalido.\n");
	} while (*monto <= 0);
}

void selecRegionAp(void *f)
{
    char *region = ((filtroIPC *)f)->region;
    printf("Seleccione la region: \n");
    printf("1. GBA\n");
    printf("2. Pampeana\n");
    printf("3. Cuyo\n");
    printf("4. Noroeste\n");
    printf("5. Noreste\n");
    printf("6. Patagonia\n");

    int opcion = 0;
    do {
        printf("Ingrese una opcion (1-6): ");
        scanf("%d", &opcion);
        switch (opcion)
        {
            case 1:
                strcpy(region, "GBA");
                break;
            case 2:
                strcpy(region, "Pampeana");
                break;
            case 3:
                strcpy(region, "Cuyo");
                break;
            case 4:
                strcpy(region, "Noroeste");
                break;
            case 5:
                strcpy(region, "Noreste");
                break;
            case 6:
                strcpy(region, "Patagonia");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }
    } while (opcion < 1 || opcion > 6);
}

void selecPeriodoAp(void *f)
{
    char *fechaDesde = ((filtroIPC *)f)->fechaDesde;
    bool fechaValida = false;
    do {
        printf("Ingrese una fecha desde (AAAAMM): ");
        scanf("%s", fechaDesde);
        if (strlen(fechaDesde) != 6)
            printf("Fecha invalida. Intente de nuevo.\n");
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
                printf("Fecha invalida. Intente de nuevo.\n");
        }
    } while (!fechaValida);
}

bool formatearAperturas(char *c, void *elem)
{
    serie_ipc_aperturas *s = (serie_ipc_aperturas *)elem;
    char *act;

    // eliminar salto de línea final
    act = strrchr(c, '\n');
    if (act)
        *act = '\0';

    // region
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->region, act + 1);
    *act = '\0';

    // v_a_ipc
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->v_a_ipc, act + 1);
    *act = '\0';

    // v_m_ipc
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->v_m_ipc, act + 1);
    *act = '\0';

    // indice_ipc
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->indice_ipc, act + 1);
    // reemplazar coma por punto
    for (char *p = s->indice_ipc; *p; p++)
        if (*p == ',')
            *p = '.';
    *act = '\0';

    // periodo
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->periodo, act + 1);
    formatearFechaAperturas(s->periodo);
    *act = '\0';

    // clasificador
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->clasificador, act + 1);
    *act = '\0';

    // desc
    act = strrchr(c, ';');
    if (!act) return false;
    strcpy(s->desc, act + 1);
    if (s->desc[0])
        s->desc[0] = toupper((unsigned char)s->desc[0]);
    for (char *p = s->desc + 1; *p; p++)
        *p = tolower((unsigned char)*p);
    *act = '\0';

    strcpy(s->code, c);
    return true;
}

void printAperturas(const void *elem)
{
	aperturas *a = (aperturas *)elem;
	printf("%15s, %10.2f, %4.2f, %10.2f\n", a->periodo, a->indice, a->variacion,
	       a->montoAjustado);
}

void printAP(const Vector *v)
{
    serie_ipc_aperturas *ptr = (serie_ipc_aperturas *)v->vec;
    serie_ipc_aperturas *fin = ptr + v->cantElem;
    for (; ptr < fin; ptr++)
    {
        printf("%s;%s;%s;%s;%s;%s;%s;%s\n",
               ptr->code, ptr->desc, ptr->clasificador, ptr->periodo,
               ptr->indice_ipc, ptr->v_m_ipc, ptr->v_a_ipc, ptr->region);
    }
}



