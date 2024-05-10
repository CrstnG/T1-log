#include "../node.h"
#include "../aux_functions.cpp"



int accesos;

vector <puntosbd> random_q(vector <puntosbd> p,int k){
    vector <puntosbd> k_random;
    vector <int> unique_indexes;
    while (k > 0 && !p.empty()){
        int index = rand()% p.size(); //p.size es suficiente dado a que vamos variando el tamaño de p
        if(is_in_index(index, unique_indexes)) {
            continue;
        }
        else{
            puntosbd r_tuple = p[index];
            k_random.push_back(r_tuple);
            p.erase(p.begin()+index);
            k--;
        }
    }
    return k_random;
}

//Función de búsqueda en un MTree, recibe un Nodo y un Consulta de búsqueda(bola) q.
//retorna un vector con los puntos encerrados por la bola q.
vector <puntosbd> search_tree(Node* mtree,tuple <puntosbd, double> q){
   vector <puntosbd> p_selected;
   if(mtree->is_leaf){
     for (const auto &entry : mtree->keys){
        if(euc_distance(entry.point,get<0>(q)) < get<1>(q)){
            accesos++;
            p_selected.push_back(entry.point);
        }
     }
     return p_selected;
   }
   else{  
    //vector de puntos p_selected_F
     for (const auto &entry : mtree->keys){
         if (euc_distance(entry.point,get<0>(q)) < get<1>(q) + entry.cr){
            accesos++;
            vector <puntosbd> tmp_vector= search_tree(entry.child, q);
            if(tmp_vector.empty() == false){
                p_selected.insert(p_selected.end(), tmp_vector.begin(), tmp_vector.end());
            }
         }
     }
   }
   return p_selected;
}

int main(){
    cout << B << endl;
    int N = pow(2,14);
    
    vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(N);
    cout << "cree los puntos" << endl;
    cout << "los primeros 50 puntos" << endl;

    for (int i = 0; i < 10; i++){
        double x = get<0>(puntos[i]);
        double y = get<1>(puntos[i]);
        cout << "Las coordenadas son: (" << x << ", " << y << ")" << endl;
    }
    vector<puntosbd> queries = random_q(puntos,100);
    cout << "empeze" << endl;
    Node mtree = cp(puntos);
    cout << "terminé" << endl;
    return 0;
   
}
