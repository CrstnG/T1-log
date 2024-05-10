#include "../node.h"
#include "../aux_functions.cpp"


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
                        double distancia = euc_distance(punto1, punto2);
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
};
