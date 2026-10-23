#ifndef ALUMNO_H
#define ALUMNO_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm> // Para std::max

/*
 * Clase Alumno
 * Representa a un alumno y calcula su factor de riesgo de no terminar
 * el servicio social a tiempo.
 *
 * Todas las operaciones de esta clase son O(1) en mejor, promedio y peor caso:
 * no dependen del numero de alumnos, solo trabajan con los atributos propios.
 */
class Alumno {
private:
    int id;
    std::string nombre;
    std::string carrera;
    int semestre;
    int creditos;             // Cantidad de creditos (minimo 0)
    int horasServicioSocial;  // Minimo 0

public:
    static const int HORAS_REQUERIDAS = 480; 
    static const int SEMESTRES_CARRERA = 8;
    // Ritmo "normal": 480 h / 8 semestres = 60 h por semestre.
    // Un factor mayor a 60 significa que el alumno va atrasado.
    static constexpr double RITMO_NORMAL = 60.0;

    // Constructor por defecto - O(1)
    Alumno()
        : id(0), nombre(""), carrera(""), semestre(0), creditos(0), horasServicioSocial(0) {}

    // Constructor parametrizado - O(1)
    Alumno(int _id, std::string _nombre, std::string _carrera, int _semestre, int _creditos, int _horasServicio)
        : id(_id), nombre(_nombre), carrera(_carrera), semestre(_semestre) {
        setCreditos(_creditos);
        setHorasServicioSocial(_horasServicio);
    }

    // GETTERSO (1)
    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getCarrera() const { return carrera; }
    int getSemestre() const { return semestre; }
    int getCreditos() const { return creditos; }
    int getHorasServicioSocial() const { return horasServicioSocial; }

    // SETTERS O(1)
    void setId(int _id) { id = _id; }
    void setNombre(const std::string& _nombre) { nombre = _nombre; }
    void setCarrera(const std::string& _carrera) { carrera = _carrera; }
    void setSemestre(int _semestre) { semestre = _semestre; }
    void setCreditos(int _creditos) { creditos = std::max(0, _creditos); }
    void setHorasServicioSocial(int _horasServicio) { horasServicioSocial = std::max(0, _horasServicio); }

    // Convierte los datos a una linea lista para guardar en el CSV - O(1)
    std::string toCSV() const {
        std::stringstream ss;
        ss << id << "," << nombre << "," << carrera << ","
           << semestre << "," << creditos << "," << horasServicioSocial;
        return ss.str();
    }

    /*
     * Factor de riesgo = horas de servicio que le faltan por cada semestre restante.
     *   - Si ya completo las horas: 0
     *   - Si ya no le quedan semestres: todas las horas faltantes
     * Complejidad: O(1) en todos los casos (solo operaciones aritmeticas).
     */
    double calcularFactorRiesgoServicio() const {
        int horasFaltantes = HORAS_REQUERIDAS - horasServicioSocial;
        int semestresFaltantes = SEMESTRES_CARRERA - semestre;

        if (horasFaltantes <= 0) return 0.0;
        if (semestresFaltantes <= 0) return static_cast<double>(horasFaltantes);
        return static_cast<double>(horasFaltantes) / semestresFaltantes;
    }

    // Clasificacion legible del factor de riesgo - O(1)
    std::string nivelRiesgo() const {
        double f = calcularFactorRiesgoServicio();
        if (f == 0.0) return "Completo";
        if (f <= RITMO_NORMAL) return "Bajo";
        if (f <= 2 * RITMO_NORMAL) return "Medio";
        return "Alto";
    }

    // Imprime una fila de tabla - O(1)
    void mostrarInformacion() const {
        std::cout << std::left
                  << std::setw(8)  << id
                  << std::setw(12) << nombre
                  << std::setw(27) << carrera
                  << std::setw(5)  << semestre
                  << std::setw(9)  << creditos
                  << std::setw(9)  << horasServicioSocial
                  << std::setw(9)  << std::fixed << std::setprecision(2) << calcularFactorRiesgoServicio()
                  << nivelRiesgo() << "\n";
    }

    // Encabezado de la tabla - O(1)
    static void mostrarEncabezado() {
        std::cout << std::left
                  << std::setw(8)  << "ID"
                  << std::setw(12) << "Nombre"
                  << std::setw(27) << "Carrera"
                  << std::setw(5)  << "Sem"
                  << std::setw(9)  << "Creditos"
                  << std::setw(9)  << "Horas SS"
                  << std::setw(9)  << "Riesgo"
                  << "Nivel" << "\n"
                  << std::string(85, '-') << "\n";
    }
};

// --- Criterios de comparacion para el ordenamiento --- O(1)

inline bool mayorRiesgo(const Alumno& a, const Alumno& b) {
    double ra = a.calcularFactorRiesgoServicio();
    double rb = b.calcularFactorRiesgoServicio();
    if (ra != rb) return ra > rb;
    return a.getId() < b.getId();
}

inline bool menorId(const Alumno& a, const Alumno& b) {
    return a.getId() < b.getId();
}

#endif // ALUMNO_H
