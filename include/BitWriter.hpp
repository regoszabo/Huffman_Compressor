#ifndef BITWRITER_H
#define BITWRITER_H

#include <fstream>
#include<vector>
using namespace std;

class BitWriter {
private:
    ofstream& out;  // referenciaként kapja meg a kimeneti fájlt
    unsigned char buffer;  // ide gyűjtjük a biteket
    int bitCount;          // hány bit van jelenleg a bufferben (0–7)

public:
    // Konstruktor
    BitWriter(ofstream& output) : out(output), buffer(0), bitCount(0) {}

    // Egyetlen bit kiírása
    void writeBit(bool bit);

    // Több bit kiírása egyszerre
    void writeBits(const vector<bool>& bits);

    // A maradék bitek kiírása (ha nem teljes bájt)
    void flush();
};

#endif



