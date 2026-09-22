#include "Compressor.hpp"
#include "Decompress.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Teszt fálj létrehozó ismétlődő mintával adott bájt nagyságban
void createTestFile(const string& filename, const string& pattern, size_t sizeBytes) {
    ofstream out(filename, ios::binary);
    if (!out) {
        throw runtime_error("Nem sikerült a fájlt létrehozni: " + filename);
    }

    size_t written = 0;
    while (written < sizeBytes) {
        size_t len = min(pattern.size(), sizeBytes - written);
        out.write(pattern.data(), len);
        written += len;
    }
    out.close();
}

// bájt-bájt összehasonlítás
bool filesAreIdentical(const string& file1, const string& file2) {
    ifstream f1(file1, ios::binary);
    ifstream f2(file2, ios::binary);

    if (!f1 || !f2) return false;

    char b1, b2;
    while (true) {
        f1.get(b1);
        f2.get(b2);

        // if one file ends before the other
        if (f1.eof() && f2.eof()) return true;
        if (f1.eof() || f2.eof()) return false;

        if (b1 != b2) return false;
    }
}

int main() {
    try {
        Compressor compressor;
        Decompress decoder;

        // Teszt 1. kis fájl
        cout << "\n TEST 1: SMALL FILE \n";
        string inputSmall = "small_input.bin";
        string compressedSmall = "small_compressed.bin";
        string decodedSmall = "small_decoded.bin";
        createTestFile(inputSmall, "ABCDEFG", 200); // 200 bájtnyi ismétlődő minta
        compressor.compress(inputSmall, compressedSmall);
        decoder.decompress(compressedSmall, decodedSmall);

        auto sizeOrig1 = fs::file_size(inputSmall);
        auto sizeComp1 = fs::file_size(compressedSmall);
        auto sizeDec1 = fs::file_size(decodedSmall);

        cout << "Original size: " << sizeOrig1 << " B\n";
        cout << "Compressed size: " << sizeComp1 << " B\n";
        cout << "Decoded size: " << sizeDec1 << " B\n";

        bool identicalSmall = filesAreIdentical(inputSmall, decodedSmall);
        cout << "Files identical (byte by byte): " << (identicalSmall ? "YES" : "NO") << "\n\n";


        //Test 2: nagy fájl
        cout << "\n TEST 2: LARGE FILE \n";
        string inputLarge = "large_input.bin";
        string compressedLarge = "large_compressed.bin";
        string decodedLarge = "large_decoded.bin";

        createTestFile(inputLarge, "ABCDEABCDEABCDE", 200000); // 200 KB adat

        compressor.compress(inputLarge, compressedLarge);
        decoder.decompress(compressedLarge, decodedLarge);

        auto sizeOrig2 = fs::file_size(inputLarge);
        auto sizeComp2 = fs::file_size(compressedLarge);
        auto sizeDec2 = fs::file_size(decodedLarge);

        cout << "Original size: " << sizeOrig2 << " B\n";
        cout << "Compressed size: " << sizeComp2 << " B\n";
        cout << "Decoded size: " << sizeDec2 << " B\n";
        cout << "Decode success (size check): "
             << ((sizeOrig2 == sizeDec2) ? "YES" : "NO") << "\n";

        cout << "\nAll tests completed.\n";
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
