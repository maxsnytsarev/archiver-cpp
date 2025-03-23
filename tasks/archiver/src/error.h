#pragma once

#include "string"
#include "fstream"
#include "stdexcept"
#include "iostream"
#include "iosfwd"
#include <sstream>

class Error {
public:
    explicit Error() {
        msg_ << "something went wrong";
    }
    explicit Error(const std::string& error1) {
        msg_ << error1;
    }
    void ThrowError() {
        throw std::runtime_error(msg_.str());
    }
    ~Error() {
        msg_.clear();
    }

private:
    std::ostringstream msg_;
};

inline void Log(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}