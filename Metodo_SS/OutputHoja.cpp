#include "../node.h"
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <array>
#include <random>
#include <map>
#include <cmath>
using namespace std;

// Función para calcular la distancia euclidiana entre dos puntos
double euc_distance_alt(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<0>(p2);
    return sqrt(dx * dx + dy * dy);
}


/**
 * Función OutputHoja que retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es el radio cobertor y a la dirección del hijo respectivo.
 * @param Cin Un conjunto de puntos
 * @return Una tupla (g, r, a) como entry
 */
Entry OutputHoja(const vector<puntosbd>& Cin) {
    /* 1. Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja). */
    // Suponiendo que g es el primer punto en Cin, puedes personalizar esta lógica.
    puntosbd g = Cin[0]; // Medoide primario de Cin (puede cambiarse por tu lógica específica para obtener el medoide)
    double r = 0; // Radio cobertor inicializado a 0
    Node* a = new Node(); // Creamos un nuevo nodo hoja
    a->is_leaf = true; // El nodo es una hoja

    /* 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p)) */
    for (const auto& p : Cin) {
        double distancia = euc_distance_alt(g, p); // Calculamos la distancia entre g y p
        r = max(r, distancia); // Actualizamos el radio cobertor como el máximo entre r y la distancia

        // Creamos una entrada (punto, radio 0, nullptr) y la añadimos a las keys de a
        Entry entrada{p, 0, nullptr};
        a->keys.push_back(entrada);
    }

    /* 3. Guardamos el puntero a C como a */
    // El puntero al nodo hoja se ha guardado en la variable `a` que ya se declaró y utilizó arriba

    /* 4. Retornamos (g, r, a) */
    return Entry{g, r, a};
}