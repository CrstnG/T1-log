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



int B = 4096/(sizeof(Entry)+sizeof(bool)+sizeof(int));

double euc_distance(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<1>(p2);
    return sqrt(dx * dx + dy * dy);
}

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
    vector <int> unique_indexes;
    while (k > 0 && !p.empty()){
        int index = rand()% p.size(); //p.size es suficiente dado a que vamos variando el tamaño de p
        puntosbd r_tuple = p[index];
        k_random.push_back(r_tuple);
        p.erase(p.begin()+index);
        unique_indexes.push_back(index);
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
map <puntosbd, vector<puntosbd>> point_assign(vector<puntosbd> rand_points, vector<puntosbd> points){
    map <puntosbd, vector<puntosbd>> pivot_dict;
    //int k_length = rand_points.size();
    //int p_length = points.size();
    //vector<puntosbd> vec = NULL;
    for (auto &pivote : rand_points){
      pivot_dict[pivote] = vector<puntosbd>();//listavacia;
    }
    for (auto &punto : points){
      puntosbd key_dict = search_key_closest(punto,rand_points);
      pivot_dict[key_dict].push_back(punto);
    }
    return pivot_dict;
}
//función de redistribución (Punto 4).
map <puntosbd, vector<puntosbd>> redistribution(vector <puntosbd> puntos){
    // estimar B y b){
    cout << "inicia función de redistribución"<<endl;
    int b = 0.5*B;
    int n = puntos.size();
    cout << "largo de entrada:"<<endl;
    cout << n << endl;
    int k = min(B,(n/B));
    if (k == 1){
        k += 1;
    }
    cout << "minimo es:" << endl;
    cout << k << endl;
    vector <puntosbd> random_points = random_p(puntos, k);
    map <puntosbd, vector<puntosbd>> k_conjuntos = point_assign(random_points,puntos);
    int length_dict = k_conjuntos.size();
    for (int i = 0; i < length_dict; i++){
        if (k_conjuntos[random_points[i]].size() < b){
        //eliminar de los random points
        // acar el valor de la llave random_point[i]}
        puntosbd random_point_elim = random_points[i];
        vector <puntosbd> points_copy_k_elim = k_conjuntos[random_points[i]];  //REVISAR EL INDICE QUE BORRAMOS PQ NO ES EL MISMO ORDEN QUE EL DICCIONARIO
        random_points.erase(random_points.begin() + i);         
        //iterar sobre ese conjunto reasignando dichos puntos
        map <puntosbd, vector<puntosbd>> k_elim_reassigned = point_assign(random_points,points_copy_k_elim);
        k_conjuntos.erase(random_point_elim);
        int k_elim_length = k_elim_reassigned.size();
        for (int j = 0; j < k_elim_length; j++){
            int k_elim_j_length = k_elim_reassigned[random_points[j]].size();
            for (int u = 0; u < k_elim_j_length; u++){
                k_conjuntos[random_points[j]].push_back(k_elim_reassigned[random_points[j]][u]);
            }
        }
        }
    }
    return k_conjuntos;
}
vector <puntosbd> get_F(map <puntosbd, vector<puntosbd>> k_sets){
   vector <puntosbd> rd_points;
   for (auto &par : k_sets){
      cout << "Las coordenadas en get_f son: (" << get<0>(par.first) << ", " << get<1>(par.first) << ")" << endl;
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
  for (auto &entradas : entries){
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
    cout << "Entrando a insert_tj_en_tsup" << endl;
    vector<Entry> tsup_entry = tsup->keys;
    cout << "obteniendo llaves de tsup" << endl;
    vector <Entry> tj_entry = tj->keys;
    cout << "obteniendo llaves de tj" << endl;
    if(tsup->is_leaf == true){
        cout << "tsup es hoja" << endl;
        for (auto &entry : tsup_entry){
          if (get<0>(entry.point) == get<0> (tj_entry[0].point) and get<1>(entry.point) == get<1> (tj_entry[0].point)){
            entry.child = tj;
            tsup->is_leaf = false;
            return;
          }
        }
    }
    else{
        cout << "tsup no es hoja" << endl;
        for (auto &entry : tsup_entry){
            if(get<0>(entry.point) == get<0> (tj_entry[0].point) and get<1>(entry.point) == get<1> (tj_entry[0].point)){
                cout << "voy al hijo (recursión)" << endl;
                insert_tj_en_tsup(entry.child,tj); 
            }
        }   
    }
}

//Paso 11
//Recibe el puntero a T_sup y reciba la lista de punteros Nodos(trees)
//aplica la función insert_tj_en_tsup para insertar todos los T_j en T_sup.
void insert_all_tjs_en_tsup(Node* tsup, vector <Node*> tjs){
   cout << "Entrando a insert_all_tjs" << endl;
   int tjs_length = tjs.size();
   //puntosbd a = tjs[2]->keys[1].point;
   //double x = get<0>(a);
   //double y = get<1>(a);
   //cout <<  x << endl;
   //cout <<  y << endl;
   //for (auto &t_j_s : tjs){
   for(int i = 0; i < tjs_length;i++){
     insert_tj_en_tsup(tsup,tjs[i]);
   } 
}

double get_max_cr(puntosbd point,Node* sub_tree){
   set <double> cr_s; //cr_s.insert()cr
   for (auto &entry : sub_tree->keys){
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
//funcion que imprime los puntos de las entradas 
void print_tjs(vector <Node*> t_j_s){
    //int i = 0;
    cout << "entrando a print_tjs" << endl;
    int tjs_length = t_j_s.size();
    for(int i = 0;i<tjs_length;i++){
        cout <<"entrando al for de i"<<endl;
      int j = t_j_s[i]->keys.size();
      for (int u = 0; u < j; u++){
        puntosbd tmp_point_2 = t_j_s[i]->keys[u].point;
        double coorx2 = get<0>(tmp_point_2);
        double coory2 = get<1>(tmp_point_2);
        cout << "i fors: (" << i << ")" << endl;
        cout << "Las coordenadas son: (" << coorx2 << ", " << coory2 << ")" << endl;
      }
    cout << "saliendo de print_tjs"<<endl;
   }
}

//funcion que imprime los puntos bd de un conjunto conjuntos_k
void print_conj_k(vector<puntosbd> c_k){
    for(auto &param: c_k){
        double coorx2 = get<0>(param);
        double coory2 = get<1>(param);
        cout << "Las coordenadas son: (" << coorx2 << ", " << coory2 << ")" << endl;
    }
    cout << "termine uno de los conjuntos"<< endl;
}

//Constructor de M-Trees con el método de Ciacca-Patella.
Node cp(vector <puntosbd> puntos){
    // estimar B y b
    cout << "empezando cp" << endl;
    int b = 0.5*B;
    int n = puntos.size();
    if (n <= B){
        // retornar un arbol(NODE) con los puntos transformados como atributo.
        cout << "llegando al caso base" << endl;
        Node t = to_node(puntos);
        //imprimir coordenadas para comprobar.
        //vector<Entry> Entradas = t.keys;
        //for (auto &entries : Entradas){
            //puntosbd punto  = entries.point;
            //double x = get<0>(punto);
            //double y = get<1>(punto);
          //  cout << "Las coordenadas en caso baseNODE son: (" << x << ", " << y << ")" << endl;
        //}
        return t;
    }
    else{
        cout << "haciendo redistribucion" << endl;
        map <puntosbd, vector<puntosbd>> conjuntos_k = redistribution(puntos);
        cout << "redistribución temrinada" << endl;
        int conjuntos_k_length = conjuntos_k.size();
        cout << "Largo antes del while;" << endl;
        cout << conjuntos_k_length << endl;
        while (conjuntos_k_length == 1){
            cout << "conjunto k igual a 1" << endl;
            cout << "iniciando redistribucion de nuevo" << endl; 
            conjuntos_k = redistribution(puntos);
            conjuntos_k_length = conjuntos_k.size();
            cout << "Largo despues del while" << endl;
            cout << conjuntos_k_length << endl;
        }
        cout << "obteniendo conjunto F random points" << endl;
        vector <puntosbd> conjunto_F = get_F(conjuntos_k);
        cout << "obtencion lista" << endl;
        vector <Node*> tree_T_j;
        int conjuntof_length = conjunto_F.size();
        for(int g = 0; g < conjuntof_length; g++){
        //for (auto &par : conjuntos_k){  //paso 6
            cout << "comenzando recursion para obtener T_j" << endl;
            //Node tmp_node = cp(par.second);
            //print_conj_k(conjuntos_k[conjunto_F[g]]);
            Node tmp_node = cp(conjuntos_k[conjunto_F[g]]);
            //puntosbd tmp_point = tmp_node.keys[0].point;
            //double coorx = get<0>(tmp_point);
            //double coory = get<1>(tmp_point);
            //cout << "Las coordenadas para la recursion tj son: (" << coorx << ", " << coory << ")" << endl;
            //cout << "recursion hecha" << endl;
            Node* child = &tmp_node;
            //puntosbd tmp_point_2 = child->keys[0].point;
            //double coorx2 = get<0>(tmp_point_2);
            //double coory2 = get<1>(tmp_point_2);
            //cout << "Las coordenadas para la recursion tj son: (" << coorx2 << ", " << coory2 << ")" << endl;
            //cout << "recursion hecha" << endl;
            tree_T_j.push_back(child);
            cout << "Las coordenadas para verificar:" << endl;
            print_tjs(tree_T_j);
        }
        cout << "Iniciando paso 7" << endl;
        print_tjs(tree_T_j);
        int tree_T_j_length = tree_T_j.size();     //paso 7
        for (int i = 0; i < tree_T_j_length; i++){
            if (tree_T_j[i]->keys.size() < b){
                conjunto_F.erase(conjunto_F.begin() + i); //Eliminamos pf_j de F
                vector <puntosbd> entries = save_points(tree_T_j[i]->keys);
                for (auto &new_entrada : entries){   // añadimos puntos al conjunto F
                  double new_coor_x = get<0>(new_entrada);
                  double new_coor_y = get<1>(new_entrada);
                  cout << "Las coordenadas adentro del paso 7 despues de save points son: (" << new_coor_x << ", " << new_coor_y<< ")" << endl;
                  conjunto_F.push_back(new_entrada);
                }
                vector <Node*> tree_entries = save_treesptr(tree_T_j[i]->keys);
                for (auto &new_entr : tree_entries){ //añadimos los sub arboles a tree_T_j
                  tree_T_j.push_back(new_entr);
                }     
                tree_T_j.erase(tree_T_j.begin() + i); //eliminamos la raíz.
            }
        }
        cout << "terminando paso 7" << endl;
        print_tjs(tree_T_j);
        int h; // nuestra altura minima
        vector <int> heights;
        for (auto &tree : tree_T_j){
            int tmp_height = height(tree);
            heights.push_back(tmp_height);
        }
        auto min_iter = min_element(heights.begin(), heights.end());
        h = *min_iter;
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        cout << "Iniciando paso 9" << endl;
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           if(tree_T_j[j]->height == h){
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             Node* sub_node_h = search_h_height(tree_T_j[j],tree_T_j[j]->height,h);
             vector <Entry> sub_node_h_entries = sub_node_h->keys;
             for (auto &sub_nod_entr : sub_node_h_entries){ //añadimos los sub arboles a tree_T_j
                  conjunto_F.push_back(sub_nod_entr.point);
                  tree_T_j.push_back(sub_nod_entr.child);
             }     
           }
        }
        Node t_sup = cp(conjunto_F); 
        cout << "Iniciando paso 11" << endl;
        insert_all_tjs_en_tsup(&t_sup,tree_T_j); //paso 11 algoritmo cp
        cout << "Iniciando paso 12" << endl;
        set_cr(&t_sup); //paso 12
        cout << "finish"<<endl;
        return t_sup;
    }
}

int main(){
    cout << B << endl;
    int N = pow(2,6);
    
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    cout << "cree los puntos" << endl;
    vector <puntosbd> random_points = random_p(puntos, 5);
    for (int i =0; i<5;i++){
        double x = get<0>(random_points[i]);
        double y = get<1>(random_points[i]);
        cout << "Las coordenadas al azar: (" << x << ", " << y << ")" << endl;
    }
    map <puntosbd, vector<puntosbd>> dic_test = point_assign(random_points, puntos);
    int total = 0;
    for (auto &llave : dic_test){
        cout << "Llave nueva" << endl;
        double l1 = get<0>(llave.first);
        double l2 = get<1>(llave.first);
        cout << "La LLave es: (" << l1 << ", " << l2 << ")" << endl;
        for (auto &conjunto: llave.second){
            double x = get<0>(conjunto);
            double y = get<1>(conjunto);
            cout << "Las coordenadas de cada conjunto: (" << x << ", " << y << ")" << endl;
        }
        total = total + llave.second.size();
    }
    cout << "Largo total" << endl;
    cout << total << endl;

    double dist = euc_distance(puntos[5], puntos[5]);
    cout << "dist entre 2 iguales" << endl;
    cout << dist << endl;
    
    puntosbd puntodos = puntos[2];
    double c= get<0>(puntodos);
    double d= get<1>(puntodos);
    cout << "punto 2  : (" << c<< ", " << d << ")" << endl;

    puntosbd closest = search_key_closest(puntos[2], puntos);
    double a= get<0>(closest);
    double b= get<1>(closest);
    cout << "el mas cercano  : (" << a << ", " << b << ")" << endl;

    vector <puntosbd>  Efes = get_F(dic_test);

    return 0;
   
}