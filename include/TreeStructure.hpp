#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <cstdint>
#include <vector>
#include <queue>
#include <functional>
#include <iostream>
#include "FrequencyTable.hpp" // a korábbi freq-osztály
using namespace std;

// Node struct
struct Node {
    uint64_t freq;        // az összes level gyakorisága az agban
    unsigned char symbol; // csak leveleknel fontos
    Node* left; // bal gyerek(0)
    Node* right; // jobb gyerek(1)
    bool is_leaf; // level-e

    // level konstruktor
    Node(unsigned char s, uint64_t f)
            : freq(f), symbol(s), left(nullptr), right(nullptr), is_leaf(true) {}

    // belső csomópont konstruktor
    Node(Node* l, Node* r)
            : freq(l->freq + r->freq), left(l), right(r), is_leaf(false) {}
};

//Node összehasonlító
struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // kisebb gyakorisagu legyen elorebb
    }
};

// Huffman fa osztaly
class TreeStructure {
private:
    Node* root;

public:
    TreeStructure();
    ~TreeStructure();

    // fa letrehozasa frekvenciatabla alapjan
    void build(const FrequencyTable& freq);

    // gyoker elerese
    Node* getRoot() const { return root; }

    // fa melysege
    int getDepth() const;

    // összes Node felszabaditasa
    void freeTree(Node* n);
    //Kodtabla
    void buildCodeTable(std::array<std::vector<bool>, 256>& table) const;
};

#endif
