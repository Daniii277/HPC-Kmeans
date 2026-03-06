#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <ctime>

// Genera puntos aleatorios en un vector plano usando push_back
void generateRandomPoints(std::vector<float>& v, int dim, int nPoints, float maxValue, float minValue = 0.0f) {
    if (dim <= 0 || nPoints <= 0) return;
    for (int i = 0; i < nPoints; i++) {
        for (int j = 0; j < dim; j++) {
            float r = ((float)rand() / RAND_MAX) * (maxValue - minValue) + minValue;
            v.push_back(r);
        }
    }
}

// Imprime cada punto en una línea
void printMatrixConsole(const std::vector<float>& data, uint32_t nRows, uint32_t nCols) {
    for (uint32_t i = 0; i < nRows; i++) {
        for (uint32_t j = 0; j < nCols; j++) {
            std::cout << data[i * nCols + j] << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    srand((unsigned int)time(0)); // Semilla


    int nSize = 5; // dimensiones del punto
    int totalPoints = 1000000;

    // Vector vacío
    std::vector<float> data;

    // Llenar el vector de puntos aleatorios
    generateRandomPoints(data, nSize, totalPoints, 20.0f, -20.0f);

    // Guardar en archivo binario
    FILE* file = fopen("../Data/datos.bin", "wb");
    if (!file) {
        std::cerr << "Error al abrir el archivo para escritura\n";
        return 1;
    }

    uint32_t nRows = totalPoints;
    uint32_t nCols = nSize;

    std::fwrite(&nRows, sizeof(uint32_t), 1, file);
    std::fwrite(&nCols, sizeof(uint32_t), 1, file);
    std::fwrite(data.data(), sizeof(float), data.size(), file);

    fclose(file);

    // Imprimir los puntos por consola
    //printMatrixConsole(data, nRows, nCols);

    return 0;
}