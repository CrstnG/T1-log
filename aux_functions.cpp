#include <cmath>
#include <tuple>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "Clases.cpp"

// double euc_distance(const puntosbd& p1, const puntosbd& p2) {
//     // Obtener las coordenadas x e y de cada punto
//     double x1, y1, x2, y2;
//     std::tie(x1, y1) = p1;
//     std::tie(x2, y2) = p2;

//     // Calcular las diferencias en las coordenadas
//     double dx = x2 - x1;
//     double dy = y2 - y1;

//     // Calcular la distancia euclidiana
//     return std::sqrt(dx * dx + dy * dy);
// }