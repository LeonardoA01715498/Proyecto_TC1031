#ifndef ALUMNO_H
#define ALUMNO_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> // Para std::max

class Alumno {
private:
    int id;
    std::string nombre;
    std::string carrera;
    int semestre;
    int creditos;             // Cantidad de créditos (mínimo 0)
    int horasServicioSocial;  // Mínimo 0

public:
    // Constructor por defecto
    Alumno() 
        : id(0), nombre(""), carrera(""), semestre(0), creditos(0), horasServicioSocial(0) {}

    // Constructor parametrizado
    Alumno(int _id, std::string _nombre, std::string _carrera, int _semestre, int _creditos, int _horasServicio)
        : id(_id), nombre(_nombre), carrera(_carrera), semestre(_semestre) {
        setCreditos(_creditos);
        setHorasServicioSocial(_horasServicio);
    }

    // --- GETTERS ---
    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getCarrera() const { return carrera; }
    int getSemestre() const { return semestre; }
    int getCreditos() const { return creditos; }
    int getHorasServicioSocial() const { return horasServicioSocial; }

    // --- SETTERS ---
    void setId(int _id) { id = _id; }
    void setNombre(const std::string& _nombre) { nombre = _nombre; }
    void setCarrera(const std::string& _carrera) { carrera = _carrera; }
    void setSemestre(int _semestre) { semestre = _semestre; }

    void setCreditos(int _creditos) { 
        creditos = std::max(0, _creditos); 
    }

    void setHorasServicioSocial(int _horasServicio) { 
        horasServicioSocial = std::max(0, _horasServicio); 
    }

    // Convertir los datos a una línea lista para guardar en el CSV
    std::string toCSV() const {
        std::stringstream ss;
        ss << id << "," << nombre << "," << carrera << "," 
           << semestre << "," << creditos << "," << horasServicioSocial;
        return ss.str();
    }

    void mostrarInformacion() const {
        std::cout << "ID: " << id 
                  << " | Nombre: " << nombre 
                  << " | Carrera: " << carrera 
                  << " | Semestre: " << semestre 
                  << " | Créditos: " << creditos 
                  << " | Horas Servicio Social: " << horasServicioSocial << "\n";
    }

double calcularFactorRiesgoServicio(int horasTotalesRequeridas = 480, int semestresTotalesCarrera = 8) const {
    int horasFaltantes = horasTotalesRequeridas - horasServicioSocial;
    int semestresFaltantes = semestresTotalesCarrera - semestre;

    // Si ya completó las horas necesarias, el riesgo es 0
    if (horasFaltantes <= 0) return 0.0;

    // Si está en el último semestre o superior y le faltan horas, el riesgo es máximo (las horas restantes)
    if (semestresFaltantes <= 0) return static_cast<double>(horasFaltantes);

    // Horas por semestre faltante
    return static_cast<double>(horasFaltantes) / semestresFaltantes;
}
};

#endif // ALUMNO_H