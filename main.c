#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"
#include "productos.h"

int main() {
    int total_vendedores = 0;
    int total_productos = 0;

    // cargar datos en memoria
    Vendedor *vendedores = cargarVendedores(&total_vendedores);
    Producto *productos = cargarProductos(&total_productos);

    if (!vendedores || !productos) {
        printf("Error critico: No se encontraron los archivos 'vendedores.txt' o 'producto.txt'.\n");
        if (vendedores) free(vendedores);
        if (productos) free(productos);
        return 1;
    }

    // validacion de inicio de sesion 
    if (!iniciarSesion(vendedores, total_vendedores)) {
        printf("Sistema bloqueado. Saliendo...\n");
        free(vendedores);
        free(productos);
        return 1;
    }

    // registro de ventas
    int continuar = 1;
    ItemVenta *carrito = NULL;
    int items_en_carrito = 0;

    while (continuar) {
        int codigo_buscado, cantidad_req;
        printf("\nIngrese codigo de producto (o -1 para terminar): ");
        scanf("%d", &codigo_buscado);

        if (codigo_buscado == -1) break;

        // buscar producto
        Producto *prod_encontrado = NULL;
        for (int i = 0; i < total_productos; i++) {
            if (productos[i].codigo == codigo_buscado) {
                prod_encontrado = &productos[i];
                break;
            }
        }

        if (!prod_encontrado) {
            printf("Producto no encontrado.\n");
            continue;
        }

        printf("Cantidad a vender (Disponible: %d): ", prod_encontrado->cantidad_disponible);
        scanf("%d", &cantidad_req);

        if (cantidad_req > prod_encontrado->cantidad_disponible) {
            printf("Error: No hay suficiente inventario.\n");
        } else if (cantidad_req > 0) {
            ItemVenta *tempCarrito = realloc(carrito, (items_en_carrito + 1) * sizeof(ItemVenta));
            if (!tempCarrito) {
                printf("Error de memoria en el carrito.\n");
                break;
            }
            carrito = tempCarrito;
            carrito[items_en_carrito].producto_ref = prod_encontrado;
            carrito[items_en_carrito].cantidad_a_vender = cantidad_req;
            items_en_carrito++;
            printf("Producto agregado a la venta.\n");
        }
    }

    //  generacion de factura
    if (items_en_carrito > 0) {
        int num_factura = obtenerSiguienteFactura();
        char fecha[11];
        obtenerFechaActual(fecha);
        float total_factura = 0;


        printf("FACTURA NUMERO: %d\n", num_factura);
        printf("FECHA: %s\n", fecha);

        printf("%-20s %-10s %-10s %-10s\n", "Producto", "Cant", "P.Unit", "Total");
        
        for (int i = 0; i < items_en_carrito; i++) {
            float subtotal = carrito[i].cantidad_a_vender * carrito[i].producto_ref->precio_venta;
            total_factura += subtotal;
            printf("%-20s %-10d $%-9.2f $%.2f\n", 
                   carrito[i].producto_ref->nombre, 
                   carrito[i].cantidad_a_vender, 
                   carrito[i].producto_ref->precio_venta, 
                   subtotal);
        }

        printf("TOTAL A PAGAR: $%.2f\n", total_factura);

        char confirmacion;
        printf("Desea confirmar esta venta? (s/n): ");
        scanf(" %c", &confirmacion);

        if (confirmacion == 's' || confirmacion == 'S') {
            FILE *archivo_ventas = fopen("ventas.txt", "a");
            if (archivo_ventas) {
                for (int i = 0; i < items_en_carrito; i++) {
                    carrito[i].producto_ref->cantidad_disponible -= carrito[i].cantidad_a_vender;
                    fprintf(archivo_ventas, "%d,%d,%s,%d,%.2f,%.2f,%s\n",
                            num_factura, 
                            carrito[i].producto_ref->codigo, 
                            carrito[i].producto_ref->nombre, 
                            carrito[i].cantidad_a_vender, 
                            carrito[i].producto_ref->costo, 
                            carrito[i].producto_ref->precio_venta, 
                            fecha);
                }
                fclose(archivo_ventas);
                actualizarArchivoProductos(productos, total_productos);
                printf("\nVenta registrada exitosamente. Inventario actualizado.\n");
            }
        } else {
            printf("\nVenta cancelada. El inventario no fue modificado.\n");
        }
    } else {
        printf("\nNo se registraron productos para vender.\n");
    }

    // siempre liberar memoria, siempre
    free(vendedores);
    free(productos);
    if (carrito) free(carrito);

    return 0;
}