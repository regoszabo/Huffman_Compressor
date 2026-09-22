#include "TreeStructure.hpp"
#include <algorithm>

// Konstruktor
TreeStructure::TreeStructure() : root(nullptr) {}
//Destruktor
TreeStructure::~TreeStructure() {
    if (root) freeTree(root);
}

// Fa felszabadítása
void TreeStructure::freeTree(Node* n) {
    if (!n) return;
    freeTree(n->left);
    freeTree(n->right);
    delete n;
}

// Fa építése
void TreeStructure::build(const FrequencyTable& freq) {
    priority_queue<Node*, std::vector<Node*>, CompareNode> heap;//prioritás sor, ahol rendezve vannak a tagok gyakoriság alapján

    // levél Node-ok létrehozása
    for (int i = 0; i < 256; ++i) {
        if (freq.get(i) > 0) {
            heap.push(new Node(static_cast<unsigned char>(i), freq.get(i)));
        }
    }

    // fa építése
    while (heap.size() > 1) {
        Node* left = heap.top(); heap.pop();
        Node* right = heap.top(); heap.pop();
        Node* parent = new Node(left, right);
        heap.push(parent);
    }

    // gyökér
    if (!heap.empty())
        root = heap.top();
}

// Fa mélysége
int TreeStructure::getDepth() const {
    function<int(Node*)> depth = [&](Node* n) -> int { // lambda fv.
        if (!n) return 0;
        return 1 + max(depth(n->left), depth(n->right)); //max mélység megállapítása
    };
    return depth(root);
}
static void buildCodeTableDFS(Node* node, std::vector<bool>& prefix,std::array<std::vector<bool>, 256>& table) {//Depth-First-Search alapú kódtábla építés
    if (!node) return;
    if (node->is_leaf) {
        table[node->symbol] = prefix;
        return;
    }
    // balra 0
    prefix.push_back(false);
    buildCodeTableDFS(node->left, prefix, table);
    prefix.pop_back();
    // jobbra 1
    prefix.push_back(true);
    buildCodeTableDFS(node->right, prefix, table);
    prefix.pop_back();

}

// Publikus függvény a kódtábla készítéséhez
void TreeStructure::buildCodeTable(std::array<std::vector<bool>, 256>& table) const {
    if (!root) return;
    vector<bool> prefix;
    buildCodeTableDFS(root, prefix, table);
}
