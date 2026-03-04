#pragma once
#include "DataReader.hpp"
#include <fstream>
#include <iostream>
#include <cstdint>




void DataReaderBinary(const char*filename, std::vector<float> &data, uint32_t* col, uint32_t* rows){
    //Creo la clase para lectura de ficheros de la libreria fstream (ifstream)
    std::ifstream file(filename, std::ios::binary);
    //Compruebo que se haya abierto el archivo sin errores
    if(!file){
        std::cerr << "Error al abrir el archivo";
        return;
    }
    //Leo las dimensiones de los puntos
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&col), sizeof(col));
    //Modifico el tamaño del vector según el tamaño de los datos del fichero y poder leer todo con read.
    data.resize(rows * col);
    //Leo los datos y los almaceno en Data.
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    //Cierro el fichero.
    file.close();
}