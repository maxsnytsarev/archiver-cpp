#pragma once

#include "error.h"
#include "fstream"
#include "cstdint"
#include "algorithm"

const int BYTE_LENGTH = 8;
const int NEW_BYTE_LENGTH = 9;

inline std::string IntTo9bitN(int n, bool nine);

inline int Bin2intWrite(const std::string& s, bool decode);

class Buffer {
public:
    explicit Buffer() = default;
    explicit Buffer(const std::string& out_file) : fout_(out_file) {
        if (fout_.fail()) {
            std::string error = "Couldn't write into " + out_file;
            Error(error).ThrowError();
        }
    }
    ~Buffer() {
        to_write_.clear();
    }
    inline void Write();

    inline void WriteBits(const std::string str);

    inline void Close();

    inline bool Empty();

    inline void LastWrite();

    inline void SetStream(const std::string& file);

    inline void SetDecode(bool type);

private:
    std::string to_write_;
    std::ofstream fout_;
    bool decode_;
};