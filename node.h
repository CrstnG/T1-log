//include <bits/stdc++.h>
#include <tuple>
#include <array>
#include <vector>
using namespace std;
struct Entry;
struct Node;

/**
 * @typedef puntosbd
 * @brief Define un punto en un espacio bidimensional (2D).
 *
 * Esta definición utiliza un `std::tuple` de dos valores de tipo `double`
 * para representar un punto en un espacio 2D. El primer valor de la tupla
 * representa la coordenada `x` del punto, mientras que el segundo valor
 * representa la coordenada `y` del punto.
 */
typedef tuple<double, double> puntosbd;

/**
 * @struct Node
 * @brief Representa un nodo en un árbol de datos
 *
 * Un nodo contiene un vector de entradas (`keys`) que representan las claves
 * o valores almacenados en el nodo, así como información adicional sobre
 * la estructura del árbol y la posición del nodo en el árbol.
 */
struct Node{
  vector<Entry> keys;
  bool is_leaf = false;
  int height = 0;
};


/**
 * @struct Entry
 * @brief Representa una entrada en un nodo de un árbol de datos.
 * @param point Punto en un espacio bidimensional.
 * @param cr Radio cobertor.
 * @param child Puntero al nodo hijo.
 * Una entrada contiene información sobre un punto en un espacio bidimensional,
 * así como un valor asociado (`cr`) y un puntero a un nodo hijo.
 */
struct Entry{
  puntosbd point; // punto 2D
  double cr; // radio cobertor
  Node* child; // puntero al nodo hijo
};

double euc_distance(const puntosbd& p1, const puntosbd& p2) {
    // Obtener las coordenadas x e y de cada punto
    double x1, y1, x2, y2;
    std::tie(x1, y1) = p1;
    std::tie(x2, y2) = p2;

    // Calcular las diferencias en las coordenadas
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Calcular la distancia euclidiana
    return std::sqrt(dx * dx + dy * dy);
}





