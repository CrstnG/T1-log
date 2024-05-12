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
#include <chrono>
#include <ctime>
#include <iomanip> 
#include <fstream> 
using namespace std;


int accesos=0;
vector<puntosbd> p_selected;

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
void search_tree(Node* mtree,Query q){
   //vector <puntosbd> p_selected;
   //accesos++;
   if(mtree->height() == 0){
     for (auto &entry : mtree->keys){
		//cout <<"ES HOJA"<<endl;
	    //cout << get<0>(entry.point)<< "," <<get<1>(entry.point)<<endl;
	    //cout << get<0>(q.punto)<< "," <<get<1>(q.punto)<<endl;
	    //cout <<"REVISADO"<<endl;
        if(euc_distance(entry.point,q.punto) <= q.radio){
			//cout<<"caso baseee"<<endl;
            //accesos++;
            p_selected.push_back(entry.point);
        }
     }
     //return p_selected;
   }
   else{  
    //vector de puntos p_selected_F
     for (const auto &entry : mtree->keys){
		 //cout <<"NO ES HOJA"<<endl;
		 //cout << get<0>(entry.point)<< "," <<get<1>(entry.point)<<endl;
		 //cout << get<0>(q.punto)<< "," <<get<1>(q.punto)<<endl;
		 //cout <<"REVISADO"<<endl;
		 
         if (euc_distance(entry.point,q.punto) <= q.radio + entry.cr){
            accesos++;
            if(entry.child!=nullptr){
				if(entry.cr == 0.0){
					cout<<"QUE PASÓ PAPITO"<<endl;
					}
				//vector <puntosbd> tmp_vector= 
				search_tree(entry.child, q);
				//if(tmp_vector.empty() == false){
				//cout<<"caso recursivoooo"<<endl;
					//p_selected.insert(p_selected.end(), tmp_vector.begin(), tmp_vector.end());
            }
         }
	 }
     }
   //return p_selected;
}


int main(){
	//VALOR DE MI B
    int indice;
    cout<< "EJECUTAR PRUEBAS"<<endl;
    cout<< "Ingresar número (entre 10 y 20)"<<endl;
    cin>>indice;
    cout<<"Has ingresado: "<<indice<<endl;
    //LOS N PUNTOS A PROBAR
    int N = pow(2,indice);
    //CREO UN SET DE PUNTOSBD (VACÍO)
    set<puntosbd> puntos_test;
    //RELLENO CON RANDOMS HASTA COMPLETAR EL N
    cout<<"Creando 2 elevado a "<<indice<<" puntos"<<endl;
    while ( puntos_test.size()< N){
		puntosbd punto = generarPuntoAleatorio();
		puntos_test.insert(punto);
		}
    cout<<"Puntos creados"<<endl;
	//PASO LOS PUNTOS DEL SET A UN VECTOR
	vector<puntosbd> puntos(puntos_test.begin(), puntos_test.end());
    //CREO 100 PUNTOS ALEATORIOS PARA MIS CONSULTAS
    cout<<"Creando 100 consultas."<<endl;
    vector<puntosbd> puntos_q = generarPuntosAleatoriosUnicos(100);
	//VECTOR DE CONSULTAS
	vector<Query> querys_test = generarQuerys(puntos_q);
    cout<<"Consultas creadas."<<endl;
	
    //INICIAMOS EL M-TREE
    cout << "Creando M-Tree."<<endl;
    //CREAMOS EL M-TREE
    Node* tree_test = new Node;
    //CONSTRUIMOS CON METODO CP
    tree_test = cp(puntos);
    //SETEAMOS RADIO COVERTOR
    set_cr(tree_test);
	cout<<"M-Tree Terminado"<<endl;
    // INICIAR TEMPORIZADOR
    auto start = chrono::steady_clock::now();
	cout<<"COMIENZAN CONSULTAS"<<endl;
	//HAGO LAS CONSULTAS
	for (auto &query: querys_test){
		//REINICIO LOS ACCESOS
		accesos=0;
		//REALIZAR CONSULTA
        auto query_start = chrono::steady_clock::now();
        //APLICO LA BUSQUEDA DE LAS CONSULTAS
		search_tree(tree_test,query);
		auto query_end = chrono::steady_clock::now();
		// TIEMPO DE LA CONSULTA
		auto query_time = chrono::duration_cast<std::chrono::microseconds>(query_end - query_start).count();
		// MOSTRAR TIEMPO DE EJECUCION DE LA CONSULTA
        cout << "Tiempo de ejecución de la consulta: " << fixed << setprecision(3) << query_time << " us" << endl;
        //REINICIO EL VECTOR P_SELECTED
		p_selected.clear();
        // Mostrar el número de accesos
        cout << "Accesos: " << accesos << endl;
		}
	
	cout<<"CONSULTAS TERMINADAS"<<endl;
	
	 // Calcular el tiempo total de ejecución
    auto end = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    cout << "Tiempo total de ejecución: " << total_time << " us" << endl;
    return 0;
   
}
