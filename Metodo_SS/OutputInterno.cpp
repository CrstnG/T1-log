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
#include <memory>
using namespace std;

// Función para calcular la distancia euclidiana entre dos puntos
double euc_distance_alt_2(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<0>(p2);
    return sqrt(dx * dx + dy * dy);
}

/**
 * Función OutputInterno que retorna una tupla (G, R, A) donde G es el medoide primario del conjunto de puntos Cin,
 * R es el radio cobertor y A es la dirección del hijo respectivo.
 * @param Cmra Un conjunto de tuplas (g, r, a) retornadas por OutputHoja.
 * @return Una tupla (G, R, A) como Entry.
 */
Entry OutputInterno(const std::vector<Entry>& Cmra) {
    /* 1. Sea Cin = {g|∃(g, r, a) ∈ Cmra}. G el medoide primario de Cin. Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno). */
    std::vector<puntosbd> Cin; // Conjunto de puntos extraídos de Cmra
    for (const auto& [g, r, a] : Cmra) {
        Cin.push_back(g); // Extrae los puntos g de Cmra y los agrega a Cin
    }
    
    puntosbd G = Cin[0]; // Suponemos que el primer punto es el medoide primario, puedes usar una función personalizada
    double R = 0; // Inicializamos el radio cobertor a 0
    std::shared_ptr<Node> A = std::make_shared<Node>(); // Creamos un nuevo nodo interno

    /* 2. Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r) */
    for (const auto& [g, r, a] : Cmra) {
        double distancia = euc_distance_alt_2(G, g); // Calculamos la distancia entre G y g
        R = std::max(R, distancia + r); // Actualizamos el radio cobertor como el máximo entre R y distancia + r
        A->keys.push_back({g, r, a}); // Añadimos (g, r, a) a las claves del nodo interno A
    }

    /* 3. Guardamos el puntero a C como A. */
    // El puntero al nodo interno A se guarda en la variable A utilizada arriba

    /* 4. Retornamos (G, R, A) */
    return {G, R, A.get()};
}