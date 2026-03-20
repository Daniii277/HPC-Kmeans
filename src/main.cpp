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
#include <mpi.h>


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
    std::cout << "Leyendo los datos del fichero..." << std::endl;
    DataReaderBinary(filename, data, &nCols, &nRows);
    }
    //Envío los datos mediante broadcast
    MPI_Bcast(&nRows, 1, MPI_UINT32_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nCols, 1, MPI_UINT32_T, 0, MPI_COMM_WORLD);
    

    //Cada nodo MPI crea un vector de datos local al que se le pasarán los datos.
    uint32_t local_rows = nRows / size;
    std::vector<float> local_data(local_rows * nCols);

    //El nodo 0 realizará el reparto
    
        //Datos por cada nodo
        uint32_t nData = (nRows / size) * nCols;
        //Reparto con scatter
        if(rank == 0) std::cout << "Repartiendo los datos entre los nodos MPI..." << std::endl;
        MPI_Scatter( data.data(), nData, MPI_FLOAT , local_data.data() , nData , MPI_FLOAT , 0, MPI_COMM_WORLD);
    

    //Se crea la clase y se inicializan las estructuras necesarias.
    uint32_t k = 32;
    kMeans kmeans(size);

    std::vector<float> centroids(k * nCols);
    std::vector<uint32_t> global_assignments(nRows, UINT32_MAX);

    //Se sincronizan los nodos para que comiencen a la vez 
    if(rank == 0) std::cout << "Sincronizando los nodos MPI para ejecutar el algoritmo k-means..." << std::endl;
    MPI_Barrier( MPI_COMM_WORLD);
    auto start = std::chrono::high_resolution_clock::now();
    if(rank == 0) {
    std::cout << "numero de nodos : " << size << std::endl;
    std::cout << "numero de puntos por nodo : " << local_rows << std::endl;
    }
    kmeans.run(local_data, nCols, local_rows, centroids, global_assignments, rank, size);
    MPI_Barrier( MPI_COMM_WORLD);

    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();
    //Imprimo el tiempo que ha tardado en ejecutarse por pantalla.
    if(rank == 0){
        std::cout << "El algoritmo tardó " << duration << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}