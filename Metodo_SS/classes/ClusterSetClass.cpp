#include "../node.h"
#include "../aux_functions.cpp"
#include "ClusterClass.cpp"

/**
 * @class ClusterSet
 * @brief Representa un conjunto de clusters.
 * @param clusters Vector que almacena los clusters en el conjunto.
 */
class ClusterSet {
    public:
        // Vector que almacena los clusters en el conjunto
        std::vector<Cluster> clusters;

        // Constructor por defecto
        ClusterSet() {}

        // Constructor con parámetros
        ClusterSet(const std::vector<Cluster>& _clusters) : clusters(_clusters) {}

        // Métodos de la clase

        // 1. Añadir un cluster al conjunto
        void addCluster(const Cluster& cluster) {
            clusters.push_back(cluster);
        }

        // 2. Remover un cluster
        void removeCluster(const Cluster& cluster) {
            clusters.erase(std::remove(clusters.begin(), clusters.end(), cluster), clusters.end());
        }

        // 3. Encontrar pares más cercanos de clusters
        std::pair<Cluster, Cluster> encontrarParesMasCercanos() {
            double minDistancia = std::numeric_limits<double>::max();
            std::pair<Cluster, Cluster> parMasCercano;

            // Iteramos a través de todos los clusters para encontrar el par más cercano
            for (size_t i = 0; i < clusters.size(); ++i) {
                for (size_t j = i + 1; j < clusters.size(); ++j) {
                    // Calcular la distancia entre los medoides de los clusters i y j
                    double dist = euc_distance(clusters[i].medoide, clusters[j].medoide);

                    // Si se encuentra una distancia menor, actualizar el par más cercano y la distancia mínima
                    if (dist < minDistancia) {
                        minDistancia = dist;
                        parMasCercano = {clusters[i], clusters[j]};
                    }
                }
            }

            // Retornar el par de clusters más cercanos
            return parMasCercano;
        }



        // 4. Encontrar el vecino más cercano de un cluster en el conjunto
        Cluster encontrarVecinoMasCercano(const Cluster& cluster) {
            double minDistancia = std::numeric_limits<double>::max();
            Cluster vecinoMasCercano;

            for (const auto& otroCluster : clusters) {
                if (otroCluster.medoide != cluster.medoide) {
                    double dist = euc_distance(cluster.medoide, otroCluster.medoide);
                    if (dist < minDistancia) {
                        minDistancia = dist;
                        vecinoMasCercano = otroCluster;
                    }
                }
            }

            return vecinoMasCercano;
        }


        // 5. MinMaxPolicy
        std::pair<Cluster, Cluster> minMaxSplit(const Cluster& cluster) {
            const auto& puntos = cluster.clusterPoints;

            // Implementar la lógica para dividir el cluster usando MinMax split
            std::vector<puntosbd> c1, c2;

            // Dividir en dos partes iguales (puedes personalizar esta lógica)
            for (size_t i = 0; i < puntos.size() / 2; ++i) {
                c1.push_back(puntos[i]);
            }

            for (size_t i = puntos.size() / 2; i < puntos.size(); ++i) {
                c2.push_back(puntos[i]);
            }

            Cluster cluster1(cluster.medoide, c1);
            Cluster cluster2(cluster.medoide, c2);

            return {cluster1, cluster2};
        }


};


