#ifndef ARBOL_ALUMNOS_H
#define ARBOL_ALUMNOS_H

#include "Alumno.h"
#include "ListaAlumnos.h"

/*
 * ArbolAlumnos: arbol AVL (arbol binario de busqueda auto-balanceado) ordenado por ID.
 *
 * Es un indice: cada nodo guarda el ID y un apuntador al nodo de la lista
 * doblemente ligada donde vive el Alumno. Asi los datos no se duplican, la lista
 * y el arbol siempre muestran la misma informacion y, una vez encontrado en el
 * arbol, el alumno se puede quitar de la lista en O(1).
 *
 * Al ser AVL, la altura siempre es O(log n), porque despues de cada insercion o
 * eliminacion se hacen rotaciones para que las alturas de los subarboles
 * izquierdo y derecho de cada nodo no difieran en mas de 1.
 *
 * Complejidad (n = numero de alumnos):
 *   insertar   Mejor O(log n)  Promedio O(log n)  Peor O(log n)
 *   buscar     Mejor O(1)      Promedio O(log n)  Peor O(log n)
 *   eliminar   Mejor O(log n)  Promedio O(log n)  Peor O(log n)
 *   inorden    Mejor O(n)      Promedio O(n)      Peor O(n)
 *   limpiar    Mejor O(n)      Promedio O(n)      Peor O(n)
 *   altura, getTam             O(1) en todos los casos
 * Memoria: O(n) nodos; la recursion usa O(log n) de pila.
 */

struct NodoArbol {
    int id;
    Nodo* nodoLista;
    NodoArbol* izq;
    NodoArbol* der;
    int altura;
    NodoArbol(int _id, Nodo* a) : id(_id), nodoLista(a), izq(nullptr), der(nullptr), altura(1) {}
};

class ArbolAlumnos {
private:
    NodoArbol* raiz;
    int tam;

    // O(1)
    static int alturaDe(NodoArbol* n) { return n == nullptr ? 0 : n->altura; }
    static int balanceDe(NodoArbol* n) { return n == nullptr ? 0 : alturaDe(n->izq) - alturaDe(n->der); }
    static void actualizarAltura(NodoArbol* n) {
        int a = alturaDe(n->izq), b = alturaDe(n->der);
        n->altura = 1 + (a > b ? a : b);
    }

    // Rotaciones: O(1), solo reconectan apuntadores
    static NodoArbol* rotarDerecha(NodoArbol* y) {
        NodoArbol* x = y->izq;
        y->izq = x->der;
        x->der = y;
        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    static NodoArbol* rotarIzquierda(NodoArbol* x) {
        NodoArbol* y = x->der;
        x->der = y->izq;
        y->izq = x;
        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    // Revisa el factor de balance y aplica la rotacion necesaria: O(1)
    static NodoArbol* balancear(NodoArbol* n) {
        actualizarAltura(n);
        int b = balanceDe(n);
        if (b > 1) {                                       // cargado a la izquierda
            if (balanceDe(n->izq) < 0) n->izq = rotarIzquierda(n->izq);   // caso izq-der
            return rotarDerecha(n);                                       // caso izq-izq
        }
        if (b < -1) {                                      // cargado a la derecha
            if (balanceDe(n->der) > 0) n->der = rotarDerecha(n->der);     // caso der-izq
            return rotarIzquierda(n);                                     // caso der-der
        }
        return n;
    }

    // Baja por un solo camino de la raiz a una hoja: O(log n) en todos los casos
    NodoArbol* insertar(NodoArbol* n, int id, Nodo* a, bool& insertado) {
        if (n == nullptr) { insertado = true; return new NodoArbol(id, a); }
        if (id < n->id)      n->izq = insertar(n->izq, id, a, insertado);
        else if (id > n->id) n->der = insertar(n->der, id, a, insertado);
        else return n;                                     // ID repetido: no se inserta
        return balancear(n);
    }

    // O(log n): el sucesor es el nodo mas a la izquierda del subarbol derecho
    static NodoArbol* minimo(NodoArbol* n) {
        while (n->izq != nullptr) n = n->izq;
        return n;
    }

    // Busca el nodo (O(log n)), lo quita y rebalancea de regreso (O(log n))
    NodoArbol* eliminar(NodoArbol* n, int id, bool& eliminado) {
        if (n == nullptr) return nullptr;
        if (id < n->id)      n->izq = eliminar(n->izq, id, eliminado);
        else if (id > n->id) n->der = eliminar(n->der, id, eliminado);
        else {
            eliminado = true;
            if (n->izq == nullptr || n->der == nullptr) {  // 0 o 1 hijo
                NodoArbol* hijo = (n->izq != nullptr) ? n->izq : n->der;
                delete n;
                return hijo;
            }
            NodoArbol* sucesor = minimo(n->der);           // 2 hijos
            n->id = sucesor->id;
            n->nodoLista = sucesor->nodoLista;
            bool tmp = false;
            n->der = eliminar(n->der, sucesor->id, tmp);
        }
        return balancear(n);
    }

    // Recorrido izquierda - raiz - derecha: visita los n nodos en orden de ID, O(n)
    template <typename Funcion>
    static void inorden(NodoArbol* n, Funcion& f) {
        if (n == nullptr) return;
        inorden(n->izq, f);
        f(n->nodoLista->dato);
        inorden(n->der, f);
    }

    // O(n)
    static void limpiar(NodoArbol* n) {
        if (n == nullptr) return;
        limpiar(n->izq);
        limpiar(n->der);
        delete n;
    }

public:
    ArbolAlumnos() : raiz(nullptr), tam(0) {}
    ~ArbolAlumnos() { limpiar(); }
    ArbolAlumnos(const ArbolAlumnos&) = delete;
    ArbolAlumnos& operator=(const ArbolAlumnos&) = delete;

    // O(1)
    int getTam() const { return tam; }
    int altura() const { return alturaDe(raiz); }

    // O(log n) en todos los casos. Regresa false si el ID ya existia.
    bool insertar(Nodo* a) {
        bool insertado = false;
        raiz = insertar(raiz, a->dato.getId(), a, insertado);
        if (insertado) tam++;
        return insertado;
    }

    // Mejor O(1) (esta en la raiz), promedio y peor O(log n). Iterativa.
    Nodo* buscar(int id) const {
        NodoArbol* n = raiz;
        while (n != nullptr) {
            if (id == n->id) return n->nodoLista;
            n = (id < n->id) ? n->izq : n->der;
        }
        return nullptr;
    }

    // Igual que buscar: mejor O(1), promedio y peor O(log n)
    bool existe(int id) const { return buscar(id) != nullptr; }

    // O(log n) en todos los casos
    bool eliminar(int id) {
        bool eliminado = false;
        raiz = eliminar(raiz, id, eliminado);
        if (eliminado) tam--;
        return eliminado;
    }

    // O(n) en todos los casos
    template <typename Funcion>
    void recorrerInorden(Funcion f) const { inorden(raiz, f); }

    // O(n) en todos los casos
    void limpiar() {
        limpiar(raiz);
        raiz = nullptr;
        tam = 0;
    }
};

#endif // ARBOL_ALUMNOS_H
