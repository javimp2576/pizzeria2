#ifndef PIZZERIA_H
#define PIZZERIA_H

#include <string>
#include <vector>

// Constantes por defecto indicadas en el enunciado
const int N_CENTRO = 10;
const int N_VENTA  = 30;

// Estructura de una venta (nodo de lista dinámica enlazada)
struct Venta {
    int id_centro;
    std::string id_venta;
    std::string cod_pizza;
    std::string categoria;
    int unidades;
    std::string fecha_venta;
    Venta* sig;
};

// Estructura de un centro (nodo del Árbol Binario de Búsqueda - ABB)
struct NodoCentro {
    int id_centro;
    std::string localidad;
    Venta* listaVentas;
    NodoCentro* izq;
    NodoCentro* der;
};

// Catálogo de pizzas
struct PizzaInfo {
    std::string codigo;
    std::string nombre;
    std::string categoria;
};

extern const PizzaInfo CATALOGO[8];
extern const std::string LOCALIDADES_INICIALES[10];

// Utilidades de entrada/salida y memoria
void limpiarBuffer();
void pulsarEnter(const std::string& mensaje = "Pulse Enter para continuar...");
int contarVentas(Venta* cabeza);
void liberarListaVentas(Venta*& cabeza);
void liberarArbolCompleto(NodoCentro*& raiz);

// Operaciones sobre el ABB y listas de ventas
NodoCentro* buscarCentro(NodoCentro* raiz, int id);
bool insertarCentroABB(NodoCentro*& raiz, int id, const std::string& localidad);
NodoCentro* encontrarMinimo(NodoCentro* nodo);
bool borrarCentroABB(NodoCentro*& raiz, int id);
void mostrarResumenArbolInorden(NodoCentro* raiz);
void recolectarIDs(NodoCentro* raiz, std::vector<int>& ids);
std::string generarFechaAleatoria();
void agregarVentaAlFinal(Venta*& cabeza, const Venta& datosVenta);

// Opciones del menú (hasta la opción 3)
void opcionInsertarCentro(NodoCentro*& raiz);
void opcionBorrarCentro(NodoCentro*& raiz);
void opcionMostrarVentasCentro(NodoCentro* raiz);

#endif // PIZZERIA_H
