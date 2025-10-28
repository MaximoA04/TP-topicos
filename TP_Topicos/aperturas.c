#include "includes/aperturas.h"
#include "includes/vector.h"
#include "includes/comun.h"

/// Aperturas

void calcularAlquileres(Vector *v, Vector *vA)
{
    if (!v || !vA)
        return;

    filtroIPC f;
    solicitarDatos(&f, selecMonto, selecRegion, selecPeriodoAp);

    if (!formatearFecha2(f.fechaDesde))
        return;
    if (!clasificarAperturasIPC(v, &f, vA))
        return;
    if (vA->cantElem == 0)
    {
        printf("\nNo hay aperturas que cumplan los filtros.\n");
        return;
    }

    printf("\nTabla alquiler ajustada por IPC:\n");
    vectorMostrar(vA, mostrarAperturas);

    guardarTablaBinario(vA, ALQUILERES);
    Vector vLectura;
    if (leerTablaBinario(&vLectura, ALQUILERES))
    {
        printf("\nTabla desde archivo binario:\n");
        vectorMostrar(&vLectura, mostrarAperturas);
        vectorDestruir(&vLectura);
    }
    else
        printf("\nError al leer el archivo binario o está vacío.\n");

    mostrarTotales(vA, f.monto);
}

bool clasificarAperturasIPC(Vector *v, const filtroIPC *f, Vector *vA)
{
    if (!v || !f || !vA)
        return false;

    VectorIterador it;
    vectorIteradorCrear(&it, v);
    vectorIteradorPrimero(&it);
    float indiceBase = 0.0f;

    while (!vectorIteradorFin(&it))
    {
        ipc_ap *s = (ipc_ap *)vectorIteradorActual(&it);

        if (filtrarAlquiler(s, f) &&
            strcmp(s->region, f->region) == 0 &&
            strcmp(s->periodo, f->fechaDesde) >= 0)
        {
            aperturas a;
            strcpy(a.periodo, s->periodo);
            a.indice = atof(s->indice_ipc);

            if (indiceBase == 0.0f)
                indiceBase = a.indice;

            if (strcmp(s->periodo, f->fechaDesde) == 0)
            {
                a.variacion = 0.0f;
                a.montoAjustado = f->monto;
            }
            else
            {
                float factor = a.indice / indiceBase;
                a.montoAjustado = f->monto * factor;
                a.variacion = (factor - 1.0f) * 100.0f;
            }
            vectorInsertar(vA, &a);
        }
        vectorIteradorSiguiente(&it);
    }
    return true;
}

void guardarTablaBinario(Vector *v, const char *nombreArchivo)
{
    FILE *bin = fopen(nombreArchivo, "wb");
    if (!bin)
    {
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }
    if (vectorGuardarAArchivoBIN(v, bin) != TODO_OK)
        printf("Error al guardar los datos en binario.\n");

    fclose(bin);
}

bool leerTablaBinario(Vector *v, const char *nombreArchivo)
{
    if (!v || !nombreArchivo)
        return false;

    if (vectorCrear(v, sizeof(aperturas)) != TODO_OK)
    {
        printf("Error al crear el vector.\n");
        return false;
    }

    FILE *bin = fopen(nombreArchivo, "rb");
    if (!bin)
    {
        printf("No se pudo abrir el archivo para lectura.\n");
        return false;
    }

    if (vectorInsertarDeArchivoBIN(v, bin) != TODO_OK)
    {
        printf("Error al leer datos del archivo binario.\n");
        fclose(bin);
        return false;
    }
    fclose(bin);
    return true;
}


/// otras funciones

void selecPeriodoAp(void *f)
{
    char *fechaDesde = ((filtroIPC *)f)->fechaDesde;
    bool fechaValida = false;
    do
    {
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

bool filtrarAlquiler(const ipc_ap *s, const filtroIPC *f)
{
    return strcasecmp(s->desc, "Alquiler de la vivienda") == 0;
}

bool formatearAperturas(char *c, void *elem)
{
    if (!c || !elem)
        return false;

    ipc_ap *s = (ipc_ap *)elem;
    char *act;
    char *p = c;

    while (*p)
    {
        if (*p == '"')
            memmove(p, p + 1, strlen(p));
        else
            p++;
    }
    // eliminar salto de línea final
    act = strrchr(c, '\n');
    if (act)
        *act = '\0';

    // region
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->region, act + 1, sizeof(s->region) - 1);
    s->region[sizeof(s->region) - 1] = '\0';
    *act = '\0';

    // v_a_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->v_a_ipc, act + 1, sizeof(s->v_a_ipc) - 1);
    s->v_a_ipc[sizeof(s->v_a_ipc) - 1] = '\0';
    *act = '\0';

    // v_m_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->v_m_ipc, act + 1, sizeof(s->v_m_ipc) - 1);
    s->v_m_ipc[sizeof(s->v_m_ipc) - 1] = '\0';
    *act = '\0';

    // indice_ipc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->indice_ipc, act + 1, sizeof(s->indice_ipc) - 1);
    s->indice_ipc[sizeof(s->indice_ipc) - 1] = '\0';
    for (p = s->indice_ipc; *p; p++)
        if (*p == ',')
            *p = '.';
    *act = '\0';

    // periodo
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->periodo, act + 1, sizeof(s->periodo) - 1);
    s->periodo[sizeof(s->periodo) - 1] = '\0';
    if (!formatearFecha2(s->periodo))
        return false;
    *act = '\0';

    // clasificador
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->clasificador, act + 1, sizeof(s->clasificador) - 1);
    s->clasificador[sizeof(s->clasificador) - 1] = '\0';
    *act = '\0';

    // desc
    act = strrchr(c, ';');
    if (!act)
        return false;
    strncpy(s->desc, act + 1, sizeof(s->desc) - 1);
    s->desc[sizeof(s->desc) - 1] = '\0';

    if (s->desc[0])
        s->desc[0] = toupper((unsigned char)s->desc[0]);
    for (p = s->desc + 1; *p; p++)
        *p = tolower((unsigned char)*p);
    *act = '\0';

    // resto del string
    strncpy(s->code, c, sizeof(s->code) - 1);
    s->code[sizeof(s->code) - 1] = '\0';

    return true;
}

void mostrarIpcAp(const void *elem)
{
    const ipc_ap *a = (const ipc_ap *)elem;

    printf("%10s, %72s, %35s, %12s, %8s, %8s, %10s, %12s\n",
           a->code, a->desc, a->clasificador, a->periodo,
           a->indice_ipc, a->v_m_ipc, a->v_a_ipc, a->region);
}

void mostrarAperturas(const void *elem)
{
    aperturas *a = (aperturas *)elem;
    printf("%15s, %8.2f, %6.1f %%, $%10.2f\n", a->periodo, a->indice, a->variacion, a->montoAjustado);
}

void mostrarTotales(const Vector *vA, float montoInicial)
{
    if (!vA || vA->cantElem <= 0)
        return;

    VectorIterador it;
    vectorIteradorCrear(&it, (Vector *)vA);
    vectorIteradorPrimero(&it);

    aperturas *ultimo = NULL;
    while (!vectorIteradorFin(&it))
    {
        ultimo = (aperturas *)vectorIteradorActual(&it);
        vectorIteradorSiguiente(&it);
    }

    if (ultimo)
    {
        printf("\nMonto inicial: %.2f\n", montoInicial);
        printf("Monto ajustado por inflacion: %.2f\n", ultimo->montoAjustado);
        printf("Variacion porcentual: %.2f%%\n", ultimo->variacion);
    }
}

