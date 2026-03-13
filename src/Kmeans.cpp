#include "../include/Kmeans.hpp"
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <climits>


kMeans::kMeans(uint32_t k): numClusters(k) {}


float kMeans::DistanceCalculator(const float* p1, const float* p2, uint32_t dim){
    float dist = 0.0f;
    for(int i = 0; i < dim; i++){
        float dif = p1[i] - p2[i];
        dist += dif * dif;
    }
    return dist;

}



void kMeans::run(const std::vector<float>& data, uint32_t cols, uint32_t rows,
                 std::vector<float>& centroids, std::vector<uint32_t>& assignments)
{
    const uint32_t maxIterations = 2000;
    uint32_t iter = 0;
    uint32_t changes = rows;

    if(assignments.size() != rows)
        assignments.resize(rows, UINT32_MAX);

    std::vector<float> sums(numClusters * cols);
    std::vector<uint32_t> counts(numClusters);

    while(iter < maxIterations)
    {
        std::cout << "\nIteración : " << iter << std::endl;

        changes = 0;

        std::fill(sums.begin(), sums.end(), 0.0f);
        std::fill(counts.begin(), counts.end(), 0);

        // ======================
        // ASIGNACION DE CLUSTERS
        // ======================
        for(uint32_t i = 0; i < rows; i++)
        {
            uint32_t bestCluster = 0;
            uint32_t minDist = UINT32_MAX;

            uint32_t pointIndex = i * cols;

            for(uint32_t c = 0; c < numClusters; c++)
            {
                uint32_t dist = DistanceCalculator(
                    &centroids[c * cols],
                    &data[pointIndex],
                    cols);

                if(dist < minDist)
                {
                    minDist = dist;
                    bestCluster = c;
                }
            }

            if(assignments[i] != bestCluster)
            {
                assignments[i] = bestCluster;
                changes++;
            }

            counts[bestCluster]++;

            for(uint32_t d = 0; d < cols; d++)
                sums[bestCluster * cols + d] += data[pointIndex + d];
        }

        std::cout << "Cambios en esta iteracion: " << changes << "\n";

        // ======================
        // ACTUALIZAR CENTROIDES
        // ======================
        for(uint32_t c = 0; c < numClusters; c++)
        {
            if(counts[c] > 0) // Evitar división por cero
            {
                for(uint32_t d = 0; d < cols; d++)
                {
                    centroids[c * cols + d] =
                        sums[c * cols + d] / counts[c];
                }
            }
        }

        // ======================
        // CRITERIO DE PARADA 5%
        // ======================
        if (changes < (0.05f * rows)) break;


        iter++;
    }

    std::cout << "\nKMeans finalizado en " << iter << " iteraciones\n";
}




/***********PSEUDOCÓDIGO DE K-MEANS*****************


función KMeans(data, rows, cols, k, maxIterations, tolerance):
    // 1. Inicializar centroides aleatorios
    centroids = elegir k puntos aleatorios de data

    iter = 0
    cambio = infinito

    mientras (iter < maxIterations y cambio > tolerance):
        cambio = 0

        // 2. Asignar puntos a clusters
        para cada punto i en data:
            minDist = infinito
            asignacion = -1
            para cada cluster j en centroids:
                dist = calcularDistancia(punto i, centroid j)
                si dist < minDist:
                    minDist = dist
                    asignacion = j
            if assignments[i] != asignacion:
                assignments[i] = asignacion
                cambio += 1

        // 3. Actualizar centroides
        para cada cluster j:
            suma = vector de tamaño cols inicializado a 0
            contador = 0
            para cada punto i:
                si assignments[i] == j:
                    suma += punto i
                    contador += 1
            si contador > 0:
                centroids[j] = suma / contador

        iter += 1

    retornar centroids, assignments



    VARIABLES QUE NECESITA LA FUNCIÓN : 
- Data : Vector de puntos.
- rows : número de filas (número de puntos).
- cols : número de columnas(dimensiones de los puntos)
- k : número de centroides
- maxIterations : número máximo de iteraciones.
- tolerance : condición para que pare el algoritmo (evitar bucles infinitos).



Las condiciones impuestas para terminar la ejecución del algoritmo serán las 2000 iteraciones 
o que se hayan cambiado de centroide menos del 5% de los puntos que tenemos.

Se debería pasar un vector para los centroides y otro para las asignaciones a cada grupo antes que tenerlo como una variable local.
No se pasan como consts al haber modificaciones, en cambio el vector de puntos si al no modificarse las coordenadas de ningun punto.




*/