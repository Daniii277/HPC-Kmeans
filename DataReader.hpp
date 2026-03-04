#include <iostream>
#include<vector>


void DataReaderBinary(FILE *filename, std::vector<float> &data, int* col, int* rows);
void printDataReaderBinary(std::vector<float>data);