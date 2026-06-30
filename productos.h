#ifndef PRODUCTOS_H
#define PRODUCTOS_H

// estructura archivo producto.txt
typedef struct {
    int codigo;
    char nombre[50];
    int cantidad_disponible;
    float costo;
    float precio_venta;
} Producto;

// estructura carro de compra
typedef struct {
    Producto *producto_ref; 
    int cantidad_a_vender;
} ItemVenta;

// funciones
Producto* cargarProductos(int *total);
void actualizarArchivoProductos(Producto *productos, int total);
void obtenerFechaActual(char *buffer);
int obtenerSiguienteFactura();

#endif