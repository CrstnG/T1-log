# Tarea 1 - CC4102 Diseño y anáñisis de algoritmos

Repositorio del código de la tarea 1 del ramo CC4102 Diseño y análisis de algoritmos, otoño 2024. 

La tarea consiste en la implementación de un M-Tree, que es un método de acceso dínamico, se incluyen dos métodos de construcción:
1. Método Ciaccia-Patella (CP): El método principal se encuentra en el archivo `cp.cpp`.
   - `cp.cpp`: Posee la implementación del método Ciaccia-Patella.
   - `node.h`: Contiene las definiciones de tipo y estructuras utilizadas en `cp.cpp`.
   - `experiments.cpp`: Contiene los métodos para realizar la búsqueda y respectiva experimentación con distintos tipos de datos(al interior se encuentra el main solicitado).
    
### Ejecutar experiments.cpp;
   * Ingresar en la terminal `g++ -std=gnu++11 -g -o experiments experiments.cpp` para compilar y crear el ejecutable.
   * Ingresar en la terminal `./experiments` para ejecutar la experimentación.


### Instrucciones para el programa::
   * Al ejecutar, el programa le pedirá un input que será la potencia de 2 que se usará para crear los puntos.
   * Debe ingresar el input como un entero entre 10 y 20, pulsar enter.
   * El programa empezará a crear los puntos, el M-tree correspondiente y las 100 consultas(aleatorias).
   * Finalmente el programa irá imprimiendo las cantidades de accesos a discos y tiempo de ejecución de cada consulta, imprimiendo al final el tiempo total.
   * Tiene un solo uso, para probar con otro N, debe ejecutar de nuevo el programa.

### Integrantes del grupo:
- Cristián Galleguillos.
- Cristián Romero V.
- Iván Salas Aedo.
