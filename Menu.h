#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include "Alumno.h"
#include "Registro.h"
#include "Archivos.h"

class Menu {
private:
    Registro alumnos;

    static int leerEntero(const std::string& mensaje, int minimo, int maximo) {
        while (true) {
            std::cout << mensaje;
            std::string entrada;
            if (!std::getline(std::cin, entrada)) return minimo;
            if (!entrada.empty() && entrada.back() == '\r') entrada.pop_back();
            int valor;
            if (aEntero(entrada, valor) && valor >= minimo && valor <= maximo) return valor;
            std::cout << "  Valor invalido. Escribe un numero entre "
                      << minimo << " y " << maximo << ".\n";
        }
    }

    static std::string leerTexto(const std::string& mensaje) {
        while (true) {
            std::cout << mensaje;
            std::string entrada;
            if (!std::getline(std::cin, entrada)) return "";
            if (!entrada.empty() && entrada.back() == '\r') entrada.pop_back();
            if (entrada.empty()) {
                std::cout << "  No puede estar vacio.\n";
            } else if (entrada.find(',') != std::string::npos) {
                std::cout << "  No uses comas.\n";
            } else {
                return entrada;
            }
        }
    }

    static void pausa() {
        std::cout << "\nPresiona ENTER para continuar...";
        std::string tmp;
        std::getline(std::cin, tmp);
    }

    // 1. Limpiar O(n) + cargar O(n log n). Total: mejor O(1), promedio y peor O(n log n).
    void opcionCargar() {
        alumnos.limpiar();
        int cargados = cargarAlumnos(alumnos);
        if (cargados < 0) std::cout << "No se encontro " << ARCHIVO_ALUMNOS << ".\n";
        else std::cout << "Se cargaron " << cargados << " alumno(s) desde " << ARCHIVO_ALUMNOS << ".\n";
    }

    // 2. O(n)
    void opcionMostrar() const {
        alumnos.mostrar();
    }

    // 3. Validar ID en el arbol O(log n) + agregar O(log n) + escribir en archivo O(1).
    //    Total: O(log n) en todos los casos.
    void opcionAgregar() {
        int id = leerEntero("ID (5 digitos): ", 10000, 99999);
        if (alumnos.existe(id)) {
            std::cout << "Ya existe un alumno con el ID " << id << ".\n";
            return;
        }
        std::string nombre  = leerTexto("Nombre: ");
        std::string carrera = leerTexto("Carrera: ");
        int semestre = leerEntero("Semestre (1-12): ", 1, 12);
        int creditos = leerEntero("Creditos (0-400): ", 0, 400);
        int horas    = leerEntero("Horas de servicio social (0-480): ", 0, Alumno::HORAS_REQUERIDAS);

        Alumno nuevo(id, nombre, carrera, semestre, creditos, horas);
        alumnos.agregar(nuevo);

        if (agregarAlumnoArchivo(nuevo))
            std::cout << "Alumno agregado y guardado en " << ARCHIVO_ALUMNOS << ".\n";
        else
            std::cout << "Alumno agregado en memoria, pero no se pudo escribir en el archivo.\n";
        Alumno::mostrarEncabezado();
        nuevo.mostrarInformacion();
    }

    // 4. Busqueda en el arbol AVL: mejor O(1), promedio y peor O(log n)
    void opcionBuscar() const {
        int id = leerEntero("ID a buscar: ", 0, 99999);
        const Alumno* a = alumnos.buscar(id);
        if (a == nullptr) {
            std::cout << "No se encontro un alumno con el ID " << id << ".\n";
            return;
        }
        Alumno::mostrarEncabezado();
        a->mostrarInformacion();
        std::cout << "Le faltan " << std::max(0, Alumno::HORAS_REQUERIDAS - a->getHorasServicioSocial())
                  << " hora(s) de servicio social.\n";
    }

    // 5. Eliminar O(log n) + reescribir el archivo O(n). Total: O(n) en todos los casos.
    void opcionEliminar() {
        int id = leerEntero("ID a eliminar: ", 0, 99999);
        if (!alumnos.eliminar(id)) {
            std::cout << "No se encontro un alumno con el ID " << id << ".\n";
            return;
        }
        if (guardarAlumnos(alumnos) >= 0)
            std::cout << "Alumno eliminado y " << ARCHIVO_ALUMNOS << " actualizado.\n";
        else
            std::cout << "Alumno eliminado en memoria, pero no se pudo actualizar el archivo.\n";
    }

    // 6. Merge Sort O(n log n) + mostrar O(n). Total: O(n log n) en todos los casos.
    void opcionOrdenarRiesgo() {
        alumnos.ordenarPorRiesgo();
        std::cout << "Alumnos ordenados de MAYOR a MENOR riesgo:\n\n";
        alumnos.mostrar();
    }

    // 7. Recorrido inorden del arbol: O(n) en todos los casos (no se ordena nada).
    void opcionMostrarPorId() const {
        std::cout << "Alumnos por ID (recorrido inorden del arbol AVL):\n\n";
        alumnos.mostrarPorId();
        std::cout << "Altura del arbol AVL: " << alumnos.alturaArbol() << "\n";
    }

    // 8. Ordenar si hace falta O(n log n) + filtrar (mejor O(1), peor O(n)).
    //    Total: O(n log n) si hay que ordenar; si ya estaba ordenada, mejor O(1) y peor O(n).
    void opcionEnRiesgo() {
        std::cout << "El ritmo normal es " << static_cast<int>(Alumno::RITMO_NORMAL)
                  << " horas por semestre (480 h / 8 semestres).\n";
        int umbral = leerEntero("Umbral de riesgo (sugerido 60): ", 0, 480);
        std::cout << "\n";
        int encontrados = alumnos.mostrarEnRiesgo(umbral);
        if (encontrados == 0) std::cout << "Ningun alumno supera un riesgo de " << umbral << ".\n";
        else std::cout << encontrados << " alumno(s) en riesgo.\n";
    }

    // 9. Ordenar O(n log n) + escribir O(n). Total: O(n log n) en todos los casos.
    void opcionGenerarReporte() {
        if (alumnos.estaVacia()) {
            std::cout << "No hay alumnos. Carga el archivo primero (opcion 1).\n";
            return;
        }
        int escritos = generarReporteRiesgo(alumnos);
        if (escritos >= 0)
            std::cout << "Reporte generado en " << ARCHIVO_RIESGO << " (" << escritos << " alumnos).\n";
        else
            std::cout << "No se pudo crear " << ARCHIVO_RIESGO << ".\n";
    }

    // 10. O(n)
    void opcionLeerReporte() const {
        int registros = mostrarReporteRiesgo();
        if (registros < 0) std::cout << "No existe " << ARCHIVO_RIESGO << ". Generalo con la opcion 9.\n";
        else std::cout << registros << " registro(s) leidos de " << ARCHIVO_RIESGO << ".\n";
    }

    // 11. Recorrido inorden + escritura: O(n) en todos los casos.
    void opcionGuardar() const {
        int escritos = guardarAlumnos(alumnos);
        if (escritos >= 0)
            std::cout << "Se guardaron " << escritos << " alumno(s) en " << ARCHIVO_ALUMNOS
                      << ", ordenados por ID.\n";
        else
            std::cout << "No se pudo guardar " << ARCHIVO_ALUMNOS << ". El archivo anterior no se modifico.\n";
    }

    static void imprimirMenu(int tam) {
        std::cout << "\n"
                  << "   RIESGO DE SERVICIO SOCIAL  (" << tam << " alumnos en memoria)\n"
                  << "  1. Cargar alumnos desde archivo\n"
                  << "  2. Mostrar todos los alumnos\n"
                  << "  3. Agregar alumno\n"
                  << "  4. Buscar alumno por ID\n"
                  << "  5. Eliminar alumno por ID\n"
                  << "  6. Ordenar y mostrar por riesgo (mayor a menor)\n"
                  << "  7. Mostrar por ID\n"
                  << "  8. Ver alumnos en riesgo\n"
                  << "  9. Generar reporte alumnosRiesgo.csv\n"
                  << " 10. Leer reporte alumnosRiesgo.csv\n"
                  << " 11. Guardar alumnos en alumnos.csv\n"
                  << "  0. Salir\n";
    }

public:
    void ejecutar() {
        opcionCargar();

        int opcion;
        do {
            imprimirMenu(alumnos.getTam());
            opcion = leerEntero("Elige una opcion: ", 0, 11);
            std::cout << "\n";
            switch (opcion) {
                case 1:  opcionCargar();          break;
                case 2:  opcionMostrar();         break;
                case 3:  opcionAgregar();         break;
                case 4:  opcionBuscar();          break;
                case 5:  opcionEliminar();        break;
                case 6:  opcionOrdenarRiesgo();   break;
                case 7:  opcionMostrarPorId();    break;
                case 8:  opcionEnRiesgo();        break;
                case 9:  opcionGenerarReporte();  break;
                case 10: opcionLeerReporte();     break;
                case 11: opcionGuardar();         break;
                case 0:  break;
            }
            if (opcion != 0 && std::cin.good()) pausa();
        } while (opcion != 0 && std::cin.good());
    }
};

#endif // MENU_H
