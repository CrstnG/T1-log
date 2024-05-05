#include "../node.h"
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <array>
#include <random>
#include <memory>
#include <map>
#include <cmath>

// Incluimos funciones auxiliares
#include "Cluster.cpp"
#include "OutputHoja.cpp"
#include "OutputInterno.cpp"

using namespace std;

/* INSTRUCCIONES ENUNCIADO: 
Algoritmo SS: retorna la raíz del M-tree construído.
Input: Cin, un conjunto de puntos
    1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a.
    2. Sea Cout = Cluster(Cin). Sea C = {}.
    3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
    4. Mientras |C| > B:
        4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}.
        4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
        4.3 Sea C = {}.
    4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
    5. Sea (g, r, a) = OutputInterno(C)
    6. Se retorna a
*/

/**
 * Función del método de construcción SS, que permite a partir de un conjunto de puntos, construir el árbol M.
 * @param puntos Conjunto de puntos a partir de los cuales se construirá el árbol M.
 * @return Un objeto de tipo Node.
 */
Node* ss(const vector<puntosbd>& puntos) {
    
    int B = 4096/(sizeof(Entry)+sizeof(bool)+sizeof(int));
    int b = 0.5*B;

    // 1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a.
    if (puntos.size() <= B) {
        Entry entry = OutputHoja(puntos); // Obtener la salida de hoja para puntos
        Node* a = entry.child;  // Retornar el puntero al nodo hoja: entry
        return a; 
    }
    
    // 2. Sea Cout = Cluster(Cin). Sea C = {}.
    vector<vector<puntosbd>> Cout = cluster(puntos, b, B); // Clusterizar puntos
    vector<Entry> C; // Vector para almacenar las entradas de nodos internos
    
    // 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
    for (const auto& c : Cout) {
        Entry entry = OutputHoja(c); // Obtener la salida de hoja para cada cluster
        C.push_back(entry); // Añadir la entrada a C
    }

    // 4. Mientras |C| > B:
    while (C.size() > B) {
        // 4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}.
        vector<puntosbd> Cin; // Conjunto de puntos g de las entradas en C
        for (const auto& [g, r, a] : C) {
            Cin.push_back(g);
        }
        
        Cout = cluster(Cin, b, B); // Clusterizar Cin
        std::vector<std::vector<Entry>> Cmra; // Vector para almacenar las tuplas (g, r, a) de C y Cout
        
        // 4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
        for (const auto& c : Cout) {
            vector<Entry> s; // Vector temporal para almacenar las entradas correspondientes a c
            for (const auto& entry : C) {
                if (std::find(c.begin(), c.end(), entry.child) != c.end()) {
                    s.push_back(entry);
                }
            }
            for (const auto& entry : s) {
                Cmra.push_back(entry);
            }
            //Cmra.push_back(s);
        }
        
        // 4.3 Sea C = {}.
        C.clear(); // Limpiar el vector C
        
        // 4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
        for (const auto& s : Cmra) {
            Entry entry = OutputInterno(s); // Obtener la salida interna para cada s
            C.push_back(entry); // Añadir la entrada a C
        }
    }

    // 5. Sea (g, r, a) = OutputInterno(C)
    Entry entry = OutputInterno(C);
    
    // 6. Se retorna a
    return entry.child;
}