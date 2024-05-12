//#include "node.h"
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <array>
#include <random>
#include <map>
#include <cmath>
#include "cp3.cpp"
#include "puntos.cpp"
using namespace std;


int accesos=0;

//retorna un vector de consultas
// Función para generar N querys
vector<Query> generarQuerys(vector<puntosbd> puntos) {
    vector<Query> querys;
    double radio = 0.02; // Radio constante para todas las querys
    
    // Generar N querys
    for (auto &point: puntos) {
        querys.push_back(Query(point, radio));
    }
    
    return querys;
}

//vector<puntosbd> puntos = generarPuntosAleatoriosUnicos(100);
//Función de búsqueda en un MTree, recibe un Nodo y un Consulta de búsqueda(bola) q.
//retorna un vector con los puntos encerrados por la bola q.
vector <puntosbd> search_tree(Node* mtree,Query q){
   vector <puntosbd> p_selected;
   accesos++;
   if(mtree->height() == 0){
     for (auto &entry : mtree->keys){
		//cout <<"ES HOJA"<<endl;
	    //cout << get<0>(entry.point)<< "," <<get<1>(entry.point)<<endl;
	    //cout << get<0>(q.punto)<< "," <<get<1>(q.punto)<<endl;
	    //cout <<"REVISADO"<<endl;
        if(euc_distance(entry.point,q.punto) < q.radio){
			//cout<<"caso baseee"<<endl;
            //accesos++;
            p_selected.push_back(entry.point);
        }
     }
     return p_selected;
   }
   else{  
    //vector de puntos p_selected_F
     for (const auto &entry : mtree->keys){
		 //cout <<"NO ES HOJA"<<endl;
		 //cout << get<0>(entry.point)<< "," <<get<1>(entry.point)<<endl;
		 //cout << get<0>(q.punto)<< "," <<get<1>(q.punto)<<endl;
		 //cout <<"REVISADO"<<endl;
		 
         if (euc_distance(entry.point,q.punto) <= q.radio + entry.cr){
            //accesos++;
            vector <puntosbd> tmp_vector= search_tree(entry.child, q);
            if(tmp_vector.empty() == false){
				//cout<<"caso recursivoooo"<<endl;
                p_selected.insert(p_selected.end(), tmp_vector.begin(), tmp_vector.end());
            }
         }
     }
   }
   return p_selected;
}


int main(){
    cout << B << endl;
    int N = pow(2,25);
    set<puntosbd> puntos;
	    
    cout<<"CONSULTAS"<<endl;
    //vector<puntosbd> puntos;
    //for(int i=0;i<N;i++){
		//puntosbd punto = generarPuntoAleatorio();
		//puntos.push_back(punto);
		//}
	while ( puntos.size()< N){
		puntosbd punto = generarPuntoAleatorio();
		puntos.insert(punto);
		}
	vector<puntosbd> puntitos(puntos.begin(), puntos.end());
	
    cout<<puntitos.size()<<endl;
    
    cout << "INICIA EL ARBOL EN MAIN"<<endl;
    //int suma=0;
    Node* tree_test = new Node;
    tree_test = cp(puntitos);
    //set_cr(tree_test);
    //cout <<"MI ALTURA DE ARBOL TEST ES:"<<endl;
    //cout << tree_test->height() <<endl;
    //cout <<"ALTURAS DE MIS ENTRADAS"<<endl;
    //for (auto &entry : tree_test->keys){
       // for (auto &entrada: entry.child->keys){
           // suma+= entrada.child->keys.size();
        //}
		//cout <<"FINALIZO ALTURAS"<<endl;s
		//cout << entry.child->keys.size() <<endl;
		//cout << entry.child->keys[0].child->keys.size() <<endl;
		//cout <<"next"<<endl;
		//}	
	//cout <<"THE END"<<endl;
    //cout <<suma<<endl;
	
    //cout<< tree_test.height << endl;
    //map <puntosbd, vector<puntosbd>> redist= redistribution2(puntos);
    //delete tree_test;
    //cout<<tree_test->height()<<endl;
    //cout<<tree_test->keys.size()<<endl;
    //int total =0;
    
    cout <<"FINISH CTM"<<endl;

    
    

    return 0;
   
}
