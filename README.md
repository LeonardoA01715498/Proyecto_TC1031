# Proyecto: Nombre de tu proyecto
El proyecto va a analizar los requisitos faltantes de graduacion para alumnos de el Tec de Monterrey y el sistema le debera generar una planeacion o simulacion de toda la
carrera del estudiante, indicando las materias a cursar y el sersvicio social a cursar en cada semestre, al mismo tiempo el sistema debe de indicar los estudiantes en riesgo
para los administradores.

## Descripción del avance 1
En este avance crea a los alumnos y se guardan en un csv, a posterior se calcula su factor de riesgo y los organiza por medio de quicksort y lo guarda en otro csv.

## Instrucciones para compilar el avance de proyecto
Ejecuta el siguiente comando en la terminal:

`g++ main.cpp -o main.exe`

## Instrucciones para ejecutar el avance de proyecto
Ejecuta el siguiente comando en la terminal:

`./main.exe` 

## Descripción de las entradas del avance de proyecto
El programa toma como entrada un archivo de texto en formato CSV llamado alumnos.csv. 

### Formato de alumnos.csv
El archivo no requiere encabezado y consta de 6 campos separados por comas:
ID (int), Nombre (string), Carrera (string), Semestre (int), Créditos (int), HorasServicioSocial (int)

### Ejemplo de contenido de entrada:
csv
12345,Leo,Ingenieria en Sistemas,5,180,240
23456,David,Robotica,2,70,80
34567,Carlo,Datos,7,250,440
45678,Ana,Ingenieria Industrial,6,200,100
56789,Sofia,Sistemas Computacionales,8,260,400

## Descripción de las salidas del avance de proyecto
El programa actualiza tanto alumnos.csv como alumnosRiesgo.csv, con alumnos agregados y los alumnos organizados por factor de riesgo de manera descendente.

1. Archivo alumnos.csv
Contiene la lista completa de alumnos con todos sus atributos actualizados tras la ejecución de las funciones de inserción (CrearAlumno()).

2. Archivo alumnosRiesgo.csv
Genera el reporte final procesado que incluye únicamente el ID, nombre y el número de riesgo calculado, formateado a 2 decimales y ordenado estrictamente de mayor a menor riesgo. 

## Desarrollo de competencias

### SICT0301: Evalúa los componentes
#### Hace un análisis de complejidad correcto y completo para los algoritmos de ordenamiento usados en el programa.
Escribe aquí tus argumentos sobre por qué consideras que has desarrrollado esta competencia y dónde se puede observar el desarrollo que mencionas.

### SICT0302: Toma decisiones
#### Selecciona un algoritmo de ordenamiento adecuado al problema y lo usa correctamente.
Escribe aquí tus argumentos sobre por qué consideras que has desarrrollado esta competencia y dónde se puede observar el desarrollo que mencionas.
