#pragma once

#include "queue"
#include "string"
#include "error.h"
#include "map"
#include "cstdint"
#include "algorithm"

class Node {
public:
    explicit Node() : cnt_(0), left_(nullptr), right_(nullptr) {
    }
    Node(const Node& n) : cnt_(n.cnt_), code_(n.code_), left_(n.left_), right_(n.right_) {
    }
    explicit Node(uint64_t cnt) : cnt_(cnt), left_(nullptr), right_(nullptr) {
    }
    explicit Node(uint64_t cnt, uint16_t code) : cnt_(cnt), code_(code), left_(nullptr), right_(nullptr) {
    }
    explicit Node(Node* n1, Node* n2)
        : cnt_(n1->cnt_ + n2->cnt_), code_(std::min(n1->code_, n2->code_)), left_(n1), right_(n2) {
    }
    inline uint64_t GetCnt() const {
        return cnt_;
    }

    inline uint16_t GetCode() const {
        return code_;
    }

    inline Node* GetLeft() const {
        return left_;
    }

    inline Node* GetRight() const {
        return right_;
    }

    inline bool operator==(const Node& n);

private:
    uint64_t cnt_;
    uint16_t code_;
    Node* left_;
    Node* right_;
};

inline std::strong_ordering operator<=>(const Node& n1, const Node& n2);

inline Node* Build(const std::map<int, uint64_t>& frequencies);

inline void MakingCodes(std::map<Node*, std::string>& codes, std::map<uint16_t, std::string>& leaves, Node* cur);

class DecodeNode {
public:
    explicit DecodeNode() : left_(nullptr), right_(nullptr) {
    }
    explicit DecodeNode(uint16_t code, const std::string& path)
        : code_(code), path_(path), left_(nullptr), right_(nullptr) {
    }
    explicit DecodeNode(DecodeNode* n1, DecodeNode* n2)
        : code_(std::min(n1->code_, n2->code_)),
          path_(n1->path_.substr(0, n1->path_.size() - 1)),
          left_(n1),
          right_(n2) {
    }
    inline DecodeNode* GetLeftD() const;
    inline DecodeNode* GetRightD() const;
    inline std::string GetPath() const {
        return path_;
    }
    inline uint16_t GetCodeD() const;

private:
    uint16_t code_;
    std::string path_;
    DecodeNode* left_;
    DecodeNode* right_;
};

inline uint16_t Bin2intHuffman(const std::string& c);

inline std::string Int2binHuffman(int n, size_t len);

inline std::map<uint16_t, std::string> MakeCanonicalCode(std::vector<std::pair<uint16_t, std::string>>& sorted_leaves,
                                                         size_t& max_code_len);

inline std::map<size_t, int> CanonicalCodesLen(const std::vector<std::pair<uint16_t, std::string>>& sorted_leaves,
                                               const size_t max_code_len);

inline void Clear(Node* cur);
inline void Clear(DecodeNode* cur);

inline void MakeCodesDecode(const std::vector<uint16_t>& alphabet, const std::map<uint16_t, int>& code_len,
                            std::map<uint16_t, std::string>& codes);

inline DecodeNode* MakingTreeDecode(const std::map<uint16_t, std::string>& codes);

class NextNode {
public:
    NextNode(std::vector<std::pair<uint16_t, std::string>>& sorted_leaves, size_t& max_code_len)
        : sorted_leaves_(sorted_leaves), max_code_len_(max_code_len) {
    }

    inline size_t GetMaxCode() const;
    inline std::vector<std::pair<uint16_t, std::string>> GetLeaves();
    inline void Make();

private:
    std::string incr_;
    std::string cur_;
    std::vector<std::pair<uint16_t, std::string>> sorted_leaves_;
    size_t max_code_len_;
};

inline void CreatingNodes(int& word, int start, const std::pair<uint16_t, int>& e,
                          std::map<uint16_t, std::string>& codes, const std::vector<uint16_t>& alphabet);