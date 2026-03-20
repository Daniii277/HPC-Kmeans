#include "../include/Kmeans.hpp"
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <climits>
#include <omp.h>
#include <mpi.h> 


kMeans::kMeans(uint32_t k): numClusters(k) {}


float kMeans::DistanceCalculator(const float* p1, const float* p2, uint32_t dim){
    float dist = 0.0f;
    for(int i = 0; i < dim; i++){
        float dif = p1[i] - p2[i];
        dist += dif * dif;
    }
    return dist;

}



void kMeans::run(const std::vector<float>& data, uint32_t cols, uint32_t rows, std::vector<float>& centroids, std::vector<uint32_t>& assignments, int rank, int size)
{
    const uint32_t maxIterations = 2000;
    uint32_t iter = 0;
    uint32_t changes;
    uint32_t global_changes = 0;
    uint32_t total_rows = assignments.size();
    uint32_t numCentroids = centroids.size() /cols;
    //Creo el vector local de asignaciones de cada nodo MPI.
    std::vector<uint32_t> local_assignments(rows, UINT32_MAX);
    //Vcetor para sumar los puntos según su centroide asignado.
    std::vector<float> v_sum(numCentroids * cols);
    std::vector<uint32_t> v_assignation(numCentroids);
    //Creo los vectores de suma y asignaciones solo para el uso del nodo 0
    std::vector<float> v_sum_global(numCentroids * cols, 0.0);
    std::vector<uint32_t> v_assignation_global(numCentroids, 0.0);
    
    //Creo los centroides iniciales en el nodo 0 y los envio a los demas nodos con broadcast.
    if(rank == 0) std::fill(centroids.begin(), centroids.end(), 0.0);
    MPI_Bcast( centroids.data() , centroids.size() , MPI_FLOAT , 0 , MPI_COMM_WORLD);
    //std::cout << "El nodo " << rank << "tiene : " << rows << " puntos";
    //Bucle del algoritmo (2000 iteraciones max)
    while(iter < maxIterations){
        if(rank == 0) std::cout << "\nIteración : " << iter << std::endl;
        changes = 0;
        global_changes = 0;
        std::fill(v_sum.begin(), v_sum.end(), 0.0);
        std::fill(v_assignation.begin(), v_assignation.end(), 0.0);
        if(rank == 0){
            std::fill(v_sum_global.begin(), v_sum_global.end(), 0.0);
            std::fill(v_assignation_global.begin(), v_assignation_global.end(), 0.0);
        }

        //Asignación del mejor cluster para cada punto
        for(uint32_t i = 0; i < rows; i++){
            uint32_t bestCluster = 0;
            uint32_t minDist = UINT32_MAX;
            //Primer valor del punto correspondiente
            uint32_t starterPointValue = i * cols;

            for(uint32_t j = 0; j < numCentroids; j++){
                uint32_t dist = DistanceCalculator(&centroids[j * cols], &data[starterPointValue], cols);

                if(dist < minDist){
                    minDist = dist;
                    bestCluster = j;
                }
            }

            if(local_assignments[i] != bestCluster){
                local_assignments[i] = bestCluster;
                for(int k = 0; k < cols; k++) v_sum[bestCluster * cols + k] += data[starterPointValue + k];
                v_assignation[bestCluster]++;
                changes++;
            }
        }
        std::cout << "Nodo " << rank << " cambios: " << changes << "\n";
        //Uno las asignaciones,la suma de puntos por centroide y el vector del numero de puntos por centroide.
        MPI_Gather( local_assignments.data() , rows , MPI_UINT32_T , assignments.data() , rows, MPI_UINT32_T , 0 , MPI_COMM_WORLD);
        MPI_Reduce( v_sum.data() , v_sum_global.data() , v_sum.size() , MPI_FLOAT , MPI_SUM , 0 , MPI_COMM_WORLD);
        MPI_Reduce( v_assignation.data() , v_assignation_global.data() , numCentroids , MPI_UINT32_T , MPI_SUM , 0 , MPI_COMM_WORLD);
        MPI_Allreduce( &changes , &global_changes , 1 , MPI_UINT32_T , MPI_SUM , MPI_COMM_WORLD);
        if(rank == 0) std::cout << "Cambios en esta iteracion: " << global_changes << "\n";

        ///////////////////////////////////////
        ////Actualización de los centroides////
        ///////////////////////////////////////

        //El nodo 0 se encarga de la actualización de los centroides al tener el conjunto de los datos.
        if(rank == 0){
            for(int i = 0; i < numCentroids; i++){
                for(int j = 0; j < cols; j++){
                    centroids[i * cols + j] = v_sum_global[i * cols +j] / v_assignation_global[i];
                }
            }
        }
        MPI_Bcast(centroids.data(), centroids.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);
        //Comprobar el criterio del 5% de cambios.
        if (global_changes < (0.05f * total_rows)) break;        
        iter++;
    }

    if(rank == 0) std::cout << "\nKMeans finalizado en " << iter << " iteraciones\n";
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