#include <iostream>  // O(1)
#include <fstream>   // O(1)
#include <sstream>   // O(1)
#include <vector>    // O(1)
#include <string>    // O(1)
#include <algorithm> // O(1)
#include <iomanip>   // O(1)
#include "Alumno.h"  // O(1)

// O(n) - Complejidad lineal respecto al número de elementos a particionar
int particion(std::vector<Alumno>& lista, int bajo, int alto) {
    double pivote = lista[alto].calcularFactorRiesgoServicio(); // O(1)
    int i = bajo - 1;                                            // O(1)

    for (int j = bajo; j < alto; j++) {                          // O(n)
        if (lista[j].calcularFactorRiesgoServicio() > pivote) {  // O(1)
            i++;                                                 // O(1)
            std::swap(lista[i], lista[j]);                       // O(1)
        }
    }
    std::swap(lista[i + 1], lista[alto]);                        // O(1)
    return (i + 1);                                              // O(1)
}

// O(n log n) en el caso promedio / O(n^2) en el peor caso
void quickSort(std::vector<Alumno>& lista, int bajo, int alto) {
    if (bajo < alto) {                                           // O(1)
        int indicePivote = particion(lista, bajo, alto);         // O(n)

        quickSort(lista, bajo, indicePivote - 1);                // O(log n)
        quickSort(lista, indicePivote + 1, alto);                // O(log n)
    }
}

// O(n log n) - Ordenamiento de los n alumnos
void Sort(std::vector<Alumno>& lista) {
    if (!lista.empty()) {                                       // O(1)
        quickSort(lista, 0, lista.size() - 1);                  // O(n log n)
    }
}

// O(n log n) - Dominado por el proceso de ordenamiento
void AlumnosRiesgo(std::vector<Alumno>& lista) {

    Sort(lista);                                                // O(n log n)

    std::ofstream archivo("alumnosRiesgo.csv", std::ios::out);  // O(1)

    if (!archivo.is_open()) {                                   // O(1)
        std::cerr << "Error al crear el archivo alumnosRiesgo.csv" << std::endl; // O(1)
        return;                                                 // O(1)
    }

    archivo << "id,nombre,numero_riesgo\n";                     // O(1)

    for (const auto& alumno : lista) {                          // O(n)
        archivo << alumno.getId() << ","                        // O(1)
                << alumno.getNombre() << ","                    // O(1)
                << std::fixed << std::setprecision(2) << alumno.calcularFactorRiesgoServicio() << "\n"; // O(1)
    }

    archivo.close();                                            // O(1)
} 

// O(1) - Inserción de un registro
void CrearAlumno(int _id, const std::string& _name, const std::string& _carrera, int _semestre, int _creditos, int _servicio){
    Alumno alumno(_id, _name, _carrera, _semestre, _creditos, _servicio); // O(1)
    std::ofstream archivo("alumnos.csv", std::ios::app);                  // O(1)

    if (!archivo.is_open()) {                                             // O(1)
        std::cerr << "Error al abrir el archivo alumnos.csv" << std::endl;// O(1)
    }

    archivo << alumno.toCSV() << "\n";                                    // O(1)

    archivo.close();                                                      // O(1)
}

// O(n) - Recorre las n líneas del archivo CSV
std::vector<Alumno> CargarCSV() {
    std::vector<Alumno> lista;                                           // O(1)
    std::ifstream archivo("alumnos.csv");                                // O(1)

    if (!archivo.is_open()) {                                            // O(1)
        std::cerr << "Error al abrir el archivo alumnos.csv" << std::endl;// O(1)
        return lista;                                                    // O(1)
    }

    std::string linea;                                                   // O(1)
    while (std::getline(archivo, linea)) {                               // O(n)
        if (linea.empty()) continue;                                     // O(1)

        std::stringstream ss(linea);                                     // O(1)
        std::string sId, nombre, carrera, sSemestre, sCreditos, sServicio; // O(1)

        if (std::getline(ss, sId, ',') &&                                // O(1)
            std::getline(ss, nombre, ',') &&                             // O(1)
            std::getline(ss, carrera, ',') &&                            // O(1)
            std::getline(ss, sSemestre, ',') &&                          // O(1)
            std::getline(ss, sCreditos, ',') &&                          // O(1)
            std::getline(ss, sServicio, ',')) {                          // O(1)

            lista.emplace_back(                                          // O(1)
                std::stoi(sId),                                          // O(1)
                nombre,                                                  // O(1)
                carrera,                                                 // O(1)
                std::stoi(sSemestre),                                    // O(1)
                std::stoi(sCreditos),                                    // O(1)
                std::stoi(sServicio)                                     // O(1)
            );
        }
    }

    archivo.close();                                                     // O(1)
    return lista;                                                        // O(1)
}

// O(1) - Ejecución de instrucciones constantes
void CrearCSV(){
    CrearAlumno(12345, "Leo", "Ingenieria en Sistemas", 5, 180, 240);    // O(1)
    CrearAlumno(23456, "David", "Robotica", 2, 70, 80);                 // O(1)
    CrearAlumno(34567, "Carlo", "Datos", 7, 250, 440);                  // O(1)
    CrearAlumno(45678, "Ana", "Ingenieria Industrial", 6, 200, 100);    // O(1)
    CrearAlumno(56789, "Sofia", "Sistemas Computacionales", 8, 260, 400); // O(1)
    CrearAlumno(67890, "Mateo", "Mecatronica", 3, 90, 50);              // O(1)
    CrearAlumno(78901, "Valeria", "Biomedica", 7, 240, 120);            // O(1)
    CrearAlumno(89012, "Gabriel", "Datos", 4, 130, 200);                // O(1)
    CrearAlumno(90123, "Camila", "Robotica", 8, 270, 480);              // O(1)
    CrearAlumno(11223, "Santiago", "Ingenieria en Sistemas", 1, 30, 0); // O(1)
}

// O(n log n) - Complejidad general del programa
int main( int argc, char* argv[] )
{
    CrearCSV();                                                          // O(1)
    std::vector<Alumno> alumnos = CargarCSV();                           // O(n)
    AlumnosRiesgo(alumnos);                                              // O(n log n)

    //Menu (Por hacer)
}