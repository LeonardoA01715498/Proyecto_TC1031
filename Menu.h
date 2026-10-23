#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <limits>
#include "Alumno.h"
#include "ListaAlumnos.h"
#include "Archivos.h"

class Menu {
private:
    ListaAlumnos alumnos;
    bool ordenadaPorRiesgo;

    static int leerEntero(const std::string& mensaje, int minimo, int maximo) {
        while (true) {
            std::cout << mensaje;
            std::string entrada;
            if (!std::getline(std::cin, entrada)) return minimo;
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

    // 1. O(n^2) peor caso por la verificacion de duplicados
    void opcionCargar() {
        alumnos.limpiar();                       // O(n): evita duplicar datos al recargar
        int cargados = cargarAlumnos(alumnos);
        ordenadaPorRiesgo = false;
        if (cargados < 0) std::cout << "No se encontro " << ARCHIVO_ALUMNOS << ".\n";
        else std::cout << "Se cargaron " << cargados << " alumno(s) desde " << ARCHIVO_ALUMNOS << ".\n";
    }

    // 2. O(n)
    void opcionMostrar() const {
        alumnos.mostrar();
    }

    // 3. Verificar ID: O(n) peor caso. Insertar en lista: O(1). Escribir en archivo: O(1).
    //    Total: mejor O(1), promedio y peor O(n).
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
        alumnos.agregarFinal(nuevo);
        ordenadaPorRiesgo = false;

        if (agregarAlumnoArchivo(nuevo))
            std::cout << "Alumno agregado a la lista y guardado en " << ARCHIVO_ALUMNOS << ".\n";
        else
            std::cout << "Alumno agregado a la lista, pero no se pudo escribir en el archivo.\n";
        Alumno::mostrarEncabezado();
        nuevo.mostrarInformacion();
    }

    // 4. Mejor O(1), promedio y peor O(n)
    void opcionBuscar() const {
        int id = leerEntero("ID a buscar: ", 0, 99999);
        Alumno* a = alumnos.buscarPorId(id);
        if (a == nullptr) {
            std::cout << "No se encontro un alumno con el ID " << id << ".\n";
            return;
        }
        Alumno::mostrarEncabezado();
        a->mostrarInformacion();
        std::cout << "Le faltan " << std::max(0, Alumno::HORAS_REQUERIDAS - a->getHorasServicioSocial())
                  << " hora(s) de servicio social.\n";
    }

    // 5. Eliminar de la lista: mejor O(1), peor O(n). Reescribir archivo: O(n). Total: O(n).
    void opcionEliminar() {
        int id = leerEntero("ID a eliminar: ", 0, 99999);
        if (!alumnos.eliminarPorId(id)) {
            std::cout << "No se encontro un alumno con el ID " << id << ".\n";
            return;
        }
        if (guardarAlumnos(alumnos))
            std::cout << "Alumno eliminado y " << ARCHIVO_ALUMNOS << " actualizado.\n";
        else
            std::cout << "Alumno eliminado de la lista, pero no se pudo actualizar el archivo.\n";
    }

    // 6. Ordenar: O(n log n) en todos los casos. Mostrar: O(n). Total: O(n log n).
    void opcionOrdenarRiesgo() {
        alumnos.ordenar(mayorRiesgo);
        ordenadaPorRiesgo = true;
        std::cout << "Alumnos ordenados de MAYOR a MENOR riesgo:\n\n";
        alumnos.mostrar();
    }

    // 7. Ordenar: O(n log n). Mostrar: O(n). Total: O(n log n).
    void opcionOrdenarId() {
        alumnos.ordenar(menorId);
        ordenadaPorRiesgo = false;
        std::cout << "Alumnos ordenados por ID:\n\n";
        alumnos.mostrar();
    }

    // 8. Ordenar (si hace falta): O(n log n). Filtrar: mejor O(1), peor O(n). Total: O(n log n).
    void opcionEnRiesgo() {
        std::cout << "El ritmo normal es " << static_cast<int>(Alumno::RITMO_NORMAL)
                  << " horas por semestre (480 h / 8 semestres).\n";
        int umbral = leerEntero("Umbral de riesgo (sugerido 60): ", 0, 480);
        if (!ordenadaPorRiesgo) {
            alumnos.ordenar(mayorRiesgo);
            ordenadaPorRiesgo = true;
        }
        std::cout << "\n";
        int encontrados = alumnos.mostrarEnRiesgo(umbral, true);
        if (encontrados == 0) std::cout << "Ningun alumno supera un riesgo de " << umbral << ".\n";
        else std::cout << encontrados << " alumno(s) en riesgo.\n";
    }

    // 9. Ordenar: O(n log n). Escribir: O(n). Total: O(n log n).
    void opcionGenerarReporte() {
        if (alumnos.estaVacia()) {
            std::cout << "La lista esta vacia. Carga el archivo primero (opcion 1).\n";
            return;
        }
        alumnos.ordenar(mayorRiesgo);
        ordenadaPorRiesgo = true;
        if (generarReporteRiesgo(alumnos))
            std::cout << "Reporte generado en " << ARCHIVO_RIESGO << " (" << alumnos.getTam() << " alumnos).\n";
        else
            std::cout << "No se pudo crear " << ARCHIVO_RIESGO << ".\n";
    }

    // 10. O(n)
    void opcionLeerReporte() const {
        int registros = mostrarReporteRiesgo();
        if (registros < 0) std::cout << "No existe " << ARCHIVO_RIESGO << ". Generalo con la opcion 9.\n";
        else std::cout << registros << " registro(s) leidos de " << ARCHIVO_RIESGO << ".\n";
    }

    static void imprimirMenu(int tam) {
        std::cout 
                  << "   RIESGO DE SERVICIO SOCIAL  (" << tam << " alumnos en memoria)\n"
                  << "  1. Cargar alumnos desde archivo\n"
                  << "  2. Mostrar todos los alumnos\n"
                  << "  3. Agregar alumno\n"
                  << "  4. Buscar alumno por ID\n"
                  << "  5. Eliminar alumno por ID\n"
                  << "  6. Ordenar y mostrar por riesgo (mayor a menor)\n"
                  << "  7. Ordenar y mostrar por ID\n"
                  << "  8. Ver alumnos en riesgo\n"
                  << "  9. Generar reporte alumnosRiesgo.csv\n"
                  << " 10. Leer reporte alumnosRiesgo.csv\n"
                  << "  0. Salir\n"
    }

public:
    Menu() : ordenadaPorRiesgo(false) {}

    void ejecutar() {
        // Se cargan los datos al iniciar para que el usuario pueda trabajar de inmediato
        opcionCargar();

        int opcion;
        do {
            imprimirMenu(alumnos.getTam());
            opcion = leerEntero("Elige una opcion: ", 0, 10);
            std::cout << "\n";
            switch (opcion) {
                case 1:  opcionCargar();          break;
                case 2:  opcionMostrar();         break;
                case 3:  opcionAgregar();         break;
                case 4:  opcionBuscar();          break;
                case 5:  opcionEliminar();        break;
                case 6:  opcionOrdenarRiesgo();   break;
                case 7:  opcionOrdenarId();       break;
                case 8:  opcionEnRiesgo();        break;
                case 9:  opcionGenerarReporte();  break;
                case 10: opcionLeerReporte();     break;
                case 0: break;
            }
            if (opcion != 0 && std::cin.good()) pausa();
        } while (opcion != 0 && std::cin.good());
    }
};

#endif // MENU_H
