#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Alumno.h"
#include "ListaAlumnos.h"

/*
 * Manejo de archivos del proyecto.
 *   alumnos.csv        -> base de datos de alumnos (se lee, se le agregan y se reescribe)
 *   alumnosRiesgo.csv  -> reporte generado, ordenado de mayor a menor riesgo
 *
 * n = numero de lineas / alumnos.
 */

const std::string ARCHIVO_ALUMNOS = "alumnos.csv";
const std::string ARCHIVO_RIESGO  = "alumnosRiesgo.csv";
const std::string ENCABEZADO_ALUMNOS = "id,nombre,carrera,semestre,creditos,horas_servicio";

// Intenta convertir texto a entero sin lanzar excepciones - O(1) (campos de longitud acotada)
inline bool aEntero(const std::string& texto, int& valor) {
    std::stringstream ss(texto);
    ss >> valor;
    return !ss.fail() && ss.eof();
}

/*
 * Lee alumnos.csv y carga cada registro valido al final de la lista.
 * - Ignora el encabezado y lineas mal formadas.
 * - No inserta IDs repetidos (verifica con existe()).
 *
 * Complejidad:
 *   Leer y parsear cada linea: O(1). Insertar al final: O(1).
 *   Verificar duplicado: O(k) con k = alumnos ya cargados.
 *   Mejor caso (archivo vacio o solo encabezado): O(1).
 *   Promedio y peor caso: 1 + 2 + ... + n = O(n^2) por la verificacion de duplicados.
 *   (Si se omitiera la verificacion, la carga seria O(n). Para el tamano de un grupo
 *    o una generacion, esto es aceptable y protege la integridad de los datos.)
 *
 * Regresa cuantos alumnos se cargaron, o -1 si no se pudo abrir el archivo.
 */
inline int cargarAlumnos(ListaAlumnos& lista) {
    std::ifstream archivo(ARCHIVO_ALUMNOS);
    if (!archivo.is_open()) return -1;

    int cargados = 0;
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();  
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string sId, nombre, carrera, sSem, sCred, sServ;
        if (!(std::getline(ss, sId, ',') && std::getline(ss, nombre, ',') &&
              std::getline(ss, carrera, ',') && std::getline(ss, sSem, ',') &&
              std::getline(ss, sCred, ',') && std::getline(ss, sServ, ','))) {
            continue; 
        }

        int id, sem, cred, serv;
        if (!aEntero(sId, id) || !aEntero(sSem, sem) ||
            !aEntero(sCred, cred) || !aEntero(sServ, serv)) {
            continue; 
        }
        if (lista.existe(id)) continue;

        lista.agregarFinal(Alumno(id, nombre, carrera, sem, cred, serv));
        cargados++;
    }
    return cargados;
}

/*
 * Agrega un solo alumno al final de alumnos.csv (modo append).
 * Mejor, promedio y peor caso: O(1) (no se reescribe el archivo).
 */
inline bool agregarAlumnoArchivo(const Alumno& a) {
    bool existia = std::ifstream(ARCHIVO_ALUMNOS).good();
    std::ofstream archivo(ARCHIVO_ALUMNOS, std::ios::app);
    if (!archivo.is_open()) return false;
    if (!existia) archivo << ENCABEZADO_ALUMNOS << "\n";
    archivo << a.toCSV() << "\n";
    return true;
}

/*
 * Reescribe alumnos.csv con el contenido actual de la lista
 * (se usa despues de eliminar un alumno).
 * Mejor, promedio y peor caso: O(n).
 */
inline bool guardarAlumnos(const ListaAlumnos& lista) {
    std::ofstream archivo(ARCHIVO_ALUMNOS, std::ios::out);
    if (!archivo.is_open()) return false;
    archivo << ENCABEZADO_ALUMNOS << "\n";
    lista.recorrer([&archivo](const Alumno& a) { archivo << a.toCSV() << "\n"; });
    return true;
}

/*
 * Escribe alumnosRiesgo.csv con id, nombre, factor y nivel de riesgo.
 * Se asume que la lista ya fue ordenada por riesgo (lo hace el menu antes de llamar).
 * Mejor, promedio y peor caso: O(n).
 */
inline bool generarReporteRiesgo(const ListaAlumnos& lista) {
    std::ofstream archivo(ARCHIVO_RIESGO, std::ios::out);
    if (!archivo.is_open()) return false;
    archivo << "id,nombre,numero_riesgo,nivel\n";
    lista.recorrer([&archivo](const Alumno& a) {
        archivo << a.getId() << "," << a.getNombre() << ","
                << std::fixed << std::setprecision(2) << a.calcularFactorRiesgoServicio() << ","
                << a.nivelRiesgo() << "\n";
    });
    return true;
}

/*
 * Lee alumnosRiesgo.csv e imprime su contenido en forma de tabla.
 * Mejor caso: O(1) (archivo vacio). Promedio y peor caso: O(n).
 * Regresa el numero de registros leidos, o -1 si el archivo no existe.
 */
inline int mostrarReporteRiesgo() {
    std::ifstream archivo(ARCHIVO_RIESGO);
    if (!archivo.is_open()) return -1;

    std::string linea;
    int registros = 0;
    bool primera = true;
    while (std::getline(archivo, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string id, nombre, riesgo, nivel;
        std::getline(ss, id, ',');
        std::getline(ss, nombre, ',');
        std::getline(ss, riesgo, ',');
        std::getline(ss, nivel, ',');

        std::cout << std::left << std::setw(8) << id << std::setw(14) << nombre
                  << std::setw(16) << riesgo << nivel << "\n";
        if (primera) { std::cout << std::string(46, '-') << "\n"; primera = false; }
        else registros++;
    }
    return registros;
}

#endif // ARCHIVOS_H
