#include "includes/comun.h"

void solicitarDatos(void *f, Filtro monto, Filtro region, Filtro periodo)
{
    monto(f);
    region(f);
    periodo(f);
}

void selecMonto(void *f)
{
    float *monto = &((filtroIPC *)f)->monto;
    do{
        printf("\nSelecciona el monto (mayor a 0): ");
        scanf("%f", monto);
        if (*monto <= 0)
            printf("Monto invalido.\n");
    } while (*monto <= 0);
}

void selecRegion(void *f)
{
    char *region = ((filtroIPC *)f)->region;
    printf("Seleccione la region \n");
    printf("\t1. Nacional\n");
    printf("\t2. GBA\n");
    printf("\t3. Pampeana\n");
    printf("\t4. Cuyo\n");
    printf("\t5. Noroeste\n");
    printf("\t6. Noreste\n");
    printf("\t7. Patagonia\n");

    int opcion = 0;
    do{
        printf("Ingrese una opcion (1-7): ");
        scanf("%d", &opcion);
        switch (opcion)
        {
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

bool formatearFecha1(char *c)
{
    if (!c || strlen(c) < 6)
        return false;

    // si ya tiene guion y let, asumimos que esta bien
    if (strchr(c, '-') && strlen(c) > 6)
        return true;

    // formato YYYYMM
    char mesReg[3] = {0};
    strncpy(mesReg, c + 4, 2);
    int mes = atoi(mesReg);
    if (mes < 1 || mes > 12)
        return false;
    mes -= 1;

    char anio[5] = {0};
    strncpy(anio, c, 4);
    anio[4] = '\0';

    char meses[12][11] = {
        "Enero", "Febrero", "Marzo", "Abril",
        "Mayo", "Junio", "Julio", "Agosto",
        "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    size_t lenMes = strlen(meses[mes]);
    memcpy(c, meses[mes], lenMes);
    c[lenMes] = '-';
    strncpy(c + lenMes + 1, anio, 4);
    c[lenMes + 1 + 4] = '\0';

    return true;
}

bool formatearFecha2(char *c)
{
    if (!c || strlen(c) < 6)
        return false;

    // si es YYYY-MM-DD
    if (strlen(c) == 10 && c[4] == '-' && c[7] == '-')
        return true;

    // si es YYYYMM
    if (strlen(c) == 6) {
        char anio[5], mes[3];
        strncpy(anio, c, 4); anio[4] = '\0';
        strncpy(mes, c + 4, 2); mes[2] = '\0';
        int imes = atoi(mes);
        if (imes < 1 || imes > 12)
            return false;

        snprintf(c, 11, "%s-%02d-01", anio, imes);
        c[10] = '\0';
        return true;
    }

    return false;
}
