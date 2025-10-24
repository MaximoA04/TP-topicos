#include "includes/comun.h"

void menu(void *f, Filtro monto, Filtro region, Filtro periodo)
{
	monto(f);
	region(f);
	periodo(f);
}

bool formatearFecha1(char *c)
{
    char meses[12][11] = {
        "Enero", "Febrero", "Marzo", "Abril",
        "Mayo", "Junio", "Julio", "Agosto",
        "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    if (!c || strlen(c) < 6)
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

