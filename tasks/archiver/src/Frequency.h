#pragma once

#include "cstdint"
#include "vector"
#include "fstream"
#include "map"
#include "string"

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const int ERROR_CODE = 111;

inline std::map<int, uint64_t> Frequency(std::ifstream& fin, const std::string& file_n);