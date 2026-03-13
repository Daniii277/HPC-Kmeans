#include <iostream>
#include <vector>
#include <cstdint>
#include <ctime>
#include "../include/Kmeans.hpp"
#include <cstdio>

int main(){

    FILE* file = fopen("../Data/datos.bin", "rb");

    if(!file){
        std::cerr << "Error al abrir el archivo\n";
        return 1;
    }

    uint32_t nRows, nCols;

    fread(&nRows, sizeof(uint32_t), 1, file);
    fread(&nCols, sizeof(uint32_t), 1, file);

    std::vector<float> data(nRows * nCols);

    fread(data.data(), sizeof(float), data.size(), file);

    fclose(file);

    uint32_t k = 32;
    kMeans kmeans(k);

    std::vector<float> centroids(k * nCols);

    srand(time(NULL));

    for(uint32_t i = 0; i < k; i++)
    {
        uint32_t r = rand() % nRows;

        for(uint32_t j = 0; j < nCols; j++)
            centroids[i * nCols + j] =
                data[r * nCols + j];
    }

    std::vector<uint32_t> assignments(nRows, UINT32_MAX);

    clock_t start = clock();

    kmeans.run(data, nCols, nRows, centroids, assignments);

    clock_t end = clock();

    double duration =
        double(end - start) / CLOCKS_PER_SEC;

    std::cout << "El algoritmo tardó "
              << duration
              << " segundos\n";

    return 0;
}