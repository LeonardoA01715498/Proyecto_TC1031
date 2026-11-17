# Proyecto: Riesgo de Servicio Social

Para titularse, todo alumno debe completar 480 horas de servicio social a lo largo de su carrera (8 semestres). Muchos alumnos las dejan para el final y terminan en riesgo de no titularse a tiempo.

Este programa identifica a esos alumnos. Para cada uno calcula un factor de riesgo: las horas de servicio que le faltan por cada semestre que le queda.

```
factor = (480 - horas_cumplidas) / (8 - semestre_actual)
```

- Si ya cumplió las 480 horas, el factor es 0.
- Si ya no le quedan semestres, el factor es igual a todas las horas que le faltan.
- El ritmo normal es 480 / 8 = 60 horas por semestre, así que un factor mayor a 60 quiere decir que el alumno va atrasado.

Según su factor, cada alumno recibe un nivel de riesgo:

- Completo: factor 0.
- Bajo: factor de 1 a 60.
- Medio: factor de 61 a 120.
- Alto: factor mayor a 120.

Con esta información, una coordinación de servicio social puede ordenar a los alumnos de mayor a menor riesgo y atender primero a quienes más lo necesitan.

## Descripción del avance 1

En el primer avance se creó la clase `Alumno` con sus datos (ID, nombre, carrera, semestre, créditos y horas de servicio social) y el método `calcularFactorRiesgoServicio()`.

El programa:

1. Escribía 10 alumnos de ejemplo en `alumnos.csv`.
2. Leía ese archivo y cargaba los alumnos en un `std::vector<Alumno>`.
3. Ordenaba el vector de mayor a menor riesgo con un QuickSort propio (pivote = último elemento, partición de Lomuto).
4. Escribía el resultado en `alumnosRiesgo.csv` con el formato `id,nombre,numero_riesgo`.

## Descripción del avance 2

En el segundo avance los alumnos se guardaron en una lista doblemente ligada programada desde cero (`ListaAlumnos.h`), que permitía agregarlos al final, buscarlos y eliminarlos por ID, ordenarlos con **Merge Sort y mostrar solo a los que superan un umbral de riesgo.

Se agregó un menú interactivo en la terminal para cargar los alumnos desde el archivo, consultarlos, agregarlos y eliminarlos (guardando los cambios en `alumnos.csv`), y para generar el reporte `alumnosRiesgo.csv` o leerlo e imprimirlo en pantalla.

## Descripción del avance 3

En este avance se agregó una **estructura de datos no lineal**: un **árbol AVL** (árbol binario de búsqueda auto-balanceado) ordenado por ID (`ArbolAlumnos.h`).

El árbol funciona como **índice** de la lista: cada nodo del árbol guarda el ID del alumno y un apuntador al nodo de la lista donde están sus datos. Así cada estructura hace lo que mejor sabe hacer:

- **La lista doblemente ligada** guarda los datos, se recorre para mostrar o filtrar alumnos y se ordena por riesgo con Merge Sort.
- **El árbol AVL** se usa para todo lo que depende del ID: buscar un alumno, validar que un ID no esté repetido, eliminar un alumno y recorrer a los alumnos en orden de ID.

La nueva clase `Registro` (`Registro.h`) une las dos estructuras y las mantiene sincronizadas: cada vez que se agrega o elimina un alumno, se hace en las dos al mismo tiempo.

También se mejoró la **escritura de archivos**: `alumnos.csv` se guarda ordenado por ID y los archivos que se reescriben completos se escriben primero en un archivo temporal, para que el original no se pierda si algo falla.

### Archivos del proyecto

- `Alumno.h`: clase `Alumno`, cálculo del factor y del nivel de riesgo, y criterio de comparación para ordenar.
- `ListaAlumnos.h`: lista doblemente ligada; insertar, eliminar un nodo, ordenar (Merge Sort), mostrar y filtrar.
- `ArbolAlumnos.h`: árbol AVL ordenado por ID; insertar, buscar, eliminar y recorrido inorden.
- `Registro.h`: une la lista y el árbol y los mantiene sincronizados.
- `Archivos.h`: lectura y escritura de `alumnos.csv` y `alumnosRiesgo.csv`.
- `Menu.h`: menú interactivo en la terminal.
- `main.cpp`: punto de entrada; crea el menú y lo ejecuta.
- `alumnos.csv`: datos de entrada de los alumnos.
- `alumnosRiesgo.csv`: reporte que genera el programa.

### Cambios sobre el segundo avance

1. **Se corrigió un error de compilación en `Menu.h`.** Al quitar las líneas separadoras del menú se borró también el `;` del final de la instrucción `std::cout`, y el programa no compilaba. Ahora se compila y se prueba con g++ y con clang antes de entregar.
2. **Se agregó un árbol AVL como índice por ID.** En el segundo avance, buscar un alumno, validar un ID repetido y eliminar recorrían la lista completa (O(n)). Con el árbol AVL estas operaciones bajan a O(log n). Se eligió AVL y no un árbol binario de búsqueda simple porque los IDs pueden llegar ordenados (por ejemplo, si el archivo ya está ordenado por ID), y en ese caso un árbol simple se vuelve una lista con altura n y todas sus operaciones serían O(n).
3. **La carga del archivo pasó de O(n²) a O(n log n).** Antes, para no insertar IDs repetidos, cada alumno se comparaba contra todos los ya cargados. Ahora esa validación la hace el árbol en O(log n).
4. **Eliminar un alumno ahora es O(log n) en memoria.** El árbol encuentra el nodo de la lista en O(log n) y la lista lo desconecta en O(1), sin recorrerla.
5. **"Mostrar por ID" ya no ordena la lista.** Antes la opción 7 ordenaba la lista por ID con Merge Sort (O(n log n)) y perdía el orden por riesgo. Ahora usa el recorrido inorden del árbol (O(n)), que siempre entrega los alumnos en orden de ID, y no modifica el orden de la lista.
6. **Escritura de archivos más segura.** `alumnos.csv` y `alumnosRiesgo.csv` se escriben primero en un archivo `.tmp` y solo reemplazan al original si toda la escritura fue correcta. Además, cada escritura se verifica y el menú avisa si falló.
7. **Nueva opción para guardar.** La opción 11 reescribe `alumnos.csv` con todos los alumnos en memoria, ordenados por ID.
8. **Corrección del peor caso de `std::sort`.** En el segundo avance escribí que su peor caso era O(n log n) sin explicar la diferencia entre versiones del estándar. La sección SICT0302 lo explica.
9. **Se agregó la complejidad final del programa** en la sección SICT0301.

## Instrucciones para compilar el avance de proyecto

Ejecuta el siguiente comando en la terminal:

`g++ main.cpp -std=c++11 -o tercer_avance`

## Instrucciones para ejecutar el avance de proyecto

Ejecuta el siguiente comando en la terminal:

`./tercer_avance`

En Windows: `tercer_avance.exe`

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
   RIESGO DE SERVICIO SOCIAL  (10 alumnos en memoria)
  1. Cargar alumnos desde archivo
  2. Mostrar todos los alumnos
  3. Agregar alumno
  4. Buscar alumno por ID
  5. Eliminar alumno por ID
  6. Ordenar y mostrar por riesgo (mayor a menor)
  7. Mostrar por ID
  8. Ver alumnos en riesgo
  9. Generar reporte alumnosRiesgo.csv
 10. Leer reporte alumnosRiesgo.csv
 11. Guardar alumnos en alumnos.csv
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
- **Opciones 2 y 6:** lista de alumnos con su factor y nivel de riesgo, en el orden actual o de mayor a menor riesgo.
- **Opción 3:** confirmación y los datos del alumno agregado.
- **Opción 4:** los datos del alumno y cuántas horas le faltan.
- **Opción 5:** confirmación de que se eliminó y se actualizó el archivo.
- **Opción 7:** lista de alumnos en orden de ID y la altura del árbol AVL.
- **Opción 8:** solo los alumnos cuyo riesgo supera el umbral, del más al menos riesgoso.
- **Opción 9:** confirmación de que se generó el reporte y cuántos alumnos tiene.
- **Opción 10:** el contenido de `alumnosRiesgo.csv`.
- **Opción 11:** cuántos alumnos se guardaron en `alumnos.csv`.

Ejemplo de la opción 7:

```
Alumnos por ID (recorrido inorden del arbol AVL):

ID      Nombre      Carrera                    Sem  Creditos Horas SS Riesgo   Nivel
-------------------------------------------------------------------------------------
11223   Santiago    Ingenieria en Sistemas     1    30       0        68.57    Medio
12345   Leo         Ingenieria en Sistemas     5    180      240      80.00    Medio
23456   David       Robotica                   2    70       80       66.67    Medio
34567   Carlo       Datos                      7    250      440      40.00    Bajo
45678   Ana         Ingenieria Industrial      6    200      100      190.00   Alto
56789   Sofia       Sistemas Computacionales   8    260      400      80.00    Medio
67890   Mateo       Mecatronica                3    90       50       86.00    Medio
78901   Valeria     Biomedica                  7    240      120      360.00   Alto
89012   Gabriel     Datos                      4    130      200      70.00    Medio
90123   Camila      Robotica                   8    270      480      0.00     Completo
Total: 10 alumno(s)
Altura del arbol AVL: 4
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

- **`alumnos.csv`**: con la opción 3 se le añade el alumno nuevo al final; con las opciones 5 y 11 se reescribe completo, ordenado por ID.

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
- **`std::sort`**: O(n log n) en el mejor caso y en el promedio. Su peor caso se explica en la sección SICT0302.

**Aprendizaje:** en el primer avance solo había indicado la complejidad general de cada función. Ahora escribo explícitamente el resultado del mejor, promedio y peor caso, y explico qué situación produce cada uno. Esto se puede ver en los comentarios de `ListaAlumnos.h` y en la lista anterior.

#### Hace un análisis de complejidad correcto y completo de todas las estructuras de datos y cada uno de sus usos en el programa.

El análisis está comentado en el código junto a cada función. En todos los casos, *n* es el número de alumnos.

**Lista doblemente ligada (`ListaAlumnos.h`)**

- **`agregarFinal`**
  - Mejor caso: O(1). Promedio: O(1). Peor caso: O(1).
  - Se tiene un apuntador a la cola, así que no hay que recorrer la lista. Regresa el nodo creado para que el árbol lo indexe.
- **`eliminarNodo`**
  - Mejor caso: O(1). Promedio: O(1). Peor caso: O(1).
  - Recibe el nodo (que encontró el árbol) y solo reconecta los apuntadores `ant` y `sig` de sus vecinos.
- **`ordenar` (Merge Sort)**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n).
  - Ver la sección anterior.
- **`mostrar` y `recorrer`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Visitan cada nodo una vez.
- **`mostrarEnRiesgo`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n).
  - Con la lista ordenada por riesgo, se detiene en el primer alumno que no supera el umbral. Mejor caso: el primero ya no lo supera. Peor caso: todos lo superan.
- **`limpiar` y destructor**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Liberan cada nodo.
- **`getTam` y `estaVacia`**
  - O(1) en todos los casos; se guarda un contador `tam`.
- **Memoria:** O(n) para los nodos.

**Árbol AVL (`ArbolAlumnos.h`)**

En un árbol AVL, la diferencia de altura entre los subárboles izquierdo y derecho de cada nodo es como máximo 1. Eso garantiza que la altura del árbol siempre sea O(log n) (como máximo cerca de 1.44 log₂ n), sin importar el orden en que lleguen los IDs.

- **`rotarDerecha`, `rotarIzquierda` y `balancear`**
  - O(1) en todos los casos.
  - Solo reconectan dos o tres apuntadores y actualizan alturas guardadas en los nodos.
- **`insertar`**
  - Mejor caso: O(log n). Promedio: O(log n). Peor caso: O(log n).
  - Baja por un solo camino de la raíz a una hoja (altura O(log n)) y, de regreso, rebalancea cada nodo del camino en O(1). Si el ID ya existe, no lo inserta.
- **`buscar` y `existe`**
  - Mejor caso: O(1). Promedio: O(log n). Peor caso: O(log n).
  - En cada nodo se descarta la mitad del árbol. Mejor caso: el ID está en la raíz. Peor caso: está en una hoja o no existe.
- **`eliminar`**
  - Mejor caso: O(log n). Promedio: O(log n). Peor caso: O(log n).
  - Busca el nodo, lo quita (si tiene dos hijos lo reemplaza por su sucesor, el mínimo del subárbol derecho, que también se encuentra en O(log n)) y rebalancea el camino de regreso.
- **`recorrerInorden`**
  - Mejor caso: O(n). Promedio: O(n). Peor caso: O(n).
  - Visita izquierda, raíz, derecha; pasa una vez por cada nodo y los entrega en orden de ID.
- **`limpiar` y destructor**
  - O(n) en todos los casos; liberan cada nodo.
- **`getTam` y `altura`**
  - O(1) en todos los casos.
- **Memoria:** O(n) para los nodos. La recursión de `insertar`, `eliminar` e `inorden` usa O(log n) de pila, porque nunca baja más que la altura del árbol.

**Clase `Registro` (`Registro.h`)**, que une la lista y el árbol:

- **`agregar`:** O(log n) en todos los casos. Validar el ID en el árbol O(log n) + insertar en la lista O(1) + insertar en el árbol O(log n).
- **`buscar` y `existe`:** mejor caso O(1); promedio y peor caso O(log n). Usan el árbol.
- **`eliminar`:** O(log n) en todos los casos. Buscar en el árbol O(log n) + quitar del árbol O(log n) + quitar de la lista O(1).
- **`ordenarPorRiesgo`:** O(n log n) en todos los casos. Merge Sort mueve nodos completos sin copiar datos, así que el árbol sigue apuntando a los nodos correctos y no hay que actualizarlo.
- **`mostrar` y `mostrarPorId`:** O(n) en todos los casos.
- **`mostrarEnRiesgo`:** O(n log n) si primero hay que ordenar; si ya estaba ordenada, mejor caso O(1) y peor caso O(n).
- **`limpiar`:** O(n) en todos los casos.

**Clase `Alumno` (`Alumno.h`):** todas sus operaciones (`calcularFactorRiesgoServicio`, `nivelRiesgo`, `toCSV`, `mostrarInformacion`, getters, setters y el comparador `mayorRiesgo`) son **O(1) en todos los casos**, porque solo trabajan con los atributos del propio alumno.

#### Hace un análisis de complejidad correcto y completo para todos los demás componentes del programa y determina la complejidad final del programa.

**Archivos (`Archivos.h`)**

- **`aEntero` y `reemplazarArchivo`**
  - O(1) en todos los casos. Convierten un campo de longitud acotada o renombran un archivo.
- **`cargarAlumnos`**
  - Mejor caso: O(1). Promedio: O(n log n). Peor caso: O(n log n).
  - Leer y convertir cada línea es O(1); insertar cada alumno en el registro es O(log k), con k = alumnos ya cargados. log 1 + log 2 + … + log n = O(n log n). Mejor caso: archivo vacío.
- **`agregarAlumnoArchivo`**
  - O(1) en todos los casos. Se abre en modo *append*; no se reescribe el archivo.
- **`guardarAlumnos`**
  - O(n) en todos los casos. Recorrido inorden del árbol (ya sale ordenado por ID, no hay que ordenar) y una línea por alumno.
- **`generarReporteRiesgo`**
  - O(n log n) en todos los casos. Ordena por riesgo con Merge Sort, O(n log n), y escribe una línea por alumno, O(n).
- **`mostrarReporteRiesgo`**
  - Mejor caso: O(1). Promedio: O(n). Peor caso: O(n). Lee e imprime cada línea. Mejor caso: archivo vacío.

**Menú (`Menu.h`)**

- `leerEntero`, `leerTexto` y `pausa`: O(1) por cada intento del usuario.
- **1. Cargar:** limpiar O(n) + cargar O(n log n). Mejor caso O(1); promedio y peor caso O(n log n).
- **2. Mostrar:** O(n) en todos los casos.
- **3. Agregar:** O(log n) en todos los casos (validar e insertar en el árbol; escribir en el archivo es O(1)).
- **4. Buscar:** mejor caso O(1); promedio y peor caso O(log n).
- **5. Eliminar:** eliminar O(log n) + reescribir el archivo O(n) = O(n) en todos los casos.
- **6. Ordenar y mostrar por riesgo:** O(n log n) en todos los casos.
- **7. Mostrar por ID:** O(n) en todos los casos.
- **8. Alumnos en riesgo:** O(n log n) si hay que ordenar primero; si ya estaba ordenada, mejor caso O(1) y peor caso O(n).
- **9. Generar reporte:** O(n log n) en todos los casos.
- **10. Leer reporte:** O(n) en todos los casos.
- **11. Guardar:** O(n) en todos los casos.

**Complejidad final del programa**

- **Al iniciar**, el programa carga `alumnos.csv`: O(n log n).
- **Después**, cada opción del menú cuesta como máximo **O(n log n)** (cargar, ordenar por riesgo, ver alumnos en riesgo y generar el reporte). Ninguna opción cuesta más.
- Si el usuario ejecuta k opciones antes de salir, el programa completo cuesta O(n log n) de la carga inicial más k veces O(n log n): **O(k · n log n)** en el peor caso.
- Como cada operación está acotada por O(n log n), la **complejidad final del programa es O(n log n) por operación**, dominada por el ordenamiento con Merge Sort y por la carga del archivo al árbol AVL.
- **Memoria total:** O(n). La lista y el árbol guardan un nodo por alumno cada uno (los datos del alumno solo se guardan una vez, en la lista), y la recursión usa como máximo O(log n) de pila.

Sin el árbol AVL, la carga sería O(n²) y la complejidad final del programa también lo sería. Con el árbol, ninguna operación supera O(n log n).

### SICT0302: Toma decisiones

#### Selecciona un algoritmo de ordenamiento adecuado al problema y lo usa correctamente.

Antes de decidir, comparé con un análisis de complejidad el QuickSort que había usado en el primer avance contra `std::sort` de C++. También incluí Merge Sort, porque es el que se puede aplicar a una lista ligada.

- **Mi QuickSort** (pivote = último elemento, partición de Lomuto)
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: **O(n²)**.
  - Memoria extra: O(log n) en promedio, O(n) en el peor caso.
  - No es estable.
  - No funciona en una lista ligada, porque necesita acceso por índice.
- **`std::sort`**
  - Mejor caso: O(n log n). Promedio: O(n log n).
  - Peor caso: **O(n²)**. El estándar original de C++ (C++98/03) solo exigía que `std::sort` hiciera O(n log n) comparaciones **en promedio**, porque está basado en QuickSort; en el peor caso, cuando los pivotes quedan siempre en un extremo, hace O(n²) comparaciones. Desde C++11 el estándar exige O(n log n) también en el peor caso, y compiladores como GCC y Clang lo cumplen con **Introsort**: empieza como QuickSort y, si la recursión se hace demasiado profunda (señal de que se acerca al peor caso), cambia a HeapSort.
  - Memoria extra: O(log n).
  - No es estable.
  - No funciona en una lista ligada, porque necesita iteradores de acceso aleatorio.
- **Merge Sort en lista ligada**
  - Mejor caso: O(n log n). Promedio: O(n log n). Peor caso: O(n log n), garantizado por el propio algoritmo, sin depender de la versión del estándar ni del compilador.
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

**Decisión:** sí era necesario cambiar de algoritmo, porque mi QuickSort se degrada justo en los casos que más se repiten en este problema. `std::sort` no sirve porque solo funciona con estructuras de acceso aleatorio (`vector`, arreglos), su peor caso depende de la versión del estándar y no es estable. Como los datos están en una lista ligada, elegí **Merge Sort**, que:

- garantiza O(n log n) en el mejor, promedio y peor caso;
- en una lista ligada no necesita memoria auxiliar O(n), porque solo reconecta apuntadores;
- es **estable**, así que los alumnos con el mismo riesgo conservan su orden (además se desempata por ID).

**Uso correcto:** Merge Sort se usa en las opciones 6, 8 y 9 del menú, con el criterio `mayorRiesgo` (`Alumno.h`). Gracias a eso, la opción 8 puede detener el recorrido en cuanto encuentra al primer alumno que no supera el umbral. Ordenar por ID ya no hace falta, porque el árbol AVL entrega los alumnos en ese orden con su recorrido inorden.

**Aprendizaje:** una decisión sobre un algoritmo se toma comparando opciones, y el análisis de complejidad es uno de los criterios. Al comparar, vi que mi QuickSort no era la mejor opción para estos datos, que la estructura de datos también limita qué algoritmos se pueden usar, y que la complejidad de una función de biblioteca puede depender de la versión del estándar.

#### Selecciona una estructura de datos adecuada al problema y la usa correctamente.

El programa hace dos tipos de operaciones muy distintas, y por eso usa dos estructuras:

1. **Operaciones sobre todos los alumnos:** mostrarlos, ordenarlos por riesgo, filtrar a los que están en riesgo y generar el reporte.
2. **Operaciones sobre un solo alumno, por su ID:** buscarlo, validar que no esté repetido al cargar o agregar, y eliminarlo.

**Estructura lineal: lista doblemente ligada**, para las operaciones del tipo 1.

- Comparada con `std::vector`: insertar al final es O(1) siempre (el vector es O(1) amortizado, pero O(n) cuando realoca) y eliminar un nodo conocido es O(1) (el vector tiene que desplazar los elementos siguientes, O(n)).
- Recorrer es O(n) en cualquier estructura, así que la lista no pierde nada en las operaciones del tipo 1.
- Se ordena con Merge Sort sin arreglo auxiliar.

**Estructura no lineal: árbol AVL**, para las operaciones del tipo 2. Opciones consideradas:

- **Seguir buscando en la lista:** buscar, validar y eliminar cuestan O(n). Cargar n alumnos validando duplicados cuesta O(n²).
- **Árbol binario de búsqueda simple:** O(log n) en promedio, pero **O(n) en el peor caso**, cuando los IDs llegan ordenados (por ejemplo, al cargar un `alumnos.csv` que se guardó ordenado por ID con la opción 11). En ese caso el árbol queda como una lista de altura n.
- **Árbol AVL:** O(log n) en el peor caso para buscar, insertar y eliminar, porque se rebalancea con rotaciones después de cada cambio. Además, su recorrido inorden entrega a los alumnos ordenados por ID en O(n), sin ordenar nada.

Elegí el **árbol AVL**. Para comprobarlo, medí con 20,000 alumnos:

- **Cargar a los 20,000 alumnos validando IDs repetidos:** 400 ms buscando en la lista contra **6.9 ms** con el árbol AVL.
- **20,000 búsquedas por ID:** 385 ms en la lista contra **1.9 ms** en el árbol AVL.
- La altura del árbol con 20,000 alumnos fue **17**; log₂(20,000) ≈ 14.3, así que se mantiene dentro del límite teórico del AVL (≈ 1.44 log₂ n ≈ 20.6).

**Por qué el árbol es un índice y no una copia de los datos:** cada nodo del árbol guarda el ID y un apuntador al nodo de la lista. Así los datos de cada alumno existen una sola vez, la lista y el árbol nunca muestran información distinta, y al eliminar, el árbol entrega directamente el nodo que la lista debe desconectar en O(1).

**Uso correcto:**

- La clase `Registro` es la única que modifica las dos estructuras, y siempre lo hace en las dos al mismo tiempo, así que no pueden quedar desincronizadas.
- Merge Sort reacomoda los nodos de la lista sin copiar los datos, por lo que los apuntadores del árbol siguen siendo válidos después de ordenar.
- La lista mantiene `cabeza`, `cola` y `tam` consistentes en todas las operaciones, y el árbol actualiza la altura de cada nodo y se rebalancea después de cada inserción y eliminación.
- Los destructores liberan toda la memoria y la copia está deshabilitada para evitar liberar dos veces el mismo nodo.
- Se probó con 200,000 operaciones aleatorias de agregar, eliminar, buscar y ordenar, comparando cada resultado contra `std::set` de C++ y verificando que la altura del árbol se mantuviera dentro del límite del AVL, con *AddressSanitizer* y *UndefinedBehaviorSanitizer* activados y sin errores.

### SICT0303: Implementa acciones científicas

#### Implementa mecanismos para consultar información de las estructuras correctos.

Desde el menú (`Menu.h`), las estructuras se pueden consultar de varias formas:

- **Todos los alumnos** en el orden actual de la lista (opción 2, `Registro::mostrar`).
- **Un alumno por ID** (opción 4, `Registro::buscar`, que usa el árbol AVL), incluyendo cuántas horas le faltan.
- **Ordenados por riesgo** (opción 6, `Registro::ordenarPorRiesgo`, Merge Sort sobre la lista).
- **Ordenados por ID** (opción 7, `Registro::mostrarPorId`, recorrido inorden del árbol), junto con la altura del árbol.
- **Solo los alumnos en riesgo**, según un umbral que elige el usuario (opción 8, `Registro::mostrarEnRiesgo`). Aprovecha que la lista está ordenada para detenerse en cuanto un alumno ya no supera el umbral.
- **El reporte guardado**, leído desde el archivo (opción 10).

Además, las estructuras se pueden modificar: **agregar** (opción 3, el árbol valida que el ID no esté repetido) y **eliminar** (opción 5). Todas las entradas del usuario se validan, así que una consulta con un dato inválido no rompe el programa.

#### Implementa mecanismos de lectura de archivos para cargar datos a las estructuras de manera correcta.

- `cargarAlumnos` (`Archivos.h`) lee `alumnos.csv` línea por línea y, por cada alumno, crea un nodo en la lista y lo indexa en el árbol AVL (`Registro::agregar`).
- **Es robusta:** ignora el encabezado, las líneas vacías, las líneas incompletas y los valores no numéricos (usa conversión sin excepciones), quita el `\r` de los archivos guardados en Windows y **no inserta IDs repetidos**, lo que valida el árbol en O(log n).
- Al recargar (opción 1), las dos estructuras se vacían primero para no duplicar datos en memoria.
- **Se lee e imprime un archivo de resultados:** la opción 10 lee `alumnosRiesgo.csv` (`mostrarReporteRiesgo`) y lo muestra en pantalla.

#### Implementa mecanismos de escritura de archivos para guardar los datos de las estructuras de manera correcta.

El programa escribe tres veces en archivos, y cada una toma los datos de la estructura adecuada:

- **Agregar un alumno** (opción 3, `agregarAlumnoArchivo`): añade solo la línea del alumno nuevo al final de `alumnos.csv`, en modo *append*, O(1). Si el archivo no existe, lo crea con su encabezado.
- **Guardar a todos los alumnos** (opciones 5 y 11, `guardarAlumnos`): reescribe `alumnos.csv` con todos los alumnos en memoria, **ordenados por ID** gracias al recorrido inorden del árbol AVL, O(n). Así, después de eliminar un alumno, el archivo refleja exactamente lo que hay en memoria.
- **Generar el reporte** (opción 9, `generarReporteRiesgo`): ordena la lista por riesgo con Merge Sort y escribe `alumnosRiesgo.csv` con `id,nombre,numero_riesgo,nivel`, O(n log n).

Para que la escritura sea correcta y segura:

- **Escritura en archivo temporal:** `guardarAlumnos` y `generarReporteRiesgo` escriben primero en `alumnos.csv.tmp` o `alumnosRiesgo.csv.tmp`. Solo si toda la escritura terminó sin errores, el temporal reemplaza al original (`reemplazarArchivo`). Si algo falla a la mitad, se borra el temporal y el archivo original queda intacto.
- **Verificación:** después de cada escritura se cierra el archivo y se revisa que no haya fallado. Las funciones regresan cuántos alumnos escribieron, o -1 si hubo un error, y el menú le avisa al usuario en cualquiera de los dos casos.
- **Formato consistente:** todos los alumnos se escriben con `Alumno::toCSV()`, el mismo formato que lee `cargarAlumnos`, y no se permiten comas en los textos. Así, lo que el programa guarda siempre se puede volver a cargar.
