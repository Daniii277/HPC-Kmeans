#include "../include/Kmeans.hpp"
#include <stdlib.h>
#include <ctime>
#include <iostream>

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
        uint32_t iter = 0;
        float tolerance = 100.0f;
        uint32_t changes = 0;
        //Mientras se cumplan los requisitos para seguir iterando en el bucle del algoritmo.
        while(iter < maxIter && tolerance > 5){
            //Para cada punto comprobamos su centroide más cercano
            for(uint32_t i = 0; i < rows; i++){
                uint32_t minDist = 1000; //Numero alto al principio, ya que queremos guardar la distancia más corta.
                int asignacion = -1;
                //Para cada centroide / cluster.
                int nPoint = 0; //variable para indicar en que punto nos encontramos para la funcion de distancia.
                for(uint32_t j = 0; j < numClusters; j++){
                    int dist = DistanceCalculator(&centroids[j], &data[nPoint], cols);
                    if(dist < minDist){
                        //Asigno la nueva distancia mínima
                        minDist = dist;
                        //Indico a que centroide está asignado el punto.
                        asignacion = j;
                        if(assignments[i] != asignacion){
                            assignments[i] = asignacion;
                            changes++;
                        }
                    }
                    nPoint += cols; //Actualizo el valor para la posición del primer valor del punto.
                    //std::cout << "Esto es la iteracion de puntos: " + i;
                }

            }

            //ACTUALIZACIÓN DE LOS CENTROIDES
            for(uint32_t i = 0; i < numClusters; i++){
                uint32_t cont = 0;
                //Vector donde sumar las coordenadas de los puntos de un centroide;
                std::vector<uint32_t>suma(cols);
                //Para cada punto
                for(uint32_t j; j < rows; j++){
                    if(assignments[j] == i ){
                        for(int l = 0; l < cols; l++){
                            suma[l] += data[j * cols];

                        }
                        cont++;

                    }
                    //Con el vector suma de los puntos de un centroide hago la media de cada coordenada.
                    if(cont > 0){
                        for(uint32_t k = 0; k < cols; k++){
                        centroids[i + k] = suma[j + k] / cont;
                        }

                    }
                }


                //std::cout << "Esta es la iteracion de actualización de puntos : " + i;
            }
            tolerance = (changes / rows) * 100;
            iter++;

        }
        std::cout << "El algoritmo ha finalizado.";
        return;


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