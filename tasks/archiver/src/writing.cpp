#include "writing.h"

std::string IntTo9bitN(int n, bool nine) {
    std::string ans;
    while (n > 0) {
        if (n % 2 == 0) {
            ans += '0';
        } else {
            ans += '1';
        }
        n /= 2;
    }
    if (nine) {
        return ans + std::string(NEW_BYTE_LENGTH - ans.size(), '0');
    }
    return ans + std::string(BYTE_LENGTH - ans.size(), '0');
}

int Bin2intWrite(const std::string& s, bool decode) {
    std::string copy(s);
    if (!decode) {
        std::reverse(copy.begin(), copy.end());
    }
    int i = 1;
    int cnt = 0;
    for (char e : copy) {
        if (e == '1') {
            cnt += i;
        }
        i *= 2;
    }
    return cnt;
}

void Buffer::Write() {
    int c = 0;
    if (!decode_) {
        c = Bin2intWrite(to_write_, false);
    } else {
        c = Bin2intWrite(to_write_, true);
    }
    char* chr = new char[1];
    chr[0] = static_cast<char>(c);
    fout_.write(chr, 1);
    delete[] chr;
    to_write_.clear();
}

void Buffer::WriteBits(const std::string str) {
    try {
        for (char c : str) {
            to_write_ += c;
            if (to_write_.size() == BYTE_LENGTH) {
                Write();
            }
        }
    } catch (const std::exception& e) {
        Error("failed while writing into the file").ThrowError();
    }
}

void Buffer::Close() {
    fout_.close();
}

bool Buffer::Empty() {
    return (to_write_.empty());
}

void Buffer::LastWrite() {
    if (!this->Empty()) {
        to_write_ += std::string(BYTE_LENGTH - to_write_.size(), '0');
        Write();
    }
}

void Buffer::SetStream(const std::string& file) {
    to_write_.clear();
    fout_.close();
    fout_ = std::ofstream(file);
}

void Buffer::SetDecode(bool type) {
    decode_ = type;
}