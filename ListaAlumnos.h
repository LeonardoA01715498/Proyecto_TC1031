#ifndef LISTA_ALUMNOS_H
#define LISTA_ALUMNOS_H

#include "Alumno.h"

/*
 * ListaAlumnos: lista doblemente ligada de alumnos.
 *
 * Por que una lista doblemente ligada?:
 *   - El registro de alumnos es una secuencia que crece (se agregan alumnos nuevos
 *     desde el menu o desde el archivo). Insertar al final es O(1) siempre, sin las
 *     realocaciones que hace std::vector al crecer.
 *   - Eliminar un alumno, una vez localizado por el arbol AVL, es O(1): solo se
 *     reconectan punteros, no hay que desplazar elementos como en un arreglo.
 *   - El uso principal del programa es RECORRER a todos los alumnos (mostrar, filtrar
 *     los que estan en riesgo, generar el reporte), y eso es O(n) en cualquier estructura.
 *   - Se puede ordenar con Merge Sort sin memoria extra (solo se mueven punteros),
 *     garantizando O(n log n) en todos los casos.
 *
 * Las busquedas por ID no se hacen aqui sino en el arbol AVL (ArbolAlumnos.h).
 *
 * Complejidad de cada operacion (n = numero de alumnos en la lista):
 *
 *   Operacion            Mejor caso   Caso promedio   Peor caso
 *   agregarFinal         O(1)         O(1)            O(1)
 *   eliminarNodo         O(1)         O(1)            O(1)
 *   ordenar (MergeSort)  O(n log n)   O(n log n)      O(n log n)
 *   mostrar / recorrer   O(n)         O(n)            O(n)
 *   mostrarEnRiesgo      O(1)         O(n)            O(n)
 *   limpiar / destructor O(n)         O(n)            O(n)
 *   getTam / estaVacia   O(1)         O(1)            O(1)
 */

struct Nodo {
    Alumno dato;
    Nodo* sig;
    Nodo* ant;
    Nodo(const Alumno& a) : dato(a), sig(nullptr), ant(nullptr) {}
};

class ListaAlumnos {
private:
    Nodo* cabeza;
    Nodo* cola;
    int tam;

    typedef bool (*Comparador)(const Alumno&, const Alumno&);

    /*
     * Parte la lista que empieza en 'inicio' en dos mitades usando la tecnica
     * del apuntador lento / rapido. Regresa el inicio de la segunda mitad.
     * Mejor, promedio y peor caso: O(n) (siempre recorre la sublista completa).
     */
    static Nodo* dividir(Nodo* inicio) {
        Nodo* lento = inicio;
        Nodo* rapido = inicio->sig;
        while (rapido != nullptr && rapido->sig != nullptr) {   // O(n/2)
            lento = lento->sig;
            rapido = rapido->sig->sig;
        }
        Nodo* segunda = lento->sig;
        lento->sig = nullptr;                                     
        return segunda;
    }

    /*
     * Mezcla dos sublistas ya ordenadas en una sola (solo usa 'sig').
     * Es estable: en empate se toma primero el elemento de la izquierda.
     * Mejor caso: O(min(a, b)) cuando una sublista se agota pronto.
     * Promedio y peor caso: O(a + b).
     * Se hace iterativo para no usar recursion de profundidad n.
     */
    static Nodo* mezclar(Nodo* a, Nodo* b, Comparador cmp) {
        Alumno vacio;
        Nodo temporal(vacio);
        Nodo* fin = &temporal;
        while (a != nullptr && b != nullptr) {
            if (cmp(b->dato, a->dato)) { fin->sig = b; b = b->sig; }
            else                       { fin->sig = a; a = a->sig; }
            fin = fin->sig;
        }
        fin->sig = (a != nullptr) ? a : b;
        return temporal.sig;
    }

    /*
     * Merge Sort recursivo sobre la lista ligada.
     * O(n log n) en mejor, promedio y peor caso.
     * Profundidad de recursion: O(log n). Memoria extra: O(log n) (pila de llamadas).
     */
    static Nodo* mergeSort(Nodo* inicio, Comparador cmp) {
        if (inicio == nullptr || inicio->sig == nullptr) return inicio;  // caso base O(1)
        Nodo* segunda = dividir(inicio);                                 // O(n)
        Nodo* izq = mergeSort(inicio, cmp);                              // T(n/2)
        Nodo* der = mergeSort(segunda, cmp);                             // T(n/2)
        return mezclar(izq, der, cmp);                                   // O(n)
    }

public:
    // O(1)
    ListaAlumnos() : cabeza(nullptr), cola(nullptr), tam(0) {}

    // O(n): libera cada nodo
    ~ListaAlumnos() { limpiar(); }

    ListaAlumnos(const ListaAlumnos&) = delete;
    ListaAlumnos& operator=(const ListaAlumnos&) = delete;

    // O(1) en todos los casos
    int getTam() const { return tam; }
    bool estaVacia() const { return tam == 0; }

    // Inserta al final usando el apuntador a la cola: O(1) en todos los casos.
    // Regresa el nodo creado para que el arbol AVL lo pueda indexar.
    Nodo* agregarFinal(const Alumno& a) {
        Nodo* nuevo = new Nodo(a);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            cola->sig = nuevo;
            nuevo->ant = cola;
            cola = nuevo;
        }
        tam++;
        return nuevo;
    }

    /*
     * Quita de la lista un nodo que ya se conoce (lo encuentra el arbol AVL).
     * Mejor, promedio y peor caso: O(1), solo se reconectan los apuntadores
     * 'ant' y 'sig' de sus vecinos; no hay que recorrer la lista.
     */
    void eliminarNodo(Nodo* nodo) {
        if (nodo->ant != nullptr) nodo->ant->sig = nodo->sig;
        else                      cabeza = nodo->sig;
        if (nodo->sig != nullptr) nodo->sig->ant = nodo->ant;
        else                      cola = nodo->ant;
        delete nodo;
        tam--;
    }

    /*
     * Ordena la lista con Merge Sort segun el criterio recibido.
     * Mejor, promedio y peor caso: O(n log n) para el ordenamiento
     * + O(n) para reconstruir los apuntadores 'ant' y la cola  =>  O(n log n).
     */
    void ordenar(Comparador cmp) {
        cabeza = mergeSort(cabeza, cmp);
        // Reconstruye los enlaces hacia atras y la cola: O(n)
        Nodo* anterior = nullptr;
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            actual->ant = anterior;
            anterior = actual;
            actual = actual->sig;
        }
        cola = anterior;
    }

    // Imprime todos los alumnos: O(n) en todos los casos
    void mostrar() const {
        if (estaVacia()) {
            std::cout << "La lista esta vacia. Carga el archivo o agrega alumnos.\n";
            return;
        }
        Alumno::mostrarEncabezado();
        for (Nodo* actual = cabeza; actual != nullptr; actual = actual->sig) {
            actual->dato.mostrarInformacion();
        }
        std::cout << "Total: " << tam << " alumno(s)\n";
    }

    /*
     * Imprime solo a los alumnos cuyo factor de riesgo es mayor que 'umbral'.
     * Si la lista no esta ordenada por riesgo: O(n) en todos los casos.
     * Si ya esta ordenada, se corta el recorrido en el primer alumno que no
     * supera el umbral: mejor caso O(1) (el primero ya no lo supera),
     * promedio y peor caso O(n) (todos lo superan).
     */
    int mostrarEnRiesgo(double umbral, bool ordenadaPorRiesgo) const {
        int encontrados = 0;
        for (Nodo* actual = cabeza; actual != nullptr; actual = actual->sig) {
            if (actual->dato.calcularFactorRiesgoServicio() > umbral) {
                if (encontrados == 0) Alumno::mostrarEncabezado();
                actual->dato.mostrarInformacion();
                encontrados++;
            } else if (ordenadaPorRiesgo) {
                break;
            }
        }
        return encontrados;
    }

    /*
     * Aplica una funcion a cada alumno, del inicio al final.
     * O(n) * costo de la funcion. Se usa para escribir archivos.
     */
    template <typename Funcion>
    void recorrer(Funcion f) const {
        for (Nodo* actual = cabeza; actual != nullptr; actual = actual->sig) {
            f(actual->dato);
        }
    }

    // Libera todos los nodos: O(n) en todos los casos
    void limpiar() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* siguiente = actual->sig;
            delete actual;
            actual = siguiente;
        }
        cabeza = cola = nullptr;
        tam = 0;
    }
};

#endif // LISTA_ALUMNOS_H
