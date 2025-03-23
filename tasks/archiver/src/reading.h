#pragma once
#include "error.h"
#include <fstream>
#include "cstdint"
#include "vector"

const int BYTE_R = 8;

inline int GetBit(const std::byte& byte, int position);

inline std::string ByteToString(const std::byte& b);

inline uint16_t StringToInt(const std::string& str);

class ReadBuffer {
public:
    explicit ReadBuffer() = default;
    ~ReadBuffer() {
        fin_.close();
        cur_.clear();
    }
    explicit ReadBuffer(const std::string& file_name) : fin_(file_name, std::ios::binary | std::ios::in) {
        if (fin_.fail()) {
            std::string error = "couldn't open " + file_name + " for reading";
            Error(error).ThrowError();
        }
    }
    inline char Read1Bit();

private:
    std::ifstream fin_;
    std::string cur_;
};
