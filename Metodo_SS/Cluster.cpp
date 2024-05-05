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

// Funciones auxiliares:

// Función para calcular la distancia euclidiana entre dos puntos
double euc_distance_alternative(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<0>(p2);
    return sqrt(dx * dx + dy * dy);
}

// Primera función: encontrarParesMasCercanos
/**
 * Función para encontrar los pares más cercanos de clusters en C.
 * Devuelve un par de índices que representan los clusters más cercanos.
 */
std::pair<size_t, size_t> encontrarParesMasCercanos(const std::vector<std::vector<puntosbd>>& C) {
    double minDistancia = std::numeric_limits<double>::max();
    std::pair<size_t, size_t> parMasCercano = {0, 1};
    
    // Iteramos a través de todos los clusters para encontrar el par más cercano
    for (size_t i = 0; i < C.size(); ++i) {
        for (size_t j = i + 1; j < C.size(); ++j) {
            double dist = euc_distance_alternative(C[i][0], C[j][0]); // Calcula la distancia entre los medoides
            if (dist < minDistancia) {
                minDistancia = dist;
                parMasCercano = {i, j};
            }
        }
    }
    return parMasCercano;
}

// Segunda función: removerCluster
/**
 * Función para remover un cluster de un conjunto de clusters.
 */
void removerCluster(std::vector<std::vector<puntosbd>>& C, size_t index) {
    C.erase(C.begin() + index);
}

// Tercera función: encontrarVecinoMasCercano
/**
 * Función para encontrar el vecino más cercano de un cluster en un conjunto de clusters.
 */
size_t encontrarVecinoMasCercano(const std::vector<puntosbd>& c, const std::vector<std::vector<puntosbd>>& Cout) {
    double minDistancia = std::numeric_limits<double>::max();
    size_t indiceVecino = 0;
    
    for (size_t i = 0; i < Cout.size(); ++i) {
        double dist = euc_distance_alternative(c[0], Cout[i][0]);
        if (dist < minDistancia) {
            minDistancia = dist;
            indiceVecino = i;
        }
    }
    return indiceVecino;
}

// Cuarta función: minMaxSplit
/**
 * Función para dividir un cluster en dos usando la política MinMax split.
 */
std::pair<std::vector<puntosbd>, std::vector<puntosbd>> minMaxSplit(const std::vector<puntosbd>& c) {
    // Implementa la lógica para dividir el cluster usando MinMax split
    // Aquí hay un ejemplo simple de división en dos partes (puedes personalizar esto):
    std::vector<puntosbd> c1, c2;
    
    for (size_t i = 0; i < c.size() / 2; ++i) {
        c1.push_back(c[i]);
    }
    
    for (size_t i = c.size() / 2; i < c.size(); ++i) {
        c2.push_back(c[i]);
    }
    
    return {c1, c2};
}


/* Instrucciones enunciado
Input: Un set de puntos C_in de tamaño mínimo b
    1. Se define Cout = {} y C = {}
    2. Por cada punto p ∈ Cin se añade {p} a C.
    3. Mientras |C| > 1:
    3.1 Sea c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|.
    3.2 Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
    3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
    4. Sea c el último elemento de C
    5. Si |Cout| > 0:
        5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de C_out
        5.2 Si no, se define c′ = {}.
    6. Si |c ∪ c′| ≤ B:
        6.1 Añadimos c ∪ c′ a Cout.
        6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
    7. Se retorna Cout
*/

/**
 * Función Cluster que retorna un conjunto de cluster de tamaño entre b y B.
 * @param puntos Un set de puntos C_in de tamaño mínimo b.
 * @return Un set de puntos C_out de tamaño entre b y B.
 */
std::vector<std::vector<puntosbd>> cluster(const std::vector<puntosbd>& puntos, size_t b, size_t B) {
    std::vector<std::vector<puntosbd>> Cout; // Inicializa un conjunto vacío para los clusters de salida
    std::vector<std::vector<puntosbd>> C; // Inicializa un conjunto vacío para los clusters en proceso

    // Añadir cada punto como un nuevo cluster a C
    for (const auto& p : puntos) {
        C.push_back({p});
    }

    // Mientras el número de clusters en C sea mayor que uno
    while (C.size() > 1) {
        // Encontrar los pares más cercanos de clusters en C
        auto [i, j] = encontrarParesMasCercanos(C);

        // Calcular el tamaño del cluster combinado
        size_t tamanoCombinado = C[i].size() + C[j].size();

        // Si el tamaño combinado es menor o igual a B, unir los clusters
        if (tamanoCombinado <= B) {
            // Combinar los dos clusters
            C[i].insert(C[i].end(), C[j].begin(), C[j].end());
            
            // Remover el segundo cluster ya que se ha combinado con el primero
            removerCluster(C, j);
        } else {
            // Si no se puede combinar, mover el primer cluster a Cout
            Cout.push_back(C[i]);
            removerCluster(C, i);
        }
    }

    // Procesar el último cluster restante en C si existe
    if (!C.empty()) {
        std::vector<puntosbd> c = C.back();
        size_t vecinoMasCercano = encontrarVecinoMasCercano(c, Cout);
        
        // Si hay clusters en Cout
        if (!Cout.empty()) {
            // Si el tamaño combinado es menor o igual a B, unir c con su vecino más cercano
            if (c.size() + Cout[vecinoMasCercano].size() <= B) {
                // Combinar c con su vecino más cercano
                Cout[vecinoMasCercano].insert(Cout[vecinoMasCercano].end(), c.begin(), c.end());
            } else {
                // Si no se puede combinar, dividir c usando MinMax split
                auto [c1, c2] = minMaxSplit(c);
                
                // Añadir los dos clusters resultantes a Cout
                Cout.push_back(c1);
                Cout.push_back(c2);
            }
        } else {
            // Si Cout está vacío, simplemente añadir c a Cout
            Cout.push_back(c);
        }
    }

    // Retornar el conjunto de clusters de salida
    return Cout;
}
