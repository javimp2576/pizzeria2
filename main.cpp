#include "pizzeria.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    NodoCentro* raiz = nullptr;
    int contadorVentasGlobal = 10000;

    // 1. Creación inicial de N_CENTRO centros con IDs únicos de 3 cifras y localidades distintas
    cout << "Creando el ABB con " << N_CENTRO << " centros:\n";
    int creados = 0;
    while (creados < N_CENTRO) {
        int idAleatorio = 100 + rand() % 900;
        if (insertarCentroABB(raiz, idAleatorio, LOCALIDADES_INICIALES[creados])) {
            creados++;
        }
    }

    cout << "Arbol inicial:\n";
    mostrarResumenArbolInorden(raiz);

    // 2. Generar N_VENTA ventas aleatorias asignadas a centros existentes
    vector<int> idsExistentes;
    recolectarIDs(raiz, idsExistentes);

    vector<Venta> ventasGeneradas;
    ventasGeneradas.reserve(N_VENTA);

    cout << "\nVentas nuevas:\n";
    cout << "| " << setw(9) << "ID Centro"
         << "| " << setw(8) << "ID Venta"
         << "| " << setw(5) << "Pizza"
         << "| " << setw(11) << "Categoria"
         << "| " << setw(8) << "Unidades"
         << "| " << setw(10) << "Fecha" << "|\n";
    cout << string(62, '-') << "\n";

    for (int i = 0; i < N_VENTA; i++) {
        int idxCentro = rand() % static_cast<int>(idsExistentes.size());
        int idxPizza  = rand() % 8;
        int unidades  = 1 + rand() % 20;

        Venta v;
        v.id_centro   = idsExistentes[idxCentro];
        v.id_venta    = "V" + to_string(contadorVentasGlobal++);
        v.cod_pizza   = CATALOGO[idxPizza].codigo;
        v.categoria   = CATALOGO[idxPizza].categoria;
        v.unidades    = unidades;
        v.fecha_venta = generarFechaAleatoria();
        v.sig         = nullptr;

        ventasGeneradas.push_back(v);

        cout << "| " << right << setw(9) << v.id_centro
             << "| " << setw(8) << v.id_venta
             << "| " << setw(5) << v.cod_pizza
             << "| " << setw(11) << v.categoria
             << "| " << setw(8) << v.unidades
             << "| " << setw(10) << v.fecha_venta << "|\n";
    }

    // Esperar Enter para distribuir las ventas
    pulsarEnter("Pulse Enter para distribuir las ventas...");

    for (const Venta& v : ventasGeneradas) {
        NodoCentro* destino = buscarCentro(raiz, v.id_centro);
        if (destino != nullptr) {
            agregarVentaAlFinal(destino->listaVentas, v);
        }
    }

    cout << "\nVentas distribuidas. Estado del arbol:\n";
    mostrarResumenArbolInorden(raiz);

    // 3. Bucle del menú principal
    int opcion = -1;
    do {
        cout << "\nOpciones disponibles:\n";
        cout << "1- Insertar un centro.\n";
        cout << "2- Borrar un centro.\n";
        cout << "3- Mostrar las ventas de un centro.\n";
        cout << "4- Buscar una venta.\n";
        cout << "5- Extraer una venta.\n";
        cout << "6- Trasladar una venta a otro centro.\n";
        cout << "7- Mostrar estadisticas de los centros.\n";
        cout << "8- Generar y distribuir nuevas ventas.\n";
        cout << "0- Salir del programa.\n";
        cout << "Seleccione una opcion del menu: ";

        if (!(cin >> opcion)) {
            limpiarBuffer();
            cout << "Opcion no valida. Introduzca un numero.\n";
            opcion = -1;
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                opcionInsertarCentro(raiz);
                break;
            case 2:
                opcionBorrarCentro(raiz);
                break;
            case 3:
                opcionMostrarVentasCentro(raiz);
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                cout << "Esta opcion no esta implementada en esta entrega (solo hasta la opcion 3).\n";
                break;
            case 0:
                cout << "Saliendo del programa y liberando memoria...\n";
                break;
            default:
                cout << "Opcion no reconocida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 0);

    liberarArbolCompleto(raiz);
    return 0;
}
