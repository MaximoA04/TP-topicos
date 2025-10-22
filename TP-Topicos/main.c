#include <stddef.h>
#include <stdio.h>
#include <locale.h>
#include "includes/vector.h"
#include "includes/divisiones.h"
#include "includes/aperturas.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"
#define IPC_AP "./public/serie_ipc_aperturas copy.csv"
#define ALQUILERES "./public/alquileres.dat"

void print(const Vector *v);

int main()
{
    //setlocale(LC_ALL, "es_AR.UTF-8");
	int code = 0;
/*
	///Primera Parte
	Vector v;
	FILE *f = fopen(IPC_DIV, "r");
	if (!f) {
		printf("Error al abrir el archivo %s\n", IPC_DIV);
		return -1;
	}
	code = vectorCrear(&v, sizeof(serie_ipc_divisiones));
	if (code != OK) {
		fclose(f);
		return code;
	}
	code = vectorInsertarDeArchivoTXT(&v, f, formatearDivisiones, 0);
	if (code != OK) {
		fclose(f);
		vectorDestruir(&v);
		return code;
	}
	fclose(f);

	//mostrar ipc divisiones
    //print(&v);

	//actualizarMontoDivisiones(&v);
	calcularIPCPromedio(&v);
	vectorDestruir(&v);
*/

	///Segunda Parte
	Vector v2, vA;
    FILE *f2 = fopen(IPC_AP, "r");
    if (!f2) {
        printf("Error al abrir el archivo %s\n", IPC_AP);
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

	return code;
}

void print(const Vector *v)
{
    serie_ipc_divisiones *ptr = (serie_ipc_divisiones *)v->vec;
    serie_ipc_divisiones *fin = ptr + v->cantElem;
    for (; ptr < fin; ptr++)
    {
        printf("%12s;%56s;%36s;%10s;%6s;%6s;%10s;%15s\n",
               ptr->code, ptr->desc, ptr->clasificador, ptr->indice_ipc,
               ptr->v_m_ipc, ptr->v_a_ipc, ptr->region, ptr->periodo);
    }
}
