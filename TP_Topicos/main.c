#include <stddef.h>
#include <stdio.h>
#include <locale.h>

#include "includes/divisiones.h"
#include "includes/aperturas.h"
#include "includes/vector.h"

int main()
{
    //setlocale(LC_ALL, "es_AR.UTF-8");
    int code = 0;

    /// Primera Parte: IPC Divisiones
    Vector v;
    FILE *f = fopen(DIVISIONES, "r");
    if (!f)
    {
        printf("Error al abrir el archivo %s\n", DIVISIONES);
        return ERR;
    }

    if (vectorCrear(&v, sizeof(ipc_div)) != TODO_OK)
    {
        fclose(f);
        return ERR_MEM;
    }

    if (vectorInsertarDeArchivoTXT(&v, f, formatearDivisiones, 0) != TODO_OK)
    {
        fclose(f);
        vectorDestruir(&v);
        return ERR_MEM;
    }
    fclose(f);

    //printf("\n--- Datos de IPC Divisiones ---\n\n");
    //vectorMostrar(&v,mostrarIpcDiv);

    actualizarMontoDivisiones(&v);
    calcularIPCPromedio(&v);

    vectorDestruir(&v);

    /// Segunda Parte: Aperturas y Alquileres
    Vector v2, vA;
    FILE *f2 = fopen(APERTURAS, "r");
    if (!f2)
    {
        printf("Error al abrir el archivo %s\n", APERTURAS);
        return ERR_ARCH;
    }

    FILE *alquileres = fopen(ALQUILERES, "wb+");
    if (!alquileres)
    {
        printf("Error al abrir el archivo %s\n", ALQUILERES);
        fclose(f2);
        return ERR_ARCH;
    }

    if ((code = vectorCrear(&v2, sizeof(ipc_ap))) != TODO_OK)
    {
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    if ((code = vectorCrear(&vA, sizeof(aperturas))) != TODO_OK)
    {
        vectorDestruir(&v2);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    if ((code = vectorInsertarDeArchivoTXT(&v2, f2, formatearAperturas, 0)) != TODO_OK)
    {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(f2);
        fclose(alquileres);
        return code;
    }
    fclose(f2);
    //vectorMostrar(&v2,mostrarIpcAp);
    calcularAlquileres(&v2, &vA);

    if ((code = vectorGuardarAArchivoBIN(&vA, alquileres)) != TODO_OK)
    {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(alquileres);
        return code;
    }

    Vector vA2;
    if ((code = vectorCrear(&vA2, sizeof(aperturas))) != TODO_OK)
    {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(alquileres);
        return code;
    }

    rewind(alquileres);
    if ((code = vectorInsertarDeArchivoBIN(&vA2, alquileres)) != TODO_OK)
    {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        vectorDestruir(&vA2);
        fclose(alquileres);
        return code;
    }

    vectorDestruir(&v2);
    vectorDestruir(&vA);
    vectorDestruir(&vA2);
    fclose(alquileres);

    return code;
}

