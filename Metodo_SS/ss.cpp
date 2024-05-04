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
Node ss(vector <puntosbd> puntos){
    // implementación del método de construcción SS
    int B = 4096/(sizeof(Entry)+sizeof(bool)+sizeof(int));


}