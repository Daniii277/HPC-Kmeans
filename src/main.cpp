#include <iostream>
#include <vector>
#include <cstdint>
#include <ctime>
#include <chrono>
#include "../include/Kmeans.hpp"
#include <cstdio>
#include "../include/Statistics.hpp"
#include "../include/DataReader.hpp"
#include <omp.h>
#include <mpi>


int main(int argc, char** argv){

    MPI_Init( &argc , &argv);

    int size, rank;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &size);


    uint32_t nRows, nCols;
    std::vector<float> data;

    //El nodo 0 es el único que abre el archivo y guarda los datos en un vector. Se comunica a los nodos MPI el número
    //de filas y de columnas.
    if(rank == 0){
    char* filename = "../Data/datos.bin";
    DataReaderBinary(filename, data, &nRows, &nCols);

    //Envío los dtaos mediante broadcast
    MPI_Bcast(&nRows, 1, MPI_UINT32_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nCols, 1, MPI_UNIT32_T, 0, MPI_COMM_WORLD);
    }

    //Cada nodo MPI crea un vector de datos local al que se le pasarán los datos.
    uint32_t local_rows = nRows / size;
    std::vector<float> local_data(local_rows * nCols);

    //El nodo 0 realizará el reparto
    if(rank == 0){
        //Datos por cada nodo
        uint32_t nData = (nRows / size) * nCols;
        //Reparto con scatter
        MPI_Scatter( data.data(), nData, MPI_FLOAT , local_data.data() , nData , MPI_FLOAT , 0, MPI_COMM_WORLD);
    }

    //Se crea la clase y se inicializan las estructuras necesarias.
    uint32_t k = 32;
    kMeans kmeans(k);

    std::vector<float> centroids(k * nCols);
    std::vector<uint32_t> assignments(nRows, UINT32_MAX);

    //Se sincronizan los nodos para que comiencen a la vez 
    MPI_Barrier( MPI_COMM_WORLD);
    auto start = std::chrono::high_resolution_clock::now();
    kmeans.run(data, nCols, nRows, centroids, assignments);
    MPI_Barrier( MPI_COMM_WORLD);

    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();
    //Imprimo el tiempo que ha tardado en ejecutarse por pantalla.
    if(rank == 0){
        std::cout << "El algoritmo tardó " << duration << " segundos" << std::endl;
    }

    return 0;
}