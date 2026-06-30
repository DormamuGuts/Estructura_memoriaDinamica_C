#ifndef USUARIOS_H
#define USUARIOS_H

// estructura para vendedores.txt
typedef struct {
    char usuario[30];
    char clave[30];
    char nombre[50];
} Vendedor;

//  funciones
Vendedor* cargarVendedores(int *total);
int iniciarSesion(Vendedor *vendedores, int total_vendedores);
#endif