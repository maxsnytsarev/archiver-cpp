#pragma once

#include "vector"
#include "algorithm"
#include "error.h"
#include "Huffman.h"

inline bool CmpNodes(const Node* a, const Node* b) {
    if (a->GetCnt() < b->GetCnt()) {
        return true;
    } else if (a->GetCnt() > b->GetCnt()) {
        return false;
    } else {
        if (a->GetCode() < b->GetCode()) {
            return true;
        }
        return false;
    }
}

inline bool CmpNodes(const DecodeNode* n1, const DecodeNode* n2) {
    if (n1->GetPath().size() > n2->GetPath().size()) {
        return true;
    } else if (n1->GetPath().size() < n2->GetPath().size()) {
        return false;
    } else {
        if (n1->GetPath() > n2->GetPath()) {
            return true;
        }
        return false;
    }
}

template <typename T>
inline bool Cmp(const T a, const T b) {
    if (std::is_same<T, Node*>::value or std::is_same<T, DecodeNode*>::value) {
        return CmpNodes(a, b);
    }
    try {
        return a < b;
    } catch (const std::exception& e) {
        Log(e);
        Error("types in heap are incomparable").ThrowError();
    }
    return true;
}

template <typename T>
class Heap {
public:
    Heap() : size_(0), nodes_({T()}) {
    }
    inline void ShiftUp(int v) {
        while (v > 1 && Cmp(nodes_[v], nodes_[v / 2])) {
            std::swap(nodes_[v], nodes_[v / 2]);
            v /= 2;
        }
    }
    inline void ShiftDown(int v) {
        while (2 * v <= size_) {
            int l = 2 * v;
            int r = 2 * v + 1;
            T left = nodes_[l];
            T right = nodes_[r];
            int min_node = -1;
            if (Cmp(right, left) && r <= size_) {
                min_node = r;
            } else {
                min_node = l;
            }
            if (Cmp(nodes_[min_node], nodes_[v])) {
                std::swap(nodes_[v], nodes_[min_node]);
                v = min_node;
            } else {
                break;
            }
        }
    }
    inline T Top() {
        return nodes_[1];
    }
    inline void Pop() {
        std::swap(nodes_[1], nodes_[size_]);
        nodes_.pop_back();
        size_ -= 1;
        ShiftDown(1);
    }
    inline void Push(T& node) {
        nodes_.push_back(node);
        size_ += 1;
        ShiftUp(size_);
    }
    inline int Size() const {
        return size_;
    }

private:
    int size_;
    std::vector<T> nodes_;
};