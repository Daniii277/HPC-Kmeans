#include <iostream>
#include <vector>
#include <cstdint>
#include "../include/Kmeans.hpp"

int main(){
    //Abrir el archivo binario con los datos
    FILE* file = fopen("../Data/datos.bin", "rb");
    if (!file) {
        std::cerr << "Error al abrir el archivo para lectura\n";
        return 1;
    }
    uint32_t nRows;
    uint32_t nCols;

    // Leer dimensiones
    if (fread(&nRows, sizeof(uint32_t), 1, file) != 1) {
    std::cerr << "Error leyendo nRows\n";
    return 1;
}

if (fread(&nCols, sizeof(uint32_t), 1, file) != 1) {
    std::cerr << "Error leyendo nCols\n";
    return 1;
}
// Crear vector para los datos
std::vector<float> data(nRows * nCols);

//Leer la matriz
if (fread(data.data(), sizeof(float), data.size(), file) != data.size()) {
    std::cerr << "Error leyendo datos\n";
    return 1;
}
    
   
    //Cerrar fichero binario
    fclose(file);

    std::vector<float> centroids(nCols);
    std::vector<uint32_t> assignaments(nRows);

    kMeans k(12);
    k.run(data, nCols, nRows, centroids, assignaments);


    std::cout << "algoritmo finalizado";


    return 0;
}