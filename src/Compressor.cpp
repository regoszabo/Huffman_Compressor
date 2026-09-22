#include "Compressor.hpp"
#include <fstream>
#include <array>
#include <vector>
#include <iostream>

void Compressor::compress(const string& inputFile, const string& outputFile) {
    // Frekvenciatábla felépítése
    FrequencyTable freq;
    freq.countFromFile(inputFile);

    // fa építése
    TreeStructure tree;
    tree.build(freq);

    //  Kódtábla elkészítése
    array<std::vector<bool>, 256> codeTable;
    tree.buildCodeTable(codeTable);

    // Kimeneti fájl megnyitása
    ofstream out(outputFile, ios::binary);
    if (!out) throw runtime_error("cannot create file: " + outputFile);

    // Használt karakterek listájának előkészítése
    vector<pair<unsigned char, uint64_t>> usedChars;
    for (int i = 0; i < 256; ++i) {
        if (freq.get(i) > 0) {
            usedChars.push_back({static_cast<unsigned char>(i), freq.get(i)});
        }
    }

    //  Kódtábla (frekvencia tábla) írása
    uint16_t usedCount = static_cast<uint16_t>(usedChars.size());
    out.write(reinterpret_cast<const char*>(&usedCount), sizeof(usedCount));

    for (auto& [ch, f] : usedChars) {
        out.put(ch); // karakter
        out.write(reinterpret_cast<const char*>(&f), sizeof(f)); // gyakoriság
    }
    uint64_t validBits = 0;
    for (int i = 0; i < 256; ++i) {
        if (freq.get(i) > 0) {
            validBits += freq.get(i) * codeTable[i].size();//előfordulás * a tömörített bitsorozat hossza = érvényes bitek
        }
    }

//  Írjuk ki a validBits-et a fejlécbe
    out.write(reinterpret_cast<const char*>(&validBits), sizeof(validBits));

    //  Tömörített bitek írása
    BitWriter bw(out);
    ifstream in(inputFile, ios::binary);

    unsigned char byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) {
        bw.writeBits(codeTable[byte]);
    }
    bw.flush();
    in.close();
    out.close();

    std::cout << "Compression is done: " << outputFile << "\n";
}
