# Tarea 1 - CC4102 Diseño y anáñisis de algoritmos

Repositorio del código de la tarea 1 del ramo CC4102 Diseño y análisis de algoritmos, otoño 2024. 

La tarea consiste en la implementación de un M-Tree, que es un método de acceso dínamico, se incluyen dos métodos de construcción:
1. Método Ciaccia-Patella (CP): El método principal se encuentra en la carpeta `Metodo_SS` en el archivo `cp.cpp`, pero también se ocupan tres funciones auxiliares:
    - `Cluster.cpp`: Retorna un conjunto de clusters de tamaño entre b y B.
    - `OutputHoja.cpp`: Retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el radio cobertor y a la dirección del hijo respectivo.
    - `OutputInterno.cpp`: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.


2. Método Sexton-Swinback (SS): Se encuentra en el archivo `ss.cpp`

### Integrantes del grupo:
- Cristián ...
- Cristián Romero V.
- Iván Salas Aedo.
