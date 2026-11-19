#include "pizzeria.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <limits>

using namespace std;

const PizzaInfo CATALOGO[8] = {
    {"NP01", "Margarita",     "Tradicional"},
    {"NP02", "Marinara",      "Tradicional"},
    {"NP03", "Barbacoa",      "Carne"},
    {"NP04", "Pollo BBQ",     "Carne"},
    {"NP05", "Vegetal",       "Verduras"},
    {"NP06", "Espinacas",     "Verduras"},
    {"NP07", "Cuatro quesos", "Quesos"},
    {"NP08", "Gorgonzola",    "Quesos"}
};

const string LOCALIDADES_INICIALES[10] = {
    "Torrejon de Ardoz",
    "Parla",
    "Mostoles",
    "Alcala de Henares",
    "Getafe",
    "Coslada",
    "Fuenlabrada",
    "Alcobendas",
    "Leganes",
    "Alcorcon"
};

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pulsarEnter(const string& mensaje) {
    cout << "\n" << mensaje;
    cin.get();
}

int contarVentas(Venta* cabeza) {
    int contador = 0;
    Venta* actual = cabeza;
    while (actual != nullptr) {
        contador++;
        actual = actual->sig;
    }
    return contador;
}

void liberarListaVentas(Venta*& cabeza) {
    Venta* actual = cabeza;
    while (actual != nullptr) {
        Venta* temp = actual;
        actual = actual->sig;
        delete temp;
    }
    cabeza = nullptr;
}

void liberarArbolCompleto(NodoCentro*& raiz) {
    if (raiz == nullptr) return;
    liberarArbolCompleto(raiz->izq);
    liberarArbolCompleto(raiz->der);
    liberarListaVentas(raiz->listaVentas);
    delete raiz;
    raiz = nullptr;
}

NodoCentro* buscarCentro(NodoCentro* raiz, int id) {
    if (raiz == nullptr) return nullptr;
    if (id == raiz->id_centro) return raiz;
    if (id < raiz->id_centro) return buscarCentro(raiz->izq, id);
    return buscarCentro(raiz->der, id);
}

bool insertarCentroABB(NodoCentro*& raiz, int id, const string& localidad) {
    if (raiz == nullptr) {
        NodoCentro* nuevo = new NodoCentro;
        nuevo->id_centro = id;
        nuevo->localidad = localidad;
        nuevo->listaVentas = nullptr;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        raiz = nuevo;
        return true;
    }
    if (id == raiz->id_centro) {
        return false;
    }
    if (id < raiz->id_centro) {
        return insertarCentroABB(raiz->izq, id, localidad);
    } else {
        return insertarCentroABB(raiz->der, id, localidad);
    }
}

NodoCentro* encontrarMinimo(NodoCentro* nodo) {
    while (nodo != nullptr && nodo->izq != nullptr) {
        nodo = nodo->izq;
    }
    return nodo;
}

bool borrarCentroABB(NodoCentro*& raiz, int id) {
    if (raiz == nullptr) return false;

    if (id < raiz->id_centro) {
        return borrarCentroABB(raiz->izq, id);
    } else if (id > raiz->id_centro) {
        return borrarCentroABB(raiz->der, id);
    } else {
        // Liberar toda la lista dinámica de ventas asociada al centro
        liberarListaVentas(raiz->listaVentas);

        // Caso 1: sin hijos
        if (raiz->izq == nullptr && raiz->der == nullptr) {
            delete raiz;
            raiz = nullptr;
        }
        // Caso 2: solo hijo derecho
        else if (raiz->izq == nullptr) {
            NodoCentro* temp = raiz;
            raiz = raiz->der;
            delete temp;
        }
        // Caso 3: solo hijo izquierdo
        else if (raiz->der == nullptr) {
            NodoCentro* temp = raiz;
            raiz = raiz->izq;
            delete temp;
        }
        // Caso 4: dos hijos -> reemplazar con el sucesor inorden
        else {
            NodoCentro* sucesor = encontrarMinimo(raiz->der);
            raiz->id_centro = sucesor->id_centro;
            raiz->localidad = sucesor->localidad;
            raiz->listaVentas = sucesor->listaVentas;
            sucesor->listaVentas = nullptr;

            NodoCentro* padre = raiz;
            NodoCentro* actual = raiz->der;
            while (actual->izq != nullptr) {
                padre = actual;
                actual = actual->izq;
            }
            if (padre == raiz) {
                raiz->der = actual->der;
            } else {
                padre->izq = actual->der;
            }
            delete actual;
        }
        return true;
    }
}

void mostrarResumenArbolInorden(NodoCentro* raiz) {
    if (raiz == nullptr) return;
    mostrarResumenArbolInorden(raiz->izq);
    cout << "ID: " << setw(3) << raiz->id_centro
         << " Localidad: " << setw(22) << raiz->localidad
         << " Num ventas: " << contarVentas(raiz->listaVentas) << "\n";
    mostrarResumenArbolInorden(raiz->der);
}

void recolectarIDs(NodoCentro* raiz, vector<int>& ids) {
    if (raiz == nullptr) return;
    recolectarIDs(raiz->izq, ids);
    ids.push_back(raiz->id_centro);
    recolectarIDs(raiz->der, ids);
}

string generarFechaAleatoria() {
    int mes = 1 + rand() % 12;
    int diasMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dia = 1 + rand() % diasMes[mes];

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d-%02d-2026", dia, mes);
    return string(buffer);
}

void agregarVentaAlFinal(Venta*& cabeza, const Venta& datosVenta) {
    Venta* nueva = new Venta(datosVenta);
    nueva->sig = nullptr;

    if (cabeza == nullptr) {
        cabeza = nueva;
    } else {
        Venta* actual = cabeza;
        while (actual->sig != nullptr) {
            actual = actual->sig;
        }
        actual->sig = nueva;
    }
}

void opcionInsertarCentro(NodoCentro*& raiz) {
    int id;
    string localidad;

    cout << "\n--- OPCION 1: INSERTAR UN CENTRO ---\n";
    cout << "Introduzca el identificador de 3 cifras (100-999): ";
    if (!(cin >> id) || id < 100 || id > 999) {
        limpiarBuffer();
        cout << "Error: El identificador debe ser un numero de 3 cifras (100 a 999).\n";
        return;
    }
    limpiarBuffer();

    cout << "Introduzca la localidad del centro: ";
    getline(cin, localidad);
    if (localidad.empty()) {
        cout << "Error: La localidad no puede estar vacia.\n";
        return;
    }

    if (insertarCentroABB(raiz, id, localidad)) {
        cout << "Centro insertado correctamente en el ABB (ID: " << id
             << ", Localidad: " << localidad << ", Ventas: 0).\n";
    } else {
        cout << "Error: Ya existe un centro con el identificador " << id
             << ". No se ha modificado el arbol.\n";
    }
}

void opcionBorrarCentro(NodoCentro*& raiz) {
    int id;

    cout << "\n--- OPCION 2: BORRAR UN CENTRO ---\n";
    cout << "Introduzca el identificador del centro a borrar: ";
    if (!(cin >> id)) {
        limpiarBuffer();
        cout << "Error: Entrada no valida.\n";
        return;
    }
    limpiarBuffer();

    if (borrarCentroABB(raiz, id)) {
        cout << "Centro " << id << " eliminado del ABB junto con todas sus ventas (memoria liberada).\n";
    } else {
        cout << "No existe ningun centro con el identificador " << id << ".\n";
    }
}

void opcionMostrarVentasCentro(NodoCentro* raiz) {
    int id;

    cout << "\n--- OPCION 3: MOSTRAR LAS VENTAS DE UN CENTRO ---\n";
    cout << "Introduzca el identificador del centro: ";
    if (!(cin >> id)) {
        limpiarBuffer();
        cout << "Error: Entrada no valida.\n";
        return;
    }
    limpiarBuffer();

    NodoCentro* centro = buscarCentro(raiz, id);
    if (centro == nullptr) {
        cout << "No se ha encontrado ningun centro con el ID " << id << ".\n";
        return;
    }

    cout << "\nCENTRO " << centro->id_centro << " - " << centro->localidad << "\n";

    if (centro->listaVentas == nullptr) {
        cout << "Este centro no tiene ventas registradas actualmente (lista vacia).\n";
        return;
    }

    cout << left << setw(10) << "Venta"
         << setw(8)  << "Pizza"
         << setw(14) << "Categoria"
         << right << setw(10) << "Unidades"
         << "  " << left << setw(12) << "Fecha" << "\n";
    cout << string(58, '-') << "\n";

    Venta* actual = centro->listaVentas;
    while (actual != nullptr) {
        cout << left << setw(10) << actual->id_venta
             << setw(8)  << actual->cod_pizza
             << setw(14) << actual->categoria
             << right << setw(10) << actual->unidades
             << "  " << left << setw(12) << actual->fecha_venta << "\n";
        actual = actual->sig;
    }
}
