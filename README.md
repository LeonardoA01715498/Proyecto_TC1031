# Proyecto: Riesgo de Servicio Social

Para titularse, todo alumno debe completar **480 horas de servicio social** a lo largo de su carrera (8 semestres). Muchos alumnos las dejan para el final y terminan en riesgo de no titularse a tiempo.

Este programa identifica a esos alumnos. Para cada uno calcula un **factor de riesgo**: las horas de servicio que le faltan por cada semestre que le queda.

```
factor = (480 - horas_cumplidas) / (8 - semestre_actual)
```

- Si ya cumplió las 480 horas, el factor es 0.
- Si ya no le quedan semestres, el factor es igual a todas las horas que le faltan.
- El ritmo normal es 480 / 8 = **60 horas por semestre**, así que un factor mayor a 60 quiere decir que el alumno va atrasado.

Según su factor, cada alumno recibe un nivel de riesgo:

- **Completo:** factor 0.
- **Bajo:** factor de 1 a 60.
- **Medio:** factor de 61 a 120.
- **Alto:** factor mayor a 120.

Con esta información, una coordinación de servicio social puede ordenar a los alumnos de mayor a menor riesgo y atender primero a quienes más lo necesitan.

## Descripción del avance 1

En el primer avance se creó la clase `Alumno` con sus datos (ID, nombre, carrera, semestre, créditos y horas de servicio social) y el método `calcularFactorRiesgoServicio()`.

El programa:

1. Escribía 10 alumnos de ejemplo en `alumnos.csv`.
2. Leía ese archivo y cargaba los alumnos en un `std::vector<Alumno>`.
3. Ordenaba el vector de mayor a menor riesgo con un **QuickSort** propio (pivote = último elemento, partición de Lomuto).
4. Escribía el resultado en `alumnosRiesgo.csv` con el formato `id,nombre,numero_riesgo`.

## Descripción del avance 2

En este avance los alumnos se guardan en una **lista doblemente ligada** programada desde cero (`ListaAlumnos.h`). La lista permite:

- agregar alumnos al final;
- buscarlos y eliminarlos por ID;
- ordenarlos por riesgo o por ID con **Merge Sort**;
- mostrar solo a los alumnos que superan un umbral de riesgo.

El programa se controla con un **menú interactivo** en la terminal. Desde el menú se pueden cargar los alumnos desde el archivo, consultarlos, agregarlos y eliminarlos (los cambios se guardan en `alumnos.csv`), y generar el reporte `alumnosRiesgo.csv` o leerlo e imprimirlo en pantalla.

### Archivos del proyecto

- `Alumno.h`: clase `Alumno`, cálculo del factor y del nivel de riesgo, y criterios de comparación.
- `ListaAlumnos.h`: lista doblemente ligada; insertar, buscar, eliminar, ordenar (Merge Sort), mostrar y filtrar.
- `Archivos.h`: lectura y escritura de `alumnos.csv` y `alumnosRiesgo.csv`.
- `Menu.h`: menú interactivo en la terminal.
- `main.cpp`: punto de entrada; crea el menú y lo ejecuta.
- `alumnos.csv`: datos de entrada de los alumnos.
- `alumnosRiesgo.csv`: reporte que genera el programa.

### Cambios sobre el primer avance

1. **Cambio de `std::vector` a una lista doblemente ligada.** El programa recibe alumnos uno por uno (desde el archivo o desde el menú) y también los elimina. En la lista, agregar al final es O(1) siempre gracias al apuntador a la cola, sin las realocaciones de `vector`, y eliminar solo reconecta dos apuntadores, sin desplazar a los demás alumnos. Además, el uso principal del programa es recorrer a todos los alumnos, que cuesta O(n) en cualquier estructura.
2. **Cambio de QuickSort a Merge Sort.** Antes de cambiarlo, comparé con un análisis de complejidad mi QuickSort contra `std::sort` de C++. Mi QuickSort cae en O(n²) cuando los datos ya están ordenados o cuando muchos alumnos tienen el mismo riesgo (por ejemplo, todos los que ya terminaron tienen riesgo 0), y las dos situaciones son comunes en este programa. `std::sort` garantiza O(n log n), pero solo funciona con estructuras de acceso aleatorio, no con una lista ligada. Merge Sort garantiza O(n log n) en todos los casos, en una lista ligada no necesita memoria auxiliar y es estable. La comparación completa está en la sección SICT0302.
3. **Menú interactivo.** Antes el programa solo generaba los archivos y terminaba. Ahora el usuario elige qué hacer: cargar alumnos, agregar uno, buscarlo, eliminarlo, ver a los alumnos en riesgo o generar y leer el reporte.
4. **Los archivos ahora se leen, se imprimen y se les agrega información.** Antes los archivos solo se escribían. Ahora el reporte `alumnosRiesgo.csv` se puede leer y mostrar en pantalla, los alumnos nuevos se añaden al final de `alumnos.csv` y, al eliminar uno, el archivo se actualiza.
5. **Se dejaron de generar alumnos duplicados.** En el primer avance, `main` escribía los mismos 10 alumnos en `alumnos.csv` cada vez que se ejecutaba, así que el archivo se llenaba de registros repetidos. Ahora los datos solo se leen del archivo y la carga ignora los IDs repetidos. Al agregar un alumno desde el menú, también se valida que su ID no exista.
6. **Análisis de complejidad más explícito.** Cada función indica en el código su complejidad en el mejor caso, el caso promedio y el peor caso, y explica cuándo ocurre cada uno.
7. **Reorganización del código.** Antes se incluían archivos `.cpp` dentro de otros `.cpp`; ahora el código está organizado en archivos de encabezado (`.h`). Se eliminaron los archivos vacíos (`Carrera.h`, `Menu.cpp`) y el ejecutable `main.exe`, que ya no correspondía al código.

## Instrucciones para compilar el avance de proyecto

Ejecuta el siguiente comando en la terminal:

`g++ main.cpp -std=c++11 -o segundo_avance`

## Instrucciones para ejecutar el avance de proyecto

Ejecuta el siguiente comando en la terminal:

`./segundo_avance`

En Windows: `segundo_avance.exe`

`alumnos.csv` debe estar en la carpeta desde la que se ejecuta el programa.

## Descripción de las entradas del avance de proyecto

### Archivo `alumnos.csv`

Es un archivo de texto separado por comas, con un alumno por línea. La primera línea es el encabezado. Sus columnas son:

- `id` (entero): matrícula del alumno; debe ser única.
- `nombre` (texto): nombre del alumno, sin comas.
- `carrera` (texto): carrera, sin comas.
- `semestre` (entero): semestre que cursa.
- `creditos` (entero): créditos aprobados.
- `horas_servicio` (entero): horas de servicio social cumplidas.

Ejemplo:

```
id,nombre,carrera,semestre,creditos,horas_servicio
12345,Leo,Ingenieria en Sistemas,5,180,240
23456,David,Robotica,2,70,80
34567,Carlo,Datos,7,250,440
45678,Ana,Ingenieria Industrial,6,200,100
78901,Valeria,Biomedica,7,240,120
90123,Camila,Robotica,8,270,480
```

El programa ignora el encabezado, las líneas vacías o incompletas, los valores que no son números y los IDs repetidos.

### Entradas del teclado (menú)

Al iniciar, el programa carga `alumnos.csv` y muestra el menú:

```
==============================================
   RIESGO DE SERVICIO SOCIAL  (10 alumnos en memoria)
==============================================
  1. Cargar alumnos desde archivo
  2. Mostrar todos los alumnos
  3. Agregar alumno
  4. Buscar alumno por ID
  5. Eliminar alumno por ID
  6. Ordenar y mostrar por riesgo (mayor a menor)
  7. Ordenar y mostrar por ID
  8. Ver alumnos en riesgo
  9. Generar reporte alumnosRiesgo.csv
 10. Leer reporte alumnosRiesgo.csv
  0. Salir
```

El usuario escribe el número de la opción. Algunas opciones piden datos adicionales:

- **Opción 3:** ID (5 dígitos, no repetido), nombre, carrera, semestre (1 a 12), créditos (0 a 400) y horas de servicio (0 a 480).
- **Opciones 4 y 5:** el ID del alumno.
- **Opción 8:** el umbral de riesgo (se sugiere 60, el ritmo normal).

Todas las entradas se validan: si el usuario escribe texto donde se espera un número, o un número fuera de rango, el programa lo vuelve a pedir. Los nombres y las carreras no pueden contener comas porque romperían el formato CSV.

## Descripción de las salidas del avance de proyecto

### Salidas en pantalla

- **Opción 1:** cuántos alumnos se cargaron.
- **Opciones 2, 6 y 7:** lista de alumnos con su factor y nivel de riesgo, en el orden actual, por riesgo o por ID.
- **Opción 3:** confirmación y los datos del alumno agregado.
- **Opción 4:** los datos del alumno y cuántas horas le faltan.
- **Opción 5:** confirmación de que se eliminó y se actualizó el archivo.
- **Opción 8:** solo los alumnos cuyo riesgo supera el umbral, del más al menos riesgoso.
- **Opción 9:** confirmación de que se generó el reporte.
- **Opción 10:** el contenido de `alumnosRiesgo.csv`.

Ejemplo de la opción 6:

```
ID      Nombre      Carrera                    Sem  Creditos Horas SS Riesgo   Nivel
-------------------------------------------------------------------------------------
78901   Valeria     Biomedica                  7    240      120      360.00   Alto
45678   Ana         Ingenieria Industrial      6    200      100      190.00   Alto
67890   Mateo       Mecatronica                3    90       50       86.00    Medio
12345   Leo         Ingenieria en Sistemas     5    180      240      80.00    Medio
56789   Sofia       Sistemas Computacionales   8    260      400      80.00    Medio
89012   Gabriel     Datos                      4    130      200      70.00    Medio
11223   Santiago    Ingenieria en Sistemas     1    30       0        68.57    Medio
23456   David       Robotica                   2    70       80       66.67    Medio
34567   Carlo       Datos                      7    250      440      40.00    Bajo
90123   Camila      Robotica                   8    270      480      0.00     Completo
Total: 10 alumno(s)
```

Ejemplo de la opción 8 con umbral 100:

```
ID      Nombre      Carrera                    Sem  Creditos Horas SS Riesgo   Nivel
-------------------------------------------------------------------------------------
78901   Valeria     Biomedica                  7    240      120      360.00   Alto
45678   Ana         Ingenieria Industrial      6    200      100      190.00   Alto
2 alumno(s) en riesgo.
```

### Archivos de salida

- **`alumnosRiesgo.csv`** (opción 9): reporte ordenado de mayor a menor riesgo, con el formato `id,nombre,numero_riesgo,nivel`.

  ```
  id,nombre,numero_riesgo,nivel
  78901,Valeria,360.00,Alto
  45678,Ana,190.00,Alto
  67890,Mateo,86.00,Medio
  ...
  90123,Camila,0.00,Completo
  ```

- **`alumnos.csv`**: con la opción 3 se le añade el alumno nuevo al final; con la opción 5 se reescribe sin el alumno eliminado.

## Desarrollo de competencias

### SICT0301: Evalúa los componentes

#### Hace un análisis de complejidad correcto y completo para los algoritmos de ordenamiento usados en el programa.

El algoritmo de ordenamiento del programa es **Merge Sort** sobre la lista ligada (`ListaAlumnos.h`). Se compone de cuatro funciones:

- **`dividir`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Encuentra la mitad de la lista con un apuntador lento y uno rápido; siempre recorre la sublista.
- **`mezclar`**
  - Mejor caso: O(min(a, b)). Promedio: O(a + b). Peor caso: O(a + b).
  - Une dos sublistas ordenadas. En el mejor caso, una de ellas se acaba pronto y el resto se conecta de un solo paso.
- **`mergeSort`**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - T(n) = 2T(n/2) + O(n). Siempre parte la lista a la mitad, así que hay log n niveles y en cada uno se hace trabajo O(n), sin importar cómo vengan los datos.
- **`ordenar`**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - Llama a `mergeSort` y después reconstruye los apuntadores `ant` y la `cola` en O(n).

**Memoria extra:** O(log n), solo por la pila de recursión, porque en una lista ligada se reconectan apuntadores en lugar de copiar los datos a un arreglo auxiliar.

Para elegirlo también analicé las otras dos opciones:

- **QuickSort** (el del primer avance): O(n log n) en el mejor caso y en el promedio, pero **O(n²) en el peor caso**, que ocurre cuando el pivote siempre queda en un extremo. Con el pivote en el último elemento, eso pasa cuando los datos ya están ordenados o cuando muchos tienen el mismo valor.
- **`std::sort`** (Introsort): O(n log n) en el mejor, promedio y peor caso, porque cambia a HeapSort cuando la recursión se hace demasiado profunda.

**Aprendizaje:** en el primer avance solo había indicado la complejidad general de cada función. Ahora, siguiendo la retroalimentación, escribo explícitamente el resultado del mejor, promedio y peor caso, y explico qué situación produce cada uno. Esto se puede ver en los comentarios de `ListaAlumnos.h` y en la lista anterior.

#### Hace un análisis de complejidad correcto y completo de todas las estructuras de datos y cada uno de sus usos en el programa.

El análisis está comentado en el código junto a cada función. En todos los casos, *n* es el número de alumnos.

**Lista doblemente ligada (`ListaAlumnos.h`)**

- **`agregarFinal`**
  - Mejor caso: O(1). Promedio: O(1). Peor caso: O(1).
  - Se tiene un apuntador a la cola, así que no hay que recorrer la lista.
- **`buscarPorId`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Búsqueda secuencial. Mejor caso: el alumno está en la cabeza. Peor caso: está al final o no existe. En promedio revisa n/2 nodos.
- **`existe`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Usa `buscarPorId`.
- **`eliminarPorId`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Localizar el nodo cuesta O(n); desconectarlo cuesta O(1) gracias a los apuntadores `ant` y `sig`.
- **`ordenar` (Merge Sort)**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - Ver la sección anterior.
- **`mostrar` y `recorrer`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Visitan cada nodo una vez.
- **`mostrarEnRiesgo`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Si la lista está ordenada por riesgo, se detiene en el primer alumno que no supera el umbral. Mejor caso: el primero ya no lo supera. Peor caso: todos lo superan.
- **`limpiar` y destructor**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Liberan cada nodo.
- **`getTam` y `estaVacia`**
  - Mejor caso: O(1). Promedio: O(1). Peor caso: O(1).
  - Se guarda un contador `tam`.

**Memoria:** O(n) para los nodos.

**Clase `Alumno` (`Alumno.h`):** todas sus operaciones (`calcularFactorRiesgoServicio`, `nivelRiesgo`, `toCSV`, `mostrarInformacion`, getters, setters y los comparadores `mayorRiesgo` y `menorId`) son **O(1) en todos los casos**, porque solo trabajan con los atributos del propio alumno.

**Archivos (`Archivos.h`)**

- **`cargarAlumnos`**
  - Mejor caso: O(1). Promedio: O(n²). Peor caso: O(n²).
  - Leer e insertar cada línea es O(1), pero antes de insertar se verifica que el ID no exista (O(k), con k = alumnos ya cargados). 1 + 2 + … + n = O(n²). Mejor caso: archivo vacío.
  - Sin esa verificación, la carga sería O(n); se decidió conservarla para proteger la integridad de los datos y evitar registros duplicados. Con el tamaño de un grupo o una generación, el costo es despreciable.
- **`agregarAlumnoArchivo`**
  - Mejor caso: O(1). Promedio: O(1). Peor caso: O(1).
  - Se abre en modo *append*; no se reescribe el archivo.
- **`guardarAlumnos`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Reescribe el archivo completo (se usa al eliminar).
- **`generarReporteRiesgo`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Escribe una línea por alumno.
- **`mostrarReporteRiesgo`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Lee e imprime cada línea. Mejor caso: archivo vacío.

**Opciones del menú (`Menu.h`)**

- **1. Cargar:** mejor caso O(1); promedio y peor caso O(n²), por la verificación de duplicados.
- **2. Mostrar:** O(n) en todos los casos.
- **3. Agregar:** mejor caso O(1); promedio y peor caso O(n), por validar que el ID no exista. Escribir en el archivo es O(1).
- **4. Buscar:** mejor caso O(1); promedio y peor caso O(n).
- **5. Eliminar:** O(n) en todos los casos, porque el archivo se reescribe completo.
- **6 y 7. Ordenar y mostrar:** O(n log n) en todos los casos.
- **8. Alumnos en riesgo:** O(n log n) si hay que ordenar primero. Si la lista ya estaba ordenada por riesgo, O(1) en el mejor caso y O(n) en el peor.
- **9. Generar reporte:** O(n log n) en todos los casos (ordenar + escribir).
- **10. Leer reporte:** O(n) en todos los casos.

### SICT0302: Toma decisiones

#### Selecciona un algoritmo de ordenamiento adecuado al problema y lo usa correctamente.

En el primer avance mencioné que planeaba cambiar de algoritmo. Siguiendo la recomendación, antes de decidir comparé con un análisis de complejidad el QuickSort que había usado contra `std::sort` de C++. También incluí Merge Sort, porque es el que se puede aplicar a una lista ligada.

- **Mi QuickSort** (pivote = último elemento, partición de Lomuto)
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: **O(n²)**.
  - Memoria extra: O(log n) en promedio, O(n) en el peor caso.
  - No es estable.
  - No funciona en una lista ligada, porque necesita acceso por índice.
- **`std::sort`** (Introsort)
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - Memoria extra: O(log n).
  - No es estable.
  - No funciona en una lista ligada, porque necesita iteradores de acceso aleatorio.
- **Merge Sort en lista ligada**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - Memoria extra: O(log n).
  - **Es estable.**
  - **Funciona en una lista ligada.**

Mi QuickSort cae en O(n²) cuando el pivote siempre queda en un extremo. En este problema eso pasa en dos situaciones comunes:

1. **Los datos ya están ordenados por riesgo**, por ejemplo cuando el usuario ordena y luego vuelve a ordenar.
2. **Hay muchos riesgos iguales.** Todos los alumnos que ya terminaron tienen riesgo 0, y la partición de Lomuto manda todos los valores iguales al mismo lado.

Para comprobarlo, medí los tres algoritmos con 20,000 alumnos:

- **Datos aleatorios:** mi QuickSort 5.2 ms, `std::sort` 4.9 ms, Merge Sort 5.5 ms.
- **Datos ya ordenados:** mi QuickSort **57.7 ms**, `std::sort` 1.9 ms, Merge Sort 2.4 ms.
- **Todos con el mismo riesgo:** mi QuickSort **166.5 ms**, `std::sort` 1.2 ms, Merge Sort 1.9 ms.

**Decisión:** sí era necesario cambiar de algoritmo, porque mi QuickSort se degrada justo en los casos que más se repiten en este problema. `std::sort` resolvería eso, pero solo funciona con estructuras de acceso aleatorio (`vector`, arreglos) y además no es estable. Como la estructura elegida es una lista ligada, elegí **Merge Sort**, que:

- garantiza O(n log n) en el mejor, promedio y peor caso;
- en una lista ligada no necesita memoria auxiliar O(n), porque solo reconecta apuntadores;
- es **estable**, así que los alumnos con el mismo riesgo conservan su orden (además se desempata por ID).

**Uso correcto:** Merge Sort se usa en las opciones 6, 7, 8 y 9 del menú, con dos criterios de comparación (`mayorRiesgo` y `menorId`, en `Alumno.h`). Gracias a eso, la opción 8 puede detener el recorrido en cuanto encuentra al primer alumno que no supera el umbral.

**Aprendizaje:** una decisión sobre un algoritmo se toma comparando opciones, y el análisis de complejidad es uno de los criterios. Al comparar, vi que mi QuickSort no era la mejor opción para estos datos, y que la estructura de datos también limita qué algoritmos se pueden usar.

#### Selecciona una estructura de datos adecuada al problema y la usa correctamente.

Opciones consideradas para guardar a los alumnos:

- **`std::vector`**
  - Insertar al final: O(1) amortizado, pero O(n) cuando tiene que realocar.
  - Buscar por ID: O(n).
  - Eliminar: O(n), porque desplaza los elementos siguientes.
  - Recorrer: O(n).
  - Ordenar: O(n log n) con `std::sort`.
- **Lista doblemente ligada**
  - Insertar al final: **O(1) siempre**.
  - Buscar por ID: O(n).
  - Eliminar: **O(n) para localizar el nodo, O(1) para desconectarlo**.
  - Recorrer: O(n).
  - Ordenar: O(n log n) con Merge Sort, sin memoria extra.

Elegí la **lista doblemente ligada** porque se ajusta a cómo se usan los datos en este programa:

- **El uso principal es recorrer a todos los alumnos**: mostrarlos, filtrar a los que están en riesgo y generar el reporte. Recorrer es O(n) en cualquier estructura, así que la lista no pierde nada ahí.
- **Los alumnos llegan uno por uno**, desde el archivo o desde el menú. Con el apuntador a la cola, insertar es O(1) siempre, sin las realocaciones de `vector`.
- **Al eliminar un alumno** no hay que desplazar a los demás; con los apuntadores `ant` y `sig` el nodo se desconecta en O(1).
- **Se ordena de forma eficiente** con Merge Sort, que en una lista ligada no necesita un arreglo auxiliar.
- La desventaja es que no hay acceso por índice, así que la búsqueda por ID es secuencial (O(n)). Para el tamaño de un grupo o una generación es aceptable. Si se necesitaran búsquedas muy frecuentes, una estructura jerárquica (árbol binario de búsqueda) las bajaría a O(log n).

**Uso correcto:** la lista mantiene `cabeza`, `cola` y `tam` consistentes en todas las operaciones, incluido reconstruir los apuntadores `ant` y la `cola` después de ordenar. El destructor libera toda la memoria y la copia está deshabilitada para evitar liberar dos veces el mismo nodo. Se probó con *AddressSanitizer* y *UndefinedBehaviorSanitizer* sin errores. Esto se puede ver en `ListaAlumnos.h`.

### SICT0303: Implementa acciones científicas

#### Implementa mecanismos para consultar información de las estructuras correctos.

Desde el menú (`Menu.h`), la lista se puede consultar de varias formas:

- **Todos los alumnos** (opción 2, `ListaAlumnos::mostrar`).
- **Un alumno por ID** (opción 4, `ListaAlumnos::buscarPorId`), incluyendo cuántas horas le faltan.
- **Ordenados por riesgo o por ID** (opciones 6 y 7, `ListaAlumnos::ordenar`).
- **Solo los alumnos en riesgo**, según un umbral que elige el usuario (opción 8, `ListaAlumnos::mostrarEnRiesgo`). Aprovecha que la lista está ordenada para detenerse en cuanto un alumno ya no supera el umbral.
- **El reporte guardado**, leído desde el archivo (opción 10).

Además, la lista se puede modificar: **agregar** (opción 3, valida que el ID no esté repetido) y **eliminar** (opción 5). Todas las entradas del usuario se validan, así que una consulta con un dato inválido no rompe el programa.

#### Implementa mecanismos de lectura de archivos para cargar datos a las estructuras de manera correcta.

- `cargarAlumnos` (`Archivos.h`) lee `alumnos.csv` línea por línea y crea un nodo de la lista por cada alumno.
- **Es robusta:** ignora el encabezado, las líneas vacías, las líneas incompletas y los valores no numéricos (usa conversión sin excepciones), quita el `\r` de los archivos guardados en Windows y **no inserta IDs repetidos**.
- Al recargar (opción 1), la lista se vacía primero para no duplicar datos en memoria.
- **Se escribe en archivos:** un alumno nuevo se **añade** al final de `alumnos.csv`, al eliminar se **reescribe** el archivo y el reporte `alumnosRiesgo.csv` se **genera** ordenado por riesgo.
- **Se lee e imprime un archivo de resultados:** la opción 10 lee `alumnosRiesgo.csv` (`mostrarReporteRiesgo`) y lo muestra en pantalla.