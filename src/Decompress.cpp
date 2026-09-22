#include "Decompress.hpp"
#include "FrequencyTable.hpp"
#include "TreeStructure.hpp"
#include "BitReader.hpp"
#include <fstream>
#include <iostream>

void Decompress::decompress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) throw runtime_error("Cannot open file: " + inputFile);

    // ️ Használt karakterek számának olvasása
    uint16_t usedCount;
    in.read(reinterpret_cast<char*>(&usedCount), sizeof(usedCount));

    //  Frekvenciatábla visszaállítása
    FrequencyTable freq;
    for (int i = 0; i < usedCount; ++i) {
        unsigned char ch;
        uint64_t f;
        in.read(reinterpret_cast<char*>(&ch), 1);
        in.read(reinterpret_cast<char*>(&f), sizeof(f));
        freq.set(ch, f);
    }

    //Össz érvényes bit számának kiolvasása
    uint64_t totalBits;
    in.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

    // Fa újraépítése
    TreeStructure tree;
    tree.build(freq);

    BitReader br(in);

    std::ofstream out(outputFile, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot create file: " + outputFile);

    auto* node = tree.getRoot();
    uint64_t bitsRead = 0; //már beolvasott bitek száma

    while (bitsRead < totalBits) {
        bool bit = br.readBit();
        bitsRead++;

        node = bit ? node->right : node->left;

        if (node->is_leaf) {
            out.put(static_cast<unsigned char>(node->symbol));
            node = tree.getRoot();
        }
    }

    in.close();
    out.close();

    std::cout << "Decompress is done: " << outputFile << std::endl;
}
