#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include "Alumno.h"
#include "Registro.h"

/*
 * Manejo de archivos del proyecto.
 *   alumnos.csv        -> base de datos de alumnos (se lee, se le agregan y se reescribe
 *                         ordenado por ID)
 *   alumnosRiesgo.csv  -> reporte generado, ordenado de mayor a menor riesgo
 *
 * Escritura segura: los archivos que se reescriben completos se escriben primero
 * en un archivo temporal (.tmp). Solo si toda la escritura fue correcta, el
 * temporal reemplaza al original. Asi, si algo falla a la mitad, el archivo
 * original no se pierde ni queda a medias.
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

// Reemplaza 'destino' por 'temporal' - O(1)
inline bool reemplazarArchivo(const std::string& temporal, const std::string& destino) {
    std::remove(destino.c_str());                 // en Windows rename falla si el destino existe
    return std::rename(temporal.c_str(), destino.c_str()) == 0;
}

/*
 * Lee alumnos.csv y carga cada registro valido a la lista y al arbol AVL.
 * - Ignora el encabezado y lineas mal formadas.
 * - No inserta IDs repetidos (lo valida el arbol).
 *
 * Complejidad:
 *   Leer y parsear cada linea: O(1).
 *   Registro::agregar: O(log k), con k = alumnos ya cargados (validar e insertar en el AVL).
 *   Mejor caso (archivo vacio o solo encabezado): O(1).
 *   Promedio y peor caso: log 1 + log 2 + ... + log n = O(n log n).
 *
 * Regresa cuantos alumnos se cargaron, o -1 si no se pudo abrir el archivo.
 */
inline int cargarAlumnos(Registro& registro) {
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
        if (registro.agregar(Alumno(id, nombre, carrera, sem, cred, serv))) cargados++;
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
    archivo.close();
    return !archivo.fail();                      // confirma que se escribio bien
}

/*
 * Reescribe alumnos.csv con todos los alumnos en memoria, ordenados por ID
 * (recorrido inorden del arbol AVL, no hace falta ordenar).
 * Se usa despues de eliminar un alumno y en la opcion "Guardar".
 * Mejor, promedio y peor caso: O(n).
 * Regresa cuantos alumnos se guardaron, o -1 si hubo un error.
 */
inline int guardarAlumnos(const Registro& registro) {
    const std::string temporal = ARCHIVO_ALUMNOS + ".tmp";
    std::ofstream archivo(temporal, std::ios::out);
    if (!archivo.is_open()) return -1;
    int escritos = 0;
    archivo << ENCABEZADO_ALUMNOS << "\n";
    registro.recorrerPorId([&archivo, &escritos](const Alumno& a) {
        archivo << a.toCSV() << "\n";
        escritos++;
    });
    archivo.close();
    if (archivo.fail()) { std::remove(temporal.c_str()); return -1; }
    if (!reemplazarArchivo(temporal, ARCHIVO_ALUMNOS)) return -1;
    return escritos;
}

/*
 * Escribe alumnosRiesgo.csv con id, nombre, factor y nivel de riesgo.
 * Primero ordena la lista por riesgo con Merge Sort: O(n log n).
 * Escribir: O(n). Total: O(n log n) en todos los casos.
 * Regresa cuantos alumnos se escribieron, o -1 si hubo un error.
 */
inline int generarReporteRiesgo(Registro& registro) {
    registro.ordenarPorRiesgo();
    const std::string temporal = ARCHIVO_RIESGO + ".tmp";
    std::ofstream archivo(temporal, std::ios::out);
    if (!archivo.is_open()) return -1;
    int escritos = 0;
    archivo << "id,nombre,numero_riesgo,nivel\n";
    registro.recorrer([&archivo, &escritos](const Alumno& a) {
        archivo << a.getId() << "," << a.getNombre() << ","
                << std::fixed << std::setprecision(2) << a.calcularFactorRiesgoServicio() << ","
                << a.nivelRiesgo() << "\n";
        escritos++;
    });
    archivo.close();
    if (archivo.fail()) { std::remove(temporal.c_str()); return -1; }
    if (!reemplazarArchivo(temporal, ARCHIVO_RIESGO)) return -1;
    return escritos;
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
