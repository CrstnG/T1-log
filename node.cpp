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
struct Entry{
  puntosbd point;
  double cr;
  Node* child;
};

struct Node{
  vector<Entry> keys;
  bool is_leaf = false;
  int height;
};

//Transforma los puntos a entradas y retorna un nodo
Node to_node(vector <puntosbd> points){
    Node n;
    for (int i=0;i < points.size();i++){
        puntosbd v = points[i];
        Entry e = {v,0.0,NULL};
        n.keys.push_back(e);
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
//función de redistribución (Punto 4).
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
vector <puntosbd> get_F(map <puntosbd, vector<puntosbd>> k_sets){
   vector <puntosbd> rd_points;
   for (const auto &par : k_sets){
      rd_points.push_back(par.first);
   }
   return rd_points;
}
//Función que guarda los puntos, recibe un connjunto de entrys. Rescata los  puntosbd de cada
//entry y los guarda en el vector points.
//Se usa en el punto 7.
vector <puntosbd> save_points(vector<Entry> entries){
  vector <puntosbd> points;
  //int entries_length = entries.size();
  for (const auto &entradas : entries){
    points.push_back(entradas.point);
  }
  return points;
}
//Función que guarda los arboles hijos, recibe un conjunto de entrys.
//Rescata los punteros de los aárboles hijos de cada entry y los guarda en el vector ptr_trees
vector <Node*> save_treesptr(vector<Entry> entries){
  vector <Node*> ptr_trees;
  //int entries_length = entries.size();
  for (const auto &entradas : entries){
    ptr_trees.push_back(entradas.child);
  }
  return ptr_trees;
}

//Función que calcula la altura de los árboles
int height(Node* tree){
    int tree_height = 0;
    if (tree->is_leaf == true){
        return tree_height;
    }
    else{
        int altura = 1 + height(tree->keys[0].child);
        tree->height = altura;
        //return 1 + height(tree->keys[0].child);
        return altura;
    }
}
//funcion que busca el sub-árbol con altura h
//recibe un puntero a un tree con una altura inicial j, y va  bajando en el árbol hasta encontrar con 
//un sub-árbol de altura h.
Node* search_h_height(Node* tree,int j,int h){
    if (j == h){
        return tree;
    }
    else{
        return search_h_height(tree->keys[0].child,j-1,h);
    }
    
}

//función que inserta un T_j en T_sup
//recibe el puntero a T_sup y un puntero a un Nodo.
void insert_tj_en_tsup(Node* tsup, Node* tj){
    vector<Entry> tsup_entry = tsup->keys;
    vector <Entry> tj_entry = tj -> keys;
    if(tsup->is_leaf == true){
        for (const auto &entry : tsup_entry){
          if (get<0>(entry.point) == get<0> (tj_entry[0].point) and get<1>(entry.point) == get<1> (tj_entry[0].point)){
            entry.child == tj;
            tsup->is_leaf == false;
            return;
          }
        }
    }
    else{
        for (const auto &entry : tsup_entry){
            if(get<0>(entry.point) == get<0> (tj_entry[0].point) and get<1>(entry.point) == get<1> (tj_entry[0].point)){
                insert_tj_en_tsup(entry.child,tj); 
            }
        }   
    }
}

//Recibe el puntero a T_sup y reciba la lista de punteros Nodos(trees)
//aplica la función insert_tj_en_tsup para insertar todos los T_j en T_sup.
void insert_all_tjs_en_tsup(Node* tsup, vector <Node*> tjs){
   for (const auto &t_j_s : tjs){
     insert_tj_en_tsup(tsup,t_j_s);
   } 
}

double get_max_cr(puntosbd point,Node* sub_tree){
   set <double> cr_s; //cr_s.insert()cr
   for (const auto &entry : sub_tree->keys){
      cr_s.insert(euc_distance(point, entry.point));
   }
   auto it = cr_s.rbegin(); //iterador al último elemento
   double cr = *it; //elijo mi último elemento
   return cr; //retorno mi cr
}

//Recibe un árbol con radios covertores = 0.
//Setea los radios covertores según corresponda.
void set_cr(Node* t_sup){
    if(t_sup->is_leaf){
        return;
    }
    else{
        //por cada entrada de las entradas (keys) de t_sup
        for (auto &entry : t_sup->keys){
            entry.cr = get_max_cr(entry.point,entry.child);
        }
        for (auto &entradas : t_sup->keys){
            set_cr(entradas.child);
        }
    }
}

//Constructor de M-Trees con el método de Ciacca-Patella.
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
        vector <puntosbd> conjunto_F = get_F(conjuntos_k);
        vector <Node*> tree_T_j;
        for (const auto &par : conjuntos_k){  //paso 6
            Node* child = &cp(par.second);
            tree_T_j.push_back(&cp(par.second));
        }
        int tree_T_j_length = tree_T_j.size();     //paso 7
        for (int i = 0; i < tree_T_j_length; i++){
            if (tree_T_j[i]->keys.size() < b){
                conjunto_F.erase(conjunto_F.begin() + i); //Eliminamos pf_j de F
                vector <puntosbd> entries = save_points(tree_T_j[i]->keys);
                for (const auto &new_entrada : entries){   // añadimos puntos al conjunto F
                  conjunto_F.push_back(new_entrada);
                }
                vector <Node*> tree_entries = save_treesptr(tree_T_j[i]->keys);
                for (const auto &new_entr : tree_entries){ //añadimos los sub arboles a tree_T_j
                  tree_T_j.push_back(new_entr);
                }     
                tree_T_j.erase(tree_T_j.begin() + i); //eliminamos la raíz.
            }
        }
        int h; // nuestra altura minima
        vector <int> heights;
        for (const auto &tree : tree_T_j){
            int tmp_height = height(tree);
            heights.push_back(tmp_height);
        }
        auto min_iter = min_element(heights.begin(), heights.end());
        h = *min_iter;
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           if(tree_T_j[j]->height == h){
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             Node* sub_node_h = search_h_height(tree_T_j[j],tree_T_j[j]->height,h);
             vector <Entry> sub_node_h_entries = sub_node_h->keys;
             for (const auto &sub_nod_entr : sub_node_h_entries){ //añadimos los sub arboles a tree_T_j
                  conjunto_F.push_back(sub_nod_entr.point);
                  tree_T_j.push_back(sub_nod_entr.child);
             }     
           }
        }
        Node t_sup = cp(conjunto_F); 
        insert_all_tjs_en_tsup(&t_sup,tree_T_j); //paso 11 algoritmo cp
        set_cr(&t_sup); //paso 12
        return t_sup;
    }
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