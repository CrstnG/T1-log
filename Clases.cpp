#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include "node.h"

/**
 * @class Cluster
 * @brief Representa un cluster de puntos en un espacio bidimensional (2D).
 * @param clusterPoints Puntos del cluster.
 * @param medoide Medoide del cluster.
*/
class Cluster {
    public:
        std:: vector <puntosbd> clusterPoints; // Puntos del cluster
        puntosbd medoide; // Medoide del cluster

        // Constructor por defecto
        Cluster() {}

        // Constructor con parámetros
        Cluster(const puntosbd& _medoide, const std::vector<puntosbd>& _clusterPoints) : medoide(_medoide), clusterPoints(_clusterPoints) {}

        // Métodos de la clase

        // 1. Añadir un punto al Cluster
        void addPoint(const puntosbd& p) {
            clusterPoints.push_back(p);
        }

        // 2. Calcular el medoide del Cluster
        void calcularMedoide() {
            if (clusterPoints.empty()) {
                // Si no hay puntos en el cluster, no hay nada que calcular
                return;
            }

            // Variables para almacenar el mejor medoide encontrado y la distancia mínima total
            double minDistanciaTotal = std::numeric_limits<double>::infinity();
            puntosbd mejorMedoide;

            // Calcular la distancia total de cada punto a los demás
            for (const auto& punto1 : clusterPoints) {
                double distanciaTotal = 0.0;
                for (const auto& punto2 : clusterPoints) {
                    if (punto1 != punto2) {
                        // Calcular la distancia euclidiana entre punto1 y punto2
                        double dx = std::get<0>(punto1) - std::get<0>(punto2);
                        double dy = std::get<1>(punto1) - std::get<1>(punto2);
                        double distancia = std::sqrt(dx * dx + dy * dy);
                        distanciaTotal += distancia;
                    }
                }

                // Actualizar el mejor medoide si se encuentra una distancia total menor
                if (distanciaTotal < minDistanciaTotal) {
                    minDistanciaTotal = distanciaTotal;
                    mejorMedoide = punto1;
                }
            }

            // Asignar el mejor medoide encontrado a la variable de clase
            medoide = mejorMedoide;
        }

        // 3. Calcular el máximo radio cobertor del Cluster
        double maxRadioCluster() {
            if (clusterPoints.empty()) {
                // Si no hay puntos en el cluster, el máximo radio es 0
                return 0.0;
            }

            double maxRadio = 0.0;

            // Iterar sobre cada par de puntos en el cluster
            for (size_t i = 0; i < clusterPoints.size() - 1; ++i) {
                for (size_t j = i + 1; j < clusterPoints.size(); ++j) {
                    // Calcular la distancia entre clusterPoints[i] y clusterPoints[j] usando euc_distance
                    double distancia = euc_distance(clusterPoints[i], clusterPoints[j]);

                    // Actualizar el máximo radio si se encuentra una distancia mayor
                    maxRadio = std::max(maxRadio, distancia);
                }
            }

            // Retornar el máximo radio cobertor del cluster
            return maxRadio;
        }

        // MinMaxPolicy: Divide el cluster en dos usando la política MinMax split
        std::pair<Cluster, Cluster> minMaxSplit() {
            // Si no hay suficientes puntos en el cluster para dividirlo, retorna el cluster tal cual
            if (clusterPoints.size() < 2) {
                return {Cluster(medoide, clusterPoints), Cluster()};
            }

            double minRadioCobertorMax = std::numeric_limits<double>::infinity();
            std::pair<puntosbd, puntosbd> mejoresPuntos;

            // Itera sobre todos los pares de puntos en el cluster
            for (size_t i = 0; i < clusterPoints.size() - 1; i++) {
                for (size_t j = i + 1; j < clusterPoints.size(); j++) {
                    // Selecciona un par de puntos como candidatos para la división
                    puntosbd punto1 = clusterPoints[i];
                    puntosbd punto2 = clusterPoints[j];

                    // Divide el cluster en dos subgrupos a partir de los puntos seleccionados
                    std::vector<puntosbd> grupo1, grupo2;
                    for (const auto& p : clusterPoints) {
                        // Calcula la distancia entre el punto actual y los puntos seleccionados
                        double dist1 = euc_distance(p, punto1);
                        double dist2 = euc_distance(p, punto2);

                        // Alternativamente asigna el punto actual al grupo 1 o grupo 2 según cuál sea más cercano
                        if (dist1 < dist2) {
                            grupo1.push_back(p);
                        } else {
                            grupo2.push_back(p);
                        }
                    }

                    // Calcula el máximo radio cobertor de los dos subgrupos
                    Cluster subCluster1(punto1, grupo1);
                    Cluster subCluster2(punto2, grupo2);
                    double radio1 = subCluster1.maxRadioCluster();
                    double radio2 = subCluster2.maxRadioCluster();
                    double maxRadioCobertor = std::max(radio1, radio2);

                    // Actualiza el par de puntos si se encuentra un radio cobertor máximo menor
                    if (maxRadioCobertor < minRadioCobertorMax) {
                        minRadioCobertorMax = maxRadioCobertor;
                        mejoresPuntos = {punto1, punto2};
                    }
                }
            }

            // Divide el cluster usando el par de puntos con el mínimo radio cobertor máximo
            std::vector<puntosbd> grupo1, grupo2;
            for (const auto& p : clusterPoints) {
                // Asigna los puntos a los grupos según cuál sea más cercano
                double dist1 = euc_distance(p, mejoresPuntos.first);
                double dist2 = euc_distance(p, mejoresPuntos.second);

                if (dist1 < dist2) {
                    grupo1.push_back(p);
                } else {
                    grupo2.push_back(p);
                }
            }

            // Crea y retorna los dos nuevos clusters resultantes de la división
            Cluster c1(mejoresPuntos.first, grupo1);
            Cluster c2(mejoresPuntos.second, grupo2);
            return {c1, c2};
        }

        // Método contains
        bool contains(const puntosbd& p) const {
        // Iterar sobre los puntos en clusterPoints para verificar si p existe en el cluster
            for (const auto& punto : clusterPoints) {
                if (punto == p) {
                    return true; // p se encuentra en el cluster
                }
            }
            return false; // p no se encuentra en el cluster
        }

        // Método de comparación de igualdad
        bool operator==(const Cluster& other) const {
            // Compara los medoides primero
            if (medoide != other.medoide) {
                return false;
            }

            // Compara los puntos de los clusters
            if (clusterPoints.size() != other.clusterPoints.size()) {
                return false;
            }

            // Ordena los puntos para comparar de forma más consistente
            auto sorted_this = clusterPoints;
            auto sorted_other = other.clusterPoints;

            return sorted_this == sorted_other;
        }
};



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

        // 0. Número de clusters en el conjunto
        size_t numClusters() const {
            return clusters.size();
        }

        // 1. Añadir un cluster al conjunto
        void addCluster(const Cluster& cluster) {
            clusters.push_back(cluster);
        }

        // 2. Remover un cluster
        void removeCluster(const Cluster& cluster) {
            // Encontrar el cluster a eliminar en el vector clusters
            auto it = std::find(clusters.begin(), clusters.end(), cluster);
            
            // Si se encuentra el cluster, eliminarlo del vector
            if (it != clusters.end()) {
                clusters.erase(it);
            }
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


