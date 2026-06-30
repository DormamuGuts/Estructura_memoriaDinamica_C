#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
#define MAX_VENDEDORES 100 // limite de 100

Vendedor* cargarVendedores(int *total) {
    FILE *archivo = fopen("vendedores.txt", "r");
    if (!archivo) return NULL;

    Vendedor *lista = (Vendedor*)calloc(MAX_VENDEDORES, sizeof(Vendedor));
    if (!lista) {
        fclose(archivo);
        return NULL;
    }

    *total = 0;
    while (*total < MAX_VENDEDORES && 
           fscanf(archivo, "%29[^,],%29[^,],%49[^\n]\n", 
           lista[*total].usuario, 
           lista[*total].clave, 
           lista[*total].nombre) == 3) {
        
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