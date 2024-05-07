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
    cout << "LOS RANDOM POINTS ANTES DE LA CONDICIONAL"<<endl;
    for (auto &rand : random_points){
        cout<<"randompoints antes del if: (" << get<0>(rand) << ", " << get<1>(rand) << ")" << endl;
    }
    map <puntosbd, vector<puntosbd>> k_conjuntos = point_assign(random_points,puntos);
    int length_dict = k_conjuntos.size();
    for (int i = 0; i < length_dict; i++){
        if (k_conjuntos[random_points[i]].size() < b){
        //eliminar de los random points
        // sacar el valor de la llave random_point[i]}
        puntosbd random_point_elim = random_points[i];
        vector <puntosbd> points_copy_k_elim = k_conjuntos[random_points[i]];  
        random_points.erase(random_points.begin() + i);         
        //iterar sobre ese conjunto reasignando dichos puntos
        map <puntosbd, vector<puntosbd>> k_elim_reassigned = point_assign(random_points,points_copy_k_elim);
        k_conjuntos.erase(random_point_elim); 
        int k_elim_length = k_elim_reassigned.size(); //largo del diccionario reasignado
        for (int j = 0; j < k_elim_length; j++){
            int k_elim_j_length = k_elim_reassigned[random_points[j]].size();//cada llave del diccionario reasignado
            for (int u = 0; u < k_elim_j_length; u++){
                k_conjuntos[random_points[j]].push_back(k_elim_reassigned[random_points[j]][u]);
            }
        }
        }
    }
    cout << "LOS RANDOM POINTS DESPUES DE LA CONDICIONAL"<<endl;
    for (auto &rand : random_points){
        cout<<"randompoints DESPUES del if: (" << get<0>(rand) << ", " << get<1>(rand) << ")" << endl;
    }
    for (auto &llave : k_conjuntos){
        //puntosbd clave = llave.first;
        //cout<<"Las coordenadas en las claves k_conjunto en redistribution son: (" << get<0>(llave.first) << ", " << get<1>(llave.first) << ")" << endl;
        //cout << "las coordenadas de los valores de las claves son:"<< endl;
        //cout << "el largo del valor de la llave es"<<endl;
        int size_k_c = k_conjuntos[llave.first].size();
        cout <<size_k_c<<endl;
        //for (auto &val : k_conjuntos[llave.first]){
         //   int size_k_c = k_conjuntos[llave.first].size();
            //cout << "Las coordenadas en los valores k_conjunto en redistribution son: (" << get<0>(val) << ", " << get<1>(val) << ")" << endl;
          //  cout << "el largo del valor de la llave es"<<endl;
           // cout <<size_k_c<<endl;
        //}
    }
    return k_conjuntos;
}
vector <puntosbd> get_F(map <puntosbd, vector<puntosbd>> k_sets){
   vector <puntosbd> rd_points;
   for (auto &par : k_sets){
      //cout << "Las coordenadas en get_f son: (" << get<0>(par.first) << ", " << get<1>(par.first) << ")" << endl;
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
    //int tree_height = 0;
    //if (tree->is_leaf == true){
        //cout << "CASO BASE HEIGHT" << endl;
        //return tree_height;
   // }
   // else{
        //cout << "CASO RECURSIVO HEIGHT" << endl;
        //int altura = 1 + height(tree->keys[0].child);
        //tree->height = altura;
        //return 1 + height(tree->keys[0].child);
        //return altura;
    //}
    if (tree->height == 0){
        cout << "CASO BASE HEIGHT" << endl;
        return 0;
    }
    else{
        cout << "CASO RECURSIVO HEIGHT" << endl;
        //int altura = 1 + height(tree->keys[0].child);
        //tree->height = altura;
        tree->height += 2 + height(tree->keys[0].child);
        int altura  = tree->height;
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
void insert_tj_en_tsup(Node* tsup, Node* tj,puntosbd pfj){ 
    cout << "Entrando a insert_tj_en_tsup" << endl;
    vector<Entry> tsup_entry = tsup->keys;
    cout << "obteniendo llaves de tsup" << endl;
    vector <Entry> tj_entry = tj->keys;
    cout << "obteniendo llaves de tj" << endl;
    if(tsup->is_leaf == true){
        cout << "tsup es hoja" << endl;
        for (auto &entry : tsup_entry){
          int tj_entry_length_leaf = tj->keys.size();
          if (get<0>(entry.point) == get<0> (pfj) and get<1>(entry.point) == get<1> (pfj)){
            entry.child = tj;
            tsup->is_leaf = false;
          }
        }
    }
    else{
        cout << "tsup no es hoja" << endl;
        for (auto &entry : tsup_entry){
            int tj_entry_length = tj->keys.size();
            if(get<0>(entry.point) == get<0> (pfj) and get<1>(entry.point) == get<1> (pfj)){
                //preguntar si es NULL, le asignamos el hijo 
                //else hacemos recursión
                if(entry.child == NULL){
                    entry.child = tj;
                }
                else{
                    insert_tj_en_tsup(entry.child,tj,pfj); 
                }
            }
        }   
    }
}

//Paso 11
//Recibe el puntero a T_sup y reciba la lista de punteros Nodos(trees)
//aplica la función insert_tj_en_tsup para insertar todos los T_j en T_sup.
void insert_all_tjs_en_tsup(Node* tsup, vector <Node*> tjs, vector<puntosbd> con_f){
   cout << "Entrando a insert_all_tjs" << endl;
   cout << "Entrando a insert_all_tjs CON LAAAAAAAAAAAAAAAAAAARGO" << endl;
   
   int tjs_length = tjs.size();
   cout << tjs_length << endl;
   //puntosbd a = tjs[2]->keys[1].point;
   //double x = get<0>(a);
   //double y = get<1>(a);
   //cout <<  x << endl;
   //cout <<  y << endl;
   //for (auto &t_j_s : tjs){
   for(int i = 0; i < tjs_length;i++){
     double test = get<0>(tjs[i]->keys[0].point);
     cout << "Entrando a insert_all_tjs 22222222222" << endl;
     cout << test << endl;
     insert_tj_en_tsup(tsup,tjs[i],con_f[i]);
   } 
}

double get_max_cr(puntosbd point,Node* sub_tree){
   set <double> cr_s; //cr_s.insert()cr
   cout << "ENTRANDO A GET_MAX_CR" << endl;
   for (auto &entry : sub_tree->keys){
      cout << "ENTRANDO A FOR DE GET_MAX_CR" << endl;
      cr_s.insert(euc_distance(point, entry.point));
   }
   auto it = cr_s.rbegin(); //iterador al último elemento
   double cr = *it; //elijo mi último elemento
   return cr; //retorno mi cr
}

//Recibe un árbol con radios covertores = 0.
//Setea los radios covertores según corresponda.
void set_cr(Node* t_sup){
    cout << "Entré a set_cr"<<endl;
    if(t_sup->is_leaf){
        return;
    }
    else{
        //por cada entrada de las entradas (keys) de t_sup
        for (auto &entry : t_sup->keys){
            cout << " cada entrada de las entradas (keys) de t_sup" << endl;
            entry.cr = get_max_cr(entry.point,entry.child);
        }
        //for (auto &entradas : t_sup->keys){
          //  cout << "SETEAAAANDO LOS RADIO COBERTORES" << endl;
            //set_cr(entradas.child);
        //}
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
        vector<Node>tree_T_j_nodes;
        vector <Node*> tree_T_j;
        int conjuntof_length = conjunto_F.size();
        for(int g = 0; g < conjuntof_length; g++){
        //for (auto &par : conjuntos_k){  //paso 6
            cout << "comenzando recursion para obtener T_j" << endl;
            //Node tmp_node = cp(par.second);
            //print_conj_k(conjuntos_k[conjunto_F[g]]);
            cout << "PUNTOS QUE SE PASAN AL CP" << endl;
            cout << "EL PRIMER PUNTO QUE PASA A CP: (" << get<0>(conjuntos_k[conjunto_F[g]][0]) << ", " << get<1>(conjuntos_k[conjunto_F[g]][0]) << ")" << endl;
            cout << "EL SEGUNDO PUNTO QUE PASA A CP: (" << get<0>(conjuntos_k[conjunto_F[g]][1]) << ", " << get<1>(conjuntos_k[conjunto_F[g]][1])<< ")" << endl;
            cout << "EL TERCER PUNTO QUE PASA A CP: (" << get<0>(conjuntos_k[conjunto_F[g]][2]) << ", " << get<1>(conjuntos_k[conjunto_F[g]][2]) << ")" << endl;
            Node tmp_node = cp(conjuntos_k[conjunto_F[g]]);
            //puntosbd tmp_point = tmp_node.keys[0].point;
            //double coorx = get<0>(tmp_point);
            //double coory = get<1>(tmp_point);
            //cout << "Las coordenadas para la recursion tj son: (" << coorx << ", " << coory << ")" << endl;
            //cout << "recursion hecha" << endl;
            //Node* child = &tmp_node;
            //puntosbd tmp_point_2 = child->keys[0].point;
            //double coorx2 = get<0>(tmp_point_2);
            //double coory2 = get<1>(tmp_point_2);
            //cout << "Las coordenadas para la recursion tj son: (" << coorx2 << ", " << coory2 << ")" << endl;
            //cout << "recursion hecha" << endl;
            tree_T_j_nodes.push_back(tmp_node);
            cout << "Las coordenadas para verificar:" << endl;
            //print_tjs(tree_T_j);
        }
        for (auto &nodo : tree_T_j_nodes){
            tree_T_j.push_back(&nodo);
        }
        cout << "Iniciando paso 7" << endl;
        int largo_tj = tree_T_j.size();
        cout << "largo del tre_tj"<<endl;
        cout << largo_tj << endl;
        //print_tjs(tree_T_j);
        int tree_T_j_length = tree_T_j.size();     //paso 7
        vector <int> tree_T_j_indexes_elim;
        for (int i = 0; i < tree_T_j_length; i++){
            int length_arbol_keys = tree_T_j[i]->keys.size();
            cout << "Largo del arbol: "<<endl;
            cout << length_arbol_keys <<endl;
            if (tree_T_j[i]->keys.size() < b){
                 cout << "Entrando al if del paso 7 con: "<< tree_T_j[i]->keys.size()<<""<<endl;
                conjunto_F.erase(conjunto_F.begin() + i); //Eliminamos pf_j de F
                vector <puntosbd> entries = save_points(tree_T_j[i]->keys);
                for (auto &new_entrada : entries){   // añadimos puntos al conjunto F
                  cout << "Entrando al primer for del paso 7 "<<endl;
                  //double new_coor_x = get<0>(new_entrada);
                  //double new_coor_y = get<1>(new_entrada);
                  //cout << "Las coordenadas adentro del paso 7 despues de save points son: (" << new_coor_x << ", " << new_coor_y<< ")" << endl;
                  conjunto_F.push_back(new_entrada);
                }
                //vector <Node*> tree_entries = save_treesptr(tree_T_j[i]->keys);
                //for (auto &new_entr : tree_entries){ //añadimos los sub arboles a tree_T_j
                //  cout << "Entrando al segundo for del paso 7 "<<endl;
                //  tree_T_j.push_back(new_entr);
                //}
                tree_T_j_indexes_elim.push_back(i);     
                //tree_T_j.erase(tree_T_j.begin() + i); //eliminamos la raíz.
            }
        }
        if(tree_T_j_indexes_elim.empty() == false){
          cout << "Entrando AL IF DE LA LINEA 459 "<<endl;
          for (auto &idx : tree_T_j_indexes_elim){
            for(auto &idtree : tree_T_j[idx]->keys){
                tree_T_j.push_back(idtree.child);
                if (idtree.child == NULL){
                    cout << "ARBOL NULOOOOOOOOOOOOOOOOOO AAAAAAAAAAAAAAAAAAAAAAAAAAAA "<<endl;
                }
            }
          }
          for (auto &idx : tree_T_j_indexes_elim){
            tree_T_j.erase(tree_T_j.begin() + idx);
           }
        }
        cout << "terminando paso 7" << endl;
        int tree_size = tree_T_j.size();
        cout <<"largo final"<<endl;
        cout << tree_size<< endl;
        //print_tjs(tree_T_j);

        int h; // nuestra altura minima
        vector <int> heights;
        cout <<"largo de heigths"<<endl;
        cout<<tree_size<<endl;
        for (auto &tree : tree_T_j){
            cout<<"entré al for luego de calcular height"<<endl;
            int tmp_height = height(tree);
            cout << tmp_height <<endl;
            heights.push_back(tmp_height);
            cout << "paso el push de tmp_heigth"<<endl;
        }
        cout<<"sali del for de heights"<<endl;
        auto min_iter = min_element(heights.begin(), heights.end());
        h = *min_iter;
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        cout << "Iniciando paso 9" << endl;
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           //cout << "ENTRAMOS FOR PASO 9 VEAMOS EL LARGO DE LISTA TREE_T_J" << endl;
           //cout << tree_T_j_length << endl;
           if(tree_T_j[j]->height == h){
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             Node* sub_node_h = search_h_height(tree_T_j[j],tree_T_j[j]->height,h);
             vector <Entry> sub_node_h_entries = sub_node_h->keys;
             for (auto &sub_nod_entr : sub_node_h_entries){ //añadimos los sub arboles a tree_T_j
                  cout << "FOR ANIDADO PASO 9" << endl;
                  conjunto_F.push_back(sub_nod_entr.point);
                  tree_T_j.push_back(sub_nod_entr.child);
             }     
           }
        }
        Node t_sup = cp(conjunto_F); 
        cout << "Iniciando paso 11" << endl;
        int con_f_len = conjunto_F.size();
        int subtreees_len = tree_T_j.size();
        cout << "Largo de conjunto F final: (" << con_f_len << ")" << endl;
        cout << "Largo de tree T j final: (" << subtreees_len << ")" << endl;
        insert_all_tjs_en_tsup(&t_sup,tree_T_j,conjunto_F); //paso 11 algoritmo cp
        cout << "Iniciando paso 12" << endl;
        //set_cr(&t_sup); //paso 12
        cout << "finish"<<endl;
        return t_sup;
    }
}


int main(){
    cout << B << endl;
    int N = pow(2,10);
    
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    cout << "cree los puntos" << endl;
    vector <puntosbd> random_points = random_p(puntos, 5);
    map <puntosbd, vector<puntosbd>> dic_test = point_assign(random_points, puntos);
    int total = 0;

    double dist = euc_distance(puntos[5], puntos[5]);
    
    puntosbd puntodos = puntos[2];

    puntosbd closest = search_key_closest(puntos[2], puntos);
    double a= get<0>(closest);
    double b= get<1>(closest);
    //vector <puntosbd>  Efes = get_F(dic_test);

    map <puntosbd, vector<puntosbd>> redist = redistribution(puntos);
    //vector <puntosbd>  Efes2 = get_F(redist);

    //Node tree = cp(puntos);
    cout <<"test altura"<<endl;
    Node tree;
    Node* tree_ptr = &tree;
    cout <<"test altura1"<<endl;
    cout <<tree.height<<endl;
    vector<Entry> Entradas1;
    cout <<"test altura2"<<endl;
    puntosbd p1 = puntos[1];
    puntosbd p2 = puntos[2];
    puntosbd p3 = puntos[3];


    Node tree1;
    Node* tree1_ptr = &tree1;
    Node tree2;
    Node* tree2_ptr = &tree2;
    Node tree3;
    Node* tree3_ptr = &tree3;
    //int z = 20;
    //tree_ptr->height = z;
    tree1_ptr->height = 0;
    tree2_ptr->height = 0;
    tree3_ptr->height = 0;
    Entry entrada_e1;
    Entry entrada_e2;
    Entry entrada_e3;
    entrada_e1.point = p1;
    entrada_e2.point = p2;
    entrada_e3.point = p3;
    entrada_e1.cr = 0.0;
    entrada_e2.cr = 0.0;
    entrada_e3.cr = 0.0;
    entrada_e1.child = tree1_ptr;
    entrada_e2.child = tree2_ptr;
    entrada_e3.child = tree3_ptr;
    //cout <<"test altura7"<<endl;

    vector<Entry> EntryTree; //[entrada_e1,entrada_e2,entrada_e3];
    EntryTree.push_back(entrada_e1);
    EntryTree.push_back(entrada_e2);
    EntryTree.push_back(entrada_e2);

    tree_ptr->is_leaf = false;
    tree_ptr->keys = EntryTree;
    //cout << "voy a imprimir height"<< endl;
    cout << height(tree_ptr)<< endl;
    //cout << height(tree1_ptr)<<endl;

    Node tree_test = cp(puntos);
    return 0;
   
}