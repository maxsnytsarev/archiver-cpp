#include "queue"
#include "string"
#include "error.h"
#include "Huffman.h"
#include "Heap.h"

bool Node::operator==(const Node& n) {
    return std::tie(cnt_, code_, left_, right_) == std::tie(n.cnt_, n.code_, n.left_, n.right_);
}

std::strong_ordering operator<=>(const Node& n1, const Node& n2) {
    if (n1.GetCnt() < n2.GetCnt()) {
        return std::strong_ordering::less;
    } else if (n1.GetCnt() > n2.GetCnt()) {
        return std::strong_ordering::greater;
    } else {
        if (n1.GetCode() < n2.GetCode()) {
            return std::strong_ordering::less;
        } else if (n1.GetCode() > n2.GetCode()) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }
}

Node* Build(const std::map<int, uint64_t>& frequencies) {
    Heap<Node*> heap = Heap<Node*>();
    try {
        for (auto e : frequencies) {
            Node* p = new Node(e.second, e.first);
            heap.Push(p);
        }
        while (heap.Size() > 1) {
            Node* n1 = heap.Top();
            heap.Pop();
            Node* n2 = heap.Top();
            heap.Pop();
            Node* n = new Node(n1, n2);
            heap.Push(n);
        }
        return heap.Top();
    } catch (const std::exception& e) {
        Error("failed while building queue").ThrowError();
    }
    return heap.Top();
}

void MakingCodes(std::map<Node*, std::string>& codes, std::map<uint16_t, std::string>& leaves, Node* cur) {
    try {
        if (cur->GetLeft()) {
            codes[cur->GetLeft()] = codes[cur] + "0";
            codes[cur->GetRight()] = codes[cur] + "1";
            MakingCodes(codes, leaves, cur->GetLeft());
            MakingCodes(codes, leaves, cur->GetRight());
        } else {
            leaves[cur->GetCode()] = codes[cur];
        }
    } catch (const std::exception& e) {
        Error("failed recursion while getting codes").ThrowError();
    }
}

uint16_t DecodeNode::GetCodeD() const {
    return code_;
}

DecodeNode* DecodeNode::GetLeftD() const {
    return left_;
}

DecodeNode* DecodeNode::GetRightD() const {
    return right_;
}

uint16_t Bin2intHuffman(const std::string& c) {
    std::string copy(c);
    std::reverse(copy.begin(), copy.end());
    int i = 1;
    uint16_t cnt = 0;
    for (char e : copy) {
        if (e == '1') {
            cnt += i;
        }
        i *= 2;
    }
    return cnt;
}

std::string Int2binHuffman(int n, size_t len) {
    std::string ans;
    while (n > 0) {
        if (n % 2 == 0) {
            ans += "0";
        } else {
            ans += "1";
        }
        n /= 2;
    }
    std::reverse(ans.begin(), ans.end());
    return std::string(len - ans.size(), '0') + ans;
}

size_t NextNode::GetMaxCode() const {
    return max_code_len_;
}
std::vector<std::pair<uint16_t, std::string>> NextNode::GetLeaves() {
    return sorted_leaves_;
}
void NextNode::Make() {
    sorted_leaves_[0].second = std::string(sorted_leaves_[0].second.size(), '0');
    for (int i = 1; i < sorted_leaves_.size(); ++i) {
        uint16_t bin = Bin2intHuffman(sorted_leaves_[i - 1].second);
        incr_ = Int2binHuffman(bin + 1, sorted_leaves_[i - 1].second.size());
        cur_ = incr_ + std::string(sorted_leaves_[i].second.size() - incr_.size(), '0');
        sorted_leaves_[i].second = cur_;
        max_code_len_ = std::max(max_code_len_, cur_.size());
    }
}

std::map<uint16_t, std::string> MakeCanonicalCode(std::vector<std::pair<uint16_t, std::string>>& sorted_leaves,
                                                  size_t& max_code_len) {
    std::map<uint16_t, std::string> code_final;
    try {
        NextNode next = NextNode(sorted_leaves, max_code_len);
        next.Make();
        sorted_leaves = next.GetLeaves();
        max_code_len = next.GetMaxCode();
        for (const auto& e : sorted_leaves) {
            code_final[e.first] = e.second;
        }
    } catch (const std::exception& e) {
        Error("failed while making canonical code").ThrowError();
    }
    return code_final;
}

std::map<size_t, int> CanonicalCodesLen(const std::vector<std::pair<uint16_t, std::string>>& sorted_leaves,
                                        const size_t max_code_len) {
    std::map<size_t, int> codes_len;
    for (size_t i = 1; i <= max_code_len; ++i) {
        codes_len[i] = 0;
    }
    for (const auto& e : sorted_leaves) {
        codes_len[e.second.size()] += 1;
    }
    return codes_len;
}

void Clear(Node* cur) {
    try {
        if (cur->GetLeft()) {
            Clear(cur->GetLeft());
            Clear(cur->GetRight());
            delete cur;
        } else {
            delete cur;
        }
    } catch (const std::exception& e) {
        Error("failed to delete the tree").ThrowError();
    }
}
void Clear(DecodeNode* cur) {
    try {
        if (cur->GetLeftD()) {
            Clear(cur->GetLeftD());
            Clear(cur->GetRightD());
            delete cur;
        } else {
            delete cur;
        }
    } catch (const std::exception& e) {
        Error("failed to delete the tree").ThrowError();
    }
}

void CreatingNodes(int& word, int start, const std::pair<uint16_t, int>& e, std::map<uint16_t, std::string>& codes,
                   const std::vector<uint16_t>& alphabet) {
    for (int i = start; i < e.second; ++i) {
        int bin = Bin2intHuffman(codes[alphabet[word - 1]]);
        std::string incr = Int2binHuffman(bin + 1, codes[alphabet[word - 1]].size());
        std::string cur = incr + std::string(e.first - incr.size(), '0');
        codes[alphabet[word]] = cur;
        ++word;
    }
}

void MakeCodesDecode(const std::vector<uint16_t>& alphabet, const std::map<uint16_t, int>& code_len,
                     std::map<uint16_t, std::string>& codes) {
    try {
        bool first = true;
        int word = 0;
        for (const auto& e : code_len) {
            if (first) {
                if (e.second != 0) {
                    codes[alphabet[0]] = std::string(e.first, '0');
                    first = false;
                    ++word;
                    CreatingNodes(word, 1, e, codes, alphabet);
                }
            } else {
                CreatingNodes(word, 0, e, codes, alphabet);
            }
        }
    } catch (const std::exception& e) {
        Error("failed while getting alphabet codes from given file").ThrowError();
    }
}

DecodeNode* MakingTreeDecode(const std::map<uint16_t, std::string>& codes) {
    Heap<DecodeNode*> heap = Heap<DecodeNode*>();
    for (const auto& e : codes) {
        DecodeNode* n = new DecodeNode(e.first, e.second);
        heap.Push(n);
    }
    try {
        while (heap.Size() > 2) {
            DecodeNode* n1 = heap.Top();
            heap.Pop();
            DecodeNode* n2 = heap.Top();
            heap.Pop();
            DecodeNode* n = new DecodeNode(n2, n1);
            heap.Push(n);
        }
        DecodeNode* one = heap.Top();
        heap.Pop();
        DecodeNode* zero = heap.Top();
        heap.Pop();
        DecodeNode* root = new DecodeNode(zero, one);
        return root;
    } catch (const std::exception& e) {
        Error("failed while building tree").ThrowError();
    }
    return heap.Top();
}
