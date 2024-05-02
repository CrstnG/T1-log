#include <tuple>
#include <array>
#include <vector>
#include <cmath>
#include "node.h"
#include <random>
#include <algorithm>
#include <iostream>
using namespace std;


puntosbd generarPuntoAleatorio() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);
    return make_tuple(dis(gen), dis(gen));
}

vector<puntosbd> generarPuntosAleatoriosUnicos(int N) {
    vector<puntosbd> puntos;
    puntos.reserve(N);

    while (puntos.size() < N) {
        puntosbd nuevoPunto = generarPuntoAleatorio();

        // Verificar si el punto generado ya existe en el vector
        auto it = find_if(puntos.begin(), puntos.end(), [&nuevoPunto](const puntosbd& punto) {
            return punto == nuevoPunto;
        });

        // Si el punto no está en el vector, agregarlo
        if (it == puntos.end()) {
            puntos.push_back(nuevoPunto);
        }
    }

    return puntos;
}

int main() {
    int N = pow(2,18); // Cantidad de puntos aleatorios a generar
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    puntosbd punto = puntos[5];
    // Imprimir los puntos generados
    //for (const auto& punto : puntos) {
    cout << "(" << get<0>(punto) << ", " << get<1>(punto) << ")" << endl;
    //

    return 0;
}
