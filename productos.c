#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "productos.h"

Producto* cargarProductos(int *total) {
    FILE *archivo = fopen("producto.txt", "r");
    if (!archivo) return NULL;

    Producto *lista = NULL;
    *total = 0;
    Producto temp;

    while (fscanf(archivo, "%d,%49[^,],%d,%f,%f\n", &temp.codigo, temp.nombre, 
                  &temp.cantidad_disponible, &temp.costo, &temp.precio_venta) == 5) {
        Producto *tempPtr = realloc(lista, (*total + 1) * sizeof(Producto));
        if (!tempPtr) { free(lista); fclose(archivo); return NULL; }
        lista = tempPtr;
        lista[*total] = temp;
        (*total)++;
    }
    fclose(archivo);
    return lista;
}

void actualizarArchivoProductos(Producto *productos, int total) {
    FILE *archivo = fopen("producto.txt", "w");
    if (!archivo) return;
    for (int i = 0; i < total; i++) {
        fprintf(archivo, "%d,%s,%d,%.2f,%.2f\n", 
                productos[i].codigo, productos[i].nombre, 
                productos[i].cantidad_disponible, productos[i].costo, productos[i].precio_venta);
    }
    fclose(archivo);
}

void obtenerFechaActual(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int obtenerSiguienteFactura() {
    FILE *archivo = fopen("ventas.txt", "r");
    // si no existe, de nuevo a la factura 1
    if (!archivo) return 1; 

    int num_factura, max_factura = 0;
    char linea[200];

    while (fgets(linea, sizeof(linea), archivo)) {
        if (sscanf(linea, "%d", &num_factura) == 1) {
            if (num_factura > max_factura) {
                max_factura = num_factura;
            }
        }
    }
    fclose(archivo);
    return max_factura + 1;
}