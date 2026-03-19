
#include "../include/Statistics.hpp"
#include <mpi>
#include <omp.h>
#include <limits>
#include <iostream>


void PrintStats(const std::vector<float> &localData, uint32_t cols, int rank){

    uint32_t total_rows = 0;
    uint32_t local_rows = localData.size() / cols;

    //Calculo el total de filas (puntos) y le envío el dato a todos los nodos
    MPI_Allreduce( &local_rows , &total_rows , 1 , MPI_UINT32_T , MPI_SUM , MPI_COMM_WORLD);
    //inicializo las variables locales que van a usar los distintos nodos MPI
    std::vector<float> local_min(cols, std::numeric_limits<float>::min());
    std::vector<float> local_max(cols, std::numeric_limits<float>::max());
    std::vector<float> local_sum(cols, 0.0);


    //======================================//
    //                                      //
    //      CALCULO DE MIN, MAX Y MEAN      //
    //                                      //
    //======================================//

    #pragma omp parallel
    {
        //Creo las variables privadas para el resultado de cada hilo.
        std::vector<float> local_thread_min(cols, std::numeric_limits<float>::min());
        std::vector<float> local_thread_max(cols, std::numeric_limits<float>::max());
        std::vector<float> local_thread_sum(cols, 0.0);

        //Ejecuto en paralelo el bucle for para obtener min, max y la media.
        #pragma parallel for
        for(int i = 0; i < local_rows; i++){
            //Para cada coordenada del punto compruebo si es min o max y lo sumo para la media.
            for(int j = 0; j < cols; j++){
                float coord = localData[i * cols + j];

                if(local_thread_min[j] > coord) local_thread_min[j] = coord;
                if(local_thread_max[j] < coord) local_thread_max[j] = coord;
                local_thread_sum[j] += coord;
            }
        }

        //Para unir los resultados de los threads utilizo critical.
        #pragma omp critical
        {
            for(int k = 0; k < cols; k++){
                if(local_min[k] > local_thread_min[k]) local_min[k] = local_thread_min[k];
                if(local_max[k] < local_thread_max[k]) local_max[k] = local_thread_max[k];
                local_sum[k] += local_thread_sum[k];

            }
        }
    }
    //FUSION DE LOS DATOS CON MPI

    std::vector<float> global_min(cols);
    std::vector<float> global_max(cols);
    std::vector<float> global_sum(cols);
    //Con allReduce puedo obtener min y max de todos los nodos y la suma de todos los puntos.
    MPI_Reduce( &local_min , global_min , cols , MPI_FLOAT , MPI_MIN , 0, MPI_COMM_WORLD);
    MPI_reduce(&local_max, &global_max, cols, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Allreduce(&local_sum, &global_sum, cols , MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    //Calculo la media
    std::vector<float> global_mean(cols);

    for(int i = 0; i < cols; i++){
        global_mean[i] = global_sum[i] / total_rows;
    }


    //==================================//
    //                                  //
    //      CALCULO DE LA VARIANZA      //
    //                                  //
    //==================================// 

    std::vector<float> local_variance_sum(cols);
    #pragma omp parallel{

        std::vector<float> local_thread_variance_sum(cols);

        #pragma omp parallel for 
        for(int i = 0; i < local_rows; i++){
            for(int j = 0; j < cols; j++){
                double diff = localData[i * cols + j] - global_mean[j];
                local_thread_variance_sum[j] += diff * diff;
            }
        }

        //Uno los resultados de los hilos
        #pragma omp critical
        {
            for(int k = 0; k < cols; k++){
                local_variance_sum[k] += local_thread_variance_sum[k];
            }

        }
    }

    //Fusión de los datos con MPI
    std::vector<float> global_variance_sum(cols);

    MPI_Allreduce( &local_variance_sum , &global_variance_sum , cols , MPI_FLOAT , MPI_SUM , MPI_COMM_WORLD);
    
    //Calculo la varianza
    std::vector<float> global_variance(cols);
    for(int i = 0; i < cols; i++){
        global_variance[i] = global_variance_sum[i] / total_rows;
    }
    
    //EL NODO 0 IMPRIMIRÁ LOS RESULTADOS
    if(rank == 0){
        std::cout << "ESTADÍSTICAS GENERALES :" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        //IMPRIMO MIN
        std::cout << "Valor MIN : " << std::endl;
        for(int i = 0; i < cols; i++){
            if(i == 0) std::cout << "(";
            std::cout << global_min[i] << "\t" ;
            if(i == (cols -1)) std::cout << ")" << std::endl;
        }
        std::cout << "================================================" << std::endl;
        //IMPRIMO MAX
        std::cout << "Valor MAX : " << std::endl;
        for(int j = 0; j < cols; j++){
            if(i == 0) std::cout << "(";
            std::cout << global_max[i] << "\t" ;
            if(i == (cols -1)) std::cout << ")" << std::endl;
        }
        std::cout << "================================================" << std::endl;
        //IMPRIMO MEAN
        std::cout << "Valor medio : " << std::endl;
        for(int j = 0; j < cols; j++){
            if(i == 0) std::cout << "(";
            std::cout << global_mean[i] << "\t" ;
            if(i == (cols -1)) std::cout << ")" << std::endl;
        }
        std::cout << "================================================" << std::endl;
        //IMPRIMO VARIANZA
        std::cout << "Valor Varianza : " << std::endl;
        for(int j = 0; j < cols; j++){
            if(i == 0) std::cout << "(";
            std::cout << global_variance[i] << "\t" ;
            if(i == (cols -1)) std::cout << ")" << std::endl ;
        }




    }


}