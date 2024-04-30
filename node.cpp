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

int B = 500;

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
    int length = p.size();
    for (int i = 0; i < k; i++){
        int index = rand()%length+1;
        puntosbd r_tuple = p[index];
        k_random.push_back(r_tuple);
        p.erase(p.begin()+index);
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
map <puntosbd, vector<puntosbd>> point_assign(vector<puntosbd> rand_points, vector<puntosbd> points){
    map <puntosbd, vector<puntosbd>> pivot_dict;
    int k_length = rand_points.size();
    int p_length = points.size();
    //vector<puntosbd> vec = NULL;
    for (int i = 0; i < k_length; i++){
      pivot_dict[rand_points[i]] = vector<puntosbd>();//listavacia;
    }
    for (int j = 0; j < p_length; j++){
      puntosbd key_dict = search_key_closest(points[j],rand_points);
      pivot_dict[key_dict].push_back(points[j]);
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
}

int main(){
    cout << "hello" << endl;
    
}
