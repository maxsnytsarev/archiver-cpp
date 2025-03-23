#include "error.h"
#include "reading.h"

int GetBit(const std::byte& byte, int position) {
    std::byte t = (byte >> position);
    std::byte k = (t &= std::byte(0b00000001));
    return (k == std::byte(0b00000001));
}

std::string ByteToString(const std::byte& b) {
    std::string ans;
    for (int i = BYTE_R - 1; i >= 0; --i) {
        int bit = GetBit(b, i);
        if (bit == 1) {
            ans += "1";
        } else {
            ans += "0";
        }
    }
    return ans;
}

uint16_t StringToInt(const std::string& str) {
    int i = 1;
    uint16_t ans = 0;
    for (char e : str) {
        if (e == '1') {
            ans += i;
        }
        i *= 2;
    }
    return ans;
}

char ReadBuffer::Read1Bit() {
    try {
        if (!cur_.empty()) {
            char e = cur_[0];
            cur_ = cur_.substr(1, cur_.size() - 1);
            return e;
        } else {
            std::vector<std::byte> buffer(1);
            fin_.read(reinterpret_cast<char*>(buffer.data()), 1);
            std::string cur = ByteToString(buffer[0]);
            cur_ = cur;
            return this->Read1Bit();
        }
    } catch (const std::exception& e) {
        Error("failed while reading 1 bit").ThrowError();
    }
    return '!';
}
