#include "node.h"
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

/**
 * Función Cluster que retorna un conjunto de cluster de tamaño entre b y B.
 * @param puntos Un set de puntos C_in de tamaño mínimo b.
 * @return Un set de puntos C_out de tamaño entre b y B.
 */
vector <vector <puntosbd>> cluster(vector <puntosbd> puntos){
    vector <vector <puntosbd>> clusters;
    vector <puntosbd> cluster;
    int b = 2;
    int B = 4096/(sizeof(Entry)+sizeof(bool)+sizeof(int));
    int k = rand() % (B-b) + b;
    while(!puntos.empty()){
        if(cluster.size() == k){
            clusters.push_back(cluster);
            cluster.clear();
            k = rand() % (B-b) + b;
        }
        int index = rand()% puntos.size();
        puntosbd r_tuple = puntos[index];
        cluster.push_back(r_tuple);
        puntos.erase(puntos.begin()+index);
    }
    if(!cluster.empty()){
        clusters.push_back(cluster);
    }
    return clusters;

