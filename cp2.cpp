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
        //n.is_leaf = true;
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
    for (auto &pivote : rand_points){
      pivot_dict[pivote] = vector<puntosbd>();//listavacia;
    }
    for (auto &punto : points){
      puntosbd key_dict = search_key_closest(punto,rand_points);
      pivot_dict[key_dict].push_back(punto);
    }
    return pivot_dict;
}
//Función de redistribución paso 4.
map <puntosbd, vector<puntosbd>> redistribution(vector <puntosbd> puntos){
    cout << "inicia función de redistribución"<<endl;
    int b = 0.5*B;
    int n = puntos.size();
    cout << "largo de entrada:"<<endl;
    cout << n << endl;
    int k = min(B,(n/B)) + 1;
    //if(k == 1){
	//	k += 1;
	//}
    cout << "minimo es:" << endl;
    cout << k << endl;
    vector <puntosbd> random_points = random_p(puntos, k);
    for(auto &a : random_points){
        double x = get<0>(a);
        double y = get<1>(a);
        cout<<"ramdoms points ("<<x<<","<<y<<")"<<endl;

    }
    cout << "LOS RANDOM POINTS ANTES DE LA CONDICIONAL"<<endl;
    map <puntosbd, vector<puntosbd>> k_conjuntos = point_assign(random_points,puntos);
    cout << k_conjuntos.size()<<endl;
    //vector<int> rand_index_elim;
    vector<puntosbd> rand_final;
    int length_dict = k_conjuntos.size();
    for (int i = 0; i < length_dict; i++){
        //cout <<i<<endl;
        if (k_conjuntos[random_points[i]].size() >= b){
           rand_final.push_back(random_points[i]);
        }
    }
    if(rand_final.size() == random_points.size()){
        cout <<"terminando redistribucion" <<endl;
        cout << k_conjuntos.size()<<endl;
        for (auto &llave : k_conjuntos){
         int size_k_c = llave.second.size();
         cout <<size_k_c<<endl;
    }    
    return k_conjuntos; 
    }
    map <puntosbd, vector<puntosbd>> final_dict = point_assign(rand_final,puntos);
    cout <<"terminando redistribucion" <<endl;
    cout << final_dict.size()<<endl;
    for (auto &llave : final_dict){
        int size_k_c = llave.second.size();
        cout <<size_k_c<<endl;
    }
    return final_dict;
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

//funcion que busca el sub-árbol con altura h
//recibe un puntero a un tree con una altura inicial j, y va  bajando en el árbol hasta encontrar con 
//un sub-árbol de altura h.
tuple<vector <Node*>,vector<puntosbd>> search_h_height(Node* tree,int j,int h,puntosbd p_padre){
    cout << "ENTRANDO A SEARCH_H_HEIGHT" << endl;
    cout << j << endl;
    vector <Node*> tree_h;
    vector <puntosbd> f_add;
    if (j == h){
        cout << "ENTRANDO A SEARCH_H_HEIGHT CASO BASE" << endl;
        cout << j << endl;
        f_add.push_back(p_padre);
        tree_h.push_back(tree);
        for (int i = 0; i<f_add.size(); i++){
            cout<<"Los puntos de f_add: (" << get<0>(f_add[i]) << ", " << get<1>(f_add[i]) << ")" << endl;
            cout<<"Las alturas de los tree_h" << tree_h[i]->height() <<" "<< endl;
        }
        return make_tuple(tree_h,f_add);
    }
    else{
        cout << "ENTRANDO A SEARCH_H_HEIGHT RECURSIVO" << endl;
        cout << j << endl;
        for (auto &entries: tree->keys){
            //tree_h.push_back(search_h_height(entries.child,j-1,h));
            tuple<vector <Node*>,vector<puntosbd>>  search_child = search_h_height(entries.child,j-1,h,entries.point);

            tree_h.insert(tree_h.end(),get<0>(search_child).begin(), get<0>(search_child).end());
            f_add.insert(f_add.end(),get<1>(search_child).begin(), get<1>(search_child).end());
        }
    }
    for (auto &arbol: tree_h){
        int altura = arbol->height();
        cout<<"LA ALTURA EN SEARCH H ES:"<<endl;
        cout<<altura<<endl;
    }
    return make_tuple(tree_h,f_add);
}

//vector de nodos hojas
vector <Node*> search_leafs(Node* tsup){
    vector <Node*> leafs;
    if(tsup->height() == 0){
        leafs.push_back(tsup);
        return leafs;
    }
    else{
        for (auto &entry : tsup->keys){
            vector<Node*> tmp = search_leafs(entry.child);
            leafs.insert(leafs.end(), tmp.begin(), tmp.end());
        }
        return leafs; 
    }
}
bool insert_tsup(Node* hoja,Node* t_prima,puntosbd pfj){
    cout << "Entrando a insert_tsup con largo de la llave de la hoja"<<endl;
    cout << hoja->keys.size()<<endl;
    for (auto &entrada : hoja->keys){
        cout << "ENTRANDP EN UNA KEYS DE INSERT T_SUP"<<endl;
        cout << get<0>(entrada.point)<<endl;
        if(get<0>(entrada.point) == get<0> (pfj) and get<1>(entrada.point) == get<1> (pfj)){
            cout << "Altura de T_PRIMA"<<endl;
            cout << t_prima->height()<<endl;
            entrada.child = t_prima;
            //hoja->is_leaf = false;
            
            //cout << "Lo encontré!"<<endl;
            return true;
        }
    }
    return false;
}
//función que inserta os tjs en tsup
void insert_all_tjs_en_tsup(vector<Node*>leafs_tsup,vector<Node*> t_prim,vector<puntosbd> pfjs){
    int t_prim_length = t_prim.size();
    int s = 0;
    for (int i = 0; i < t_prim_length;i++){
        cout << "LARGOOOOOO DE T_PRIM_LENGTHHHHHHHH" <<endl;
        cout << i <<endl;
        int t_leafs_length = leafs_tsup.size();
        for (int j = 0; j < t_leafs_length;j++){
                cout << j <<endl;
                cout << "COORDENADA PFJ UUUUUUUUUUUU" <<endl;
                cout << get<0>(pfjs[i]) <<endl;
                if (insert_tsup(leafs_tsup[j],t_prim[i],pfjs[i])){
                    s++;
                    if(t_prim[i]==NULL){
                        cout << "Estoy insertando un nulo"<<endl;
                    }
                    cout << "Lo encontree"<<endl;
                }
        }
    }
    cout<<"LARGO DEL T_PRIM_LENGHT:"<<endl;
    cout<<t_prim_length<<endl;
    cout<<"LARGO DEL T_PRIM INSERTADOS"<<endl;
    cout<<s<<endl;
    if(s!=t_prim_length){
        cout<<"SON DIFERENTES"<<endl;
    }
    cout<<"TERMINÉ INSERT ALL"<<endl;

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
    if(t_sup->height()){
        return;
    }
    else{
        //por cada entrada de las entradas (keys) de t_sup
        for (auto &entry : t_sup->keys){
            cout << " cada entrada de las entradas (keys) de t_sup" << endl;
            entry.cr = get_max_cr(entry.point,entry.child);
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
Node* cp(vector <puntosbd> puntos){
    // estimar B y b
    cout << "empezando cp con entrada:" << endl;
    cout <<puntos.size()<<endl;
    int b = 0.5*B;
    int n = puntos.size();
    if (n <= B){
        // retornar un arbol(NODE) con los puntos transformados como atributo.
        cout << "LLegando al caso base" << endl;
        Node* t_ptr = new Node;
        *t_ptr = to_node(puntos);
        return t_ptr;
    }
    else{
        //cout << "haciendo redistribucion" << endl;
        map <puntosbd, vector<puntosbd>> conjuntos_k = redistribution(puntos);
        //cout << "redistribución temrinada" << endl;
        int conjuntos_k_length = conjuntos_k.size();
        //cout << "Largo antes del while;" << endl;
        cout << conjuntos_k_length << endl;
        while (conjuntos_k_length == 1){
            cout << "Redistribuyendo debido a que k_length igual a 1" << endl;
            conjuntos_k = redistribution(puntos);
            conjuntos_k_length = conjuntos_k.size();
        }
        cout << "obteniendo conjunto F random points" << endl;
        vector <puntosbd> conjunto_F = get_F(conjuntos_k);
        cout << "obtencion lista" << endl;
        vector <Node*> tree_T_j;
        int conjuntof_length = conjunto_F.size();
        for(int g = 0; g < conjuntof_length; g++){
                      //paso 6
            cout << "comenzando recursion para obtener T_j" << endl;
            Node* tmp_node = new Node;
            tmp_node = cp(conjuntos_k[conjunto_F[g]]);
            tree_T_j.push_back(tmp_node);
            cout << "lA ALTURA DEL TREE_TJ INDEXADO EN (" << g << ",es: " <<tmp_node->height() << ")" << endl;
        }
        cout << "ALTURAS DE LOS TREE_T_J_NODES" <<endl;
        cout << "Iniciando paso 7" << endl;
        int largo_tj = tree_T_j.size();
        cout << "# de tree_tj"<<endl;
        cout << largo_tj << endl;
        int tree_T_j_length = tree_T_j.size();     //paso 7
        vector <int> tree_T_j_indexes_elim;
        for (int i = 0; i < tree_T_j_length; i++){
            int length_arbol_keys = tree_T_j[i]->keys.size();
            cout << "Cantidad de entradas que tiene el tree_T_j:(" << i << ",es: " <<length_arbol_keys << ")" << endl;
            if (tree_T_j[i]->keys.size() < b){
                cout << "Entrando al if del paso 7 con: "<< tree_T_j[i]->keys.size()<<""<<endl;
                conjunto_F.erase(conjunto_F.begin() + i); //Eliminamos pf_j de F
                for (auto &new_entrada : tree_T_j[i]->keys){   // añadimos puntos al conjunto F
                  cout << "Entrando al primer for del paso 7 "<<endl;
                  conjunto_F.push_back(new_entrada.point);
                }
                tree_T_j_indexes_elim.push_back(i);     
            }
        }
        cout << "Indices a eliminar:"<<endl;
        for (auto &index: tree_T_j_indexes_elim){
            cout<<index<<endl;
        }
        if(tree_T_j_indexes_elim.empty() == false){ ///
          cout << "Printeando cantidad de entradas del arbol a eliminar: "<<endl;
          for (auto &idx : tree_T_j_indexes_elim){
            for(auto &idtree : tree_T_j[idx]->keys){
                if (tree_T_j[idx]->height() == 0){
                    cout<<"SOY HOJA"<<endl;
                }
                cout << tree_T_j[idx]->keys.size()<<endl;
                //cout << "altura del childdddd"<<endl; //
                if (idtree.child == NULL){
                    cout << "ARBOL NULO al tratar de agregar el child en tree_T_j "<<endl;
                    cout <<"MI ALTURA ES: "<<tree_T_j[idx]->height()<<""<<endl;
                    cout <<"MI CANTIDAD DE ENTRADAS ES : "<<tree_T_j[idx]->keys.size()<<""<<endl;
                }
                //cout <<idtree.child->height<<endl;
                //cout <<get<0>(tree_T_j[idx]->keys[0].point)<<endl;
                tree_T_j.push_back(idtree.child);
            }
          }
          for (auto &idx : tree_T_j_indexes_elim){
            tree_T_j.erase(tree_T_j.begin() + idx);
           }
        }
        cout << "terminando paso 7" << endl;
        int tree_size = tree_T_j.size();
        cout <<"La cantidad de tree_T_j terminado el paso 7 es:"<<endl;
        cout << tree_size<< endl;
        //print_tjs(tree_T_j);

        int h; // nuestra altura minima
        vector <int> heights;
        cout <<"largo de heigths"<<endl;
        cout<<tree_size<<endl;
        for (auto &sub_tree : tree_T_j){
            cout<<"ALTURAS DE SUBTREE:"<<endl;
            cout<<sub_tree->height()<<endl;
            heights.push_back(sub_tree->height());
        }
        
    
        cout<<"sali del for de heights"<<endl;
        h = heights[0];
        for (auto &alturas : heights){
            if (alturas < h){
                h = alturas;
            }
        }
        //auto min_iter = min_element(heights.begin(), heights.end());
        //h = *min_iter;
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        cout << "Iniciando paso 9" << endl;
        cout <<"nuevo"<<endl;
        cout <<"altura minima h es: "<<h<<""<<endl;
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           //cout << "ENTRAMOS FOR PASO 9 VEAMOS EL LARGO DE LISTA TREE_T_J" << endl;
           //cout << tree_T_j_length << endl;
           cout<<"MI height en I ES:"<<tree_T_j[j]->height()<<endl;
           if(tree_T_j[j]->height() == h){
             cout << "ALTURA IGUAL A H PARA EL PASO 9" << endl;
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             cout<<"ALTURA NO ES IGUAL A H PASO 9"<<endl;
             if(tree_T_j[j] == NULL){
                cout<<"PASO 9 NULO MI PANA"<<endl;

             }
             //conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             //Node* sub_node_h = search_h_height(tree_T_j[j],tree_T_j[j]->height,h);//
             
             puntosbd padre = conjunto_F[j];
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             tuple<vector <Node*>,vector<puntosbd>> sub_nodes_h = search_h_height(tree_T_j[j],tree_T_j[j]->height(),h,padre);
             cout<<"VERIFICAR ALTURAS"<<endl;
             for (auto &alt_fin : get<0>(sub_nodes_h)){
                if (alt_fin->height() != h){
                    cout<<"ERRORRRR"<<endl;
                }
                else{
                    cout<<"IGUAL A HHH"<<endl;
                }
             }
             //vector <Entry> sub_node_h_entries = sub_node_h->keys;
             int general_length = get<0>(sub_nodes_h).size();
             for (int i = 0; i<general_length;i++){
                if(get<0>(sub_nodes_h)[i]==NULL){
                    cout<<"EN EL PASO 9 HAY T_PRIMA NULO"<<endl;
                }
                T_prim.push_back(get<0>(sub_nodes_h)[i]);
                conjunto_F.push_back(get<1>(sub_nodes_h)[i]);
             }   
           }
        }
        Node* t_sup = new Node;
        t_sup =  cp(conjunto_F);
        cout << "Iniciando paso 11" << endl;
        int con_f_len = conjunto_F.size();
        int subtreees_len = T_prim.size();
        cout << "Largo de conjunto F final: (" << con_f_len << ")" << endl;
        cout << "Largo de T prima final: (" << subtreees_len << ")" << endl;
        //insert_all_tjs_en_tsup(&t_sup,T_prim,conjunto_F); //paso 11 algoritmo cp
        cout << "PRINTEANDO ENTRADAS T_SUP"<<endl;
        for(auto &entrada : t_sup->keys){
            double new_coor_x = get<0>(entrada.point);
            double new_coor_y = get<1>(entrada.point);
            cout << "Coordenadas de las entradas: (" << new_coor_x << ", " << new_coor_y<< ")" << endl;
            
        }
        cout << "PRINTEANDO PUNTOS DE CONJUNTO_F"<<endl;
        for(auto &fs : conjunto_F){
            double coor_x = get<0>(fs);
            double coor_y = get<1>(fs);
            cout << "Las coordenadas de los puntos de conjunto_F: (" << coor_x << ", " << coor_y<< ")" << endl;
            
        }
        cout <<"PRINTEANDO ENTRADAS DE T_PRIM"<<endl;
         for(auto &l : T_prim){
            if(l == NULL){
                cout <<"HAY UN NULL EN LA LISTA DE T PRIMAAAAAAAAAAAAAS"<<endl;
            }
            
        }
        
        vector<Node*> hojas_tsup = search_leafs(t_sup);
        cout <<"TOY CANSAO JEFE"<<endl;
        cout << hojas_tsup.size()<<endl;
        insert_all_tjs_en_tsup(hojas_tsup,T_prim,conjunto_F);
        cout << "Iniciando paso 12" << endl;
        //set_cr(&t_sup); //paso 12
        cout << "finish"<<endl; 
        return t_sup;
    }
}


int main(){
    cout << B << endl;
    int N = pow(2,15);
    
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    
    cout << "INICIA EL ARBOL EN MAIN"<<endl;
    Node* tree_test = cp(puntos);
    //cout<< tree_test.height << endl;
    //map <puntosbd, vector<puntosbd>> redist= redistribution2(puntos);
    delete tree_test;
    return 0;  
}
