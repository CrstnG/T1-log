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

//Función que genera un puntobd aleatorio.
puntosbd generarPuntoAleatorio() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);
    return make_tuple(dis(gen), dis(gen));
}

//Función que genera N puntosbd aleatorios.
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


//NUESTRO B!
int B = 4096/(sizeof(Entry)+sizeof(int)); //113

//Función que calcula la distancia euclidiana entre dos puntosbd.
double euc_distance(const puntosbd& p1, const puntosbd& p2) {
    double dx = get<0>(p1) - get<0>(p2);
    double dy = get<1>(p1) - get<1>(p2);
    return sqrt(dx * dx + dy * dy);
}

// Retorna un vector con k tuplas random
// Dado un vector de puntosbd y un entero "k", retornar k puntodsbd del vector.
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
  int min_index = distance(euclidean_d.begin(), min_it);
  return l[min_index];
}

// Recibe los pivotes k random y todos los puntos, retornando un diccionario en que cada llave 
// corresponde a un pivote de los k random y su valor a un vector con los puntos en que su distancia
// euclidiana es menor respecto al resto de pivotes.
map <puntosbd, vector<puntosbd>> point_assign(vector<puntosbd> rand_points, vector<puntosbd> points){
    map <puntosbd, vector<puntosbd>> pivot_dict;
    for (auto &pivote : rand_points){
      pivot_dict[pivote] = vector<puntosbd>();
    }
    for (auto &punto : points){
      puntosbd key_dict = search_key_closest(punto,rand_points);
      pivot_dict[key_dict].push_back(punto);
    }
    return pivot_dict;
}
//Función de redistribución paso 4.
map <puntosbd, vector<puntosbd>> redistribution(vector <puntosbd> puntos){
    //cout << "inicia función de redistribución"<<endl;
    int b = 0.5*B; //56.5
    int n = puntos.size();
    int k = min(B,(n/B));
    if(k == 1){
		k += 1;
	}
	//Creo un vector de k puntosbd randoms.
    vector <puntosbd> random_points = random_p(puntos, k);
    //Se asignan subconjuntos de puntosbd a los k random en un diccionario.
    map <puntosbd, vector<puntosbd>> k_conjuntos = point_assign(random_points,puntos);
    //Se inicia un vector final que descarta las llaves con un largo del valor < b.
    vector<puntosbd> rand_final; 
    //Buscamos largo del diccionario para iterar.
    int length_dict = k_conjuntos.size();
    for (int i = 0; i < length_dict; i++){
		//Imponemos la condición de que sea mayor o igual a b para añadir al rand_final.
        if (k_conjuntos[random_points[i]].size() >= b){
           rand_final.push_back(random_points[i]);
        }
    }
    //Si tienen el mismo largo, entonces se puede retornar
    if(rand_final.size() == random_points.size()){
        for (auto &llave : k_conjuntos){
         int size_k_c = llave.second.size();
    }    
		return k_conjuntos; 
    }
    //Sino, hacemos la asignación de nuevo, descontando los puntosbd random que no tenian largo al menos b.
    //Como el resto de puntos tiene al menos b, no hace falta revisar de nuevo.
    map <puntosbd, vector<puntosbd>> final_dict = point_assign(rand_final,puntos);
    for (auto &llave : final_dict){
        int size_k_c = llave.second.size();
    }
    
    return final_dict;
}

vector <puntosbd> get_F(map <puntosbd, vector<puntosbd>> k_sets){
   vector <puntosbd> rd_points;
   for (auto &par : k_sets){
      //
      rd_points.push_back(par.first);
   }
   return rd_points;
}

//funcion que busca el sub-árbol con altura h
//recibe un puntero a un tree con una altura inicial j, y va  bajando en el árbol hasta encontrar con 
//un sub-árbol de altura h.
tuple<vector <Node*>,vector<puntosbd>> search_h_height(Node* tree,int j,int h,puntosbd p_padre){
    vector <Node*> tree_h;
    vector <puntosbd> f_add;
    if (j == h){
        if(tree->keys[0].child ==nullptr){
        }
        f_add.push_back(p_padre);
        tree_h.push_back(tree);
        for (int i = 0; i<f_add.size(); i++){
            B=B+0;
        }
        return make_tuple(tree_h,f_add);
    }
    else{
        for (auto &entries: tree->keys){
            if(entries.child == nullptr){
				}
            else{
            tuple<vector <Node*>,vector<puntosbd>>  search_child = search_h_height(entries.child,entries.child->height(),h,entries.point);

            tree_h.insert(tree_h.end(),get<0>(search_child).begin(), get<0>(search_child).end());
            f_add.insert(f_add.end(),get<1>(search_child).begin(), get<1>(search_child).end());
		}
        }
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
    for (auto &entrada : hoja->keys){
        if(get<0>(entrada.point) == get<0> (pfj) and get<1>(entrada.point) == get<1> (pfj)){
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

        int t_leafs_length = leafs_tsup.size();
        for (int j = 0; j < t_leafs_length;j++){
                //cout << j <<endl;
                //cout << "COORDENADA PFJ UUUUUUUUUUUU" <<endl;
               // cout << get<0>(pfjs[i]) <<endl;
                if (insert_tsup(leafs_tsup[j],t_prim[i],pfjs[i])){
                    s++;
    
                }
        }
    }

}

//Recibe un árbol con radios covertores = 0.
//Setea los radios covertores según corresponda.
void set_cr(Node* mtree) {
    if (mtree->height() == 0) {
        return; // No hacer nada si la altura es 0
    }

    // Llamada recursiva para establecer cr en los nodos hijos
    for (auto &entry : mtree->keys) {
        if (entry.child != nullptr) {
            set_cr(entry.child);
        }
    }

    // Actualizar cr con el mayor valor encontrado
    for (auto &entry : mtree->keys) {
        if (entry.child != nullptr) {
            for (const auto &child_entry : entry.child->keys) {
                double dist = euc_distance(entry.point, child_entry.point);
                entry.cr = max(entry.cr, dist);
            }
        }
    }
}
//Constructor de M-Trees con el método de Ciacca-Patella.
Node* cp(vector <puntosbd> puntos){
    int b = 0.5*B;
    int n = puntos.size();
    if (n <= B){
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
        map <puntosbd, vector<puntosbd>> conjuntos_k = redistribution(puntos);
        int sum =0;
        for(auto &llave :conjuntos_k){
            sum = sum + llave.second.size();
        }

        int conjuntos_k_length = conjuntos_k.size();
        while (conjuntos_k_length == 1){
            conjuntos_k = redistribution(puntos);
            conjuntos_k_length = conjuntos_k.size();
        }
        vector <puntosbd> conjunto_F = get_F(conjuntos_k);
        vector <Node*> tree_T_j;
        int conjuntof_length = conjunto_F.size();
        for(int g = 0; g < conjuntof_length; g++){ //paso 6
            Node* tmp_node = new Node;
            tmp_node = cp(conjuntos_k[conjunto_F[g]]);
            tree_T_j.push_back(tmp_node);
  
        }
        int largo_tj = tree_T_j.size();
        int tree_T_j_length = tree_T_j.size();     //paso 7
        vector <int> tree_T_j_indexes_elim;
        for (int i = 0; i < tree_T_j_length; i++){
            int length_arbol_keys = tree_T_j[i]->keys.size();
            if (tree_T_j[i]->keys.size() < b){
                conjunto_F.erase(conjunto_F.begin() + i); //Eliminamos pf_j de F
                for (auto &new_entrada : tree_T_j[i]->keys){   // añadimos puntos al conjunto F
                  conjunto_F.push_back(new_entrada.point);
                }
                tree_T_j_indexes_elim.push_back(i);     
            }
        }
        if(tree_T_j_indexes_elim.empty() == false){ ///
          for (auto &idx : tree_T_j_indexes_elim){
            for(auto &idtree : tree_T_j[idx]->keys){
                if (idtree.child == nullptr){
                }
                tree_T_j.push_back(idtree.child);
            }
          }
        }
        int cont = 0;             
        for (auto &idx : tree_T_j_indexes_elim){
            tree_T_j.erase(tree_T_j.begin() + idx - cont);
            cont++;
        }
        //terminando paso 7" 
        int tree_size = tree_T_j.size();
        vector <int> null_elim;
        int h; // nuestra altura minima
        vector <int> heights;
        int o =0;
        for (auto &sub_tree : tree_T_j){
            if(sub_tree == nullptr){
                null_elim.push_back(o);
                o++;
                
            }
			else
			{
				heights.push_back(sub_tree->height());
				o++;
		}
        }

        int cont_2 = 0;             
        for (auto &idx_2 : null_elim){
            conjunto_F.erase(conjunto_F.begin() + idx_2- cont_2);
            tree_T_j.erase(tree_T_j.begin()+idx_2-cont_2);
            cont_2++;
        }
        
        h = heights[0];
        for (auto &alturas : heights){
            if (alturas < h){
                h = alturas;
            }
        }
        tree_T_j_length = tree_T_j.size();
        vector <Node*> T_prim;
        vector <puntosbd> copy_conjunto_F = conjunto_F;
        for (int j = 0; j < tree_T_j_length; j++){ // Punto 9
           if(tree_T_j[j]->height() == h){
             T_prim.push_back(tree_T_j[j]);
           }
           else{
             
             puntosbd padre = copy_conjunto_F[j];
             conjunto_F.erase(conjunto_F.begin() + j); //eliminamos el punto pertinente en F
             tuple<vector <Node*>,vector<puntosbd>> sub_nodes_h = search_h_height(tree_T_j[j],tree_T_j[j]->height(),h,padre);
             int general_length = get<0>(sub_nodes_h).size();
             for (int i = 0; i<general_length;i++){
                T_prim.push_back(get<0>(sub_nodes_h)[i]);
                conjunto_F.push_back(get<1>(sub_nodes_h)[i]);
             }   
           }
        }

        Node* t_sup = new Node;
        t_sup =  cp(conjunto_F);
        //"Iniciando paso 11"
        int con_f_len = conjunto_F.size();
        int subtreees_len = T_prim.size();
        vector<Node*> hojas_tsup = search_leafs(t_sup);
        insert_all_tjs_en_tsup(hojas_tsup,T_prim,conjunto_F);
        //Retornamos T_sup
        return t_sup;
    }
}



