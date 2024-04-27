#include <bits/stdc++.h>
#include <node.h>
#include <tuple>
#include <vector>
using namespace std;



Node::Node(vector <entry> vector){
    this->v = vector; 
}
//Transforma los puntos a entradass
vector<entry> to_entries(vector<puntosbd> points){
    vector<entry> v;
    for (i=0,i < points.size(),i++){
        entry new_entry = {points[i],0.0,NULL};
        v.push_back(new_entry);
    }
    return v;
}

Node cp(vector <puntosbd> puntos){
    // estimar B y b
    if (puntos.size() <= B){
        //Transformar puntos a ENTRADAS
        // retornar un arbol(NODE) con los puntos transformados como atributo
    }
    else{
        


    }
}
int main(){
    
}
