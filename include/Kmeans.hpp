#include <cstdint>
#include <vector>

class kMeans{
    private : 

        uint32_t numClusters;
        uint32_t maxIter = 2000;
        //Función para calcular la distancia entre dos puntos (Se le pasan por punteros constantes y se indican las dimensiones al obtenerlos del mismo vector d puntos).
        float DistanceCalculator(const float* p1, const float* p2, uint32_t cols);


    public : 
        //CONSTRUCTOR
        kMeans(uint32_t k);
        // Función para ejecutar el algoritmo de k-means.
        void run(const std::vector<float>& data, uint32_t cols, uint32_t rows, std::vector<float>& centroids, std::vector<uint32_t>& assignments, int rank, int size);

};