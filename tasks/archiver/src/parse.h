#pragma once
#include "string"
#include "iostream"
#include "error.h"

enum class ProcessType { Encoding, Decoding, Info };

class Arg {
public:
    explicit Arg() : cur_(0){};
    explicit inline Arg(int argc, char** argv);
    ~Arg() {
    }
    inline bool NotFinished() const;
    inline std::string ParsCur();
    inline ProcessType GetType();
    inline std::string GetFile();
    inline int GetCur() const;

private:
    int argc_;
    char** argv_;
    int cur_;
    ProcessType type_;
    std::string file_;
};