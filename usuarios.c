#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"

Vendedor* cargarVendedores(int *total) {
    FILE *archivo = fopen("vendedores.txt", "r");
    if (!archivo) return NULL;

    Vendedor *lista = NULL;
    *total = 0;
    Vendedor temp;

    while (fscanf(archivo, "%29[^,],%29[^,],%49[^\n]\n", temp.usuario, temp.clave, temp.nombre) == 3) {
        Vendedor *tempPtr = realloc(lista, (*total + 1) * sizeof(Vendedor));
        if (!tempPtr) { free(lista); fclose(archivo); return NULL; }
        lista = tempPtr;
        lista[*total] = temp;
        (*total)++;
    }
    fclose(archivo);
    return lista;
}

int iniciarSesion(Vendedor *vendedores, int total_vendedores) {
    char usuario[30], clave[30];
    
    for (int intentos = 0; intentos < 3; intentos++) {
        printf("\n--- INICIO DE SESION ---\n");
        printf("Usuario: ");
        scanf("%29s", usuario);
        printf("Clave: ");
        scanf("%29s", clave);

        for (int i = 0; i < total_vendedores; i++) {
            if (strcmp(vendedores[i].usuario, usuario) == 0 && strcmp(vendedores[i].clave, clave) == 0) {
                printf("\nBienvenido, %s!\n", vendedores[i].nombre);
                return 1; // Éxito
            }
        }
        printf("Credenciales incorrectas. Intentos restantes: %d\n", 2 - intentos);
    }
    return 0; // fallo tras 3 intentos
}