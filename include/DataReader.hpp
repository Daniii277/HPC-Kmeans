#ifndef DATA_READER_HPP
#define DATA_READER_HPP

#include <cstdint>
#include <vector>

void DataReaderBinary(const char *filename, std::vector<float> &data, uint32_t *cols, uint32_t *rows);

void printDataReaderBinary(const std::vector<float> &data);

#endif // DATA_READER_HPP