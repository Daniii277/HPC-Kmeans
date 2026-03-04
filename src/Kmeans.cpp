#pragma once
#include "Kmeans.hpp"


kMeans::kMeans(uint32_t k): numClusters(k) {}


float kMeans::DistanceCalculator(const float* p1, const float* p2, uint32_t dim){
    float dist = 0.0f;
    for(int i = 0; i < dim; i++){
        float dif = p1[i] - p2[i];
        dist += dif * dif;
    }
    return dist;

}



void kMeans::run(const std::vector<float>& data, uint32_t cols, uint32_t rows, std::vector<float>& centroids, std::vector<uint32_t>& assignments){


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