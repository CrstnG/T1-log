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
 *
 * Una entrada contiene información sobre un punto en un espacio bidimensional,
 * así como un valor asociado (`cr`) y un puntero a un nodo hijo.
 */
struct Entry{
  puntosbd point; // punto 2D
  double cr; // radio cobertor
  Node* child; // puntero al nodo hijo
};





