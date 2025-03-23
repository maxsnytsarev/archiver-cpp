#include "parse.h"
#include "cstring"

Arg::Arg(int argc, char** argv) : argc_(argc), argv_(argv), cur_(0) {
    if (argc == 1) {
        Error("not enough arguments").ThrowError();
    } else if (std::strcmp(argv[1], "-c") == 0) {
        type_ = ProcessType::Encoding;
        if (argc <= 3) {
            Error("not enough arguments").ThrowError();
        }
        file_ = argv[2];
        cur_ = 3;
    } else if (std::strcmp(argv[1], "-d") == 0) {
        if (argc > 3) {
            Error("too many arguments").ThrowError();
        }
        file_ = argv[2];
        type_ = ProcessType::Decoding;
    } else if (std::strcmp(argv[1], "-h") == 0) {
        if (argc != 2) {
            Error("too many arguments").ThrowError();
        }
        type_ = ProcessType::Info;
    } else {
        Error("unknown flag").ThrowError();
    }
}

bool Arg::NotFinished() const {
    return cur_ < argc_;
}
std::string Arg::ParsCur() {
    return argv_[cur_++];
}
ProcessType Arg::GetType() {
    return type_;
}
std::string Arg::GetFile() {
    return file_;
}
int Arg::GetCur() const {
    return cur_;
}
