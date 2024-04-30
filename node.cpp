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
//#include <find>

using namespace std;

int B = 3;


double euc_distance(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<0>(p2);
    return sqrt(dx * dx + dy * dy);
}

struct PointR{
    double x;
    double y;
    double cr;
};

struct Node{
  vector<PointR>keys;
  vector <Node*> child;
  bool is_leaf;
  int height;
};

//Transforma los puntos a entradas y retorna un nodo
Node to_node(vector <puntosbd> points){
    Node n;
    for (int i=0;i < points.size();i++){
        PointR v;
        v.x = get<0>(points[i]);
        v.y = get<1>(points[i]);
        v.cr = 0.0;
        n.keys.push_back(v);
        n.child.push_back(NULL);
        n.is_leaf = true;
        n.height = 0;
    }
    return n;
}
// Retorna un vector con k tuplas random
vector <puntosbd> random_p(vector <puntosbd> p,int k){
    vector <puntosbd> k_random;
    //int length = p.size();
    while (k > 0 && !p.empty()){
        int index = rand()% p.size(); //p.size es suficiente dado a que vamos variando el tamaño de p
        puntosbd r_tuple = p[index];
        k_random.push_back(r_tuple);
        p.erase(p.begin()+index);
        k--;
    }
    return k_random;
}

// Recibe un punto y un vector con los pivotes aleatorios y entrega el pivote aleatorio en el que la
// distancia euclidiana con el punto es menor
puntosbd search_key_closest(puntosbd tup, vector<puntosbd> l){
  vector <double> euclidean_d = vector<double>();
  int length = l.size(); 
  for (int i = 0; i < length; i++){
    double tmp_euc_d = euc_distance(tup,l[i]);
    euclidean_d.push_back(tmp_euc_d);
  }
  auto min_it = min_element(euclidean_d.begin(), euclidean_d.end()); //indice al elemento más pequeño
  //int min_index = find(euclidean_d.begin(),euclidean_d.end(),min_dist);
  int min_index = distance(euclidean_d.begin(), min_it);
  return l[min_index];
}
// Recibe los pivotes k random y todos los puntos, retornando un diccionario en que cada llave 
// corresponde a un pivote de los k random y su valor a un vector con los puntos en que su distancia
// euclidiana es menor respecto al resto de pivotes.
map <puntosbd, vector<puntosbd>> point_assign(const vector<puntosbd> &rand_points, const vector<puntosbd> &points){
    map <puntosbd, vector<puntosbd>> pivot_dict;
    //int k_length = rand_points.size();
    //int p_length = points.size();
    //vector<puntosbd> vec = NULL;
    for (const auto &pivote : rand_points){
      pivot_dict[pivote] = vector<puntosbd>();//listavacia;
    }
    for (const auto &punto : points){
      puntosbd key_dict = search_key_closest(punto,rand_points);
      pivot_dict[key_dict].push_back(punto);
    }
    return pivot_dict;
}

map <puntosbd, vector<puntosbd>> redistribution(vector <puntosbd> puntos){
    // estimar B y b){
    int b = 0.5*B;
    int n = puntos.size();
    
    int k = min(B,(n/B));
    vector <puntosbd> random_points = random_p(puntos, k);
    map <puntosbd, vector<puntosbd>> k_conjuntos = point_assign(random_points,puntos);
    int length_dict = k_conjuntos.size();
    for (int i = 0; i < length_dict; i++){
        if (k_conjuntos[random_points[i]].size() < b){
        //eliminar de los random points
        // acar el valor de la llave random_point[i]}
        puntosbd random_point_elim = random_points[i];
        vector <puntosbd> points_copy_k_elim = k_conjuntos[random_points[i]];
        random_points.erase(random_points.begin() + i);
        //iterar sobre ese conjunto reasignando dichos puntos
        map <puntosbd, vector<puntosbd>> k_elim_reassigned = point_assign(random_points,points_copy_k_elim);
        k_conjuntos.erase(random_point_elim);
        int k_elim_length = k_elim_reassigned.size();
        for (int j = 0; j < k_elim_length; j++){
            int k_elim_j_length = k_elim_reassigned[random_points[j]].size();
            for (int k = 0; k < k_elim_j_length; k++){
                k_conjuntos[random_points[j]].push_back(k_elim_reassigned[random_points[j]][k]);
            }
        }
        }
    }
    return k_conjuntos;
}

Node cp(vector <puntosbd> puntos){
    // estimar B y b
    int b = 0.5*B;
    int n = puntos.size();
    if (puntos.size() <= B){
        // retornar un arbol(NODE) con los puntos transformados como atributo
        Node t = to_node(puntos);
        return t;
    }
    else{
        map <puntosbd, vector<puntosbd>> conjuntos_k = redistribution(puntos);
        while (conjuntos_k.size() == 1){
            conjuntos_k = redistribution(puntos);
        }
        
    }
    return Node();
}

int main(){
	srand(time(0)); //semilla para reiniciar los valores aleatorios
	vector<puntosbd> puntos_test;
	for (int i = 0; i < 10; ++i) {
        puntosbd punto(i * 1.0, i * 1.0); //crea puntos con coordenadas (0,0), (1,1), (2,2), ...
        puntos_test.push_back(punto);
    }
    cout << "Los 10 puntos_test son:" << endl;
    for (const auto& punto : puntos_test) {
        cout << "(" << get<0>(punto) << ", " << get<1>(punto) << ")" << endl;
    }
    int indice = puntos_test.size(); //solo printeo  el tamaño del vector de puntos
    cout << "Tamaño de puntos_test:" << endl;
    cout << indice << endl;
    cout << "Los k_random son:" << endl;
    vector<puntosbd> k_randoms = random_p(puntos_test, 2); //pruebo que elija los k puntos al azar;
    for (const auto& punto_r : k_randoms) { //printeo los puntos elegidos
        cout << "(" << get<0>(punto_r) << ", " << get<1>(punto_r) << ")" << endl;
    }
    int i = 2;
    puntosbd punto_closest = search_key_closest(puntos_test[i], k_randoms);
    cout << "El punto más cercano de "<<"(" << get<0>(puntos_test[i]) << ", " << get<1>(puntos_test[i]) << ")" <<" es:" << endl;
    cout << "(" << get<0>(punto_closest) << ", " << get<1>(punto_closest) << ")" << endl;
    
    map <puntosbd, vector<puntosbd>> dic_assign = point_assign(k_randoms, puntos_test);
    
    for (const auto &par : dic_assign) {
        cout << "Pivote: (" << get<0>(par.first) << ", " << get<1>(par.first) << ")" << endl;
        cout << "Puntos asignados:";
        for (const auto &punto : par.second) {
            cout << " (" << get<0>(punto) << ", " << get<1>(punto) << ")";
        }
        cout << endl;
    }
    
    cout << "redistribution prueba::" << endl;
    map <puntosbd, vector<puntosbd>> dic_dist = redistribution(puntos_test);
    for (const auto &par : dic_dist) {
        cout << "Pivote_2: (" << get<0>(par.first) << ", " << get<1>(par.first) << ")" << endl;
        cout << "Puntos asignados:";
        for (const auto &punto : par.second) {
            cout << " (" << get<0>(punto) << ", " << get<1>(punto) << ")";
        }
        cout << endl;
    }

 
    return 0;
}
