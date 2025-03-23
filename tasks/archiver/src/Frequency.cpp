#include "error.h"

#include "iostream"
#include <fstream>
#include "map"
#include "Frequency.h"
#include "filesystem"

std::map<int, uint64_t> Frequency(std::ifstream& fin, const std::string& file_n) {
    std::map<int, uint64_t> frequencies;
    try {
        std::ifstream file_name(file_n);
        std::vector<std::byte> buffer_fr(1);
        while (fin.read(reinterpret_cast<char*>(buffer_fr.data()), 1)) {
            int chr = std::to_integer<int>(buffer_fr[0]);
            frequencies[chr] += 1;
        }
        std::string file = std::filesystem::path(file_n).filename();
        for (char c : file) {
            unsigned char u_c = static_cast<unsigned char>(c);
            frequencies[static_cast<uint16_t>(u_c)] += 1;
        }
        frequencies[FILENAME_END] = 1;
        frequencies[ONE_MORE_FILE] = 1;
        frequencies[ARCHIVE_END] = 1;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        Error("couldn't count frequencies").ThrowError();
    }
    return frequencies;
}
