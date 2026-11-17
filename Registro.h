#ifndef REGISTRO_H
#define REGISTRO_H

#include "Alumno.h"
#include "ListaAlumnos.h"
#include "ArbolAlumnos.h"

/*
 * Registro: une las dos estructuras del programa y las mantiene sincronizadas.
 *   - ListaAlumnos (lineal): guarda los datos de los alumnos. Se usa para
 *     recorrerlos, ordenarlos por riesgo con Merge Sort y filtrar a los que
 *     estan en riesgo.
 *   - ArbolAlumnos (no lineal, AVL): indice por ID. Se usa para buscar, validar
 *     IDs repetidos, eliminar y recorrer en orden de ID.
 *
 * Complejidad (n = numero de alumnos):
 *   agregar            Mejor O(log n)    Promedio O(log n)    Peor O(log n)
 *   buscar             Mejor O(1)        Promedio O(log n)    Peor O(log n)
 *   eliminar           Mejor O(log n)    Promedio O(log n)    Peor O(log n)
 *   ordenarPorRiesgo   O(n log n) en todos los casos
 *   mostrar            O(n) en todos los casos
 *   mostrarPorId       O(n) en todos los casos
 *   mostrarEnRiesgo    Mejor O(1)        Promedio O(n)        Peor O(n)   (lista ya ordenada)
 *   limpiar            O(n) en todos los casos
 */
class Registro {
private:
    ListaAlumnos lista;
    ArbolAlumnos indice;
    bool ordenadaPorRiesgo;

public:
    Registro() : ordenadaPorRiesgo(false) {}

    // O(1)
    int getTam() const { return lista.getTam(); }
    bool estaVacia() const { return lista.estaVacia(); }
    int alturaArbol() const { return indice.altura(); }
    bool estaOrdenadaPorRiesgo() const { return ordenadaPorRiesgo; }

    // Validar ID O(log n) + insertar en lista O(1) + insertar en arbol O(log n) = O(log n)
    bool agregar(const Alumno& a) {
        if (indice.existe(a.getId())) return false;
        Nodo* nodo = lista.agregarFinal(a);
        indice.insertar(nodo);
        ordenadaPorRiesgo = false;
        return true;
    }

    // Mejor O(1), promedio y peor O(log n)
    bool existe(int id) const { return indice.existe(id); }

    // Mejor O(1), promedio y peor O(log n). Regresa nullptr si no existe.
    const Alumno* buscar(int id) const {
        Nodo* nodo = indice.buscar(id);
        return nodo == nullptr ? nullptr : &nodo->dato;
    }

    // Buscar en arbol O(log n) + quitar del arbol O(log n) + quitar de la lista O(1) = O(log n)
    bool eliminar(int id) {
        Nodo* nodo = indice.buscar(id);
        if (nodo == nullptr) return false;
        indice.eliminar(id);
        lista.eliminarNodo(nodo);
        return true;
    }

    // Merge Sort sobre la lista: O(n log n) en todos los casos.
    // Merge Sort mueve nodos completos, no copia datos, asi que el arbol sigue
    // apuntando a los nodos correctos sin tener que actualizarse.
    void ordenarPorRiesgo() {
        lista.ordenar(mayorRiesgo);
        ordenadaPorRiesgo = true;
    }

    // O(n): en el orden actual de la lista
    void mostrar() const { lista.mostrar(); }

    // O(n): recorrido inorden del arbol, sale en orden de ID sin ordenar nada
    void mostrarPorId() const {
        if (estaVacia()) {
            std::cout << "No hay alumnos. Carga el archivo o agrega alumnos.\n";
            return;
        }
        Alumno::mostrarEncabezado();
        indice.recorrerInorden([](const Alumno& a) { a.mostrarInformacion(); });
        std::cout << "Total: " << getTam() << " alumno(s)\n";
    }

    // Mejor O(1), promedio y peor O(n) (se ordena antes si hace falta)
    int mostrarEnRiesgo(double umbral) {
        if (!ordenadaPorRiesgo) ordenarPorRiesgo();
        return lista.mostrarEnRiesgo(umbral, true);
    }

    // O(n): recorre en el orden actual de la lista
    template <typename Funcion>
    void recorrer(Funcion f) const { lista.recorrer(f); }

    // O(n): recorre en orden de ID
    template <typename Funcion>
    void recorrerPorId(Funcion f) const { indice.recorrerInorden(f); }

    // O(n)
    void limpiar() {
        indice.limpiar();
        lista.limpiar();
        ordenadaPorRiesgo = false;
    }
};

#endif // REGISTRO_H
