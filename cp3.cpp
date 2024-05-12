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



int B = 4096/(sizeof(Entry)+sizeof(int)); //113

double euc_distance(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<1>(p2);
    return sqrt(dx * dx + dy * dy);
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
    int b = 0.5*B; //56.5
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

//funcion que busca el sub-árbol con altura h
//recibe un puntero a un tree con una altura inicial j, y va  bajando en el árbol hasta encontrar con 
//un sub-árbol de altura h.
tuple<vector <Node*>,vector<puntosbd>> search_h_height(Node* tree,int j,int h,puntosbd p_padre){
    cout << "ENTRANDO A SEARCH_H_HEIGHT" << endl;
    cout<<"Los puntos del p_padre son:  (" << get<0>(p_padre) << ", " << get<1>(p_padre) << ")" << endl;
    cout << j << endl;
    for(auto entrada: tree->keys){
        cout<<"Los puntos de las entradas de p_padre  (" << get<0>(entrada.point) << ", " << get<1>(entrada.point) << ")" << endl;
    }
    cout << j << endl;
    vector <Node*> tree_h;
    vector <puntosbd> f_add;
    if (j == h){
        cout << "ENTRANDO A SEARCH_H_HEIGHT CASO BASE" << endl;
        cout << j << endl;
        if(tree->keys[0].child ==nullptr){
            cout << "Hijo de uno de los insertados es NULLPTR" << endl;
        }
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
            tuple<vector <Node*>,vector<puntosbd>>  search_child = search_h_height(entries.child,entries.child->height(),h,entries.point);

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
                    if(t_prim[i] == nullptr){
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
        Node n;
        for (int i=0;i < puntos.size();i++){
          puntosbd v = puntos[i];
          Entry e = {v,0.0,nullptr};
          n.keys.push_back(e);
        }
        Node* t_ptr = new Node;
        *t_ptr = n;
        return t_ptr;
    }
    else{
        //cout << "haciendo redistribucion" << endl;
        map <puntosbd, vector<puntosbd>> conjuntos_k = redistribution(puntos);
        int sum =0;
        for(auto &llave :conjuntos_k){
            sum = sum + llave.second.size();
        }
        cout <<"PUNTOS TOTALES EN REDISTRIBUCION: " <<sum<< endl;
        cout <<"PUNTOS TOTALES: "<<puntos.size()<<endl;
        //cout << "redistribución terminada" << endl;
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
        for(int g = 0; g < conjuntof_length; g++){ //paso 6
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
        //pushear hijos 1 2 3 4 5 6 7     2 3 5
        //              1 2 4 5 6 7        -1
        //              1 2 5 6 7          -2
        if(tree_T_j_indexes_elim.empty() == false){ ///
          cout << "Printeando cantidad de entradas del arbol a eliminar: "<<endl;
          for (auto &idx : tree_T_j_indexes_elim){
            for(auto &idtree : tree_T_j[idx]->keys){
                if (tree_T_j[idx]->height() == 0){
                    cout<<"SOY HOJA"<<endl;
                }
                cout << tree_T_j[idx]->keys.size()<<endl;
                //cout << "altura del childdddd"<<endl; //
                if (idtree.child == nullptr){
                    cout << "ARBOL NULO al tratar de agregar el child en tree_T_j "<<endl;
                    cout <<"MI ALTURA ES: "<<tree_T_j[idx]->height()<<""<<endl;
                    cout <<"MI CANTIDAD DE ENTRADAS ES : "<<tree_T_j[idx]->keys.size()<<""<<endl;
                }
                //cout <<idtree.child->height<<endl;
                //cout <<get<0>(tree_T_j[idx]->keys[0].point)<<endl;
                tree_T_j.push_back(idtree.child);
            }
          }
          //for (auto &idx : tree_T_j_indexes_elim){
            //tree_T_j.erase(tree_T_j.begin() + idx);
           //}
        }
        int cont = 0;             
        for (auto &idx : tree_T_j_indexes_elim){
            tree_T_j.erase(tree_T_j.begin() + idx - cont);
            cont++;
        }
        cout << "terminando paso 7" << endl;
        int tree_size = tree_T_j.size();
        cout <<"La cantidad de tree_T_j terminado el paso 7 es:"<<endl;
        cout << tree_size<< endl;
        //print_tjs(tree_T_j);

        vector <int> null_elim;
        int h; // nuestra altura minima
        vector <int> heights;
        cout <<"largo de heigths"<<endl;
        cout<<tree_size<<endl;
        int i =0;
        for (auto &sub_tree : tree_T_j){
            cout<<"ALTURAS DE SUBTREE:"<<endl;
            if(sub_tree == nullptr){
                cout<<"NULL DE LA LINEA 438"<<endl;
                null_elim.push_back(i);
                i++;
                
            }
            //cout<<sub_tree->height()<<endl
			else
			{
            heights.push_back(sub_tree->height());
            i++;
		}
        }

        int cont_2 = 0;             
        for (auto &idx_2 : null_elim){
            conjunto_F.erase(conjunto_F.begin() + idx_2- cont_2);
            cont_2++;
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
        cout <<"SAMPLES DE F ANTES DE PASO 9"<<endl;
        for (auto &sample: conjunto_F){
            double new_coor_x = get<0>(sample);
            double new_coor_y = get<1>(sample);
            cout << "CONJUNTO F ANTES DEL PASO 9: (" << new_coor_x << ", " << new_coor_y<< ")" << endl;
        }
        for (auto &key: tree_T_j){
            cout << key->keys.size()<<endl;
        }
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        cout << "Iniciando paso 9" << endl;
        cout <<"nuevo"<<endl;
        cout <<"altura minima h es: "<<h<<""<<endl;
        vector <puntosbd> copy_conjunto_F = conjunto_F;
        cout <<"PRINTEANDO TODOS LOS PUNTOS DE CADA TREE_T_J "<<h<<""<<endl;
        for (auto &t_j: tree_T_j) {
         for(auto &p_t_j: t_j->keys){
              double new_coor6_x = get<0>(p_t_j.point);
              double new_coor6_y = get<1>(p_t_j.point);
              cout << "TREE_T_J POINTS: (" << new_coor6_x << ", " << new_coor6_y<< ")" << endl;
        }
        }
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           //cout << "ENTRAMOS FOR PASO 9 VEAMOS EL LARGO DE LISTA TREE_T_J" << endl;
           //cout << tree_T_j_length << endl

           cout<<"MI height en I ES:"<<tree_T_j[j]->height()<<endl;
           if(tree_T_j[j]->height() == h){
             cout << "ALTURA IGUAL A H PARA EL PASO 9" << endl;
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             cout<<"ALTURA NO ES IGUAL A H PASO 9"<<endl;
             if(tree_T_j[j] == nullptr){
                cout<<"PASO 9 NULO MI PANA"<<endl;

             }
             //conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             //Node* sub_node_h = search_h_height(tree_T_j[j],tree_T_j[j]->height,h);//
             
             puntosbd padre = copy_conjunto_F[j];
             cout << "CONJUNTO F ANTES DEL ERASE: " << conjunto_F.size()<<endl;
             double new_coor4_x = get<0>(conjunto_F[j]);
             double new_coor4_y = get<1>(conjunto_F[j]);
             cout << "Punto a eliminar en iteracion DEL PASO 9: (" << new_coor4_x << ", " << new_coor4_y<< ")" << endl;
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             cout << "CONJUNTO F DESPUES DEL ERASE: " << conjunto_F.size()<<endl;
             for (auto &pts : conjunto_F){
                double new_coor3_x = get<0>(pts);
                double new_coor3_y = get<1>(pts);
                cout << "Puntos post erase: (" << new_coor3_x << ", " << new_coor3_y<< ")" << endl;
            }
             tuple<vector <Node*>,vector<puntosbd>> sub_nodes_h = search_h_height(tree_T_j[j],tree_T_j[j]->height(),h,padre);
             //vector <Entry> sub_node_h_entries = sub_node_h->keys;
             for (auto puntos_add : get<1>(sub_nodes_h)){
                double new_coor3_x = get<0>(puntos_add);
                double new_coor3_y = get<1>(puntos_add);
                cout << "Puntos a añadir en cada iteracion DEL PASO 9: (" << new_coor3_x << ", " << new_coor3_y<< ")" << endl;
            }
             int general_length = get<0>(sub_nodes_h).size();
             for (int i = 0; i<general_length;i++){
                if(get<0>(sub_nodes_h)[i] == nullptr){
                    cout<<"EN EL PASO 9 HAY T_PRIMA NULO"<<endl;
                }
                T_prim.push_back(get<0>(sub_nodes_h)[i]);
                for (auto new_F : conjunto_F){
                    if (new_F == get<1>(sub_nodes_h)[i]){
                        cout <<"REPETIDO"<<endl;
                    }
                }
                conjunto_F.push_back(get<1>(sub_nodes_h)[i]);
                //T_prim.push_back(get<0>(sub_nodes_h)[i]);
             }   
           }
        }
        for (auto &punt : conjunto_F){
            double new_coor2_x = get<0>(punt);
            double new_coor2_y = get<1>(punt);
            cout << "CONJUNTO F DESPUES DEL PASO 9: (" << new_coor2_x << ", " << new_coor2_y<< ")" << endl;
        }
        for (auto &key: T_prim){
            cout << key->keys.size()<<endl;
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
            if(l == nullptr){
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
        for (auto &key : t_sup->keys){
            if(key.child == nullptr){
        //cout<< key.child->keys.size() <<endl;
        //cout<<"Coordenada x"<<endl;
        //cout<< get<0>(key.point) <<endl;
        //cout<<"Coordenada y"<<endl;
        //cout<< get<1>(key.point) <<endl;
                cout<<"SOY NULO"<<endl;
    
        }
        else{
            cout<< key.child->keys.size() <<endl;
            cout<<"Coordenada x"<<endl;
            cout<< get<0>(key.point) <<endl;
            cout<<"Coordenada y"<<endl;
            cout<< get<1>(key.point) <<endl;
        //total +=key.child->keys.size();
            cout<<"NO SOY NULO"<<endl;
	    }
        }
        cout << "finish"<<endl; 
        return t_sup;
    }
}


int main(){
    cout << B << endl;
    int N = pow(2,17);
    
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    

    cout << "INICIA EL ARBOL EN MAIN"<<endl;
    int suma=0;
    Node* tree_test = new Node;
    tree_test = cp(puntos);
    cout <<"MI ALTURA DE ARBOL TEST ES:"<<endl;
    cout << tree_test->height() <<endl;
    cout <<"ALTURAS DE MIS ENTRADAS"<<endl;
    //for (auto &entry : tree_test->keys){
       // for (auto &entrada: entry.child->keys){
           // suma+= entrada.child->keys.size();
        //}
		//cout <<"FINALIZO ALTURAS"<<endl;s
		//cout << entry.child->keys.size() <<endl;
		//cout << entry.child->keys[0].child->keys.size() <<endl;
		//cout <<"next"<<endl;
		//}	
	cout <<"THE END"<<endl;
    cout <<suma<<endl;
	
    //cout<< tree_test.height << endl;
    //map <puntosbd, vector<puntosbd>> redist= redistribution2(puntos);
    //delete tree_test;
    cout<<tree_test->height()<<endl;
    cout<<tree_test->keys.size()<<endl;
    //int total =0;
    for (auto &key : tree_test->keys){
     if(key.child == nullptr){
        //cout<< key.child->keys.size() <<endl;
        //cout<<"Coordenada x"<<endl;
        //cout<< get<0>(key.point) <<endl;
        //cout<<"Coordenada y"<<endl;
        //cout<< get<1>(key.point) <<endl;
        cout<<"SOY NULO"<<endl;
    
     }
     else{
        cout<< key.child->keys.size() <<endl;
        cout<<"Coordenada x"<<endl;
        cout<< get<0>(key.point) <<endl;
        cout<<"Coordenada y"<<endl;
        cout<< get<1>(key.point) <<endl;
        //total +=key.child->keys.size();
        cout<<"NO SOY NULO"<<endl;
	 }
    }
    
    return 0;  
}
