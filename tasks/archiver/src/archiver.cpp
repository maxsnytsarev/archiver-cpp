#include "iostream"
#include <fstream>
#include "map"
#include "ostream"
#include <sstream>
#include "vector"
#include <cassert>
#include <cstddef>
#include <utility>
#include "algorithm"
#include "Frequency.cpp"
#include "Huffman.cpp"
#include "writing.cpp"
#include "reading.cpp"
#include <cstring>
#include "Heap.h"
#include "parse.cpp"

void Encode(const std::string& file_name, Buffer& buffer, bool final_file) {
    std::string file = std::filesystem::path(file_name).filename();
    std::ifstream fin(file_name, std::ios::binary | std::ios::in);
    if (fin.fail()) {
        std::string error = "Opening file '" + file_name + "' failed, it either doesn't exist or is not accessible.";
        Error(error).ThrowError();
    }
    std::map<int, uint64_t> frequencies;
    try {
        frequencies = Frequency(fin, file_name);
    } catch (const std::exception& e) {
        Error("couldn't count frequencies").ThrowError();
    }
    std::vector<std::pair<uint16_t, std::string>> sorted_leaves;
    size_t max_code_len = 0;
    std::map<size_t, int> codes_len;
    std::map<uint16_t, std::string> codes_final;
    try {
        Node* root = Build(frequencies);
        std::map<Node*, std::string> codes;
        std::map<uint16_t, std::string> leaves;
        MakingCodes(codes, leaves, root);
        for (const auto& e : leaves) {
            sorted_leaves.emplace_back(e);
        }
        auto sort_leave = [&](const std::pair<uint16_t, std::string>& a, const std::pair<uint16_t, std::string>& b) {
            if (a.second.size() < b.second.size()) {
                return true;
            } else if (a.second.size() > b.second.size()) {
                return false;
            } else {
                return a.first < b.first;
            }
        };
        std::sort(sorted_leaves.begin(), sorted_leaves.end(), sort_leave);
        codes_final = MakeCanonicalCode(sorted_leaves, max_code_len);
        codes_len = CanonicalCodesLen(sorted_leaves, max_code_len);
        Clear(root);
    } catch (const std::exception& e) {
        Log(e);
        Error("huffman algorithm failed").ThrowError();
    }
    try {
        std::string symbols_count = IntTo9bitN(static_cast<int>(sorted_leaves.size()), true);
        buffer.WriteBits(symbols_count);
        for (const auto& e : sorted_leaves) {
            std::string cur = IntTo9bitN(e.first, true);
            buffer.WriteBits(cur);
        }
        for (size_t i = 1; i <= max_code_len; ++i) {
            std::string cur = IntTo9bitN(codes_len[i], true);
            buffer.WriteBits(cur);
        }
        for (char e : file) {
            unsigned char u_c = static_cast<unsigned char>(e);
            auto cur = static_cast<uint16_t>(u_c);
            buffer.WriteBits(codes_final[cur]);
        }
        buffer.WriteBits(codes_final[FILENAME_END]);
        fin.clear();
        fin.seekg(0, std::ios::beg);
        std::vector<std::byte> buf(1);
        while (fin.read(reinterpret_cast<char*>(buf.data()), 1)) {
            int chr = std::to_integer<int>(buf[0]);
            buffer.WriteBits(codes_final[chr]);
        }
        if (!final_file) {
            buffer.WriteBits(codes_final[ONE_MORE_FILE]);
        } else {
            buffer.WriteBits(codes_final[ARCHIVE_END]);
        }
    } catch (const std::exception& e) {
        Log(e);
        Error("writing into fail failed").ThrowError();
    }
    fin.close();
}

void Decode(const std::string& file_name) {
    try {
        ReadBuffer read(file_name);
        bool not_archive_end = true;
        while (not_archive_end) {
            std::string str_symbols_cnt;
            for (int i = 0; i < NEW_BYTE_LENGTH; ++i) {
                str_symbols_cnt += read.Read1Bit();
            }
            uint16_t symbols_cnt = StringToInt(str_symbols_cnt);
            std::vector<uint16_t> alphabet;
            for (int i = 0; i < symbols_cnt; ++i) {
                std::string str_cur;
                for (int j = 0; j < NEW_BYTE_LENGTH; ++j) {
                    str_cur += read.Read1Bit();
                }
                uint16_t cur = StringToInt(str_cur);
                alphabet.push_back(cur);
            }
            std::map<uint16_t, int> code_len;
            int cnt = 0;
            for (int i = 1; i < symbols_cnt; ++i) {
                std::string str_cur;
                for (int j = 0; j < NEW_BYTE_LENGTH; ++j) {
                    str_cur += read.Read1Bit();
                }
                uint16_t cur = StringToInt(str_cur);
                code_len[i] = cur;
                cnt += cur;
                if (cnt == symbols_cnt) {
                    break;
                }
            }
            std::map<uint16_t, std::string> codes;
            MakeCodesDecode(alphabet, code_len, codes);
            DecodeNode* root = MakingTreeDecode(codes);
            bool not_name_stop = true;
            std::string out_name;
            DecodeNode* cur_node = root;
            Buffer buffer;
            std::cout << "&" << std::endl;
            buffer.SetDecode(true);
            while (not_name_stop) {
                char e = read.Read1Bit();
                if (e == '1') {
                    cur_node = cur_node->GetRightD();
                } else {
                    cur_node = cur_node->GetLeftD();
                }
                if (!cur_node->GetRightD()) {
                    if (cur_node->GetCodeD() == FILENAME_END) {
                        not_name_stop = false;
                    } else {
                        out_name += static_cast<char>(cur_node->GetCodeD());
                    }
                    cur_node = root;
                }
            }
            buffer.SetStream(out_name);
            bool not_stop = true;
            while (not_stop) {
                char e = read.Read1Bit();
                if (e == '1') {
                    cur_node = cur_node->GetRightD();
                } else {
                    cur_node = cur_node->GetLeftD();
                }
                if (!cur_node->GetRightD()) {
                    if (cur_node->GetCodeD() == ARCHIVE_END) {
                        not_stop = false;
                        not_archive_end = false;
                    } else if (cur_node->GetCodeD() == ONE_MORE_FILE) {
                        not_stop = false;
                    } else {
                        std::string to_write = IntTo9bitN(cur_node->GetCodeD(), false);
                        buffer.WriteBits(to_write);
                    }
                    cur_node = root;
                }
            }
            Clear(root);
        }
    } catch (const std::exception& e) {
        Log(e);
        std::string error = "failed while decoding " + file_name;
        Error(error).ThrowError();
    }
}

void Info() {
    std::cout << "Usage: archiver [OPTIONS] [FILES]\n\n"
              << "Commands:\n"
              << "  -c, --create    Encode files into an archive\n"
              << "  -d, --decode    Decode an archive file\n"
              << "  -h, --help      Display this help text\n\n"
              << "Examples:\n"
              << "  archiver -c out file1 file2       Encode file1 and file2 into out\n"
              << "  archiver -d out                    Decode the archive file out\n"
              << "  archiver -h                        Show help information\n";
}

int main(int argc, char** argv) {
    try {
        Arg parse = Arg(argc, argv);
        if (parse.GetType() == ProcessType::Encoding) {
            Buffer buffer(parse.GetFile());
            while (parse.NotFinished()) {
                try {
                    if (parse.GetCur() == argc - 1) {
                        Encode(parse.ParsCur(), buffer, true);
                    } else {
                        Encode(parse.ParsCur(), buffer, false);
                    }
                } catch (const std::exception& e) {
                    Log(e);
                }
            }
            buffer.LastWrite();
            buffer.Close();
        } else if (parse.GetType() == ProcessType::Decoding) {
            Decode(parse.GetFile());
        } else if (parse.GetType() == ProcessType::Info) {
            Info();
        }
        return 0;
    } catch (const std::exception& e) {
        Log(e);
        return ERROR_CODE;
    }
}
