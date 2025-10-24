#include <stddef.h>
#include <stdio.h>
#include <locale.h>

#include "includes/divisiones.h"
#include "includes/aperturas.h"
#include "includes/vector.h"

#define DIVISIONES "./otros/serie_ipc_divisiones copy.csv"
#define APERTURAS "./otros/serie_ipc_aperturas copy.csv"
#define ALQUILERES "./otros/alquileres.dat"

void mostrar(const Vector *v);

int main()
{
    //setlocale(LC_ALL, "es_AR.UTF-8");
	int code = 0;

	///Primera Parte
	Vector v;
    FILE *f = fopen(DIVISIONES, "r");
    if (!f) {
        printf("Error al abrir el archivo %s\n", DIVISIONES);
        return ERR;
    }

    if (vectorCrear(&v, sizeof(ipc_div)) != TODO_OK) {
        fclose(f);
        return ERR_MEM;
    }

    if (vectorInsertarDeArchivoTXT(&v, f, formatearDivisiones, 0) != TODO_OK) {
        fclose(f);
        vectorDestruir(&v);
        return ERR_MEM;
    }
    fclose(f);

    printf("\n--- Datos de IPC Divisiones ---\n\n");
    mostrar(&v);

    actualizarMontoDivisiones(&v);

    printf("\n--- IPC Divisiones por tipo ---\n\n");
    calcularIPCPromedio(&v);

    vectorDestruir(&v);

/*
	///Segunda Parte
	Vector v2, vA;
    FILE *f2 = fopen(APERTURAS, "r");
    if (!f2) {
        printf("Error al abrir el archivo %s\n", APERTURAS);
        return -1;
    }

    FILE *alquileres = fopen(ALQUILERES, "wb+");
    if (!alquileres) {
        printf("Error al abrir el archivo %s\n", ALQUILERES);
        fclose(f2);
        return -1;
    }

    code = vectorCrear(&v2, sizeof(serie_ipc_aperturas));
    if (code != OK) {
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    code = vectorCrear(&vA, sizeof(aperturas));
    if (code != OK) {
        vectorDestruir(&v2);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    code = vectorInsertarDeArchivoTXT(&v2, f2, formatearAperturas, 0);
    if (code != OK) {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    calcularPromedioAlquiler(&v2, &vA);

    code = vectorGuardarAArchivoBIN(&vA, alquileres);
    if (code != OK) {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    Vector vA2;
    code = vectorCrear(&vA2, sizeof(aperturas));
    if (code != OK) {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    rewind(alquileres);
    code = vectorInsertarDeArchivoBIN(&vA2, alquileres);
    if (code != OK) {
        vectorDestruir(&v2);
        vectorDestruir(&vA);
        vectorDestruir(&vA2);
        fclose(f2);
        fclose(alquileres);
        return code;
    }

    puts("\nVariacion de alquileres desde archivo:");
    vectorMostrar(&vA2, printAperturas);

    vectorDestruir(&v2);
    vectorDestruir(&vA);
    vectorDestruir(&vA2);
    fclose(f2);
    fclose(alquileres);
*/
	return code;
}

void mostrar(const Vector *v)
{
    ipc_div *ptr = (ipc_div *)v->vec;
    ipc_div *fin = ptr + v->cantElem;
    for (; ptr < fin; ptr++)
    {
        printf("%12s;%56s;%36s;%10s;%6s;%6s;%10s;%15s\n",
               ptr->code, ptr->desc, ptr->clasificador, ptr->indice_ipc,
               ptr->v_m_ipc, ptr->v_a_ipc, ptr->region, ptr->periodo);
    }
}
