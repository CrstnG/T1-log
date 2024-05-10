#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "Clases.cpp"


/**
 * @brief Realiza operaciones de clustering basadas en un conjunto de puntos de entrada y un rango de tamaños de cluster (b y B).
 * @param puntos Un conjunto de puntos de entrada (C_in) de tamaño mínimo b.
 * @param b El tamaño mínimo de cada cluster.
 * @param B El tamaño máximo de cada cluster.
 * @return Un ClusterSet con clusters de tamaño entre b y B.
 */
ClusterSet cluster_fun(const std::vector<puntosbd>& puntos, size_t b, size_t B) {
    ClusterSet C;  // Conjunto de clusters en proceso
    ClusterSet Cout;  // Conjunto de clusters de salida

    // Añadir cada punto como un nuevo cluster a C
    for (const auto& p : puntos) {
        Cluster newCluster(p, std::vector<puntosbd>{p});
        C.addCluster(newCluster);
    }

    // Mientras el número de clusters en C sea mayor que uno
    while (C.numClusters() > 1) {
        // Encontrar los pares más cercanos de clusters en C
        auto [cluster1, cluster2] = C.encontrarParesMasCercanos();

        // Calcular el tamaño combinado de los clusters
        size_t tamanoCombinado = cluster1.clusterPoints.size() + cluster2.clusterPoints.size();

        // Si el tamaño combinado es menor o igual a B, combinar los clusters
        if (tamanoCombinado <= B) {
            // Combinar los dos clusters
            cluster1.clusterPoints.insert(cluster1.clusterPoints.end(), cluster2.clusterPoints.begin(), cluster2.clusterPoints.end());
            // Calcular el nuevo medoide del cluster combinado
            cluster1.calcularMedoide();
            // Remover el segundo cluster
            C.removeCluster(cluster2);
        } else {
            // Si no se puede combinar, mover el primer cluster a Cout
            Cout.addCluster(cluster1);
            C.removeCluster(cluster1);
        }
    }

    // Procesar el último cluster restante en C si existe
    if (C.numClusters() > 0) {
        Cluster c = C.clusters.back();

        // Encontrar el vecino más cercano de c en Cout
        Cluster vecinoMasCercano = Cout.encontrarVecinoMasCercano(c);

        // Si hay clusters en Cout
        if (!Cout.numClusters() == 0) {
            // Si el tamaño combinado es menor o igual a B, combinar c con su vecino más cercano
            if (c.clusterPoints.size() + vecinoMasCercano.clusterPoints.size() <= B) {
                c.clusterPoints.insert(c.clusterPoints.end(), vecinoMasCercano.clusterPoints.begin(), vecinoMasCercano.clusterPoints.end());
                // Calcular el nuevo medoide de c combinado
                c.calcularMedoide();
                // Remover el vecino más cercano
                Cout.removeCluster(vecinoMasCercano);
            } else {
                // Si no se puede combinar, dividir c usando MinMax split
                auto [c1, c2] = c.minMaxSplit();

                // Añadir los dos clusters resultantes a Cout
                Cout.addCluster(c1);
                Cout.addCluster(c2);
            }
        } else {
            // Si Cout está vacío, añadir c a Cout
            Cout.addCluster(c);
        }
    }

    // Retornar el conjunto de clusters de salida
    return Cout;
}

/**
 * Función OutputHoja que retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es el radio cobertor y a la dirección del hijo respectivo.
 * @param Cin Un conjunto de puntos
 * @return Una tupla (g, r, a) como entry
 */
Entry OutputHoja(const Cluster& Cin) {
    // 1. Sea g el medoide primario de Cin.
    // Calcula el medoide primario de Cin utilizando el método de la clase Cluster
    puntosbd g = Cin.medoide;
    
    // 2. Inicializa r a 0 y crea un nuevo nodo hoja
    double r = 0.0; // Radio cobertor inicializado a 0
    Node* a = new Node(); // Creamos un nuevo nodo hoja
    a->is_leaf = true; // El nodo es una hoja

    // 3. Por cada punto en Cin, añádelo a las entradas de C y actualiza r
    for (const auto& p : Cin.clusterPoints) {
        double distancia = euc_distance(g, p); // Calcula la distancia entre g y p
        r = std::max(r, distancia); // Actualiza el radio cobertor como el máximo entre r y la distancia

        // Añade el punto al nodo hoja
        Entry entrada{p, 0, nullptr}; // El radio es inicialmente 0 y no hay nodo hijo
        a->keys.push_back(entrada);
    }

    // 4. Retorna (g, r, a) como la entrada
    return Entry{g, r, a};
}


/**
 * Función OutputInterno que retorna una tupla (G, R, A) donde G es el medoide primario del conjunto de puntos Cin,
 * R es el radio cobertor y A es la dirección del hijo respectivo.
 * @param Cmra Un conjunto de tuplas (g, r, a) retornadas por OutputHoja.
 * @return Una tupla (G, R, A) como Entry.
 */
Entry OutputInterno(const std::vector<Entry>& Cmra) {
    // 1. Extrae los puntos g de Cmra y crea un objeto Cluster para Cin
    Cluster cinCluster;
    for (const auto& entry : Cmra) {
        cinCluster.addPoint(entry.point);
    }

    // Calcula el medoide primario de Cin
    cinCluster.calcularMedoide();
    puntosbd G = cinCluster.medoide;

    // Inicializa el radio cobertor a 0
    double R = 0.0;

    // Crea un nuevo nodo interno
    Node* A = new Node();
    A->is_leaf = false; // El nodo es interno

    // 2. Por cada entrada (g, r, a) en Cmra, añade (g, r, a) a A
    // y actualiza R con el máximo de R y (distancia entre G y g + r)
    for (const auto& entry : Cmra) {
        double distancia = euc_distance(G, entry.point); // Calcula la distancia entre G y entry.punto
        R = std::max(R, distancia + entry.cr); // Actualiza el radio cobertor con el máximo de R y distancia + entry.radio

        // Añade la entrada a las claves del nodo interno A
        A->keys.push_back(entry);
    }

    // 4. Retorna la tupla (G, R, A) como Entry
    return Entry{G, R, A};
}



/**
 * Función del método de construcción SS, que permite a partir de un conjunto de puntos, construir el árbol M.
 * @param C_in_puntos Conjunto de puntos a partir de los cuales se construirá el árbol M.
 * @return Un objeto de tipo Node.
 */
Node* ss(const vector<puntosbd>& C_in_puntos) {
    // Determina los valores de B y b para la poda de los nodos
    size_t B = 4096 / (sizeof(Entry) + sizeof(bool) + sizeof(int));
    size_t b = static_cast<size_t>(0.5 * B);
    
    // 1. Caso base: Si el tamaño de puntos es menor o igual a B, se aplica la función OutputHoja
    if (C_in_puntos.size() <= B) {
        Cluster cluster;
        for (const auto& punto : C_in_puntos) {
            cluster.addPoint(punto);
        }
        cluster.calcularMedoide();
        Entry entry = OutputHoja(cluster);
        return entry.child;
    }
    
    // 2. Crear Cout = Cluster(Cin)
    ClusterSet C_Out;
    C_Out = cluster_fun(C_in_puntos, b, B);
    
    // 2. Inicializa un (ClusterSet) en este caso un vector de Entrys vacio C={}
    std::vector<Entry> C;

    
    // 3. Por cada cluster en el C_Out, se aplica OutputHoja y se agrega la entrada a C
    for (const Cluster& cluster : C_Out.clusters) {
        Entry entry = OutputHoja(cluster);
        C.push_back(entry);
    }
    
    // 4. Mientras el tamaño de C sea mayor que B, se realiza la poda
    while (C.size() > B) {
        // Construye un conjunto de puntos `Cin` a partir de las entradas en C
        vector<puntosbd> Cin;
        for (const Entry& entry : C) {
            Cin.push_back(entry.point);
        };
        
        // Crea un nuevo ClusterSet a partir de Cin
        C_Out = cluster_fun(Cin, b, B);
        // Inicializa un vector para almacenar tuplas (g, r, a) de las entradas de C correspondientes a los clusters de Cout
        vector<vector<Entry>> Cmra;


        for (const Cluster& cluster : C_Out.clusters) {
            vector<Entry> s;
            for (const auto& entry : C) {
                if (cluster.contains(entry.point)) {
                    s.push_back(entry);
                }
            }
            Cmra.push_back(s);
        }
        
        // Vacía el vector C antes de volver a llenarlo con los resultados de OutputInterno
        C.clear();
        
        // 4.4 Por cada conjunto de entradas en Cmra, se aplica OutputInterno y se agrega la entrada resultante a C
        for (const auto& s : Cmra) {
            Entry entry = OutputInterno(s);
            C.push_back(entry);
        }
    }
    
    //5.  Finalmente, aplica OutputInterno a C y retorna el puntero al nodo resultante
    Entry rootEntry = OutputInterno(C);

    // 6. Se retorna "a"
    return rootEntry.child;
}


int main(){
    printf("Hello World");
    return 0;
}