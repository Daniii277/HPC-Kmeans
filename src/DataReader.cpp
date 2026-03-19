#include "../include/DataReader.hpp"
#include <fstream>
#include <iostream>

void DataReaderBinary(const char *filename, std::vector<float> &data, uint32_t *cols, uint32_t *rows) {
    // Creo la clase para lectura de ficheros de la libreria fstream (ifstream)
    std::ifstream file(filename, std::ios::binary);
    // Compruebo que se haya abierto el archivo sin errores
    if (!file) {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }

    // Leo las dimensiones de los puntos
    file.read(reinterpret_cast<char *>(rows), sizeof(*rows));
    file.read(reinterpret_cast<char *>(cols), sizeof(*cols));

    // Modifico el tamaño del vector según el tamaño de los datos del fichero
    data.resize(static_cast<size_t>(*rows) * static_cast<size_t>(*cols));

    // Leo los datos y los almaceno en data
    file.read(reinterpret_cast<char *>(data.data()),
              static_cast<std::streamsize>(sizeof(float) * data.size()));

    // Cierro el fichero.
    file.close();
}